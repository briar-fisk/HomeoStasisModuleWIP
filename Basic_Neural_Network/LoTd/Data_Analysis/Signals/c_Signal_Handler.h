//Handles the tables.

//Handles all dimensions of tables.
class c_Signal_Handler
{
protected:

     //The tables to be handled.
     c_Signal ** Signals;

     //The arrays to allow for sorting based on Dd.
     int * Signal_ID;
     float * Signal_Dd;

     //The current signal count.
     int Signal_Count;

     //The MAX Signal Count.
     int Signal_Count_MAX;

     //The table handler for the signal handler.
     c_Table_Handler * Tables;

     //A pointer to the first signal in the signal array used to track all registered tables.
     c_Table_1D * Registry_Signal;

     //The signal registry.
     c_Lookup_Tree Signal_Registry;

public:

     c_Signal_Handler(c_Table_Handler * p_Table_Handler)
     {
          Tables = p_Table_Handler;

          Signal_Count_MAX = 100;
          Signals = new c_Signal*[Signal_Count_MAX];

          Signals[0] = new c_Signal;
          Signals[0]->set_Signal_Name("Signal_Registry");
          Signals[0]->set_Signal_ID(0);
          Signal_Registry.set_int("Signal_Registry", 0);

          Registry_Signal = Tables->register_New_Table_1D("Signal_Registry");
          Registry_Signal->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Signal_Registry");

          Signal_Count = 1;

          Signal_ID = NULL;
          Signal_Dd = NULL;

     }

     c_Signal_Handler()
     {
     }

     ~c_Signal_Handler()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               delete Signals[cou_Index];
               Signals[cou_Index] = NULL;
          }
          delete [] Signals;
          Signals = NULL;
     }

     void initialize(c_Table_Handler * p_Table_Handler)
     {
          Tables = p_Table_Handler;

          Signals = new c_Signal*[1];

          Signals[0] = new c_Signal;
          Signals[0]->set_Signal_Name("Signal_Registry");
          Signals[0]->set_Signal_ID(0);
          Signal_Registry.set_int("Signal_Registry", 0);

          Registry_Signal = Tables->register_New_Table_1D("Signal_Registry");
          Registry_Signal->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Signal_Registry");

          Signal_Count = 1;

          Signal_ID = NULL;
          Signal_Dd = NULL;
     }

     //Sets the tables for the signal handler.
     void set_Tables(c_Table_Handler * p_Tables)
     {
          Tables = p_Tables;
     }

     //Creates a new signal and returns the ID.
     c_Signal * register_New_Signal(string p_Signal_Name)
     {
          //Search for a NULL index, if found register the signal there.
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               if (Signals[cou_Index] == NULL)
               {
                    Signals[cou_Index] = new c_Signal;
                    Signals[cou_Index]->set_Signal_ID(cou_Index);
                    Signals[cou_Index]->set_Signal_Name(p_Signal_Name);
                    Signals[cou_Index]->set_Signal_Output_Table(Tables->register_New_Table_1D(p_Signal_Name));
                    Registry_Signal->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((cou_Index), 0, p_Signal_Name);

                    Signal_Registry.set_int(p_Signal_Name, cou_Index);
                    return (c_Signal*) Signals[cou_Index];
               }
          }

          //Make room for the new signal.
          increment_Signal_Array();

          Signals[Signal_Count - 1] = new c_Signal;
          Signals[Signal_Count - 1]->set_Signal_ID(Signal_Count - 1);
          Signals[Signal_Count - 1]->set_Signal_Name(p_Signal_Name);
          Signals[Signal_Count - 1]->set_Signal_Output_Table(Tables->register_New_Table_1D(p_Signal_Name));

          //Add the signal to the lookup tree.
          Signal_Registry.set_int(p_Signal_Name, (Signal_Count - 1));

          //Add the current signal to the registry.
          Registry_Signal->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((Signal_Count - 1), 0, p_Signal_Name);


          return (c_Signal*) Signals[Signal_Count - 1];

     }

     //Removes a signal from the registry.
     void delete_Signal(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }

          //cout << "\n\n\t Deleting Signal[" << p_SID << "]";

          //Remove the signal from the registry.
          Signal_Registry.remove_Node(Signals[p_SID]->get_Signal_Name());

          //Delete the signal and set to NULL.
          delete Signals[p_SID];
          Signals[p_SID] = NULL;
          Registry_Signal->set_string(p_SID, 0, " ");
     }

     //Removes a signal using the signal name.
     void delete_Signal(string p_Signal_Name)
     {
          delete_Signal(get_Signal_ID(p_Signal_Name));
     }

     //Gets a reference to the current signal.
     void * get_Signal_Reference(int p_SID = 0)
     {
          if (!bounds_TID(p_SID)){ return Signals[p_SID]; }

          return Signals[p_SID];
     }
     void * get_Signal_Reference(string p_TName)
     {
          return get_Signal_Reference(get_Signal_ID(p_TName));
     }

     //Gets the ID of a given signal from the given signal name.
     int get_Signal_ID(string p_Signal_Name)
     {
          return Signal_Registry.get_int(p_Signal_Name);
     }

     //Gets the name of a given signal.
     string get_Signal_Name(int p_SID)
     {
          if (bounds_TID(p_SID)){ return Signals[p_SID]->get_Signal_Name(); }
          return "Signal_ID_Not_Found";
     }

     //Updates the name of a signal in the registry.
     void set_Signal_Name(int p_SID, string p_Signal_Name)
     {
          if (!bounds_TID(p_SID)){ return; }

          //Update the registry lookup tree.
          Signal_Registry.change_name(Signals[p_SID]->get_Signal_Name(), p_Signal_Name);

          Signals[p_SID]->set_Signal_Name(p_Signal_Name);
          Registry_Signal->set_string(p_SID, 0, p_Signal_Name);
     }
     void set_Signal_Name(string p_TName, string p_Signal_Name)
     {
          set_Signal_Name(get_Signal_ID(p_TName), p_Signal_Name);
     }



     //--     Signal MANIPULATIONS     --//


     //Submit an entry to the signal.
     void submit_Signal       (int p_SID, int p_Index, float p_Value) { if (bounds_TID(p_SID)){ Signals[p_SID]->submit_Signal(p_Index, p_Value); } }
     void submit_False_Signal (int p_SID, float p_Value)              { if (bounds_TID(p_SID)){ Signals[p_SID]->submit_False_Signal(p_Value);    } }
     void submit_True_Signal  (int p_SID, float p_Value)              { if (bounds_TID(p_SID)){ Signals[p_SID]->submit_True_Signal(p_Value);     } }


     //Submit an entry to the signal.
     void submit_Signal       (string p_Signal_Name, int p_Index, float p_Value) { submit_Signal(get_Signal_ID(p_Signal_Name), p_Index, p_Value); }
     void submit_False_Signal (string p_Signal_Name, float p_Value)              { submit_False_Signal(get_Signal_ID(p_Signal_Name), p_Value);    }
     void submit_True_Signal  (string p_Signal_Name, float p_Value)              { submit_True_Signal(get_Signal_ID(p_Signal_Name), p_Value);     }




     //--     ARRAY MANIPULATIONS     --//

     //Expands the signal array by one.
     void increment_Signal_Array()
     {
          expand_Signal_Array(Signal_Count + 1);
     }

     //Expands the signal array.
     void expand_Signal_Array(int p_Depth)
     {
          if (p_Depth <= Signal_Count){ return; }
          if (p_Depth <= Signal_Count_MAX){ Signal_Count = p_Depth; return; }

          //Setup the tmp_tables
          c_Signal ** tmp_Signals = new c_Signal*[Signal_Count_MAX];

          //Copy the current tables into the temporary tables.
          for (int cou_Signal=0;cou_Signal<Signal_Count_MAX;cou_Signal++)
          {
               tmp_Signals[cou_Signal] = Signals[cou_Signal];
               Signals[cou_Signal] = NULL;
          }

          //Reallocate the Signal array.
          delete [] Signals;
          Signals = NULL;

          Signals = new c_Signal*[int (p_Depth * 1.5)];

          //Read the original tables back into the reallocated array.
          for (int cou_Index=0;cou_Index<Signal_Count_MAX;cou_Index++)
          {
               Signals[cou_Index] = tmp_Signals[cou_Index];
               tmp_Signals[cou_Index] = NULL;
          }

          //NULL the newly minted indexes.
          for (int cou_Index=Signal_Count_MAX;cou_Index<int (p_Depth * 1.5);cou_Index++)
          {
               Signals[cou_Index] = NULL;
          }

          //Set the signal count.
          Signal_Count_MAX = int (p_Depth * 1.5);
          Signal_Count = p_Depth;

          //Cleanup the tmp array.
          delete [] tmp_Signals;

     }

     //Checks for bounding errors.
     bool bounds_TID(int p_SID)
     {
          if (p_SID >= Signal_Count){ return 0; }
          if (p_SID < 0){ return 0; }
          if (Signals[p_SID] == NULL){ return 0; }
          return 1;
     }







     //Outputs a given signal.
     void output_Signal(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }
          ostr(0, 12, "\n\n\t __________");
          for (unsigned int cou_Index=0;cou_Index<((Signals[p_SID]->get_Signal_Name()).size());cou_Index++)
          {
               ostr(0, 12, "_");
          }
          Signals[p_SID]->output_Signal();
     }
     void output_Signal(string p_TName)
     {
          output_Signal(get_Signal_ID(p_TName));
     }

     //Outputs all of the tables.
     void output_All_Signals()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               output_Signal(cou_Index);
          }
     }

     //Outputs a given signal.
     void output_Signal_Bar(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }
          ostr(0, 12, "\n\n\t __________");
          for (unsigned int cou_Index=0;cou_Index<((Signals[p_SID]->get_Signal_Name()).size());cou_Index++)
          {
               ostr(0, 12, "_");
          }
          Signals[p_SID]->output_Signal_Bar();
     }
     void output_Signal_Bar(string p_TName)
     {
          output_Signal_Bar(get_Signal_ID(p_TName));
     }

     //Outputs all of the tables.
     void output_All_Signals_Bar()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               output_Signal_Bar(cou_Index);
          }
     }


     //Outputs a given signal.
     void output_Signal_Bar_Condensed(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }
          ostr(0, 12, "\n\n\t __________");
          for (unsigned int cou_Index=0;cou_Index<((Signals[p_SID]->get_Signal_Name()).size());cou_Index++)
          {
               ostr(0, 12, "_");
          }
          Signals[p_SID]->output_Signal_Bar_Condensed();
     }
     void output_Signal_Bar_Condensed(string p_TName)
     {
          output_Signal_Bar_Condensed(get_Signal_ID(p_TName));
     }

     //Outputs all of the tables.
     void output_All_Signals_Bar_Condensed()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               output_Signal_Bar_Condensed(cou_Index);
          }
     }

     //Outputs a tables header.
     void output_Signal_Verbose(int p_SID)
     {
          if (!bounds_TID(p_SID)){ return; }

          cout << "\n\t SID[" << p_SID << "]";
          Signals[p_SID]->output_Signal();
          Signals[p_SID]->output_Signal_Bar();
     }
     void output_Signal_Verbose(string p_TName)
     {
          output_Signal_Verbose(get_Signal_ID(p_TName));
     }

     //Outputs every signal header.
     void output_All_Signals_Verbose()
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               output_Signal_Verbose(cou_Index);
          }
     }

     //Outputs the headers for registered tables.
     void output_Signal_Registry()
     {
          output_Signal(0);
     }

     //Outputs the registry tree.
     void output_Signal_Registry_Tree()
     {
          Signal_Registry.output_Tree();
     }

     //Saves all of the signals.
     void save_Signals(string tmp_Filename)
     {
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               Signals[cou_Index]->save_Signal(tmp_Filename);
          }
     }



     //Gets the tmp_Dd for all signals and bubble sorts them.
     void sort_Signals()
     {
          delete [] Signal_ID;
          Signal_ID = NULL;
          Signal_ID = new int[Signal_Count];

          delete [] Signal_Dd;
          Signal_Dd = NULL;
          Signal_Dd = new float[Signal_Count];

          //Gather the Dd.
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               Signal_ID[cou_Index] = cou_Index;
               Signal_Dd[cou_Index] = Signals[cou_Index]->get_tmp_Dd();
          }

          //Bubble sort them sombitches.
          int tmp_Signal_ID = 0;
          float tmp_Signal_Dd = 0.0f;
          bool tmp_Done = false;

          while(!tmp_Done)
          {
               tmp_Done = true;

               for (int cou_Index=0;cou_Index<Signal_Count - 1;cou_Index++)
               {
                    if (Signal_Dd[cou_Index] < Signal_Dd[cou_Index + 1])
                    {
                         tmp_Done = false;

                         tmp_Signal_Dd = Signal_Dd[cou_Index];
                         tmp_Signal_ID = Signal_ID[cou_Index];

                         Signal_Dd[cou_Index] = Signal_Dd[cou_Index + 1];
                         Signal_ID[cou_Index] = Signal_ID[cou_Index + 1];

                         Signal_Dd[cou_Index + 1] = tmp_Signal_Dd;
                         Signal_ID[cou_Index + 1] = tmp_Signal_ID;
                    }
               }
          }
     }

     //Saves the sorted signals.
     void save_Sorted_Signals(string tmp_Filename)
     {
          system("CLS");
          cout << "\n\t Sorting_Signals...";
          sort_Signals();

          cout << "\n\t Saving_Signals...";
          cout << "\n";
          for (int cou_Index=0;cou_Index<Signal_Count;cou_Index++)
          {
               cout << char (cou_Index);
               Signals[Signal_ID[cou_Index]]->save_Signal(tmp_Filename);
               if ((cou_Index % 200) == 0){ cout << "\n"; }
          }
     }
};



