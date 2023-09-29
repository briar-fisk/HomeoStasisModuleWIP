/*
 *   ---========================================---
 * ---===     LoTd General Purpose Library     ===---
 *   ---========================================---
 *
 * This file contains the includes for the general use LoTd library
 * files to be used across projects, such as the u_Data or Tables.
 *
 */






//--     Includes for this library.

//--  Global Variables.

//The number of cores available for threads.
int Central_Core_Count;

//The mutex for thread protections.
//std::mutex Mutex;

//--  Low Level Classes.

//Include for the definitions used in the NeuroTox.2 such as u_Data.
#include "Definitions.h"

//For the mathematical functions such as absolute().
#include "c_Math.h"

//For working with console graphics.
#include "Console_Graphics/Console_Graphics.h"

//Includes the binary (and non-binary) trees.
#include "Bin_Trees/Bin_Trees.h"

//Includes the KENO Ticket.
#include "c_Keno.h"

//Includes the utilities such as the memory mapping class.
#include "Utilities/Utilities.h"

//--  High Level Classes (Dependent upon lower classes)

//This include allows access to the table handler and the tables.
#include "Tables/Tables.h"

//For working with graphics
//#include "Image/Image.h"

//This allows for the analysis of data for things such as walk forward analysis.
#include "Data_Analysis/Data_Analysis.h"

//The command text parser separated as standalone.
#include "Command_Text/Command_Text.h"

//The init routines for libraries contained within such as console.
void init_LoTd(int p_Central_Core_Count = 1)
{
     init_Console();
	 set_Console_Color(0, 10);
     Central_Core_Count = p_Central_Core_Count;
}
