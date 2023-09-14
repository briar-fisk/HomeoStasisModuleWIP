#ifndef c_NT3_Buffer_H
#define c_NT3_Buffer_H




//This is the linked list for the buffer.
class c_NT3_Linked_List
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_List * Next;
     c_NT3_Linked_List * Previous;
     u_Data_3 Charge;
     c_NT3_Base_Node_1D * NID;
     
     c_NT3_Linked_List()
     {
          Next = NULL;
          Previous = NULL;
          Charge.I = 0;
          NID = NULL;
     }
     
     ~c_NT3_Linked_List()
     {
          Charge.I = 0;
          if (NID != NULL){ NID->Ref = NULL; }
          NID = NULL;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
     void output_LL()
     {
          //*std::cout << " [";
          if (NID != NULL){ NID->bp_O(); }
          std::cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Linked_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_List * Root;
     c_NT3_Linked_List ** Current_LL;
     long long int Depth;
     
     c_NT3_Linked_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_NT3_Linked_List_Handler()
     {
          reset();
     }
     
     //Init
     void init()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     //Adds a node.
     c_NT3_Linked_List * new_LL(c_NT3_Base_Node_1D * p_NID, long long int p_Charge)
     {
          c_NT3_Linked_List * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_List;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->NID = p_NID;
          
          tmp_LL = *Current_LL;
          
          (*Current_LL)->Previous = tmp_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          Depth++;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Linked_List * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_List * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               tmp_LL->Previous = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          Current_LL = &Root;
          
          Depth = 0;
     }
     
     /*
     //Bubble sorts the linked list.
     void bubble_Sort()
     {
          //Tmp vars
          c_NT3_Linked_List * tmp_Prv = NULL;
          c_NT3_Linked_List * tmp_Next = NULL;
          c_NT3_Linked_List * tmp_Current = NULL;
          bool flg_Done = false;
          
          while(!flg_Done)
          {
               flg_Done = true;
               
               
          }
     }
      * */
};


class c_NT3_Buffer_Node
{
     friend class c_NT3_Buffer_Tree; 
     private:
          c_NT3_Buffer_Node * Right;
          c_NT3_Buffer_Node * Center;
          c_NT3_Buffer_Node * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_List * LL;
          
          c_NT3_Buffer_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Buffer_Node()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Buffer_Tree
{
private:
             
             
     public:
          c_NT3_Buffer_Node * Root;
          c_NT3_Buffer_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;

          c_NT3_Buffer_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Buffer_Tree()
          {
               delete Root;
          }
          
          void init()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          //Now returning whether or not the node was found as the foundit flag does not work with multiple threads.
          //This returns the node at the end of the chain, whether it was found or not does not matter the 
          //buffer node is returned and has to be checked for values.
          c_NT3_Buffer_Node * search(c_NT3_Base_Node_1D * p_Data)
          {
               flg_Foundit = false;
               
               if (p_Data == NULL){ Current = NULL; return NULL; }
               
               u_Data_3 tmp_SSData;
               tmp_SSData.NR = p_Data;
               
               
               
               //--std::cout << "\n\n SEARCHING ->" << tmp_SData.U;
               //system("PAUSE > NULL");
               return query_Node(Root, tmp_SSData, 31);
          }
          
          c_NT3_Buffer_Node * query_Node(c_NT3_Buffer_Node *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               //These are used because we have to be carefull not to leave Mutex locked with a return.
               c_NT3_Buffer_Node * tmp_Return = NULL;
               bool flg_Done = false;
               
               u_Data_3 tmp_DData;
               tmp_DData.U = p_Data.U;
               tmp_DData.U = tmp_DData.U >> p_Shift;
               //--std::cout << "\n\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               
               
               //--std::cout << "\n\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //--std::cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //--std::cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //--std::cout << "\n\t   tmp_DData.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         tmp_Return = p_Node;
                         flg_Done = true;
                         //return 0;
                    }
               }
               
               //If the node was not found then return NULL;
               if (flg_Done){ return tmp_Return; }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_DData.U && p_Shift > 0)
               {
                    //--std::cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //--std::cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               if (p_Node->Data.U == p_Data.U)
               {
                    //--std::cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    tmp_Return = p_Node;
                    flg_Done = true;
                    //return 1;
               }
               
               //If the node was found then return tmp_Return;
               if (flg_Done){ return tmp_Return; }
               
               if (tmp_DData.U < p_Node->Data.U)
               {
                    //--std::cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, p_Shift);
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //--std::cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Right, p_Data, p_Shift);
               }
               return tmp_Return;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Buffer_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               //*std::cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //*std::cout << "  ";
               }
               //*std::cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ std::cout << "->" << (p_Node->LL->NID); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Linked_List * get_Node_LL(c_NT3_Buffer_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Buffer_Node * &p_Node, c_NT3_Linked_List * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_List * set_Current_Node_LL(c_NT3_Linked_List * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_List * get_Current_Node_LL()
          {
               return get_Node_LL(*Current);
          }
          
          
          //Resets the tree.
          void reset()
          {
               delete Root;
               Root = NULL;
               
               Current = NULL;
               flg_Foundit = false;
          }
                    
};



//The class that houses the charging buffer tree and the LL.
class c_NT3_Buffer
{
private:
     
public:
     
     //The search tree.
     c_NT3_Buffer_Tree Tree;
     
     //The linked lists.
     c_NT3_Linked_List_Handler Current_Charge;
     
     c_NT3_Linked_List_Handler Output;
     
     c_NT3_Linked_List_Handler Treetops;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
          
     c_NT3_Buffer()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
     }
     
     ~c_NT3_Buffer()
     {
          Current_Charge.reset();
          Output.reset();
          Treetops.reset();
     }
     
     //=====--            --=====//
     //==--     CHARGING     --==//
     //=====--            --=====//
     
     
     //--  CHARGING FOR ORIGINAL ALGO, USED FOR MSC WHERE ALL PATTERNS ARE TO BE FOUND
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs(int p_Use_Spike, int p_Left_Leg = 1, int p_Right_Leg = 1)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          double tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = long long int(tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = long long int((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = long long int((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               if (p_Left_Leg)
               {
                    for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
                    {
                         submit(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                         //*std::cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                         //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
                    }
               }
               
               if (p_Right_Leg)
               {
                    for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
                    {
                         submit(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                         //*std::cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                         //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
                    }
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }

               tmp_LL->NID->output_GUI(1);

               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = long long int (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = long long int (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = long long int ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*std::cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*std::cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_L(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          double tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*std::cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_R(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*std::cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
                    //*tmp_LL->NID->Axons_R[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_L(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*std::cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC_R(int p_Use_Spike)
     {
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*std::cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
                    //*tmp_LL->NID->Axons_R[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }

     //=====--              --=====//
     //==--     SUBMISSION     --==//
     //=====--              --=====//
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          if (p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I += p_Charge;
               if (((c_NT3_Linked_List*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I; }
          }
          
          if ((c_NT3_Linked_List*) p_Node->Ref == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, p_Charge);
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          if ((c_NT3_Linked_List*) p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I += (p_Charge * ((c_NT3_Linked_List*) p_Node->Ref)->NID->get_RC_Score());
               if (((c_NT3_Linked_List*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List*) p_Node->Ref)->Charge.I; }
          }
          
          if (((c_NT3_Linked_List*) p_Node->Ref) == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, (p_Node->get_RC_Score() * p_Charge));
          }
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void Old_submit(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void Old_submit_RC(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += (p_Charge * (Tree.get_Current_Node_LL())->NID->get_RC_Score());
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     
     //--  SUBMISSION FOR MT
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_MT(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          c_NT3_Buffer_Node * tmp_Current_Node = Tree.search(p_Node);
          
          
          if (tmp_Current_Node->LL != NULL)
          {
               (tmp_Current_Node->LL)->Charge.I += p_Charge;
               if ((tmp_Current_Node->LL)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (tmp_Current_Node->LL)->Charge.I; }
          }
          
          if ((tmp_Current_Node->LL == NULL) && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               tmp_Current_Node->LL = (Current_Charge.new_LL(p_Node, p_Charge));
          }
          
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC_MT(c_NT3_Base_Node_1D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          c_NT3_Buffer_Node * tmp_Current_Node = Tree.search(p_Node);
          
          
          if (tmp_Current_Node->LL != NULL)
          {
               (tmp_Current_Node->LL)->Charge.I += (p_Charge * (tmp_Current_Node->LL)->NID->get_RC_Score());
               if ((tmp_Current_Node->LL)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (tmp_Current_Node->LL)->Charge.I; }
          }
          
          if ((tmp_Current_Node->LL == NULL) && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               tmp_Current_Node->LL = (Current_Charge.new_LL(p_Node, p_Charge));
          }
          
     }
     
     
     //=====--             --=====//
     //==--     GATHERING     --==//
     //=====--             --=====//
     
     //For gathering the Opp
     //>Target Tier - 1
     //>charge
     //>gather
     //>flush(Target_Tier)
     
     //Gathers the output nodes and resets the tree for the next iteration.
     void gather()
     {
          //Delete the old Output.
          Output.reset();
          
          //Reassign the linked lists.
          Output.Root = Current_Charge.Root;
          Output.Depth = Current_Charge.Depth;
          
          Current_Charge.Root = NULL;
          Current_Charge.reset();
          
          //Delete the tree.
          Tree.reset();
          
          //Sets the done flag if Output == NULL.
          flg_Not_Done = 1;
          if ((Output.Root == NULL)){ flg_Not_Done = 0; }
     }
     
     //Charges the outputs back into the charging buffer.
     long long int gather_Treetops()
     {
          c_NT3_Linked_List * tmp_LL = Treetops.Root;
          c_NT3_Linked_List * tmp_LL_Root = Treetops.Root;
          
          long long int tmp_Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          
          //Find the current highest charge.
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_Current_Highest_Charge < tmp_LL->Charge.I){ tmp_Current_Highest_Charge = tmp_LL->Charge.I; }
               
               tmp_LL = tmp_LL->Next;    
          }
          
          tmp_LL = Treetops.Root;
          Treetops.Root = NULL;
          Treetops.reset();
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / tmp_Current_Highest_Charge) * Base_Charge);
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Treetops.new_LL(tmp_LL->NID, (tmp_Charge));
               
               tmp_LL = tmp_LL->Next;
          }
          
          delete tmp_LL_Root;
          
          return tmp_Current_Highest_Charge;
     }
     
     //=====--                       --=====//
     //==--     CLEANUP & ALLOCATION    --==//
     //=====--                       --=====//
     
     
     //Destroys the output buffers.
     void destroy_Output_Buffers()
     {
          //Delete the old Output.
          Output.reset();    
     }
     
     //=====--          --=====//
     //==--     OUTPUT     --==//
     //=====--          --=====//
     
     //Outputs all of the buffers.
     void output_All_Buffers()
     {
          //system("CLS");
          std::cout << "\n Charge_LL->" << Current_Charge.Depth << " ->";
          output_Charge_LL();
          std::cout << "\n Output_LL->" << Output.Depth << " ->";
          output_Output_LL();
          output_Treetops();
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
          output_Treetops();
     }
     
     //Outputs the Current_Charge_LL.
     void output_Charge_LL()
     {
          //*std::cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ std::cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
          //*std::cout << " ~Fing"; std::cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          //*std::cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ std::cout << "empty..."; return; }
          Output.Root->output_LL();
          //*std::cout << " ~Fing"; std::cout.flush();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          //*std::cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ std::cout << "empty..."; return; }
          Treetops.Root->output_LL();
          //*std::cout << " ~Fing"; std::cout.flush();
     }
};




















































/*
//This is the linked list for the buffer.
class c_NT3_Linked_List_2D
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_List_2D * Next;
     c_NT3_Linked_List_2D * Previous;
     u_Data_3 Charge;
     c_NT3_Base_Node_2D * NID;
     
     c_NT3_Linked_List_2D()
     {
          Next = NULL;
          Previous = NULL;
          Charge.I = 0;
          NID = NULL;
     }
     
     ~c_NT3_Linked_List_2D()
     {
          Charge.I = 0;
          if (NID != NULL){ NID->Ref = NULL; }
          NID = NULL;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
     void output_LL()
     {
          std::cout << "\n[ " << NID << " ";
          if (NID != NULL){ NID->bp_0(); } else { ostr(0, 12, "NULL"); }
          std::cout << " $"; oint(0, 14, Charge.I); std::cout << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

*/
//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.

/*
class c_NT3_Linked_List_Handler_2D
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_List_2D * Root;
     c_NT3_Linked_List_2D ** Current_LL;
     long long int Depth;
     
     c_NT3_Linked_List_Handler_2D()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     ~c_NT3_Linked_List_Handler_2D()
     {
          reset();
     }
     
     //Init
     void init()
     {
          Root = NULL;
          Current_LL = &Root;
          Depth = 0;
     }
     
     //Adds a node.
     c_NT3_Linked_List_2D * new_LL(c_NT3_Base_Node_2D * p_NID, long long int p_Charge)
     {
          c_NT3_Linked_List_2D * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_List_2D;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->NID = p_NID;
          
          tmp_LL = *Current_LL;
          
          (*Current_LL)->Previous = tmp_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          Depth++;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Linked_List_2D * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_List_2D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               tmp_LL->Previous = NULL;
               tmp_LL->NID->Ref = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          Current_LL = &Root;
          
          Depth = 0;
     }
     
     //Bubble sorts the linked list.
     void bubble_Sort()
     {
          //Tmp vars
          c_NT3_Linked_List_2D * tmp_Prv = NULL;
          c_NT3_Linked_List_2D * tmp_Next = NULL;
          c_NT3_Linked_List_2D * tmp_Current = NULL;
          bool flg_Done = false;
          
          while(!flg_Done)
          {
               flg_Done = true;
               
               
          }
     }
};
*/
/*
class c_NT3_Buffer_Node_2D
{
     friend class c_NT3_Buffer_Tree_2D; 
     private:
          c_NT3_Buffer_Node_2D * Right;
          c_NT3_Buffer_Node_2D * Center;
          c_NT3_Buffer_Node_2D * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_List_2D * LL;
          
          c_NT3_Buffer_Node_2D()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Buffer_Node_2D()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};
*/
/*
class c_NT3_Buffer_Tree_2D
{
private:
             
             
     public:
          c_NT3_Buffer_Node_2D * Root;
          c_NT3_Buffer_Node_2D ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Buffer_Tree_2D()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Buffer_Tree_2D()
          {
               delete Root;
          }
          
          void init()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          //Now returning whether or not the node was found as the foundit flag does not work with multiple threads.
          //This returns the node at the end of the chain, whether it was found or not does not matter the 
          //buffer node is returned and has to be checked for values.
          c_NT3_Buffer_Node_2D * search(c_NT3_Base_Node_2D * p_Data)
          {
               flg_Foundit = false;
               
               if (p_Data == NULL){ Current = NULL; return NULL; }
               
               u_Data_3 tmp_SSData;
               tmp_SSData.NR = p_Data;
               
               
               
               //--std::cout << "\n\n SEARCHING ->" << tmp_SData.U;
               //system("PAUSE > NULL");
               return query_Node(Root, tmp_SSData, 31);
          }
          
          c_NT3_Buffer_Node_2D * query_Node(c_NT3_Buffer_Node_2D *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               //These are used because we have to be carefull not to leave Mutex locked with a return.
               c_NT3_Buffer_Node_2D * tmp_Return = NULL;
               bool flg_Done = false;
               
               u_Data_3 tmp_DData;
               tmp_DData.U = p_Data.U;
               tmp_DData.U = tmp_DData.U >> p_Shift;
               //--std::cout << "\n\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               
               
               //--std::cout << "\n\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               
               if (p_Node == NULL)
               {      
                    //--std::cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node_2D;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //--std::cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //--std::cout << "\n\t   tmp_DData.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         tmp_Return = p_Node;
                         flg_Done = true;
                         //return 0;
                    }
               }
               
               //If the node was not found then return NULL;
               if (flg_Done){ return tmp_Return; }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_DData.U && p_Shift > 0)
               {
                    //--std::cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //--std::cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               if (p_Node->Data.U == p_Data.U)
               {
                    //--std::cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    tmp_Return = p_Node;
                    flg_Done = true;
                    //return 1;
               }
               
               //If the node was found then return tmp_Return;
               if (flg_Done){ return tmp_Return; }
               
               if (tmp_DData.U < p_Node->Data.U)
               {
                    //--std::cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, p_Shift);
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //--std::cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Right, p_Data, p_Shift);
               }
               return tmp_Return;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Buffer_Node_2D *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               //*std::cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //*std::cout << "  ";
               }
               //*std::cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ std::cout << "->" << (p_Node->LL->NID); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Linked_List_2D * get_Node_LL(c_NT3_Buffer_Node_2D * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Buffer_Node_2D * &p_Node, c_NT3_Linked_List_2D * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_List_2D * set_Current_Node_LL(c_NT3_Linked_List_2D * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_List_2D * get_Current_Node_LL()
          {
               return get_Node_LL(*Current);
          }
          
          
          //Resets the tree.
          void reset()
          {
               delete Root;
               Root = NULL;
               
               Current = NULL;
               flg_Foundit = false;
          }
                    
};
*/

/*
//The class that houses the charging buffer tree and the LL.
class c_NT3_Buffer_2D
{
private:
     
public:
     
     //The search tree.
     c_NT3_Buffer_Tree_2D Tree;
     
     //The linked lists.
     c_NT3_Linked_List_Handler_2D Current_Charge;
     
     c_NT3_Linked_List_Handler_2D Output;
     
     c_NT3_Linked_List_Handler_2D Treetops;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
          
     c_NT3_Buffer_2D()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
     }
     
     ~c_NT3_Buffer_2D()
     {
          Current_Charge.reset();
          Output.reset();
          Treetops.reset();
          Tree.reset();
     }
     
     //=====--            --=====//
     //==--     CHARGING     --==//
     //=====--            --=====//
     
     
     //--  CHARGING FOR ORIGINAL ALGO, USED FOR MSC WHERE ALL PATTERNS ARE TO BE FOUND
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs(int p_Use_Spike)
     {
          c_NT3_Linked_List_2D * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = ((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_0;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_0[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[0])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_0[cou_A];
                    //*tmp_LL->NID->Axons_0[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_1;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_1[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_1[cou_A]->flg_Double_Legged[1])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_1[cou_A];
                    //*tmp_LL->NID->Axons_1[cou_A]->bp_O();
               }               
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_2;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_2[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_2[cou_A]->flg_Double_Legged[2])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_2[cou_A];
                    //*tmp_LL->NID->Axons_2[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_3;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_3[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_3[cou_A]->flg_Double_Legged[3])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_3[cou_A];
                    //*tmp_LL->NID->Axons_3[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_RC(int p_Use_Spike)
     {
          c_NT3_Linked_List_2D * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " aL->" << tmp_LL->NID->Axon_Count_0;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_0;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_0[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[0])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_0[cou_A];
                    //*tmp_LL->NID->Axons_0[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_1;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_1[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[1])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_1[cou_A];
                    //*tmp_LL->NID->Axons_1[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_2;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_2[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[2])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_2[cou_A];
                    //*tmp_LL->NID->Axons_2[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_3;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_3[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[3])));
                    //*std::cout << " A->" << tmp_LL->NID->Axons_3[cou_A];
                    //*tmp_LL->NID->Axons_3[cou_A]->bp_O();
               }
               
               if (tmp_LL->NID->get_Type() == 2 || tmp_LL->NID->get_Type() == 3)
               {
                    Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               }
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_Into_Treetops(int p_Use_Spike)
     {
          c_NT3_Linked_List_2D * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = ((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               
               tmp_LL = tmp_LL->Next;
          }
          Output.reset();
     }
     
     //Charges the outputs back into the charging buffer.
     void charge_Outputs_Into_Treetops_RC(int p_Use_Spike)
     {
          c_NT3_Linked_List_2D * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*std::cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*std::cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*std::cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Treetops.new_LL((tmp_LL->NID), (tmp_LL->Charge.I));
               
               tmp_LL = tmp_LL->Next;
          }
          Output.reset();
     }
     
     //=====--              --=====//
     //==--     SUBMISSION     --==//
     //=====--              --=====//
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(c_NT3_Base_Node_2D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          if (p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List_2D*) p_Node->Ref)->Charge.I += p_Charge;
               if (((c_NT3_Linked_List_2D*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List_2D*) p_Node->Ref)->Charge.I; }
          }
          
          if ((c_NT3_Linked_List_2D*) p_Node->Ref == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, p_Charge);
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void submit_RC(c_NT3_Base_Node_2D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          if ((c_NT3_Linked_List_2D*) p_Node->Ref != NULL)
          {
               ((c_NT3_Linked_List_2D*) p_Node->Ref)->Charge.I += (p_Charge * ((c_NT3_Linked_List_2D*) p_Node->Ref)->NID->get_RC_Score());
               if (((c_NT3_Linked_List_2D*) p_Node->Ref)->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = ((c_NT3_Linked_List_2D*) p_Node->Ref)->Charge.I; }
          }
          
          if (((c_NT3_Linked_List_2D*) p_Node->Ref) == NULL && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               p_Node->Ref = Current_Charge.new_LL(p_Node, (p_Node->get_RC_Score() * p_Charge));
          }
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void Old_submit(c_NT3_Base_Node_2D * p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
          
     //Accepts a submission of nodes to charge, used for CAN.
     void Old_submit_RC(c_NT3_Base_Node_2D * p_Node, long long int p_Charge = 10000)
     {
          
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += (p_Charge * (Tree.get_Current_Node_LL())->NID->get_RC_Score());
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit && p_Node != NULL)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     
     //--  SUBMISSION FOR MT
     
     //=====--             --=====//
     //==--     GATHERING     --==//
     //=====--             --=====//
     
     //For gathering the Opp
     //>Target Tier - 1
     //>charge
     //>gather
     //>flush(Target_Tier)
     
     //Gathers the output nodes and resets the tree for the next iteration.
     void gather()
     {
          //Delete the old Output.
          Output.reset();
          
          //Reassign the linked lists.
          Output.Root = Current_Charge.Root;
          Output.Depth = Current_Charge.Depth;
          
          Current_Charge.Root = NULL;
          Current_Charge.reset();
          
          //Delete the tree.
          Tree.reset();
          
          //Sets the done flag if Output == NULL.
          flg_Not_Done = 1;
          if ((Output.Root == NULL)){ flg_Not_Done = 0; }
     }
     
     //Charges the outputs back into the charging buffer.
     long long int gather_Treetops()
     {
          c_NT3_Linked_List_2D * tmp_LL = Treetops.Root;
          c_NT3_Linked_List_2D * tmp_LL_Root = Treetops.Root;
          
          long long int tmp_Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          
          //Find the current highest charge.
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_Current_Highest_Charge < tmp_LL->Charge.I){ tmp_Current_Highest_Charge = tmp_LL->Charge.I; }
               
               tmp_LL = tmp_LL->Next;    
          }
          
          tmp_LL = Treetops.Root;
          Treetops.Root = NULL;
          Treetops.reset();
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / tmp_Current_Highest_Charge) * Base_Charge);
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               Treetops.new_LL(tmp_LL->NID, (tmp_Charge));
               
               tmp_LL = tmp_LL->Next;
          }
          
          delete tmp_LL_Root;
          return tmp_Current_Highest_Charge;
     }
     
     //=====--                       --=====//
     //==--     CLEANUP & ALLOCATION    --==//
     //=====--                       --=====//
     
     
     //Destroys the output buffers.
     void destroy_Output_Buffers()
     {
          //Delete the old Output.
          Output.reset();    
     }
     
     //Resets everything.
     void reset()
     {
          Current_Charge.reset();
          Output.reset();
          Treetops.reset();
          Tree.reset();
          
     }
     
     //=====--          --=====//
     //==--     OUTPUT     --==//
     //=====--          --=====//
     
     //Outputs all of the buffers.
     void output_All_Buffers()
     {
          //system("CLS");
          std::cout << "\n    Charge_LL->" << Current_Charge.Depth << " ->";
          output_Charge_LL();
          std::cout << "\n    Output_LL->" << Output.Depth << " ->";
          output_Output_LL();
          std::cout << "\n    Output_Tt->" << Treetops.Depth << " ->";
          output_Treetops();
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
          output_Treetops();
     }
     
     //Outputs the Current_Charge_LL.
     void output_Charge_LL()
     {
          //*std::cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ std::cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
          //*std::cout << " ~Fing"; std::cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          //*std::cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ std::cout << "empty..."; return; }
          Output.Root->output_LL();
          //*std::cout << " ~Fing"; std::cout.flush();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          //*std::cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ std::cout << "empty..."; return; }
          Treetops.Root->output_LL();
          //*std::cout << " ~Fing"; std::cout.flush();
     }
};
*/


#endif // !c_NT3_Buffer_H