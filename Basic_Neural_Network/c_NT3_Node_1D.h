#include "olcPixelGameEngine.h"


//This class holds the info for positioning of the network.
//Allows for output without calling the explicit GUI output.
class c_NT3_XY_Kernel
{
private:

    friend class c_NT3_Node_Network_1D;
    friend class c_NT3_Base_Node;


public:

    //The padding for the output.
    int X_Padd;
    int Y_Padd;

    int X_Offset;
    int Y_Offset;

    int X_MAX;
    int Y_MAX;

    double Highest_RC;

    olc::PixelGameEngine* PGE;

    int Highest_Tier;
    int Tier_Count[1000];
    int Tier_X_Offsets[1000];
    int Tier_Count_Biggest;



    c_NT3_XY_Kernel()
    {
        Highest_Tier = 0;
        PGE = NULL;
        X_Offset = 0;
        Y_Offset = 0;
        X_Padd = 1;
        Y_Padd = 30;
        X_MAX = 100;
        Y_MAX = 100;
        Tier_Count_Biggest = 0;
        Highest_RC = 0;
        for (int cou_Index = 0; cou_Index < 1000; cou_Index++)
        {
            Tier_Count[cou_Index] = 0;
            Tier_X_Offsets[cou_Index] = 0;
        }
    }

    //Sets the pixel game engine object reference.
    void set_PGE(olc::PixelGameEngine* p_PGE)
    {
        PGE = p_PGE;

        X_MAX = PGE->ScreenWidth();
        Y_MAX = PGE->ScreenHeight();
    }

    int get_Highest_RC()
    {
        return Highest_RC;
    }

    int set_Highest_RC(int p_Highest_RC)
    {
        Highest_RC = p_Highest_RC;
    }



    int X_Offset_Less(int p_Increment)
    {
        return X_Offset -= p_Increment;
    }
    int X_Offset_More(int p_Increment)
    {
        return X_Offset += p_Increment;
    }
    int Y_Offset_Less(int p_Increment)
    {
        return Y_Offset -= p_Increment;
    }
    int Y_Offset_More(int p_Increment)
    {
        return Y_Offset += p_Increment;
    }


    int X_Set(int p_X)
    {
        return X_Padd = p_X;
    }
    int Y_Set(int p_Y)
    {
        return Y_Padd = p_Y;
    }

	int X_Padd_Less(int p_Increment)
    {
        return X_Padd -= p_Increment;
    }
    int X_Padd_More(int p_Increment)
    {
        return X_Padd += p_Increment;
    }
    int Y_Padd_Less(int p_Increment)
    {
        return Y_Padd -= p_Increment;
    }
    int Y_Padd_More(int p_Increment)
    {
        return Y_Padd += p_Increment;
    }
    
    //Find the center of the largest tier.
    //Finds offsets.
    void find_X_Offsets()
    {
        Tier_Count_Biggest = 0;
        for (int cou_Index = 0; cou_Index < Highest_Tier; cou_Index++)
        {
            if (Tier_Count[cou_Index] > Tier_Count_Biggest) { Tier_Count_Biggest = Tier_Count[cou_Index]; }
        }
        //std::cout << "\n\n X_Largest: " << Tier_Count_Biggest;

        for (int cou_Index = 0; cou_Index < Highest_Tier; cou_Index++)
        {
            Tier_X_Offsets[cou_Index] = (Tier_Count_Biggest / 2) - (Tier_Count[cou_Index] / 2);
            //std::cout << "\n\n " << cou_Index << ":: " << Tier_X_Offsets[cou_Index];
            Tier_X_Offsets[cou_Index] = (X_Offset + (Tier_X_Offsets[cou_Index] * X_Padd));
        }

    }

    void center_Screen()
    {
        Tier_Count_Biggest = 0;
        for (int cou_Index = 0; cou_Index < Highest_Tier; cou_Index++)
        {
            if (Tier_Count[cou_Index] > Tier_Count_Biggest) { Tier_Count_Biggest = Tier_Count[cou_Index]; }
        }
        //Center to screen.
        // 1/2 of the screen width minus one half the current rows width.
        int tmp_Center_Offset = PGE->ScreenWidth() / 2;

        X_Offset = tmp_Center_Offset - (Tier_Count_Biggest / 2);

        for (int cou_Index = 0; cou_Index < Highest_Tier; cou_Index++)
        {
            Tier_X_Offsets[cou_Index] = (Tier_Count_Biggest / 2) - (Tier_Count[cou_Index] / 2);
            //std::cout << "\n\n " << cou_Index << ":: " << Tier_X_Offsets[cou_Index];
            Tier_X_Offsets[cou_Index] = (X_Offset + (Tier_X_Offsets[cou_Index] * X_Padd));
        }
    }
    void set_Padding(int p_X_Padd, int p_Y_Padd)
    {
        X_Padd = p_X_Padd;
        Y_Padd = p_Y_Padd;
    }
};


//The basic polymorphic node to use for the node network.
class c_NT3_Base_Node_1D
{
public:
     
    friend class c_NT3_XY_Kernel;

     virtual ~c_NT3_Base_Node_1D(){};
     
     //This is a reference to the network that owns this node. Allows for drawing on params like MAX_X and X_Padd
     c_NT3_XY_Kernel* XY_Kernel;

     //The ID of this node.
     u_Data_3 NID;
     
     //The next node in the Linked List.
     c_NT3_Base_Node_1D * Next;
     
     //Axons and their counters.
     c_NT3_Base_Node_1D ** Axons_L;
     int Axon_Count_L;
     
     c_NT3_Base_Node_1D ** Axons_R;
     int Axon_Count_R;
     
     //Dendrites.
     c_NT3_Base_Node_1D * Dendrite_L;
     c_NT3_Base_Node_1D * Dendrite_R;
     
     //Reinforcement Counters.
     double RC_Lvl;
     
     //Whether or not the node is a state node.
     char Type;
     
     //This is used for things such as the buffer trees or anything that relies on references.
     void * Ref;

     //The tier of the node.
     int Tier;

     //The position used by the GUI.
     int X;
     int Y;

     double Current_Charge;
     
     //Member Functions.
     virtual void set_XY_Kernel(c_NT3_XY_Kernel* p_XY_Kernel = 0)=0;

     virtual void set_Tier(int p_Tier)=0;

     virtual void add_Axon_L(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the left leg.
     virtual void add_Axon_R(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the right leg.
     
     virtual void add_Predefined_Axon_L(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the left leg.
     virtual void add_Predefined_Axon_R(c_NT3_Base_Node_1D * p_Axon)=0; //Adds an axon on the right leg.
     
     virtual void expand_Axon_L(int p_Count)=0; //Adds an axon on the left leg.
     virtual void expand_Axon_R(int p_Count)=0; //Adds an axon on the right leg.
     
     virtual c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_Node)=0;//Searches the axons to see if an upper tier connection exists on the left leg.
     
     virtual bool does_Lower_Connection_Exist_LL(c_NT3_Base_Node_1D * p_Node)=0; //Checks if the given node matches a dendrite on the left leg.
     virtual bool does_Lower_Connection_Exist_LR(c_NT3_Base_Node_1D * p_Node)=0; //Checks if the given node matches a dendrite on the right leg.
     
     virtual void set_Dendrite_L(c_NT3_Base_Node_1D * p_Dendrite)=0; //Sets the left dendrite to the given dendrite.
     virtual void set_Dendrite_R(c_NT3_Base_Node_1D * p_Dendrite)=0; //Sets the right dendrite to the given dendrite.
     
     virtual double reinforce()=0; //Reinforces the node.
     virtual double get_RC_Score()=0; //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     virtual double get_RC_Lvl()=0; //Returns the current reinforcement Lvl.
     
     virtual void set_State(u_Data_3 p_State)=0; //Sets the state if the node is a state node, otherwise it does nothing.
     virtual long long int get_State()=0; //Returns the state if it is a state node otherwise it does nothing.
     
     virtual int get_Type()=0; //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     virtual void set_Type(int p_Type)=0; //Sets the type of node.
     
     virtual void bp_O()=0; //Initiates a backpropagation that outputs to the given table with the given index.
     virtual void bp_L()=0; //bp_Output the left node.
     virtual void bp_R()=0; //bp_Output the right node.

     virtual void output_GUI(olc::PixelGameEngine* pge, int p_X_Offset = 0, int p_X_Offset_Dend = 0, int p_Y_Offset = 0, int p_X_Padd = 0, int p_Y_Padd = 0, double p_HRC = 0.0, int p_X_MAX = 100, int p_Y_MAX = 100, int p_Color_Scheme = 0)=0;
     virtual void output_GUI(int p_Color_Scheme = 0)=0;

     virtual void charge(double p_Charge, double p_APT)=0; //Charge recursively the old way. Will cause issues with enough nodes.
};

//The normal node to use.
class c_NT3_Node_1D: public c_NT3_Base_Node_1D
{
public:
     
     c_NT3_Node_1D()
     {
          //The next node.
          Next = NULL;

          //Axons and their counters.
          Axons_L = NULL;
          Axon_Count_L = 0;
          
          Axons_R = NULL;
          Axon_Count_R = 0;
          
          //Dendrites.
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
               
          //The node type.
          Type = 1;
          
          Ref = NULL;

          Tier = 0;

          X = 0;
          Y = 0;

          XY_Kernel = NULL;

          Current_Charge = 0.0;
     }
     
     ~c_NT3_Node_1D()
     {
          /*
          std::cout << "\n   ~c_NT3_Node_1D " << this << " NID->" << NID.U; std::cout.flush();
          std::cout << " Axons_L->" << Axons_L; std::cout.flush();
          std::cout << " Axon_Count_L->" << Axon_Count_L; std::cout.flush();
          std::cout << " Axons_R->" << Axons_R; std::cout.flush();
          std::cout << " Axon_Count_R->" << Axon_Count_R; std::cout.flush();
          std::cout << " Next->" << Next; std::cout.flush();
          */
          
          if (Axons_L != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_L;cou_Index++)
               {
                    Axons_L[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_L;
               Axons_L = NULL;
          }
          
          if (Axons_R != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_R;cou_Index++)
               {
                    Axons_R[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_R;
               Axons_R = NULL;
          }
          
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //std::cout << " delete Next->" << Next; std::cout.flush();
          //delete Next; //This one.
          Next = NULL;
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.
     
     //Sets the NNet
     void set_XY_Kernel(c_NT3_XY_Kernel * p_XY_Kernel)
     {
         if (XY_Kernel != NULL)
         {
             delete XY_Kernel;
             XY_Kernel = NULL;
         }
         XY_Kernel = p_XY_Kernel;
     }

     //Sets the tier.
     void set_Tier(int p_Tier)
     {
         Tier = p_Tier;
     }

     //Adds an axon on the left leg.
     void add_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_L];
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_L[cou_A];
               Axons_L[cou_A] = NULL;
          }
          
          delete [] Axons_L;
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[Axon_Count_L + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               Axons_L[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++; 
     }
     
     //Adds an axon on the right leg.
     void add_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_R];
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_R[cou_A];
               Axons_R[cou_A] = NULL;
          }
          
          delete [] Axons_R;
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[Axon_Count_R + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               Axons_R[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++;
     }
     
     void add_Predefined_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_L(int p_Count)
     {
          if (Axons_L != NULL){ delete [] Axons_L; }
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_L[cou_A] = NULL;
          }
           
          Axon_Count_L = 0; 
     }
     
     void expand_Axon_R(int p_Count)
     {
          if (Axons_R != NULL){ delete [] Axons_R; }
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_R[cou_A] = NULL;
          }
           
          Axon_Count_R = 0; 
     }
     
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_Node)
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               if (Axons_L[cou_A]->does_Lower_Connection_Exist_LR(p_Node))
               {
                    return Axons_L[cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_LL(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_L == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the right leg.
     bool does_Lower_Connection_Exist_LR(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_R == p_Node){ return 1; }
          return 0; 
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_L(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_L = p_Dendrite;
     }
     
     //Sets the right dendrite to the given dendrite.
     void set_Dendrite_R(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_R = p_Dendrite;
     }
     
     //Reinforces the node.
     double reinforce()
     {
          //RC_Lvl += double (1.0f / RC_Lvl);
          return RC_Lvl++;
     }
     
     //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     double get_RC_Score(){ return RC_Lvl; }
     
     //Returns the current reinforcement Lvl.
     double get_RC_Lvl(){ return RC_Lvl; }
     
     //Sets the state if the node is a state node, otherwise it does nothing.
     void set_State(u_Data_3 p_State){}
     
     //Returns the state if it is a state node otherwise it does nothing.
     long long int get_State(){ return 0; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          std::cout << "<";
          if (Dendrite_L != NULL){ Dendrite_L->bp_L(); } else { std::cout << "(LNULL)"; }
          if (Dendrite_R != NULL){ Dendrite_R->bp_R(); } else { std::cout << "(RNULL)"; }
          std::cout << ">";
     }
     
     //bp_Output the left node.
     void bp_L()
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (Dendrite_L != NULL)
          {
               if (Dendrite_L != NULL){ Dendrite_L->bp_L(); }
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    std::cout << tmp_State;
          }
     }
     
     //bp_Output the right node.
     void bp_R()
     {
          //If a right leg exists then initiate a backpropagation.
          if (Dendrite_R != NULL)
          {
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
                    char tmp_State = char (get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    std::cout << tmp_State;
          }
     }


     
     //Outputs the node graphically based on XY through the olcPGE library.
     //Referenced in Node_Network, CAN, and now Charging Buffer.
     void output_GUI(olc::PixelGameEngine* pge, int p_X_Offset = 0, int p_X_Offset_Dend = 0, int p_Y_Offset = 0, int p_X_Padd = 0, int p_Y_Padd = 0, double p_HRC = 0.0, int p_X_MAX = 100, int p_Y_MAX = 100, int p_Color_Scheme = 0)
     {
         olc::Pixel tmp_Pixel[2];
         double tmp_RC_Lvl = get_RC_Lvl();

         if (p_HRC > 40) { p_HRC = 40; }

         tmp_Pixel[0].a = char((tmp_RC_Lvl / p_HRC) * 250);

         if ((tmp_RC_Lvl / p_HRC) > 1.0) { tmp_Pixel[0].a = 255; }

         tmp_Pixel[1].a = tmp_Pixel[0].a;
         if ((unsigned int)(tmp_Pixel[1].a) > 255) { tmp_Pixel[1].a = 255; }

         //std::cout << "\n char((tmp_RC_Lvl: " << tmp_RC_Lvl << " / p_HRC: " << p_HRC << "): " << (tmp_RC_Lvl / p_HRC) << " * 250): " << unsigned int(char((get_RC_Lvl() / p_HRC) * 250));

         tmp_Pixel[0].r = 255;
         tmp_Pixel[0].g = 0;
         tmp_Pixel[0].b = 0;

         tmp_Pixel[1].r = 0;
         tmp_Pixel[1].g = 255;
         tmp_Pixel[1].b = 0;
         //std::cout << "\n Pixel[0].a :" << int(tmp_Pixel[0].a);
         //std::cout << "\n Pixel[1].a :" << int(tmp_Pixel[1].a);

         int tmp_X[2] = { 0,0 };
         tmp_X[0] = (X * p_X_Padd) + p_X_Offset;
         int tmp_Y[2] = { 0,0 };
         tmp_Y[0] = ((Y * p_Y_Padd) + p_Y_Offset);

         if (((tmp_X[0] > 0) && (tmp_Y[0] > 0)) || ((tmp_X[1] <= p_X_MAX) && (tmp_Y[1] <= p_Y_MAX)))
         {
             if (Dendrite_L != NULL)
             {
                 tmp_X[1] = ((Dendrite_L->X * p_X_Padd) + p_X_Offset_Dend);
                 tmp_Y[1] = ((Dendrite_L->Y * p_Y_Padd) + p_Y_Offset);

                 if (p_Color_Scheme == 0) { pge->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], tmp_Pixel[0]); }
                 if (p_Color_Scheme == 1) { pge->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], olc::GREEN); }
             }
             if (Dendrite_R != NULL)
             {
                 tmp_X[1] = ((Dendrite_R->X * p_X_Padd) + p_X_Offset_Dend);
                 tmp_Y[1] = ((Dendrite_R->Y * p_Y_Padd) + p_Y_Offset);

                 if (p_Color_Scheme == 0) { pge->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], tmp_Pixel[1]); }
                 if (p_Color_Scheme == 1) { pge->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], olc::GREEN); }
             }
             pge->Draw(tmp_X[0], tmp_Y[0]);
         }

     }

     //Outputs the node graphically based on XY through the olcPGE library.
    //Referenced in Node_Network, CAN, and now Charging Buffer.
     //void output_GUI(olc::PixelGameEngine* pge, int p_X_Offset = 0, int p_X_Offset_Dend = 0, int p_Y_Offset = 0, int p_X_Padd = 0, int p_Y_Padd = 0, double p_HRC = 0.0, int p_X_MAX = 100, int p_Y_MAX = 100, int p_Color_Scheme = 0)
     void output_GUI(int p_Color_Scheme = 0)
     {
         olc::Pixel tmp_Pixel[2];
         double tmp_RC_Lvl = get_RC_Lvl();

         int tmp_Highest_RC = XY_Kernel->get_Highest_RC();

         if (tmp_Highest_RC > 40) { tmp_Highest_RC = 40; }

         tmp_Pixel[0].a = char((tmp_RC_Lvl / tmp_Highest_RC) * 250);

         if ((tmp_RC_Lvl / tmp_Highest_RC) > 1.0) { tmp_Pixel[0].a = 255; }

         tmp_Pixel[1].a = tmp_Pixel[0].a;
         if ((unsigned int)(tmp_Pixel[1].a) > 255) { tmp_Pixel[1].a = 255; }

         //std::cout << "\n char((tmp_RC_Lvl: " << tmp_RC_Lvl << " / p_HRC: " << p_HRC << "): " << (tmp_RC_Lvl / p_HRC) << " * 250): " << unsigned int(char((get_RC_Lvl() / p_HRC) * 250));

         tmp_Pixel[0].r = 255;
         tmp_Pixel[0].g = 255;
         tmp_Pixel[0].b = 255;

         tmp_Pixel[1].r = 0;
         tmp_Pixel[1].g = 255;
         tmp_Pixel[1].b = 0;
         //std::cout << "\n Pixel[0].a :" << int(tmp_Pixel[0].a);
         //std::cout << "\n Pixel[1].a :" << int(tmp_Pixel[1].a);

         int tmp_X[2] = { 0,0 };
         tmp_X[0] = (X * XY_Kernel->X_Padd) + XY_Kernel->X_Offset;
         int tmp_Y[2] = { 0,0 };
         tmp_Y[0] = ((Y * XY_Kernel->Y_Padd) + XY_Kernel->Y_Offset);

         if (((tmp_X[0] > 0) && (tmp_Y[0] > 0)) || ((tmp_X[1] <= XY_Kernel->X_MAX) && (tmp_Y[1] <= XY_Kernel->Y_MAX)))
         {
             if (Dendrite_L != NULL)
             {
                 tmp_X[1] = ((Dendrite_L->X * XY_Kernel->X_Padd) + XY_Kernel->Tier_X_Offsets[Tier - 1]); 
                 tmp_Y[1] = ((Dendrite_L->Y * XY_Kernel->Y_Padd) + XY_Kernel->Y_Offset);

                 if (p_Color_Scheme == 0) { XY_Kernel->PGE->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], tmp_Pixel[1]); }
                 if (p_Color_Scheme == 1) { XY_Kernel->PGE->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], olc::BLUE); }
             }
             if (Dendrite_R != NULL)
             {
                 tmp_X[1] = ((Dendrite_R->X * XY_Kernel->X_Padd) + XY_Kernel->Tier_X_Offsets[Tier - 1]);
                 tmp_Y[1] = ((Dendrite_R->Y * XY_Kernel->Y_Padd) + XY_Kernel->Y_Offset);

                 if (p_Color_Scheme == 0) { XY_Kernel->PGE->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], tmp_Pixel[1]); }
                 if (p_Color_Scheme == 1) { XY_Kernel->PGE->DrawLine(tmp_X[0], tmp_Y[0], tmp_X[1], tmp_Y[1], olc::BLUE); }
             }
             XY_Kernel->PGE->Draw(tmp_X[0], tmp_Y[0]);
         }

     }

     void charge(double p_Charge, double p_APT)
     {
         Current_Charge += p_Charge;

         if (Type == 2 || Type == 3)
         {
             //std::cout << "\n Current_Charge: " << Current_Charge;
             //bp_O();
         }
         else
         {
             if (Current_Charge > p_APT)
             {
                 for (int cou_A = 0; cou_A < Axon_Count_L; cou_A++)
                 {
                     Axons_L[cou_A]->charge((Current_Charge * .5), p_APT);
                 }
                 for (int cou_A = 0; cou_A < Axon_Count_R; cou_A++)
                 {
                     Axons_R[cou_A]->charge((Current_Charge * .5), p_APT);
                 }


                 output_GUI(1);
                 Current_Charge == 0.0;
             }
         }
     }
};

//State node.
class c_NT3_State_Node_1D: public c_NT3_Base_Node_1D
{
private:
     
public:
     
     //The state.
     u_Data_3 State;
     
     c_NT3_State_Node_1D()
     {
          State.I = 0;
          
          //Node network.
          XY_Kernel = NULL;

          //The linked list.
          Next = NULL;
          
          //Axons and their counters.
          Axons_L = NULL;
          Axon_Count_L = 0;
          
          Axons_R = NULL;
          Axon_Count_R = 0;
          
          //Dendrites.
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //Reinforcement Counters.
          RC_Lvl = 1.0;
          
          //The nodes type.
          Type = 0;
          
          Ref = NULL;

          X = 0;
          Y = 0;
     }
     
     ~c_NT3_State_Node_1D()
     {
          //std::cout << " ~c_NT3_State_Node_1D " << this << " NID->" << NID.U; std::cout.flush();
          
          if (Axons_L != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_L;cou_Index++)
               {
                    Axons_L[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_L;
               Axons_L = NULL;
          }
          
          if (Axons_R != NULL)
          {
               for (int cou_Index=0;cou_Index<Axon_Count_R;cou_Index++)
               {
                    Axons_R[cou_Index] = NULL; //Node destruction is handled by the linked list.
               }
               delete [] Axons_R;
               Axons_R = NULL;
          }
          
          Dendrite_L = NULL;
          Dendrite_R = NULL;
          
          //std::cout << " delete Next->" << Next; std::cout.flush();
          
          delete Next; //This one.
          
          //std::cout << "   ~~~c_NT3_State_Node_1D " << this << " NID->" << NID.U; std::cout.flush();
          
          Ref = NULL;
     }
     
     
     //==--  Member Functions.

     //Sets the NNet
     void set_XY_Kernel(c_NT3_XY_Kernel* p_XY_Kernel)
     {
         if (XY_Kernel != NULL)
         {
             delete XY_Kernel;
             XY_Kernel = NULL;
         }
         XY_Kernel = p_XY_Kernel;
     }

     //Sets the tier.
     void set_Tier(int p_Tier)
     {
         Tier = p_Tier;
     }

     //Adds an axon on the left leg.
     void add_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_L];
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_L[cou_A];
               Axons_L[cou_A] = NULL;
          }
          
          delete [] Axons_L;
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[Axon_Count_L + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               Axons_L[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++; 
     }
     
     //Adds an axon on the right leg.
     void add_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          c_NT3_Base_Node_1D ** tmp_Axons;
          tmp_Axons = new c_NT3_Base_Node_1D*[Axon_Count_R];
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               tmp_Axons[cou_A] = Axons_R[cou_A];
               Axons_R[cou_A] = NULL;
          }
          
          delete [] Axons_R;
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[Axon_Count_R + 1]; 
          
          for (int cou_A=0;cou_A<Axon_Count_R;cou_A++)
          {
               Axons_R[cou_A] = tmp_Axons[cou_A];
               tmp_Axons[cou_A] = NULL;
          }
          delete [] tmp_Axons;
          tmp_Axons = NULL;
          
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++; 
     }
     
     //Adds an axon connection to the given index, it assumes the array is already defined and will not overbound.
     void add_Predefined_Axon_L(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_L[Axon_Count_L] = p_Axon; 
          Axon_Count_L++;
     }
     
     void add_Predefined_Axon_R(c_NT3_Base_Node_1D * p_Axon)
     {
          Axons_R[Axon_Count_R] = p_Axon; 
          Axon_Count_R++;
     }
     
     //Destroys the current axons if any and expands the array to the given depth.
     void expand_Axon_L(int p_Count)
     {
          if (Axons_L != NULL){ delete [] Axons_L; }
          Axons_L = NULL;
          
          Axons_L = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_L[cou_A] = NULL;
          }
           
          Axon_Count_L = 0; 
     }
     
     void expand_Axon_R(int p_Count)
     {
          if (Axons_R != NULL){ delete [] Axons_R; }
          Axons_R = NULL;
          
          Axons_R = new c_NT3_Base_Node_1D*[p_Count]; 
          
          for (int cou_A=0;cou_A<p_Count;cou_A++)
          {
               Axons_R[cou_A] = NULL;
          }
           
          Axon_Count_R = 0; 
     }
     //Searches the axons to see if an upper tier connection exists.
     c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_Node)
     {
          //Check the axons on the right side for the 
          for (int cou_A=0;cou_A<Axon_Count_L;cou_A++)
          {
               if (Axons_L[cou_A]->does_Lower_Connection_Exist_LR(p_Node))
               {
                    return Axons_L[cou_A];
               }
          }
          return NULL; 
     }
     
     //Checks if the given node matches a dendrite on the left leg.
     bool does_Lower_Connection_Exist_LL(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_L == p_Node){ return 1; }
          return 0; 
     }
     
     //Checks if the given node matches a dendrite on the right leg.
     bool does_Lower_Connection_Exist_LR(c_NT3_Base_Node_1D * p_Node)
     { 
          if (Dendrite_R == p_Node){ return 1; }
          return 0; 
     }
     
     //Sets the left dendrite to the given dendrite.
     void set_Dendrite_L(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_L = p_Dendrite;
     }
     
     //Sets the right dendrite to the given dendrite.
     void set_Dendrite_R(c_NT3_Base_Node_1D * p_Dendrite)
     {
          Dendrite_R = p_Dendrite;
     }
     
     //Reinforces the node.
     double reinforce()
     {

          RC_Lvl++;
          return RC_Lvl;
          //RC_Lvl += double (1.0f / int (RC_Lvl));
     }
     
     //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     double get_RC_Score(){ return RC_Lvl; }
     
     //Returns the current reinforcement Lvl.
     double get_RC_Lvl(){ return RC_Lvl; }
     
     //Sets the state if the node is a state node, otherwise it does nothing.
     void set_State(u_Data_3 p_State){ State = p_State; }
     
     //Returns the state if it is a state node otherwise it does nothing.
     long long int get_State(){ return State.I; }
     
     //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     int get_Type(){ return Type; }
     
     //Sets the type of node.
     void set_Type(int p_Type){ Type = p_Type; }
     
     //Initiates a backpropagation that outputs to the given table with the given index.
     void bp_O() 
     {
          std::cout << "~<";
          std::cout << State.C;
          std::cout << ">~";
     }
     
     //bp_Output the left node.
     void bp_L()
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (Dendrite_L != NULL)
          {
               if (Dendrite_L != NULL){ Dendrite_L->bp_L(); }
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
               char tmp_State = char (get_State());
               if (tmp_State == char (0)){ tmp_State = '?'; }
               if (tmp_State == char (7)){ tmp_State = '?'; }
               if (tmp_State == char (8)){ tmp_State = '?'; }
               if (tmp_State == char (9)){ tmp_State = '?'; }
               if (tmp_State == char (10)){ tmp_State = '?'; }
               if (tmp_State == char (13)){ tmp_State = '?'; }
               std::cout << tmp_State;
          }
     }
     
     //bp_Output the right node.
     void bp_R()
     {
          //If a right leg exists then initiate a backpropagation.
          if (Dendrite_R != NULL)
          {
               if (Dendrite_R != NULL){ Dendrite_R->bp_R(); }
          }
          else
          {
               char tmp_State = char (get_State());
               if (tmp_State == char (0)){ tmp_State = '?'; }
               if (tmp_State == char (7)){ tmp_State = '?'; }
               if (tmp_State == char (8)){ tmp_State = '?'; }
               if (tmp_State == char (9)){ tmp_State = '?'; }
               if (tmp_State == char (10)){ tmp_State = '?'; }
               if (tmp_State == char (13)){ tmp_State = '?'; }
               std::cout << tmp_State;
          }
     }

     
     //Outputs the node graphically based on XY through the olcPGE library.
     void output_GUI(olc::PixelGameEngine* pge, int p_X_Offset = 0, int p_X_Offset_Dend = 0, int p_Y_Offset = 0, int p_X_Padd = 0, int p_Y_Padd = 0, double p_HRC = 0.0, int p_X_MAX = 100, int p_Y_MAX = 100, int p_Color_Scheme = 0)
     {
         if ((p_X_Padd > 0) && (p_Y_Padd > 0))
         {
             pge->Draw(((X * p_X_Padd) + p_X_Offset), ((Y * p_Y_Padd) + p_Y_Offset));
         }
         else
         {
             pge->Draw(((X * XY_Kernel->X_Padd) + p_X_Offset), ((Y * XY_Kernel->Y_Padd) + p_Y_Offset));
         }
         /*
         for (int cou_AL = 0; cou_AL < Axon_Count_L; cou_AL++)
         {
             pge->DrawLine((X * X_Padd), (Y * Y_Padd), (Axons_L[cou_AL]->X * X_Padd), (Axons_L[cou_AL]->Y * Y_Padd), olc::BLUE);
         }

         for (int cou_AR = 0; cou_AR < Axon_Count_R; cou_AR++)
         {
             pge->DrawLine((X * X_Padd), (Y * Y_Padd), (Axons_R[cou_AR]->X * X_Padd), (Axons_R[cou_AR]->Y * Y_Padd), olc::RED);
         }
         */

     }
     
     //Outputs the node graphically based on XY through the olcPGE library.
     void output_GUI(int p_Color_Scheme)
     {
         XY_Kernel->PGE->Draw(((X * XY_Kernel->X_Padd) + XY_Kernel->X_Offset), ((Y * XY_Kernel->Y_Padd) + XY_Kernel->Y_Offset));
         /*
         for (int cou_AL = 0; cou_AL < Axon_Count_L; cou_AL++)
         {
             pge->DrawLine((X * X_Padd), (Y * Y_Padd), (Axons_L[cou_AL]->X * X_Padd), (Axons_L[cou_AL]->Y * Y_Padd), olc::BLUE);
         }

         for (int cou_AR = 0; cou_AR < Axon_Count_R; cou_AR++)
         {
             pge->DrawLine((X * X_Padd), (Y * Y_Padd), (Axons_R[cou_AR]->X * X_Padd), (Axons_R[cou_AR]->Y * Y_Padd), olc::RED);
         }
         */

     }


     void charge(double p_Charge, double p_APT)
     {
         Current_Charge += p_Charge;

         while (Current_Charge > p_APT)
         {
             for (int cou_A = 0; cou_A < Axon_Count_L; cou_A++)
             {
                 Axons_L[cou_A]->charge(1.0, p_APT);
             }
             for (int cou_A = 0; cou_A < Axon_Count_R; cou_A++)
             {
                 Axons_R[cou_A]->charge(1.0, p_APT);
             }

             if (Type == 2 || Type == 3)
             {
                 std::cout << "\n Current_Charge: " << Current_Charge;
                 bp_O();
             }

             output_GUI(1);

             Current_Charge -= 1.0;
             if (Current_Charge < 0) { Current_Charge = 0.0; }
         }
     }
};
