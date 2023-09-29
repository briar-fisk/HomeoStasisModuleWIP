//This class allows for the analysis of a single single based on a binary standard.


//The Signal Class.
class c_Signal
{
public:
     
     //The total for the true and false stats.
     float Total[2];
     
     //The number of entries to divide by to get the averages.
     int Number_Of_Entries[2];
     
     //The averages for the two stats.
     float Average[2];
     
     //The Dd.
     float Dd;
     
     //The highest entry for each standard.
     float Highest_Entry[2];
     
     //The Lowest Entry for each standard.
     float Lowest_Entry[2];
     
     //The table for output.
     c_Table_1D * Output_Table;
     
     //The name of the signal.
     string Signal_Name;
     
     //The ID of the Signal.
     int SID;
     
     c_Signal(c_Table_1D * p_Output_Table, int p_SID, string p_Signal_Name = "_____ Signal")
     {
          Signal_Name = p_Signal_Name;
          
          SID = p_SID;
          
          Total[0] = 0;
          Total[1] = 0;
          
          Number_Of_Entries[0] = 0;
          Number_Of_Entries[1] = 0;
          
          Average[0] = 0.0f;
          Average[1] = 0.0f;
          
          Dd = 0.0f;
          
          Highest_Entry[0] = 0.0f;
          Highest_Entry[1] = 0.0f;
          
          Lowest_Entry[0] = 0.0f;
          Lowest_Entry[1] = 0.0f;
          
          Output_Table = p_Output_Table;
          
     }
     
     c_Signal()
     {
          Signal_Name = "_____ Signal";
          
          Total[0] = 0;
          Total[1] = 0;
          
          Number_Of_Entries[0] = 0;
          Number_Of_Entries[1] = 0;
          
          Average[0] = 0.0f;
          Average[1] = 0.0f;
          
          Dd = 0.0f;
          
          Highest_Entry[0] = 0.0f;
          Highest_Entry[1] = 0.0f;
          
          Lowest_Entry[0] = 0.0f;
          Lowest_Entry[1] = 0.0f;
          
          Output_Table = NULL;
     }
     
     ~c_Signal()
     {
          
     }
     
     
     void set_Signal_Name(string p_Name)               { Signal_Name = p_Name; }
     void set_Signal_Output_Table(c_Table_1D * p_Table){ Output_Table = p_Table; }
     void set_Signal_ID(int p_SID)                     { SID = p_SID; }
     
     //Sets the signal name.
     string get_Signal_Name()                { return Signal_Name; }
     c_Table_1D * get_Signal_Output_Table()  { return Output_Table; }
     int get_Signal_ID()                     { return SID; }
     
     //Submit an entry to the signal.
     void submit_Signal(int p_Index, float p_Value)
     {
          Total[p_Index] += p_Value;
          
          Number_Of_Entries[p_Index]++;
          
          if (p_Value < Lowest_Entry[p_Index]){ Lowest_Entry[p_Index] = p_Value; }
          if (p_Value > Highest_Entry[p_Index]){ Highest_Entry[p_Index] = p_Value; }
     }
     
     //Submit a false entry to the signal.
     void submit_False_Signal(float p_Value)
     {
          submit_Signal(0, p_Value);
     }
     
     //Submit a true signal to the signal.
     void submit_True_Signal(float p_Value)
     {
          submit_Signal(1, p_Value);
     }
     
     //Gathers the averages.
     void gather_Averages()
     {
          //Gather the averages.
          Average[0] = (Total[0] / float (Number_Of_Entries[0]));
          Average[1] = (Total[1] / float (Number_Of_Entries[1]));
          
          //If there are no entries the averages = 0.
          if (Number_Of_Entries[0] == 0){ Average[0] = 0.0f; }
          if (Number_Of_Entries[1] == 0){ Average[1] = 0.0f; }
          
          //Gather the Dd.
          Dd = absolutef((Average[0] - Average[1]));
          
     }
     
     //Outputs the signal after computing the averages.
     void output_Signal()
     {
          Output_Table->reset_Table();
          
          //Gather the averages.
          gather_Averages();
          
          //Push the values onto the table and output it.
          Output_Table->set_string(0, 0, "F:");
          Output_Table->set_float(0, 1, Average[0]);
          Output_Table->set_float(0, 2, Lowest_Entry[0]);
          Output_Table->set_float(0, 3, Highest_Entry[0]);
          
          Output_Table->set_string(1, 0, "T:");
          Output_Table->set_float(1, 1, Average[1]);
          Output_Table->set_float(1, 2, Lowest_Entry[1]);
          Output_Table->set_float(1, 3, Highest_Entry[1]);
          
          Output_Table->set_string(2, 0, "Dd:");
          Output_Table->set_float(2, 1, Dd);
          
          //Output the finished product.
          Output_Table->output_Table();
     }
     
     //Outputs the signal in horizontal bar format.
     void output_Signal_Bar(float p_Length = 100)
     {
          //Tmp vars.
          float tmp_Multiplier = 0.0f;
          float tmp_F = 0.0f;
          float tmp_T = 0.0f;
          float tmp_Dd = 0.0f;
          float tmp_Top_Value = 0.0f;
          
          //Gather the values.
          gather_Averages();
          
          //The top of the graph is ((Lowest_Average + (Dd / 2)) * 2) which gives us the point between the two averages * 2.
          if (Average[0] <= Average[1])
          {
               tmp_Top_Value = (Average[0] + (Dd / 2)) * 2;
          }
          if (Average[0] > Average[1])
          {
               tmp_Top_Value = (Average[1] + (Dd / 2)) * 2;
          }
          
          //The multiplier is the top value / p_Length to give us the value of each unit.
          tmp_Multiplier = tmp_Top_Value / p_Length;
          
          //The Dd and averages are converted to percentages of the top value.
          tmp_F = (Average[0] / tmp_Top_Value) * 100;
          tmp_T = (Average[1] / tmp_Top_Value) * 100;
          tmp_Dd = (Dd / tmp_Top_Value) * 100;
          
          cout << "\n\n\t ";
          ostr(0, 15, "Bar Graph");
          ostr(0, 7, "->");
          ostr(0, 15, Signal_Name);
          
          cout << "\n\t   ";
          ostr(0, 7, "-> M:");
          cout << tmp_Multiplier;
          
          cout << "\n\t    ";
          ochr(0, 8, char (195));
          ostr(0, 7, "<");
          cout << "F ";
          ostr(0, 7, ">-");
          for (int cou_Index=0;cou_Index<(int (tmp_F));cou_Index++)
          {
               ochr(0, 12, 'X');
          }
          
          cout << "\n\t    ";
          ochr(0, 8, char (195));
          ostr(0, 7, "<");
          cout << "T ";
          ostr(0, 7, ">-");
          for (int cou_Index=0;cou_Index<(int (tmp_T));cou_Index++)
          {
               ochr(0, 10, 'X');
          }
          
          cout << "\n\t    ";
          ochr(0, 8, char (192));
          ostr(0, 7, "<");
          cout << "Dd";
          ostr(0, 7, ">-");
          for (int cou_Index=0;cou_Index<(int (tmp_Dd));cou_Index++)
          {
               ochr(0, 15, 'X');
          }
          
          
          
     }
     
     
     
     
     
     //Outputs the signal in horizontal bar format, for condensed output.
     void output_Signal_Bar_Condensed(float p_Length = 100)
     {
          //Tmp vars.
          float tmp_Multiplier = 0.0f;
          float tmp_Dd = 0.0f;
          float tmp_Top_Value = 0.0f;
          
          //Gather the values.
          gather_Averages();
          
          //The top of the graph is ((Lowest_Average + (Dd / 2)) * 2) which gives us the point between the two averages * 2.
          if (Average[0] <= Average[1])
          {
               tmp_Top_Value = (Average[0] + (Dd / 2)) * 2;
          }
          if (Average[0] > Average[1])
          {
               tmp_Top_Value = (Average[1] + (Dd / 2)) * 2;
          }
          
          //The multiplier is the top value / p_Length to give us the value of each unit.
          tmp_Multiplier = tmp_Top_Value / p_Length;
          
          //The Dd and averages are converted to percentages of the top value.
          tmp_Dd = (Dd / tmp_Top_Value) * 100;
          
          cout << "\n\t ->";
          for (int cou_Index=0;cou_Index<(int (tmp_Dd));cou_Index++)
          {
               ochr(0, 15, 'X');
          }
          
          cout << "\t ";
          ostr(0, 15, Signal_Name);
          
          
          cout << "  ";
          ostr(0, 7, "-> M:");
          cout << tmp_Multiplier;
          
          
          
     }
     
     //Gets the tmp_Dd for bubble sorting to find the best signals.
     float get_tmp_Dd(int p_Chunk = 100)
     {

          //Tmp vars.
          float tmp_Dd = 0.0f;
          float tmp_Top_Value = 0.0f;
          
          //Gather the values.
          gather_Averages();
          
          //The top of the graph is ((Lowest_Average + (Dd / 2)) * 2) which gives us the point between the two averages * 2.
          if (Average[0] <= Average[1])
          {
               tmp_Top_Value = (Average[0] + (Dd / 2)) * 2;
          }
          if (Average[0] > Average[1])
          {
               tmp_Top_Value = (Average[1] + (Dd / 2)) * 2;
          }
          
          tmp_Dd = (Dd / tmp_Top_Value) * 100;
          
          return tmp_Dd;

     }
     
     //Saves a signal to a given file.
     void save_Signal(string p_File_Name)
     {
          //cout << "\n\t    Saving Signal->" << Signal_Name;
          
          //Open the file to write to.
          ofstream SF;
          SF.open(p_File_Name.c_str(), ios::app);
          
          //Tmp vars.
          float tmp_Multiplier = 0.0f;
          //float tmp_F = 0.0f;
          //float tmp_T = 0.0f;
          float tmp_Dd = 0.0f;
          float tmp_Top_Value = 0.0f;
          
          //Gather the values.
          gather_Averages();
          
          //The top of the graph is ((Lowest_Average + (Dd / 2)) * 2) which gives us the point between the two averages * 2.
          if (Average[0] <= Average[1])
          {
               tmp_Top_Value = (Average[0] + (Dd / 2)) * 2;
          }
          if (Average[0] > Average[1])
          {
               tmp_Top_Value = (Average[1] + (Dd / 2)) * 2;
          }
          
          //The multiplier is the top value / p_Length to give us the value of each unit.
          tmp_Multiplier = tmp_Top_Value / 100;
          
          //The Dd and averages are converted to percentages of the top value.
          //tmp_F = (Average[0] / tmp_Top_Value) * 100;
          //tmp_T = (Average[1] / tmp_Top_Value) * 100;
          tmp_Dd = (Dd / tmp_Top_Value) * 100;
          
          SF << "\n "; 
          for (int cou_Index=0;cou_Index<(int (tmp_Dd));cou_Index++)
          {
               SF << "X";
          }
          /*
          SF << " "; 
          for (int cou_Index=0;cou_Index<(int (tmp_F));cou_Index++)
          {
               SF << "/";
          }
          
          SF << " ";
          for (int cou_Index=0;cou_Index<(int (tmp_T));cou_Index++)
          {
               SF << "#";
          }
          */
          
          
          /*
          SF << " F->" << Average[0] << " F.L->" << Lowest_Entry[0] << " F.H->" << Highest_Entry[0];
          
          SF << " T->" << Average[1] << " T.L->" << Lowest_Entry[1] << " T.H->" << Highest_Entry[1];
          */
          
          SF << " A->" << Average[0] << "|" << Average[1] << " tmp_Dd->" << tmp_Dd << " Dd->" << Dd;
          
          SF << " F->" << Lowest_Entry[0] << "|" << Highest_Entry[0];
          
          SF << " T->" << Lowest_Entry[1] << "|" << Highest_Entry[1];
          
          SF << " M->" << tmp_Multiplier;
          
          SF << " ->" << Signal_Name;
     }
};
