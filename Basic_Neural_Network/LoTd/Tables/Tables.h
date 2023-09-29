
class c_Data_IMG
{
public:
     
     u_Data_3 ** DATA;
     int Width;
     int Height;
     
     c_Data_IMG()
     {
          Width = 0;
          Height = 0;
          DATA = NULL;
          
          initialize_Data();
     }
     
     ~c_Data_IMG()
     {
          reset();
     }
     
     void resize(int p_X, int p_Y)
     {
          reset();
          
          Width = p_X;
          Height = p_Y;
          
          DATA = new u_Data_3*[Width];
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               DATA[cou_X] = new u_Data_3[Height];
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    DATA[cou_X] [cou_Y].U = 0;
               }
          }
     }
     
     void reset()
     {
          if (DATA != NULL)
          {
               for (int cou_X=0;cou_X<Width;cou_X++)
               {
                    delete [] DATA[cou_X]; DATA[cou_X] = NULL;
               }
               delete [] DATA; DATA = NULL;
          }
     }
     
     void initialize_Data()
     {
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    DATA[cou_X] [cou_Y].F = 0.0f;
               }
          }
     }
     
     void randomize_Data()
     {
          int tmp_Result = 0;
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    tmp_Result = (rand() % 10);
                    
                    DATA[cou_X] [cou_Y].F = 0;
                    if (tmp_Result == 0)
                    { 
                         tmp_Result = (rand() % 10);
                         if (tmp_Result == 0)
                         {
                              DATA[cou_X] [cou_Y].F = 0; 
                         }
                    }
                    if (tmp_Result == 1)
                    { 
                         tmp_Result = (rand() % 6);
                         if (tmp_Result == 0)
                         {
                              DATA[cou_X] [cou_Y].F = 1; 
                         }
                    }
                    if (tmp_Result == 2)
                    { 
                         tmp_Result = (rand() % 4);
                         if (tmp_Result == 0)
                         {
                              DATA[cou_X] [cou_Y].F = 2; 
                         }
                    }
                    if (tmp_Result == 3)
                    { 
                         tmp_Result = (rand() % 10);
                         if (tmp_Result == 0)
                         {
                              DATA[cou_X] [cou_Y].F = 3; 
                         }
                    }
                    if (tmp_Result == 4)
                    { 
                         tmp_Result = (rand() % 15);
                         if (tmp_Result == 0)
                         {
                              DATA[cou_X] [cou_Y].F = 4; 
                         }
                    }
               }
          }
     }
     
     void randomize_Minor_Data()
     {
          int tmp_Result = (rand() % 10);
          
          int tmp_X = (rand() % Width);
          int tmp_Y = (rand() % Height);
          
          DATA[tmp_X] [tmp_Y].F = 0;
          if (tmp_Result == 0)
          { 
               DATA[tmp_X] [tmp_Y].F = 0; 
          }
          if (tmp_Result == 1)
          { 
               DATA[tmp_X] [tmp_Y].F = 1; 
          }
          if (tmp_Result == 2)
          { 
               DATA[tmp_X] [tmp_Y].F = 2; 
          }
          if (tmp_Result == 3)
          { 
               DATA[tmp_X] [tmp_Y].F = 3; 
          }
          if (tmp_Result == 4)
          { 
               DATA[tmp_X] [tmp_Y].F = 4; 
          }
     }
          
     void randomize_Pixel(int p_X, int p_Y)
     {
          int tmp_Result = (rand() % 10);
          
          DATA[p_X] [p_Y].F = 0;
          if (tmp_Result == 0)
          { 
               DATA[p_X] [p_Y].F = 0; 
          }
          if (tmp_Result == 1)
          { 
               DATA[p_X] [p_Y].F = 1; 
          }
          if (tmp_Result == 2)
          { 
               DATA[p_X] [p_Y].F = 2; 
          }
          if (tmp_Result == 3)
          { 
               DATA[p_X] [p_Y].F = 3; 
          }
          if (tmp_Result == 4)
          { 
               DATA[p_X] [p_Y].F = 4; 
          }
     }
          
     void set_Pixel(int p_X, int p_Y, int p_Bit)
     {
          DATA[p_X] [p_Y].F = p_Bit;
     }
     
     void output_IMG()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy((cou_X + tmp_X), (cou_Y + tmp_Y));
                    ochr(1, (int (DATA[cou_X] [cou_Y].F)), char (176)); continue;
               }
          }
          
     }
     
     void output_IMG_Grayscale()
     {
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               for (int cou_Y=0;cou_Y<Height;cou_Y++)
               {
                    xy((cou_X + tmp_X), (cou_Y + tmp_Y));
                    //ochr(1, (int (DATA[cou_X] [cou_Y].F)), char (176)); continue;
                    
                    //Is the data over the range? If yes then continue so that it does not fall through.
                    if (DATA[cou_X] [cou_Y].F < 1 ){ ochr(0, 0, char(176)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 2 ){ ochr(0, 8, char(176)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 3 ){ ochr(0, 8, char(177)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 4 ){ ochr(0, 8, char(178)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 5 ){ ochr(0, 7, char(177)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 6 ){ ochr(0, 7, char(178)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 7 ){ ochr(0, 15, char(177)); continue; }
                    if (DATA[cou_X] [cou_Y].F < 8 ){ ochr(0, 15, char(178)); continue; }
                    //Fall through.
                    ochr(0, 15, char(178)); continue;
               }
          }
     }
};



//Includes for the tables.
#include "c_Cell.h"
#include "c_Row.h"
#include "c_Table.h"
#include "c_Table_Handler.h"
#include "c_Raw_Table_1D.h"
#include "c_Raw_Table_2D.h"
#include "c_Raw_Table_3D.h"
