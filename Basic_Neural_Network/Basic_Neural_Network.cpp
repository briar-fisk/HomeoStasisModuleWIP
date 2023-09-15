#define OLC_PGE_APPLICATION
//#define OLC_PGEX_NT3

#include "olcPixelGameEngine.h"
#include "NT3_Master.h"

//Use the bindings to select the output traces.




class c_Sim
{
public:

	int Iteration;

	int O2;
	int O2_Delta;

	int Temp;
	int Temp_Delta;

	int O2_Pump;
	int Heater;

	c_Sim()
	{
		Iteration = 0;

		O2 = 5;
		O2_Delta = 1;

		Temp = 5;
		Temp_Delta = 1;

		O2_Pump = 0;
		Heater = 0;
	}

	// Afferent Sensors - Goals
	// O2      | -1/iteration //Breathing   | Goal: 5 | G:5 Y:(4 & 6)      R: (< 4 & > 6)  | Priority: 2  
	// Temp    | -2/iteration //Winter      | Goal: 8 | G:8 Y:(6-7 & 9-10) R: (< 6 & > 10) | Priority: 2  

	// Actuator Systems - Feedback
	// O2 Pump | +4/Iteration [O2]          | -1/Iteration [Temperature] | if (O2 < 5) (Heater = 1) else (Heater = 0)
	// Heater  | +3/Iteration [Temperature] |                            | if (Temp < 8) (Heater = 1) else (Heater = 0)

	int iterate()
	{
		Iteration++;

		//Environmental
		int tmp_O2 = O2;
		O2 -= 1; //Breathing

		int tmp_Temp = Temp;
		Temp -= 2; //Winter

		//Calculate actuator effects.
		if (O2_Pump) { O2 += 4; Temp -= 1; } //O2 Pump
		if (Heater) { Temp += 3; } //Heater

		//Calculate the environmental changes based on the previous iterations.
		if (O2 < tmp_O2) { O2_Delta = 0; }
		if (O2 == tmp_O2) { O2_Delta = 1; }
		if (O2 > tmp_O2) { O2_Delta = 2; }

		if (Temp < tmp_Temp) { Temp_Delta = 0; }
		if (Temp == tmp_Temp) { Temp_Delta = 1; }
		if (Temp > tmp_Temp) { Temp_Delta = 2; }

		//Calculate the actuator state.
		if (tmp_Temp < 8) { Heater = 1; }
		if (tmp_O2 < 5) { O2_Pump = 1; }

		if (tmp_Temp >= 8) { Heater = 0; }
		if (tmp_O2 >= 5) { O2_Pump = 0; }

		return Iteration;
	}

	void output()
	{
		std::cout << "\n";
		olc::NT3::oindex(Iteration);
		std::cout << " - O2________: "; olc::NT3::oint(0, (O2 + 1), O2);
		std::cout << " - O2_Delta__: "; olc::NT3::oint(0, (O2_Delta + 1), O2_Delta);

		std::cout << " - Temp______: "; olc::NT3::oint(0, (Temp + 1), Temp);
		std::cout << " - Temp_Delta: "; olc::NT3::oint(0, (Temp_Delta + 1), Temp_Delta);

		std::cout << " - O2_Pump___: "; olc::NT3::oint(0, (O2_Pump + 1), O2_Pump);
		std::cout << " - Heater____: "; olc::NT3::oint(0, (Heater + 1), Heater);
	}
};


//06 31:25 Space Arcologies


class c_Homeostasis
{
public:

	olc::NT3::c_NT3_Construct_1D* Raw_Goal;
	olc::NT3::c_NT3_Construct_1D* Raw_Feedback;
	int Raw_Goal_Count;
	int Raw_Feedback_Count;

	olc::NT3::c_NT3_Construct_1D MSC;
	olc::NT3::c_NT3_Construct_1D Chrono;
	void* Chrono_Array[3]; //Switch to dynamic tomorrow
	int Chrono_Depth;

	//Going to strip this out, or rewrite the NT3 engine, either way this isn't cross platform so olc shall be cast out.
	olc::PixelGameEngine Empty_Boi;

	//Afferent Goal States are the sensors we use as our goals.
	olc::NT3::c_Table_1D Afferent_Goal_State; //For now integer, any decimals will be brought up to keep significant value, will discuss with the team.
	olc::NT3::c_Table_1D Afferent_Goal_State_Previous; //Saved each update, used to calculate deltas.
	olc::NT3::c_Table_1D Afferent_Goal_State_Delta; //Integer, for reduced state spread.
	olc::NT3::c_Table_1D Afferent_Goal_State_Names; //For naming each index.
	olc::NT3::c_Table_1D Afferent_Goal_State_Types; //For setting the data types.

	//Afferent Feedback is the current state of things such as motors and other actuators, whether they are on, off, etc.
	//This is needed in the afferent side so the output includes it.
	olc::NT3::c_Table_1D Afferent_Feedback_State;
	olc::NT3::c_Table_1D Afferent_Feedback_State_Names;
	olc::NT3::c_Table_1D Afferent_Feedback_State_Types;

	//These tables are for the target goals, we have the precision goal, and range goal.
	//We have one set for the high priority variables such as oxygen, and one set which is a range.
	//The high priority the network will constantly fine tune as the network 'chases' a single target value.
	//The lower priority, such as for humidity, will not be constantly trying to maintain a single value, but rather keeping the variable within a range. 
	//Keeping the variable within a range should be more resource efficient as it's not constantly running the controls for it.
	olc::NT3::c_Table_1D Precision_Goal_Target; //Cell 0 holds the value.
	olc::NT3::c_Table_1D Range_Goal_Target; //Cell 0 is the low, cell 1 the high.
	
	//The Efferent States are the output to the motors and other actuators/output devices.
	olc::NT3::c_Table_1D Efferent_State;

	//This section will be for defining the settings of the homeostasis module, such as adaptive or fixed.
	int setting_Adaptive_Or_Fixed; //Adaptive (0) means the network trains on data when it reads in the current state, Fixed (1) means it is 'fixed' to the initial training.

	c_Homeostasis()
	{
		Chrono_Depth = 3;
		for (int cou_Chrono = 0; cou_Chrono < Chrono_Depth; cou_Chrono++)
		{
			Chrono_Array[cou_Chrono] = NULL;
		}

		Raw_Feedback = NULL;
		Raw_Goal = NULL;

		Raw_Goal_Count = 0;
		Raw_Feedback_Count = 0;

		MSC.init(&Empty_Boi);
		Chrono.init(&Empty_Boi);

		MSC.set_Name("MSC");
		Chrono.set_Name("Chrono");

		setting_Adaptive_Or_Fixed = 0; //Default to adaptive module.
	}

	//Goal states are associated with an input index on the Afferent Goal State table.
	//Each registered state has a raw allocated & associated with it.
	//These have the delta computed and entered with the concrete state then read into the raw.
	//You can name them later.
	void register_Goal_States(int p_Count)
	{
		Raw_Goal_Count = p_Count;
		Raw_Goal = new olc::NT3::c_NT3_Construct_1D[Raw_Goal_Count];

		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			std::string tmp_Name = "Raw_Goal.";
			tmp_Name += (char(cou_Raw + 44));
			Raw_Goal[cou_Raw].init(&Empty_Boi); //Going to strip the OLC stuff out but for now this works.
			Raw_Goal[cou_Raw].set_Name(tmp_Name);
			tmp_Name = "Goal_State_";
			tmp_Name += char(cou_Raw + 48);
			Afferent_Goal_State_Names.set_string(cou_Raw, 0, tmp_Name);
			Afferent_Goal_State_Types.set_int(cou_Raw, 0, 1); //Default to float for now.
		}
	}

	//Each Feedback State is associated with an index on the Feedback State table and then read into a Raw.
	void register_Feedback_States(int p_Count)
	{
		Raw_Feedback_Count = p_Count;
		Raw_Feedback = new olc::NT3::c_NT3_Construct_1D[Raw_Feedback_Count];

		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			std::string tmp_Name = "Raw_Feedback." + (char(cou_Raw + 44));
			Raw_Feedback[cou_Raw].init(&Empty_Boi); //Going to strip the OLC stuff out but for now this works.
			Raw_Feedback[cou_Raw].set_Name(tmp_Name);
			tmp_Name = "Feedback_State_" + (char(cou_Raw + 44));
			Afferent_Feedback_State_Names.set_string(cou_Raw, 0, tmp_Name);
			Afferent_Feedback_State_Types.set_int(cou_Raw, 0, 1);
		}
	}

	void rename_Goal_State(int p_Index, std::string p_Name)
	{
		Afferent_Goal_State_Names.set_string(p_Index, 0, p_Name);
		Raw_Goal[p_Index].set_Name(p_Name);
	}

	void rename_Feedback_State(int p_Index, std::string p_Name)
	{
		Afferent_Feedback_State_Names.set_string(p_Index, 0, p_Name);
		Raw_Feedback[p_Index].set_Name(p_Name);
	}

	//0: int, 1: float, 2: string
	void set_Goal_State_Type(int p_Index, int p_Type)
	{
		Afferent_Goal_State_Types.set_int(p_Index, 0, p_Type);
	}

	//0: int, 1: float, 2: string
	void set_Feedback_State_Type(int p_Index, int p_Type)
	{
		Afferent_Feedback_State_Types.set_int(p_Index, 0, p_Type);
	}

	//Sets the target high for a given goal.
	void set_Precision_Goal_Target(int p_Index, float p_Target_High)
	{
		Precision_Goal_Target.set_float(p_Index, 0, p_Target_High);
	}

	//Sets the target low for a given goal.
	void set_Range_Goal_Target(int p_Index, float p_Target_Low, float p_Target_High)
	{
		Range_Goal_Target.set_float(p_Index, 0, p_Target_Low);
		Range_Goal_Target.set_float(p_Index, 1, p_Target_High);
	}

	void calculate_Delta_Int(int p_Index)
	{
		int tmp_State = Afferent_Goal_State.get_int(p_Index, 0);
		int tmp_State_Prev = Afferent_Goal_State_Previous.get_int(p_Index, 0);
		int tmp_Delta = 0;

		if (tmp_State < tmp_State_Prev) { tmp_Delta = 0; }
		if (tmp_State == tmp_State_Prev) { tmp_Delta = 1; }
		if (tmp_State > tmp_State_Prev) { tmp_Delta = 2; }

		std::cout << "\n << State: " << tmp_State << " State_Prev: " << tmp_State_Prev << " Delta: " << tmp_Delta;

		Afferent_Goal_State_Delta.set_int(p_Index, 0, tmp_Delta);

		Afferent_Goal_State_Previous.set_int(p_Index, 0, Afferent_Goal_State.get_int(p_Index, 0));
	}

	void calculate_Delta_Float(int p_Index)
	{
		float tmp_State = Afferent_Goal_State.get_float(p_Index, 0);
		float tmp_State_Prev = Afferent_Goal_State_Previous.get_float(p_Index, 0);
		int tmp_Delta = 0;

		if (tmp_State < tmp_State_Prev) { tmp_Delta = 0; }
		if (tmp_State == tmp_State_Prev) { tmp_Delta = 1; }
		if (tmp_State > tmp_State_Prev) { tmp_Delta = 2; }

		std::cout << "\n << State: " << tmp_State << " State_Prev: " << tmp_State_Prev << " Delta: " << tmp_Delta;

		Afferent_Goal_State_Delta.set_int(p_Index, 0, tmp_Delta);

		Afferent_Goal_State_Previous.set_float(p_Index, 0, Afferent_Goal_State.get_float(p_Index, 0));
	}

	void calculate_Delta(int p_Index)
	{
		if (Afferent_Goal_State_Types.get_int(p_Index, 0) == 0) { calculate_Delta_Int(p_Index); }
		if (Afferent_Goal_State_Types.get_int(p_Index, 0) == 1) { calculate_Delta_Float(p_Index); }
	}

	void clear_Nets()
	{
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			Raw_Goal[cou_Raw].reset_Inputs();
		}
		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			Raw_Feedback[cou_Raw].reset_Inputs();
		}
	}

	void read_Data_Into_Nets()
	{
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			if (Afferent_Goal_State_Types.get_int(cou_Raw, 0) == 0)
			{
				int tmp_Goal[2] = { Afferent_Goal_State.get_int(cou_Raw, 0), Afferent_Goal_State_Delta.get_int(cou_Raw, 0) };
				Raw_Goal[cou_Raw].ina_A(0, 0, tmp_Goal, 2);
			}
			if (Afferent_Goal_State_Types.get_int(cou_Raw, 0) == 1)
			{
				double tmp_Goal[2] = { Afferent_Goal_State.get_float(cou_Raw, 0), float(Afferent_Goal_State_Delta.get_int(cou_Raw, 0)) };
				Raw_Goal[cou_Raw].ina_A(0, 0, tmp_Goal, 2);
			}
		}
		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			if (Afferent_Feedback_State_Types.get_int(cou_Raw, 0) == 0)
			{
				Raw_Feedback[cou_Raw].in(0, 0, Afferent_Feedback_State.get_int(cou_Raw, 0));
			}
			if (Afferent_Feedback_State_Types.get_int(cou_Raw, 0) == 1)
			{
				Raw_Feedback[cou_Raw].in(0, 0, Afferent_Feedback_State.get_float(cou_Raw, 0));
			}
		}
	}

	//Builds the raw tier networks.
	void build_Raw()
	{
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			std::cout << "\n\n__________________________________________________";
			std::cout << "\n " << Afferent_Goal_State_Names.get_string(cou_Raw, 0);
			Raw_Goal[cou_Raw].output_Input_Table_F();
			Raw_Goal[cou_Raw].Build_RC(1);
			Raw_Goal[cou_Raw].output_CAN();
			Raw_Goal[cou_Raw].output_CAN_State();
		}
		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			std::cout << "\n\n__________________________________________________";
			std::cout << "\n " << Afferent_Feedback_State_Names.get_string(cou_Raw, 0);
			Raw_Feedback[cou_Raw].output_Input_Table_F();
			Raw_Feedback[cou_Raw].Build_RC();
			Raw_Feedback[cou_Raw].output_CAN();
			Raw_Feedback[cou_Raw].output_CAN_State();
		}
	}

	void build_Raw_Nets()
	{
		build_Raw();
	}

	void read_Raw_Treetops_Into_MSC()
	{
		//Clear the current MSC.
		MSC.reset_Inputs();

		//Read in the first Goal raw treetops
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			MSC.ina(0, 0, Raw_Goal[cou_Raw].get_Current_Treetop());
		}

		//Start where goal raw left off
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			MSC.ina(0, 0, Raw_Feedback[cou_Raw].get_Current_Treetop());
		}
	}

	void increment_Chrono()
	{
		for (int cou_Chrono = 0; cou_Chrono < (Chrono_Depth - 1); cou_Chrono++)
		{
			Chrono_Array[cou_Chrono] = Chrono_Array[cou_Chrono + 1];
		}
		Chrono_Array[Chrono_Depth - 1] = MSC.get_Current_Treetop();
	}

	void read_Chrono_Array_Into_Chrono()
	{
		Chrono.reset_Inputs();

		Chrono.ina_A(0, 0, Chrono_Array, Chrono_Depth);
	}

	void train()
	{

		//Build the raw nets on the lowest level.
		build_Raw_Nets();

		//Read in the Treetops from the raw CAN scaffolds to the MSC input.
		read_Raw_Treetops_Into_MSC();

		//Build the MSC
		MSC.Build_RC(1);

		MSC.output_Input_Table_NR();
		MSC.output_CAN();
		MSC.output_CAN_State();

		//Increment the Chrono array
		increment_Chrono();

		//Reset input, read in the current Chrono, and Build
		read_Chrono_Array_Into_Chrono();
		Chrono.Build_RC(1);

		std::cout << "\n\n CHRONO: ";

		Chrono.output_Input_Table_NR();
		Chrono.output_CAN();
		Chrono.output_CAN_State();
	}

	//Takes the current interface input, calculates the delta for the goal-seekers, then reads the data into the NNet Raw
	void gather_Current_Input()
	{
		//Calculate Delta
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			calculate_Delta(cou_Raw);
		}

		//Prep networks for new input set.
		clear_Nets();

		//Read in Afferent & Delta
		read_Data_Into_Nets();
	}

	//This is for training the network.
	void update_Train()
	{
		train();
	}

	void find_Target_Delta()
	{

	}

	//Gathers the changes needed to bring the system in line with the goal states.
	void compile_Delta_Targets()
	{
		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			find_Target_Delta();
		}
	}

	//This generates predictions and selects the traces putting them into the output array.
	void update_Predict()
	{
		//Determine which goals states are out of bounds and need corrected based upon the target values and current states
		compile_Delta_Targets();

		//The delta changes needed for each goal are calculated, those that are within ranges are entered as (1). 

		//Enter the delta changes desired into the raw networks, enter the current states as well

		//Charge the raw constructs

		//Gather the Raw treetop nodes & their charges

		//Charge the Multi-Sensory-Construct (MSC) with these nodes & their charges

		//Gather the MSC treetops

		//Charge (Right leg only) the Chronological-Construct (Chrono) with the MSC treetops & charges

		//Gather the resultant MSC treetops

		//Gather the state quanta patterns represented by these MSC treetops

		//Find those that have the most optimal delta changes in relation to the goal deltas desired, weigh them by reinforcement counter (RC)

		//From these do a weighted selection of the Feedback_States to determine the Efferent output to the systems actuators
	}

	//Update behavior is determined by p_How, 0: Train, 1: Predict, though predict can train as well if the setting is on.
	void update(int p_How = 0)
	{
		//The current input & deltas are gathered regardless.
		gather_Current_Input();

		if (p_How == 0)
		{
			update_Train();
		}

		if (p_How == 1)
		{
			if (setting_Adaptive_Or_Fixed == 0)
			{
				update_Train();
			}

			update_Predict();
		}
	}

	void output()
	{
		std::cout << "\n\nGoal States:\n";
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			std::cout << "\n\n/___-___-___-___-___\\";
			std::cout << "\n---Goal State[" << cou_Raw << "]: " << Afferent_Goal_State_Names.get_string(cou_Raw, 0);
			std::cout << "\n Interface:";
			std::cout << " -Goal State: ";
			std::cout << Afferent_Goal_State.get_float(cou_Raw, 0);
			std::cout << " -Type: ";
			std::cout << Afferent_Goal_State_Types.get_int(cou_Raw, 0);
			//Afferent_Goal_State.output_Row(cou_Raw, -1);
			std::cout << " -Goal State Delta: ";
			std::cout << Afferent_Goal_State_Delta.get_int(cou_Raw, 0);
			Raw_Goal[cou_Raw].output_Input_Table_F();
			std::cout << "\n Nodes: ";
			Raw_Goal[cou_Raw].output_Nodes_Stats();
			std::cout << "\n\\---_---_---_---_---/";
		}

		std::cout << "\n\nFeedback States:\n";
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			std::cout << "\n\n/___-___-___-___-___\\";
			std::cout << "\n---Feedback State[" << cou_Raw << "]: " << Afferent_Feedback_State_Names.get_string(cou_Raw, 0);
			std::cout << "\n Interface:";
			std::cout << " -Feedback State: ";
			std::cout << Afferent_Feedback_State.get_float(cou_Raw, 0);
			std::cout << " -Type: ";
			std::cout << Afferent_Feedback_State_Types.get_int(cou_Raw, 0);
			Raw_Goal[cou_Raw].output_Input_Table_F();
			std::cout << "\n Nodes: ";
			Raw_Feedback[cou_Raw].output_Nodes_Stats();
			std::cout << "\n\\---_---_---_---_---/";
		}

		std::cout << "\nMSC:\n";
		MSC.output_Nodes();

		std::cout << "\nChrono:\n";
		Chrono.output_Nodes();
	}
};


int main()
{
	olc::NT3::init_LoTd();

	//Declare the homeostasis module
	c_Homeostasis testerman;

	//Register two goal states, can register more but the test data only has 2
	testerman.register_Goal_States(2);

	//Name the goal states.
	testerman.rename_Goal_State(0, "O2");
	testerman.rename_Goal_State(1, "Temperature");

	//Set the data type of the goal states. 0: int, 1: float
	//The first argument is the index, corresponds to the goal state index.
	testerman.set_Goal_State_Type(0, 1);
	testerman.set_Goal_State_Type(1, 1);

	//Set the ranges of target values for the goal network to seek.
	testerman.set_Precision_Goal_Target(0, 5.0); //The O2 is considered a 'priority' variable needed increased precision and maintanance.
	testerman.set_Range_Goal_Target(1, 7.0, 9.0); //The temperature is considered a lower priority and we don't want the heater kicking on and off constantly if the temp is somewhat stable around the goal value.

	//Register and name the two feedback states for the current dataset.
	testerman.register_Feedback_States(2);
	testerman.rename_Feedback_State(0, "Heater");
	testerman.rename_Feedback_State(1, "Oxygen_Pump");
	
	//Now set the data types of the feedback states.
	testerman.set_Feedback_State_Type(0, 1);
	testerman.set_Feedback_State_Type(1, 1);


	std::string tmp_String = "";

	c_Sim Simboi; //A basic simulation to generate a test dataset.

	while (1)
	{
		system("CLS");

		testerman.update();
		testerman.output();

		Simboi.output();

		//These read the data into the afferent interface. The deltas for the goal states are calculated internally. The Feedback is read into the trace so that it can be retrieved later.

		//O2, Simboi is just a test simulation to generate data for this development, the actual data from the systems will be read in instead.
		testerman.Afferent_Goal_State.set_float(0, 0, float(Simboi.O2));

		//Temperature
		testerman.Afferent_Goal_State.set_float(1, 0, float(Simboi.Temp));

		//O2_Pump
		testerman.Afferent_Feedback_State.set_float(0, 0, float(Simboi.O2_Pump));

		//Heater
		testerman.Afferent_Feedback_State.set_float(1, 0, float(Simboi.Heater));

		Simboi.iterate();
		olc::NT3::xy(0, 0);
		system("PAUSE > NULL");
	}


	return 0;
}