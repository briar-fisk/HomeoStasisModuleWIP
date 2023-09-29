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



void output_Line_Graph(int p_X, int p_Y, int p_Depth, int p_Height)
{
	for (int cou_Height = 0; cou_Height < p_Height; cou_Height++)
	{
		olc::NT3::xy(p_X, (p_Y + cou_Height));

		for (int cou_Depth = 0; cou_Depth < p_Depth; cou_Depth++)
		{

		}
	}
}





//06 31:25 Space Arcologies

//Using tables to handle all data is overkill and wasteful, will not be that way in the full implementation. Vectors would probably do the same job but I've never used them.
//Concrete vs delta, when a value is referred to as 'concrete' it means the scalar value input into the interface, delta is an abstraction of the change of the values.
//Concrete thermal might be 73 degrees, whereas delta thermal might be '2.0' for 'rising'.
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

	//Afferent Feedback is the current state of things such as motors and other actuators, whether they are on, off, etc.
	//This is needed in the afferent side so the output includes it.
	olc::NT3::c_Table_1D Afferent_Feedback_State;
	olc::NT3::c_Table_1D Afferent_Feedback_State_Names;

	//For precision goals such as oxygen we reduce the range to a smaller value, for more lax values such as humidity we can open the range up a bit.
	//The high priority the network will constantly fine tune as the network 'chases' a single target value.
	//The lower priority, such as for humidity, will not be constantly trying to maintain a single value, but rather keeping the variable within a range. 
	//Keeping the variable within a range should be more resource efficient as it's not constantly running the controls for it.
	//When searching for deltas that match what we need we want to input a value for the desired goal state, the specific goal target is used for this purpose. It should fall between the range of goal values.
	olc::NT3::c_Table_1D Range_Goal_Target; //Cell 0 is the low, cell 1 the high.
	olc::NT3::c_Table_1D Specific_Goal_Target; //This is read in as the specific goal to target, when searching for desired deltas we need a single value to input as the goal, this will be that value.
	olc::NT3::c_Table_1D Goal_Target_Deltas; //The resultant deltas gathered.

	//The Efferent States are the output to the motors and other actuators/output devices.
	olc::NT3::c_Table_1D Efferent_State;

	//This table holds the output traces from the Chrono discharging for further selection into the final efferent prediction table
	olc::NT3::c_Raw_Table_1D Efferent_Prediction_Traces;
	olc::NT3::c_Raw_Table_1D Efferent_Prediction;

	//This section will be for defining the settings of the homeostasis module, such as adaptive or fixed.
	int setting_Adaptive_Or_Fixed; //Adaptive (0) means the network trains on data when it reads in the current state, Fixed (1) means it is 'fixed' to the initial training.

	float Trace_Filter_BG_Ratio; //This is the threshold for trace selection, above this threshold and the trace is selected for output.

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

		MSC.settings_Action_Potential_Threshold = 0.0f; //Allow all charges to pass through the MSC
		Chrono.settings_Action_Potential_Threshold = 0.0f; //Allow all charges to pass through the Chrono
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

	//Sets the target low for a given goal.
	void set_Range_Goal_Target(int p_Index, float p_Target_Low, float p_Target_High)
	{
		Range_Goal_Target.set_float(p_Index, 0, p_Target_Low);
		Range_Goal_Target.set_float(p_Index, 1, p_Target_High);
	}

	//Sets the specific target for the given goal, should fall between the range given in the set_Range_Goal_Target()
	void set_Specific_Goal_Target(int p_Index, float p_Target_Specific)
	{
		Specific_Goal_Target.set_float(p_Index, 0, p_Target_Specific);
	}

	//This is used to calculate the delta for the goal variables, decouples the movement of the signal from the concrete value.
	void calculate_Delta(int p_Index)
	{
		float tmp_State = Afferent_Goal_State.get_float(p_Index, 0);
		float tmp_State_Prev = Afferent_Goal_State_Previous.get_float(p_Index, 0);
		float tmp_Delta = 0;

		if (tmp_State < tmp_State_Prev) { tmp_Delta = 0.0; }
		if (tmp_State == tmp_State_Prev) { tmp_Delta = 1.0; }
		if (tmp_State > tmp_State_Prev) { tmp_Delta = 2.0; }

		std::cout << "\n << State: " << tmp_State << " State_Prev: " << tmp_State_Prev << " Delta: " << tmp_Delta;

		Afferent_Goal_State_Delta.set_float(p_Index, 0, tmp_Delta);

		Afferent_Goal_State_Previous.set_float(p_Index, 0, Afferent_Goal_State.get_float(p_Index, 0));
	}

	//This wipes the networks inputs. If not done the data is appended when the user reads more in.
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
			double tmp_Goal[2] = { Afferent_Goal_State.get_float(cou_Raw, 0), Afferent_Goal_State_Delta.get_float(cou_Raw, 0) };
			Raw_Goal[cou_Raw].ina_A(0, 0, tmp_Goal, 2);
		}
		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			//This is kind of hacky but it will work, for some reason the fractal treetop tree in Construct.Nodes isn't finding the treetops on tier 0.
			//The engine will be rewritten so I'm using this for now and not going to mess with something that will likely be ommited in production version.
			//Just putting both indexes as the same input value to bump the tier count up to 2.
			double tmp_Feedback[2] = { Afferent_Feedback_State.get_float(cou_Raw, 0), Afferent_Feedback_State.get_float(cou_Raw, 0) };
			Raw_Feedback[cou_Raw].ina_A(0, 0, tmp_Feedback, 2);
		}
	}

	//Builds the raw tier networks.
	void build_Raw()
	{
		for (int cou_Raw = 0; cou_Raw < Raw_Goal_Count; cou_Raw++)
		{
			//std::cout << "\n\n__________________________________________________";
			//std::cout << "\n " << Afferent_Goal_State_Names.get_string(cou_Raw, 0);
			//Raw_Goal[cou_Raw].output_Input_Table_F();
			Raw_Goal[cou_Raw].Build_RC(1);
			//Raw_Goal[cou_Raw].output_CAN();
			//Raw_Goal[cou_Raw].output_CAN_State(2);
		}
		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			//std::cout << "\n\n__________________________________________________";
			//std::cout << "\n " << Afferent_Feedback_State_Names.get_string(cou_Raw, 0);
			//Raw_Feedback[cou_Raw].output_Input_Table_F();
			Raw_Feedback[cou_Raw].Build_RC(1);
			//Raw_Feedback[cou_Raw].output_CAN();
			//Raw_Feedback[cou_Raw].output_CAN_State(2);
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
			MSC.ina(0, 0, (Raw_Goal[cou_Raw].get_Current_Treetop_I() + (cou_Raw * 1000000000000000)));
			//(Raw_Goal[cou_Goal].tbl_Treetops_Output_C.Rows[0]->Cells[cou_Index]->Data[0].U + (cou_Goal * 1000000000000000)); //Salt the goals so the MSC can differentiate between Raws
		}

		//Start where goal raw left off
		for (int cou_Raw = 0; cou_Raw < Raw_Feedback_Count; cou_Raw++)
		{
			MSC.ina(0, 0, (Raw_Feedback[cou_Raw].get_Current_Treetop_I() + ((cou_Raw + Raw_Goal_Count) * 1000000000000000)));
		}

		//MSC.output_Input_Table_I();
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

		//MSC.output_Input_Table_NR();
		//MSC.output_CAN();
		//MSC.output_CAN_State();

		//Increment the Chrono array
		increment_Chrono();

		//Reset input, read in the current Chrono, and Build
		read_Chrono_Array_Into_Chrono();
		Chrono.Build_RC(1);

		//std::cout << "\n\n CHRONO: ";

		//Chrono.output_Input_Table_NR();
		//Chrono.output_Input_Table_U();
		//Chrono.output_CAN();
		//Chrono.output_CAN_State();
		
		
		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			Raw_Goal[cou_Goal].output_CAN_Col((5 + (cou_Goal * 6)), 4, 2); 
		}
		for (int cou_Feedback = 0; cou_Feedback < Raw_Feedback_Count; cou_Feedback++)
		{
			Raw_Feedback[cou_Feedback].output_CAN_Col(((Raw_Goal_Count * 6) + (cou_Feedback * 6) + 5), 4, 2);
		}
		MSC.output_CAN_Col((((Raw_Goal_Count * 6) + (Raw_Feedback_Count * 6) + 5) + 5), 4);
		Chrono.output_CAN_Col((((Raw_Goal_Count * 6) + (Raw_Feedback_Count * 6) + 5) + (Raw_Goal_Count * 2) + (Raw_Feedback_Count) + 15), 4);

		olc::NT3::xy(0, ((Raw_Goal_Count * 2) + (Raw_Feedback_Count)) + 2);

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

	//Conversly to the delta figured from the previous state we are seeking deltas needed, therefore if the current state is less than the goal it will be 2.0, if over it will be 0.0.
	void find_Delta_Target(int p_Goal)
	{
		//Range goals
		float tmp_State = Afferent_Goal_State.get_float(p_Goal, 0);
		float tmp_Target_Goal_State_Low = Range_Goal_Target.get_float(p_Goal, 0);
		float tmp_Target_Goal_State_High = Range_Goal_Target.get_float(p_Goal, 1);
		float tmp_Delta = 0;

		if (tmp_State < tmp_Target_Goal_State_Low) { tmp_Delta = 2.0; }
		if ((tmp_State > tmp_Target_Goal_State_Low) && (tmp_State < tmp_Target_Goal_State_High)) { tmp_Delta = 1.0; }
		if (tmp_State > tmp_Target_Goal_State_High) { tmp_Delta = 0.0; }

		//std::cout << "\n Goal_Index: " << p_Goal;
		//std::cout << " State: >" << tmp_State << "< tmp_Target_Goal_State: >" << tmp_Target_Goal_State_Low << "< >" << tmp_Target_Goal_State_High << "< Delta: >" << tmp_Delta << "<";

		Goal_Target_Deltas.set_float(p_Goal, 0, tmp_Delta);
	}

	//Gathers the changes needed to bring the system in line with the goal states.
	void compile_Delta_Targets()
	{
		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			find_Delta_Target(cou_Goal);
		}
	}

	//We enter the goal states, either precision or range, 
	void enter_Goals_Into_Raw()
	{
		for (int cou_Goal = 0; cou_Goal < Goal_Target_Deltas.get_Row_Count(); cou_Goal++)
		{
			//std::cout << "\n Inputting Goal Delta Index: " << cou_Goal;
			//std::cout << " " << Goal_Target_Deltas.get_float(cou_Goal, 0);

			double tmp_Goal[2] = { Specific_Goal_Target.get_float(cou_Goal, 0), Goal_Target_Deltas.get_float(cou_Goal, 0) };
			Raw_Goal[cou_Goal].ina_A(0, 0, tmp_Goal, 2);
		}
	}

	void charge_Raw()
	{
		for (int cou_Goal = 0; cou_Goal < Goal_Target_Deltas.get_Row_Count(); cou_Goal++)
		{
			Raw_Goal[cou_Goal].Eval();
			//Raw_Goal[cou_Goal].output_Input_Table_F();
			//Raw_Goal[cou_Goal].output_Output_Tables_M();
		}
	}

	void select_Raw_Treetops()
	{
		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			if (Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Number_Of_Rows > 0)
			{
				for (int cou_Index = 0; cou_Index < Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[0]->Depth; cou_Index++)
				{
					if (Raw_Goal[cou_Goal].tbl_Pattern_Output_C.get_F(0, cou_Index, 1) == Goal_Target_Deltas.get_float(cou_Goal, 0, 0))
					{
						//std::cout << "\n Goal[" << cou_Goal << "] Delta_Target: " << Goal_Target_Deltas.get_float(cou_Goal, 0, 0);
						//std::cout << "\n   -";
						//Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[0]->Cells[cou_Index]->output_F();
						//std::cout << "\n   -";
						//std::cout << (Raw_Goal[cou_Goal].tbl_Treetops_Output_C.Rows[0]->Cells[cou_Index]->Data[0].U + (cou_Goal * 1000000000000000)); //Salt the goals so the MSC can differentiate between Raws
						MSC.ina(0, 0, (Raw_Goal[cou_Goal].tbl_Treetops_Output_C.Rows[0]->Cells[cou_Index]->Data[0].U + (cou_Goal * 1000000000000000)));
					}
				}
			}
		}
	}

	void gather_MSC_Treetops()
	{
		if (MSC.tbl_Treetops_Output_C.Number_Of_Rows > 0)
		{
			for (int cou_T = 0; cou_T < MSC.tbl_Treetops_Output_C.Rows[0]->Depth; cou_T++)
			{
				Chrono.ina(0, 0, MSC.tbl_Treetops_Output_C.Rows[0]->Cells[cou_T]->Data[0].U);
			}
			//Chrono.output_Input_Table_U();
		}
	}

	//Charges the chrono on the right leg only causing it to seek those that are previous input traces based on the current input set.
	void charge_Chrono_Seek_Past()
	{
		Chrono.setting_Charge_L = 0;//When charging Chrono charges backwards.
		Chrono.setting_Charge_R = 1;
		Chrono.Eval();
		//Chrono.output_Output_Tables_MSC();
	}

	// Efferent_Prediction_Traces
	// Loop through each Chrono
	// For each Chrono take the states and read them separately through the MSC
	//   For each the MSC & Raw are cleared.
	//   Then starting with the [0] index chrono discharge through the MSC, and Raw, into the Efferent_Prediction_Traces
	//   Each trace is stored in a seperate cell

	//-- == Chrono___Pattern_Output_C == --
	//	Number_Of_Rows->1
	//	--[0]{ 7 } {0} 79 85 40 {1} 72 79 85 {2} 66 72 79 {3} 85 40 45 {4} 40 45 52 {5} 34 40 45 {6} 27 34 40


	/*
	--== MSC___Pattern_Output_C ==--
		Number_Of_Rows->7
		--[0] {3}  {0} 7 1000000000000018 2000000000000001 3000000000000000 {1} 9 1000000000000019 2000000000000001 3000000000000001 {2} 11 1000000000000009 2000000000000000 3000000000000001
		--[1] {3}  {0} 5 1000000000000017 2000000000000000 3000000000000000 {1} 7 1000000000000018 2000000000000001 3000000000000000 {2} 9 1000000000000019 2000000000000001 3000000000000001
		--[2] {3}  {0} 17 1000000000000015 2000000000000000 3000000000000001 {1} 5 1000000000000017 2000000000000000 3000000000000000 {2} 7 1000000000000018 2000000000000001 3000000000000000
		--[3] {3}  {0} 9 1000000000000019 2000000000000001 3000000000000001 {1} 11 1000000000000009 2000000000000000 3000000000000001 {2} 13 1000000000000002 2000000000000000 3000000000000001
		--[4] {3}  {0} 11 1000000000000009 2000000000000000 3000000000000001 {1} 13 1000000000000002 2000000000000000 3000000000000001 {2} 15 1000000000000011 2000000000000000 3000000000000001
		--[5] {3}  {0} 9 1000000000000009 2000000000000001 3000000000000001 {1} 11 1000000000000009 2000000000000000 3000000000000001 {2} 13 1000000000000002 2000000000000000 3000000000000001
		--[6] {3}  {0} 7 1000000000000007 2000000000000001 3000000000000001 {1} 9 1000000000000009 2000000000000001 3000000000000001 {2} 11 1000000000000009 2000000000000000 3000000000000001
		Each row is a separate chrono output. Each cell is a separate trace within that chrono.

		So we need to iterate through each cell and gather the goal & feedback states for that cell into the prediction traces table.

		Each of the prediction traces will be output in the cells they appear in above.
	*/

	/*
	
	--== O2___Pattern_Output_C ==--
		Number_Of_Rows->7
		--[0] {3}  {0} 3 0 {1} 6 2 {2} 9 2
		--[1] {3}  {0} 4 0 {1} 3 0 {2} 6 2
		--[2] {3}  {0} 5 0 {1} 4 0 {2} 3 0
		--[3] {3}  {0} 6 2 {1} 9 2 {2} 8 0
		--[4] {3}  {0} 9 2 {1} 8 0 {2} 7 0
		--[5] {3}  {0} 6 2 {1} 9 2 {2} 8 0
		--[6] {3}  {0} 3 0 {1} 6 2 {2} 9 2

	--== Temperature___Pattern_Output_C ==--
		Number_Of_Rows->7
		--[0] {3}  {0} 7 0 {1} 4 0 {2} 4 1
		--[1] {3}  {0} 9 2 {1} 7 0 {2} 4 0
		--[2] {3}  {0} 8 2 {1} 9 2 {2} 7 0
		--[3] {3}  {0} 4 0 {1} 4 1 {2} 5 2
		--[4] {3}  {0} 4 1 {1} 5 2 {2} 6 2
		--[5] {3}  {0} 4 1 {1} 4 1 {2} 5 2
		--[6] {3}  {0} 4 2 {1} 4 1 {2} 4 1

	--== Oxygen_Pump___Pattern_Output_C ==--
		Number_Of_Rows->7
		--[0] {3}  {0} 1 1 {1} 1 1 {2} 0 0
		--[1] {3}  {0} 0 0 {1} 1 1 {2} 1 1
		--[2] {3}  {0} 0 0 {1} 0 0 {2} 1 1
		--[3] {3}  {0} 1 1 {1} 0 0 {2} 0 0
		--[4] {3}  {0} 0 0 {1} 0 0 {2} 0 0
		--[5] {3}  {0} 1 1 {1} 0 0 {2} 0 0
		--[6] {3}  {0} 1 1 {1} 1 1 {2} 0 0

	--== Heater___Pattern_Output_C ==--
		Number_Of_Rows->7
		--[0] {3}  {0} 0 0 {1} 1 1 {2} 1 1
		--[1] {3}  {0} 0 0 {1} 0 0 {2} 1 1
		--[2] {3}  {0} 1 1 {1} 0 0 {2} 0 0
		--[3] {3}  {0} 1 1 {1} 1 1 {2} 1 1
		--[4] {3}  {0} 1 1 {1} 1 1 {2} 1 1
		--[5] {3}  {0} 1 1 {1} 1 1 {2} 1 1
		--[6] {3}  {0} 1 1 {1} 1 1 {2} 1 1

	Now to go through each one, select the traces from the goal ones that match the desired delta, then use those to select the actuator output.
	*/

	//Outputs the tables to check things after discharging.
	void output_BPOTables()
	{

		MSC.output_Output_Tables_MSC();

		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			Raw_Goal[cou_Goal].output_Output_Tables_F();
		}

		for (int cou_Feedback = 0; cou_Feedback < Raw_Feedback_Count; cou_Feedback++)
		{
			Raw_Feedback[cou_Feedback].output_Output_Tables_F();
		}
	}

	//Gathers the trace matches to the goals.
	void rate_Traces()
	{
		Efferent_Prediction_Traces.reset();

		//For every trace, select the ones that match the desired delta.
		for (int cou_Row = 0; cou_Row < (MSC.tbl_Pattern_Output_C.Number_Of_Rows); cou_Row++)
		{
			//If the trace contains relevant delta then flag it for capture.
			float flg_Good_Trace = 0;
			float flg_Bad_Trace = 0;

			//By starting the cou_Cell on 1 instead of 0 we skip the first chrono input as we don't know the previous states, the states of the chrono will effect 1, but 0 is undefined.
			for (int cou_Cell = 1; cou_Cell < MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth; cou_Cell++)
			{
				//For each cell check the two goals against the deltas returned to see if they match.
				for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
				{
					if (Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[1].F == Goal_Target_Deltas.get_float(cou_Goal, 0))
					{
						//std::cout << "\n\n Row[" << cou_Row << "] Cell[" << cou_Cell << "] Goal: [" << cou_Goal << "] ";
						//std::cout << " " << Goal_Target_Deltas.get_float(cou_Goal, 0);
							//std::cout << "\n          -" << Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[0].F;
							//std::cout << "\n          -" << Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[1].F;
							//Raw_Goal[cou_Goal].output_Output_Tables_F();
						flg_Good_Trace++;
					}

					if ((Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[1].F != Goal_Target_Deltas.get_float(cou_Goal, 0)) && (Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[1].F != 1))
					{
						flg_Bad_Trace++;
					}
				}
			}

			if (flg_Good_Trace > 0)
			{
				std::cout << "\n < ";
				olc::NT3::oindex(cou_Row);
				std::cout << " > G: ";
				olc::NT3::oint(0, (flg_Good_Trace + 1), flg_Good_Trace);
				std::cout << " / " << (Raw_Goal_Count * MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth);
				std::cout << " | B: ";
				olc::NT3::oint(0, (flg_Bad_Trace + 1), flg_Bad_Trace);
				std::cout << " / " << (Raw_Goal_Count * MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth);
				std::cout << " Trace: ";


				for (int cou_Cell = 0; cou_Cell < MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth; cou_Cell++)
				{
					olc::NT3::ostr(0, 13, " [{<- ");
					olc::NT3::ostr(0, 14, " G[ ");
					for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
					{
						std::cout << " ";
						olc::NT3::ostr(0, 7, Afferent_Goal_State_Names.get_string(cou_Goal, 0));
						std::cout << " " << Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[0].F;
						std::cout << " " << Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[1].F;

						Efferent_Prediction_Traces.add_Data_Float(cou_Row, cou_Cell, Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[0].F);
						Efferent_Prediction_Traces.add_Data_Float(cou_Row, cou_Cell, Raw_Goal[cou_Goal].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[1].F);
					}
					olc::NT3::ostr(0, 14, " ] ");
					olc::NT3::ostr(0, 12, " F[ ");
					for (int cou_Feedback = 0; cou_Feedback < Raw_Feedback_Count; cou_Feedback++)
					{
						std::cout << " ";
						olc::NT3::ostr(0, 8, Afferent_Feedback_State_Names.get_string(cou_Feedback, 0));
						std::cout << " " << Raw_Feedback[cou_Feedback].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[0].F;
						Efferent_Prediction_Traces.add_Data_Float(cou_Row, cou_Cell, Raw_Feedback[cou_Feedback].tbl_Pattern_Output_C.Rows[cou_Row]->Cells[cou_Cell]->Data[0].F);
					}
					olc::NT3::ostr(0, 12, " ] ");
					olc::NT3::ostr(0, 13, " ->}] ");
				}

				float flg_BG_Ratio = float(int((flg_Good_Trace / (flg_Good_Trace + flg_Bad_Trace)) * 100) / 100.0);
				if (flg_Bad_Trace == 0) { flg_BG_Ratio = 1.00; }

				//We put the rating at the end, this allows for extending the ratings while keeping the pattern at cell 0.
				Efferent_Prediction_Traces.add_Data_Float(cou_Row, (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth), flg_Good_Trace);
				Efferent_Prediction_Traces.add_Data_Float(cou_Row, (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth + 1), flg_Bad_Trace);
				Efferent_Prediction_Traces.add_Data_Float(cou_Row, (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth + 2), flg_BG_Ratio);
				flg_BG_Ratio = flg_BG_Ratio * (flg_Good_Trace + flg_Bad_Trace);
				Efferent_Prediction_Traces.add_Data_Float(cou_Row, (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth + 3), flg_BG_Ratio);
				flg_BG_Ratio = float(int((flg_BG_Ratio / (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth * 2)) * 100) / 100.0);
				Efferent_Prediction_Traces.add_Data_Float(cou_Row, (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth + 4), flg_BG_Ratio);


				for (int cou_Feedback = 0; cou_Feedback < Raw_Feedback_Count; cou_Feedback++)
				{
					//Average the efferent feedback states.
					float tmp_Avg = 0.0;
					float tmp_Avg_Total = 0.0;
					float tmp_Count = 0.0;

					for (int cou_Cell = 0; cou_Cell < MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth; cou_Cell++)
					{
						tmp_Avg_Total += Efferent_Prediction_Traces.get_F(cou_Row, cou_Cell, ((cou_Feedback) + (Raw_Goal_Count * 2)));
						tmp_Count++;
					}

					tmp_Avg = tmp_Avg_Total / tmp_Count;

					Efferent_Prediction_Traces.add_Data_Float(cou_Row, (MSC.tbl_Pattern_Output_C.Rows[cou_Row]->Depth + 5), tmp_Avg);
				}
			}
		}

		Efferent_Prediction_Traces.output_F();

		//Output the goals.
		olc::NT3::ostr(0, 13, "\n\n Goals: ");
		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			std::cout << " ";
			olc::NT3::ostr(0, 7, Afferent_Goal_State_Names.get_string(cou_Goal, 0));
			std::cout << " " << Goal_Target_Deltas.get_float(cou_Goal, 0);
		}
	}

	//Go through the returned traces and select the appropriate ones.
	//First finds out how well each trace matches the goal deltas
	//Then selects the top n traces
	//After that it gathers the feedback states from these
	//Then uses the feedback states to create the efferent actuator states
	void sort_Traces()
	{
		//output_BPOTables();
		rate_Traces();
	}

	//Discharges the raw treetops from MSC into Raw
	//In this case the p_Row & p_Cell point to the MSC output to gather from.
	void discharge_Raw_Goal(int p_Row, int p_Cell)
	{
		//The raw goal count is used as that is how many of the returned MSC states are the treetops to the raw.
		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			//std::cout << "\n" << MSC.tbl_Pattern_Output_C.Rows[p_Row]->Cells[p_Cell]->Data[cou_Goal].U;
			
			olc::NT3::u_Data_3 tmp_NID;
			tmp_NID.U = MSC.tbl_Pattern_Output_C.Rows[p_Row]->Cells[p_Cell]->Data[cou_Goal].U - (cou_Goal * 1000000000000000); //Sorry salt, no longer needed.

			Raw_Goal[cou_Goal].discharge_Given_Treetop(p_Row, p_Cell, tmp_NID, 100);
			//Raw_Goal[cou_Goal].output_Output_Tables_F();
		}
	}

	//Discharges the raw treetops from MSC into Raw
	//In this case the p_Row & p_Cell point to the MSC output to gather from.
	//This is the goal, the gold, the milk, the meat n taters of the prediction.
	void discharge_Raw_Feedback(int p_Row, int p_Cell)
	{
		//The raw goal count is used as that is how many of the returned MSC states are the treetops to the raw.
		for (int cou_Feedback = Raw_Goal_Count ; cou_Feedback < (Raw_Feedback_Count + Raw_Goal_Count); cou_Feedback++)
		{
			//std::cout << "\n" << MSC.tbl_Pattern_Output_C.Rows[p_Row]->Cells[p_Cell]->Data[cou_Feedback].U;
			
			olc::NT3::u_Data_3 tmp_NID;
			//((cou_Raw + Raw_Goal_Count) * 1000000000000000)
			tmp_NID.U = MSC.tbl_Pattern_Output_C.Rows[p_Row]->Cells[p_Cell]->Data[cou_Feedback].U - (cou_Feedback * 1000000000000000); //Sorry salt, no longer needed.
			//std::cout << " unsalted: " << tmp_NID.U;
			Raw_Feedback[cou_Feedback - Raw_Goal_Count].discharge_Given_Treetop(p_Row, p_Cell, tmp_NID, 100);
			//Raw_Feedback[cou_Feedback - Raw_Goal_Count].output_Output_Tables_F();
		}
	}

	//Discharges one treetop and collect the output.
	void discharge_MSC_Treetop(int p_Row, int p_Cell, uint64_t p_NID, uint64_t p_Charge)
	{
		olc::NT3::u_Data_3 tmp_Node;
		tmp_Node.U = p_NID;

		//std::cout << "\n\n " << p_Row << " " << p_Cell << " " << p_NID << "\n\n";

		//Discharge the MSC into the given output row and cell.
		MSC.discharge_Given_Treetop(p_Row, p_Cell, tmp_Node, p_Charge);

		//MSC.output_Output_Tables_MSC();

		discharge_Raw_Goal(p_Row, p_Cell); //In this case the p_Row & p_Cell point to the MSC output to gather from.
		discharge_Raw_Feedback(p_Row, p_Cell); //In this case the p_Row & p_Cell point to the MSC output to gather from.
	}

	//Discharges and collects one set of Chrono outputs.
	//p_Chrono identifies the Chrono cell to discharge as well as the row to put them in.
	void discharge_Chrono(int p_Chrono)
	{
		for (int cou_Index = 0; cou_Index < (Chrono.tbl_Pattern_Output_C.Rows[0]->Cells[p_Chrono]->Length); cou_Index++)
		{
			discharge_MSC_Treetop(p_Chrono, cou_Index, Chrono.tbl_Pattern_Output_C.Rows[0]->Cells[p_Chrono]->Data[cou_Index].U, Chrono.tbl_Charge_Output_C.Rows[0]->Cells[p_Chrono]->Data[cou_Index].U);
		}
	}

	//Reset all the IO
	void reset_All_IO()
	{
		MSC.reset_IO();

		for (int cou_Goal = 0; cou_Goal < Raw_Goal_Count; cou_Goal++)
		{
			Raw_Goal[cou_Goal].reset_IO();
		}
		for (int cou_Feedback = 0; cou_Feedback < Raw_Feedback_Count; cou_Feedback++)
		{
			Raw_Feedback[cou_Feedback].reset_IO();
		}
	}

	//Gathers the outputs from the Chrono and discharges them into the final output trace table for selection.
	//First we need to go through every chrono trace collected, then loop through each of the indexes in the chrono getting the MSC/Raw/State outputs.
	void discharge_Chrono()
	{
		reset_All_IO();

		if (Chrono.tbl_Pattern_Output_C.Number_Of_Rows > 0)
		{
			for (int cou_Chrono = 0; cou_Chrono < (Chrono.tbl_Pattern_Output_C.Rows[0]->Depth); cou_Chrono++)
			{
				discharge_Chrono(cou_Chrono);
			}
		}

		//  output_BPOTables();
	}

	//This generates predictions and selects the traces putting them into the output array.
	void update_Predict()
	{
		//Determine which goals states are out of bounds and need corrected based upon the target values and current states
		//The delta changes needed for each goal are calculated, those that are within ranges are entered as (1). 
		compile_Delta_Targets();
		/*
		for (int cou_Row = 0; cou_Row < Goal_Target_Deltas.get_Row_Count(); cou_Row++)
		{
			std::cout << "\n Row: " << cou_Row;
			std::cout << " " << Goal_Target_Deltas.get_float(cou_Row, 0);
		}*/

		//Wipe the networks for the preparation of seeking changes.
		clear_Nets();

		//Enter the desired delta changes and the goal states into the raw networks
		enter_Goals_Into_Raw();

		//Charge the raw constructs
		charge_Raw();

		//Gather the Raw treetop nodes & their charges, but only the ones that match the delta needed.
		select_Raw_Treetops();

		//Charge the Multi-Sensory-Construct (MSC) with these nodes & their charges
		MSC.Eval();
		//MSC.output_Input_Table_U();
		//MSC.output_Output_Tables_MSC();

		//Gather the MSC treetops into Chrono
		gather_MSC_Treetops();

		//Charge Chrono
		charge_Chrono_Seek_Past();

		//Gather the resultant MSC treetops frin Chrono output patterns & discharge them through the MSC.
		discharge_Chrono();

		//Gather the state quanta patterns represented by these MSC treetops

		//Find those that have the most optimal delta changes in relation to the goal deltas desired, weigh them by reinforcement counter (RC)

		sort_Traces();

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

	//Set the ranges of target values for the goal network to seek.
	testerman.set_Range_Goal_Target(0, 4.9, 5.1); //The O2 is considered a 'priority' variable needed increased precision and maintanance.
	testerman.set_Specific_Goal_Target(0, 5.0); //Set the value for the specific goal we want, used to search the network for optimal solutions.
	testerman.set_Range_Goal_Target(1, 7.0, 9.0); //The temperature is considered a lower priority and we don't want the heater kicking on and off constantly if the temp is somewhat stable around the goal value.
	testerman.set_Specific_Goal_Target(1, 8.0);

	//Register and name the two feedback states for the current dataset.
	testerman.register_Feedback_States(2);
	testerman.rename_Feedback_State(0, "Oxygen_Pump");
	testerman.rename_Feedback_State(1, "Heater");
	

	std::string tmp_String = "";

	c_Sim Simboi; //A basic simulation to generate a test dataset.

	for (int cou_Index = 0; cou_Index < 100; cou_Index++)
	{
		//olc::NT3::xy(0, 0);
		//O2, Simboi is just a test simulation to generate data for this development, the actual data from the systems will be read in instead.
		testerman.Afferent_Goal_State.set_float(0, 0, float(Simboi.O2));

		//Temperature
		testerman.Afferent_Goal_State.set_float(1, 0, float(Simboi.Temp));

		//O2_Pump
		testerman.Afferent_Feedback_State.set_float(0, 0, float(Simboi.O2_Pump));

		//Heater
		testerman.Afferent_Feedback_State.set_float(1, 0, float(Simboi.Heater));


		testerman.update();

		//Simboi.output();

		Simboi.iterate();

		//std::cout << "\n\n++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++";
		//system("PAUSE");
	}
	system("CLS");
	while (1)
	{
		//system("CLS");
		//olc::NT3::cls(0, 0, 50, 20);
		olc::NT3::xy(0, 0);
		
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

		testerman.update(1);
		//testerman.output();

		Simboi.output();
		
		//system("PAUSE > NULL");
	}


	return 0;
}