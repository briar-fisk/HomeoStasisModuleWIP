
#ifndef OLC_PGEX_NT3_H
#define OLC_PGEX_NT3_H

#include <cstdint>
#include <iostream>
#include <fstream>
#include <windows.h>
#include <sstream>

//Used by the console for drawing lines.
#include <cstdlib>
#include <cmath>
#include <math.h>
#include <chrono>
#include <thread>

#include "olcPixelGameEngine.h"



namespace olc
{
    namespace NT3
    {


        
        #include "NT3_Definitions.h"

        #include "c_Console.h"

        #include "c_Name_Lookup_Tree.h"

            //Includes for the tables.
        #include "c_Cell.h"
        #include "c_Row.h"
        #include "c_Table.h"
        #include "c_Table_Handler.h"
        #include "c_Raw_Table_1D.h"


        //The init routines for libraries contained within such as console.
            void init_LoTd()
            {
                init_Console();
                set_Console_Color(0, 10);
            }


            //The base NT3 class that allows for hooking into some basic API manipulations with polymorphic classes.
            class c_NT3_Base
            {
            private:

            public:

                int X;
                int Y;
                int Width;
                int Height;
                int Object_ID;

                c_NT3_Base()
                {

                }

                ~c_NT3_Base()
                {

                }
            };


            //--     Includes for this library.


            //Includes for the base nodes.
        #include "c_NT3_Node_1D.h"

        //Includes the state tree.
        #include "c_NT3_Fractal_Tree.h"


        //Includes for the Node Network.
        #include "c_NT3_Node_Network_1D.h"

                //Includes the buffer tree.
        #include "c_NT3_Buffer.h"
        #include "c_NT3_Data_Buffer.h"


        //Includes the Vision_buffer tree.
        #include "c_NT3_Vision_Buffer.h"

        //Includes for the CAN.
        //#include "CAN/c_NT3_CAN_1D.h"
        #include "c_NT3_CAN_1D.h"


        //Includes for the Constructs.
        #include "c_NT3_Base_Construct.h"
        #include "c_NT3_Construct_1D.h"
        
    }
}



#endif