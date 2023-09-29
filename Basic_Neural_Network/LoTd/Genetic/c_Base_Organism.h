
//The organism class holds DNA of an organism as well as its health status.
//The health is modified by a call to damage() and grow()
class c_Base_Organism
{
     //This is the DNA for the current organism.
     c_DNA DNA;
     
     //The health of the organism.
     float HP;
     
     //The total HP gained and lost.
     float HP_Total;
     
     //The number of ticks the organism has lived.
     int Age;
     
     //The average change in health over time. HP_Total / Age
     float Average_HP_Change;
     
     //Whether or not the organism is alive.
     int State;
     
     c_Base_Organism(int p_DNA_Length, int p_DNA_Ranges[], string p_Name, int p_HP)
     {
          //Initialize the DNA
          DNA.init(p_DNA_Length, p_DNA_Ranges);
          
          //Set the name.
          Name = p_Name;
          
          //Sets the HP.
          HP = p_HP;
          
          //Age is one, to avoid the /0 error.
          Age = 1;
          
          //Currently alive.
          State = 1;
     }
     
     ~c_Base_Organism()
     {
          
     }
     
     //Initializes the organism for dynamic allocation.
     c_Init(int p_DNA_Length, int p_DNA_Ranges[], string p_Name, int p_HP)
     {
          //Initialize the DNA, randomizes every bit of DNA based on given ranges.
          DNA.init(p_DNA_Length, p_DNA_Ranges);
          
          //Set the name.
          Name = p_Name;
          
          //Sets the HP.
          HP = p_HP;
          
          //Age is one, to avoid the /0 error.
          Age = 1;
          
          //Currently alive.
          State = 1;
     }
};
