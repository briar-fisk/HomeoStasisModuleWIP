//This class allows for the creation of a raw profile for finding promising signals to build strategies upon.

class c_Raw_Profile
{
public:
     
     //The name of the profile.
     string Profile_Name;
     
     //The ID of the profile.
     int RPID;
     
     //The Signals to track.
     c_Signal ** Signals;
     
     //The number of signals.
     int Signal_Count;
     
     //The Signal handler to interact with.
     c_Signal_Handler * Signal_Handler;
     
     //The Table handler to interact with.
     c_Table_Handler * Tables;
     
     c_Raw_Profile()
     {
          Profile_Name = "______ Profile";
          
          RPID = 0;
          
          Signals = NULL;
          
          Signal_Count = 0;
          
          Signal_Handler = NULL;
          
          Tables = NULL;
     }
     
     //--     SETS AND GETS
     
     //Profile
     void set_Profile_Name                        (string p_Name){ Profile_Name = p_Name; }
     void set_Profile_ID                          (int p_RPID){ RPID = p_RPID; }
     void set_Signal_Handler                      (c_Signal_Handler * p_Signal_Handler){ Signal_Handler = p_Signal_Handler; }
     void set_Tables(c_Table_Handler * p_Tables)  { Tables = p_Tables; }
     
     string get_Profile_Name(){ return Profile_Name; }
     int get_Profile_ID(){ return RPID; }
     
     //Signals
     
     int get_Signal_Count(){ return Signal_Count; }
     c_Signal * get_Signal_Reference(int p_SID){ if (!bounds_SID(p_SID)){ return NULL; } return Signals[p_SID]; }
     
     //--     PROFILE MANIPULATIONS
     
     //Add a signal to the profile.
     c_Signal * add_Signal(string p_Signal_Name)
     {
          increment_Signals();
          
          if (Signal_Handler == NULL){ cout << "\n\n\n\t ERROR->NO_SIGNAL_HANDLER"; return NULL; }
          
          Signals[Signal_Count -1] = Signal_Handler->register_New_Signal(p_Signal_Name);
          
          return Signals[Signal_Count -1];
     }
     
     //--     SIGNAL MANIPULATIONS
     
     //Submits a false signal.
     void submit_False_Signal(string p_Signal_Name, float p_Value)
     {
          Signal_Handler->submit_False_Signal(p_Signal_Name, p_Value);
     }
     
     //Submits a true signal.
     void submit_True_Signal(string p_Signal_Name, float p_Value)
     {
          Signal_Handler->submit_True_Signal(p_Signal_Name, p_Value);
     }
     
     //--     ARRAY MANIPULATIONS
     
     //Increments the size of the signal array by one.
     void increment_Signals()
     {
          increase_Signals_Array(Signal_Count + 1);
     }
     
     //Increases the number of signals in the profile.
     void increase_Signals_Array(int p_SID)
     {
          if (p_SID <= Signal_Count){ return; }
          
          //Setup the temporary vars.
          c_Signal ** tmp_Signals;
          tmp_Signals = new c_Signal*[Signal_Count];
          
          //Ini the tmp signal array and copy the old one into it.
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               tmp_Signals[cou_Index] = Signals[cou_Index];
               Signals[cou_Index] = NULL;
          }
          
          //Reallocate the signals array.
          delete [] Signals;
          Signals = NULL;
          Signals = new c_Signal*[p_SID];
          
          //copy the tmp into the new signals array and null the new indexes.
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               Signals[cou_Index] = tmp_Signals[cou_Index];
               tmp_Signals[cou_Index] = NULL;
          }
          
          for (int cou_Index=Signal_Count;cou_Index<p_SID;cou_Index++)
          {
               Signals[cou_Index] = NULL;
          }
          
          //null and erase the tmp array.
          delete [] tmp_Signals;
          tmp_Signals = NULL;
          
          //Set the counters to reflect the new signal count.
          Signal_Count = p_SID;
     }
     
     //Checks to see if the given SID is in bounds.
     int bounds_SID(int p_SID)
     {
          if (p_SID < 0){ return 0; }
          if (p_SID >= Signal_Count){ return 0; }
          if (Signals[p_SID] == NULL){ return 0; }
     }
     
     //--     OUTPUT FUNCTIONS
     
     //--  Profiles
     
     
     //Outputs the profile header.
     void output_Profile_Header()
     {

          ostr(0, 12, "\n\n\t __________");
          for (unsigned int cou_Index=0;cou_Index<Profile_Name.size();cou_Index++)
          {
               ostr(0, 12, "_");
          }
          cout << "\n\t ";
          ostr(0, 10, Profile_Name);
          
          cout << "\n\t   Signal_Count->";
          oint(0, 13, Signal_Count);
     }
     
     //Outputs all of the signals in the profile.
     void output_All_Signals()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               if (Signals[cou_Index] == NULL){ continue; }
               cout << "\n";
               Signals[cou_Index]->output_Signal();
               Signals[cou_Index]->output_Signal_Bar();
          }
     }
     
     
     //Outputs all of the signals in the profile, CONDENSED BAR.
     void output_All_Signals_Condensed()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               if (Signals[cou_Index] == NULL){ continue; }
               
               Signals[cou_Index]->output_Signal_Bar_Condensed();
          }
     }
     
     //Outputs the profile.
     void output_Profile()
     {
          output_Profile_Header();
          output_All_Signals();
     }
     
     //Outputs a condensed version of the signals.
     void output_Profile_Condensed()
     {
          output_Profile_Header();
          output_All_Signals_Condensed();
     }
     
     //Saves all of the signals into the given file.
     void save_Signals(string p_File)
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               if (Signals[cou_Index] == NULL){ continue; }
               
               Signals[cou_Index]->save_Signal(p_File);
          }
     }
     
     //Saves the profile into a file.
     void save_Profile(int p_Iteration = 0)
     {
          
          string tmp_Filename = Profile_Name + int2str(p_Iteration);
          
          //Open the file to write to.
          ofstream SF;
          SF.open(tmp_Filename.c_str());
          

          SF << "\n\n\t __________";
          for (unsigned int cou_Index=0;cou_Index<Profile_Name.size();cou_Index++)
          {
               SF << "_";
          }
          SF << "\n\t " << Profile_Name;
          
          SF << "\n\t   Signal_Count->" << Signal_Count;

          
          save_Signals(tmp_Filename);
          
     }
     
     //Saves the profile into a file.
     void save_Profile(string p_Filename)
     {
          
          //cout << "\n\t Saving Profile->" << Profile_Name;
          
          string tmp_Filename = p_Filename;
          
          //Open the file to write to.
          ofstream SF;
          SF.open(tmp_Filename.c_str(), ios::app);
          
          
          SF << "\n\n\t __________";
          for (unsigned int cou_Index=0;cou_Index<Profile_Name.size();cou_Index++)
          {
               SF << "_";
          }
          SF << "\n\t " << Profile_Name;
          
          SF << "\n\t   Signal_Count->" << Signal_Count;

          
          save_Signals(tmp_Filename);
          
     }
};