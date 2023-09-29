//Handles the tables.

//Handles all dimensions of tables.
class c_Raw_Profile_Handler
{
protected:
     
     //The tables to be handled.
     c_Raw_Profile ** Profiles;
     
     //The current Profile count.
     int Profile_Count;
     
     //The table handler for the Profile handler.
     c_Table_Handler * Tables;
     
     //The signal handler for the Profile Handler.
     c_Signal_Handler Signals;
     
     //A pointer to the first Profile in the Profile array used to track all registered tables.
     c_Table_1D * Registry_Profile;
     
     //The Profile registry.
     c_Lookup_Tree Profile_Registry;
     
public:
     
     c_Raw_Profile_Handler(c_Table_Handler * p_Table_Handler)
     {
          cout << "\n\t c_Raw_Profile_Handler(" << p_Table_Handler << ")";
          
          Tables = p_Table_Handler;
          
          Profiles = new c_Raw_Profile*[1];
          
          Profiles[0] = new c_Raw_Profile;
          Profiles[0]->set_Profile_Name("Profile_Registry");
          Profiles[0]->set_Profile_ID(0);
          Profiles[0]->set_Signal_Handler(&Signals);
          Profiles[0]->set_Tables(Tables);
          Profile_Registry.set_int("Profile_Registry", 0);
          
          Registry_Profile = Tables->register_New_Table_1D("Profile_Registry");
          Registry_Profile->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Profile_Registry");
          
          Profile_Count = 1;
     }
     
     c_Raw_Profile_Handler()
     {
     }
     
     ~c_Raw_Profile_Handler()
     {
          for (int cou_Index=0;cou_Index<Profile_Count;cou_Index++)
          {
               delete Profiles[cou_Index];
               Profiles[cou_Index] = NULL;
          }
          delete [] Profiles;
          Profiles = NULL;
     }
     
     void initialize(c_Table_Handler * p_Table_Handler = NULL)
     {
          Tables = p_Table_Handler;
          
          if (Tables == NULL){ Tables = new c_Table_Handler; }
          
          Signals.initialize(Tables);
          
          Profiles = new c_Raw_Profile*[1];
          
          Profiles[0] = new c_Raw_Profile;
          Profiles[0]->set_Profile_Name("Profile_Registry");
          Profiles[0]->set_Profile_ID(0);
          Profiles[0]->set_Signal_Handler(&Signals);
          Profiles[0]->set_Tables(Tables);
          Profile_Registry.set_int("Profile_Registry", 0);
          
          Registry_Profile = Tables->register_New_Table_1D("Profile_Registry");
          Registry_Profile->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Profile_Registry");
          
          Profile_Count = 1;
     }
     
     //Sets the tables for the Profile handler.
     void set_Tables(c_Table_Handler * p_Tables)
     {
          Tables = p_Tables;
     }
     
     //Creates a new Profile and returns the ID.
     c_Raw_Profile * register_New_Profile(string p_Profile_Name)
     {
          //Search for a NULL index, if found register the Profile there.
          for (int cou_Index=0;cou_Index<Profile_Count;cou_Index++)
          {
               if (Profiles[cou_Index] == NULL)
               {
                    Profiles[cou_Index] = new c_Raw_Profile;
                    Profiles[cou_Index]->set_Profile_ID(cou_Index);
                    Profiles[cou_Index]->set_Profile_Name(p_Profile_Name);
                    Profiles[cou_Index]->set_Signal_Handler(&Signals);
                    Profiles[0]->set_Tables(Tables);
                    Registry_Profile->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((cou_Index), 0, p_Profile_Name);
                    
                    Profile_Registry.set_int(p_Profile_Name, cou_Index);
                    return (c_Raw_Profile*) Profiles[cou_Index];
               }
          }
          
          //Make room for the new Profile.
          increment_Profile_Array();
          
          Profiles[Profile_Count - 1] = new c_Raw_Profile;
          Profiles[Profile_Count - 1]->set_Profile_ID(Profile_Count - 1);
          Profiles[Profile_Count - 1]->set_Profile_Name(p_Profile_Name);
          Profiles[Profile_Count - 1]->set_Signal_Handler(&Signals);
          Profiles[0]->set_Tables(Tables);
          
          //Add the Profile to the lookup tree.
          Profile_Registry.set_int(p_Profile_Name, (Profile_Count - 1));
          
          //Add the current Profile to the registry.
          Registry_Profile->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((Profile_Count - 1), 0, p_Profile_Name);
          
          
          return (c_Raw_Profile*) Profiles[Profile_Count - 1];
          
     }
     
     //Removes a Profile from the registry.
     void delete_Profile(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }
          
          //cout << "\n\n\t Deleting Profile[" << p_SID << "]";
          
          //Remove the Profile from the registry.
          Profile_Registry.remove_Node(Profiles[p_SID]->get_Profile_Name());
          
          //Delete the Profile and set to NULL.
          delete Profiles[p_SID];
          Profiles[p_SID] = NULL;
          Registry_Profile->set_string(p_SID, 0, " ");
     }
     
     //Removes a Profile using the Profile name.
     void delete_Profile(string p_Profile_Name)
     {
          delete_Profile(get_Profile_ID(p_Profile_Name));
     }
     
     //Gets a reference to the current Profile.
     void * get_Profile_Reference(int p_SID = 0)
     {
          if (!bounds_TID(p_SID)){ return Profiles[p_SID]; }
          
          return Profiles[p_SID];
     }
     void * get_Profile_Reference(string p_TName)
     {
          return get_Profile_Reference(get_Profile_ID(p_TName));
     }
     
     //Gets the ID of a given Profile from the given Profile name.
     int get_Profile_ID(string p_Profile_Name)
     {
          return Profile_Registry.get_int(p_Profile_Name);
     }
     
     //Gets the name of a given Profile.
     string get_Profile_Name(int p_SID)
     {
          if (bounds_TID(p_SID)){ return Profiles[p_SID]->get_Profile_Name(); }
          return "Profile_ID_Not_Found";
     }
     
     //Updates the name of a Profile in the registry.
     void set_Profile_Name(int p_SID, string p_Profile_Name)
     {
          if (!bounds_TID(p_SID)){ return; }
          
          //Update the registry lookup tree.
          Profile_Registry.change_name(Profiles[p_SID]->get_Profile_Name(), p_Profile_Name);
          
          Profiles[p_SID]->set_Profile_Name(p_Profile_Name);
          Registry_Profile->set_string(p_SID, 0, p_Profile_Name);
     }
     void set_Profile_Name(string p_TName, string p_Profile_Name)
     {
          set_Profile_Name(get_Profile_ID(p_TName), p_Profile_Name);
     }
     
     
     
     //--     Profile MANIPULATIONS     --//
     
     //Adds a signal to a given profile.
     void add_Signal (int p_RPID, string p_Signal_Name){ if (bounds_TID(p_RPID)){ Profiles[p_RPID]->add_Signal(p_Signal_Name);    } }
     void add_Signal (string p_Profile_Name, string p_Signal_Name){ add_Signal(get_Profile_ID(p_Profile_Name), p_Signal_Name);    }
     
     //Submit an entry to the Profile.
     void submit_False_Signal (int p_RPID, string p_Signal_Name, float p_Value){ if (bounds_TID(p_RPID)){ Profiles[p_RPID]->submit_False_Signal(p_Signal_Name, p_Value);    } }
     void submit_True_Signal  (int p_RPID, string p_Signal_Name, float p_Value){ if (bounds_TID(p_RPID)){ Profiles[p_RPID]->submit_True_Signal(p_Signal_Name, p_Value);     } }
     
     //Submit an entry to the Profile.
     void submit_False_Signal (string p_Profile_Name, string p_Signal_Name, float p_Value){ submit_False_Signal(get_Profile_ID(p_Profile_Name), p_Signal_Name, p_Value);    }
     void submit_True_Signal  (string p_Profile_Name, string p_Signal_Name, float p_Value){ submit_True_Signal(get_Profile_ID(p_Profile_Name), p_Signal_Name, p_Value);     }
     
     
     
     
     //--     ARRAY MANIPULATIONS     --//
     
     //Expands the Profile array by one.
     void increment_Profile_Array()
     {
          expand_Profile_Array(Profile_Count + 1);
     }
     
     //Expands the Profile array.
     void expand_Profile_Array(int p_Depth)
     {
          if (p_Depth <= Profile_Count){ return; }
          
          //Setup the tmp_tables
          c_Raw_Profile ** tmp_Profiles = new c_Raw_Profile*[Profile_Count];
          
          //Copy the current tables into the temporary tables.
          for (int cou_Profile=0;cou_Profile<Profile_Count;cou_Profile++)
          {
               tmp_Profiles[cou_Profile] = Profiles[cou_Profile];
               Profiles[cou_Profile] = NULL;
          }
          
          //Reallocate the Profile array.
          delete [] Profiles;
          Profiles = NULL;
          
          Profiles = new c_Raw_Profile*[p_Depth];
          
          //Read the original tables back into the reallocated array.
          for (int cou_Index=0;cou_Index<Profile_Count;cou_Index++)
          {
               Profiles[cou_Index] = tmp_Profiles[cou_Index];
               tmp_Profiles[cou_Index] = NULL;
          }
          
          //NULL the newly minted indexes.
          for (int cou_Index=Profile_Count;cou_Index<p_Depth;cou_Index++)
          {
               Profiles[cou_Index] = NULL;
          }
          
          //Set the Profile count.
          Profile_Count = p_Depth;
          
          //Cleanup the tmp array.
          delete [] tmp_Profiles;
          
     }
     
     //Checks for bounding errors.
     bool bounds_TID(int p_SID)
     {
          if (p_SID >= Profile_Count){ return 0; }
          if (p_SID < 0){ return 0; }
          if (Profiles[p_SID] == NULL){ return 0; }
          return 1;
     }
     
     //Outputs a given Profile.
     void output_Profile(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }
          
          Profiles[p_SID]->output_Profile();
     }
     void output_Profile(string p_TName)
     {
          output_Profile(get_Profile_ID(p_TName));
     }
     
     //Outputs a given Profile_Condensed.
     void output_Profile_Condensed(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }
          
          Profiles[p_SID]->output_Profile_Condensed();
     }
     void output_Profile_Condensed(string p_TName)
     {
          output_Profile_Condensed(get_Profile_ID(p_TName));
     }
     
     //Outputs all of the tables.
     void output_All_Profiles()
     {
          for (int cou_Index=0;cou_Index<Profile_Count;cou_Index++)
          {
               output_Profile(cou_Index);
          }
     }
     
     //Outputs all of the prifiles in a condensed manner.
     void output_All_Profiles_Condensed()
     {
          for (int cou_Index=0;cou_Index<Profile_Count;cou_Index++)
          {
               output_Profile_Condensed(cou_Index);
          }
     }
     
     //Saves all of the profiles with the given iteration into the given file.
     void save_Profiles(string p_Filename)
     {
          for (int cou_Index=0;cou_Index<Profile_Count;cou_Index++)
          {
               Profiles[cou_Index]->save_Profile(p_Filename);
          }
     }
     
     //Saves the signals in the signal handler.
     void save_Signals(string tmp_Filename)
     {
          Signals.save_Signals(tmp_Filename);
     }
     
     //Saves the sorted signals in the signal handler.
     void save_Sorted_Signals(string tmp_Filename)
     {
          Signals.save_Sorted_Signals(tmp_Filename);
     }
     
     //Outputs the headers for registered tables.
     void output_Profile_Registry()
     {
          output_Profile(0);
     }
     
     //Outputs the registry tree.
     void output_Profile_Registry_Tree()
     {
          Profile_Registry.output_Tree();
     }
     
};



