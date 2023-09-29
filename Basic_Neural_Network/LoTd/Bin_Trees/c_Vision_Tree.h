
class c_State_Vision_Node
{
     friend class c_State_Vision_Tree; 
     private:
          c_State_Vision_Node * Right;
          c_State_Vision_Node * Left;
          
     public:
          u_Data Data;
          float Charge;
          
          c_State_Vision_Node()
          {
               Right = NULL;
               Left = NULL;
          }
          
          ~c_State_Vision_Node()
          {
               delete Right; Right = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_State_Vision_Tree
{
     private:
             
     public:
          c_State_Vision_Node * Root;
          bool flg_Foundit;//the flag that set when a node is found already set
          int Number_Of_Nodes;
          int Node_Color;
          bool flg_Color;
          
          c_State_Vision_Tree()
          {
               Root = NULL;
               flg_Foundit = false;
               Number_Of_Nodes = 0;
               Node_Color = 9;
               flg_Color = 0;
          }
          
          ~c_State_Vision_Tree()
          {
               delete Root;
          }
          
          void search(u_Data p_Data, float p_Charge)
          {
               flg_Foundit = false;
               query_Node(Root, p_Data, p_Charge);
          }
          
          int query_Node(c_State_Vision_Node *&p_Node, u_Data p_Data, float p_Charge)
          {
               
               if (p_Node == NULL)
               {     
                    p_Node = new c_State_Vision_Node;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Data.NR = p_Data.NR;
                    p_Node->Charge = p_Charge;
                    Number_Of_Nodes++;
                    /*
                    if (!flg_Color)
                    {
                         Node_Color++;
                         if (Node_Color == 15){ flg_Color = 1; }
                    }
                    else 
                    {
                         Node_Color--;
                         if (Node_Color == 9){ flg_Color = 0; }
                    }*/
                    return 1;
               }
               
               if (p_Node->Data.NR == p_Data.NR)
               {
                    p_Node->Charge += p_Charge;
                    return 0;
               }
                    
               if (p_Data.NR < p_Node->Data.NR)
               {
                    query_Node(p_Node->Left, p_Data, p_Charge);
                    return 0;
               }
                    
               if (p_Data.NR > p_Node->Data.NR)
               {
                    query_Node(p_Node->Right, p_Data, p_Charge);
                    return 0;
               }
                    
               return 0;
          }
          
          void output_Tree()
          {
               int tmp_Y = 5;
               output_Node(Root, &tmp_Y);
          }
          
          void output_Node(c_State_Vision_Node *&p_Node, int * p_Y)
          {
               if (p_Node == NULL){ return; }
               
               output_Node(p_Node->Left, p_Y);
               cout << "\n\t\t";
               ochr(0, Node_Color, char (p_Node->Data.I));
               cout << "->";
               oflt(0, Node_Color, p_Node->Charge);
               
               output_Node(p_Node->Right, p_Y);
          }
          
          void output_Plain_Node(c_State_Vision_Node *&p_Node)
          {
               if (p_Node == NULL){ return; }
               
               cout << "\n\t\t p_Node->" << char (p_Node->Data.I) << "->" << p_Node->Charge;
          }
          
          void output_Tree_SJW()
          {
               int tmp_Y = 5;
               output_Node(Root, &tmp_Y);
          }
          
          void output_Node_SJW(c_State_Vision_Node *&p_Node, int * p_Y)
          {
               if (p_Node == NULL){ return; }
               
               output_Node_SJW(p_Node->Left, p_Y);
               
               *p_Y += 1;
               xy(int (p_Node->Charge), *p_Y);
               
               ochr(0, Node_Color, char (p_Node->Data.I));
               cout << "->";
               oflt(0, int(p_Node->Charge), p_Node->Charge);
               output_Node_SJW(p_Node->Right, p_Y);
          }
          
          //The caller passes a u_Data bit to this function, which then searches all the nodes to find the strongest one.
          void get_Strongest(u_Data * p_State)
          {
               float Current_Challenger = 0.0;
               return find_Strongest(Root, *p_State, Current_Challenger);
          }
          
          void find_Strongest(c_State_Vision_Node *&p_Node, u_Data &p_Strongest, float &p_Current_Challenger)
          {
               if (p_Node == NULL){ return; }
               
               if (p_Current_Challenger < p_Node->Charge){ p_Strongest.NR = p_Node->Data.NR; p_Current_Challenger = p_Node->Charge; }
               
               find_Strongest(p_Node->Left, p_Strongest, p_Current_Challenger);
               find_Strongest(p_Node->Right, p_Strongest, p_Current_Challenger);
          }
};

