
//I will need a base matrix to map the whole output too. Then I will need to ability to navigate and selectively 
// display a small viewscreen.
class c_Shipyard_Canvas
{
public:
     
     int Width;
     int Height;
     
     //A muldimensional character map that can be dynamically reallocated.
     char ** Canvas;
     
     //The color to output the characters as.
     int ** Canvas_Color;
     
     //The viewscreen that can be moved and ajusted.
     int V_Width;
     int V_Height;
     
     //The current coordinates for the corner of the viewscreen.
     int V_X;
     int V_Y;
     
     c_Shipyard_Canvas()
     {
          Width = 0;
          Height = 0;
          
          Canvas = NULL;
          Canvas_Color = NULL;
          
          V_Width = 0;
          V_Height = 0;
          
          V_X = 0;
          V_Y = 0;
          
          resize_Canvas(1000, 1000);
          resize_Viewscreen(100, 100);
     }
     
     ~c_Shipyard_Canvas()
     {
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               delete [] Canvas[cou_X]; Canvas[cou_X] = NULL;
               delete [] Canvas_Color[cou_X]; Canvas_Color[cou_X] = NULL;
          }
          
          delete [] Canvas; Canvas = NULL;
          delete [] Canvas_Color; Canvas_Color = NULL;
     }
     
     //==--      CANVAS MANIPULATION
     
     //Display Whole Canvas.
     void display_Raw_Canvas(int p_DX = 0, int p_DY = 0)
     {
          for (int cou_Y=0;cou_Y<Height;cou_Y++)
          {
               xy(p_DX, (cou_Y + p_DY));
               
               for (int cou_X=0;cou_X<Width;cou_X++)
               {
                    ochr(0, Canvas_Color[cou_X] [cou_Y], Canvas[cou_X] [cou_Y]);
               }
          }
     }
     
     //Update a portion of the canvas with a string.
     void plot_String(int p_X, int p_Y, std::string p_String, int p_Color = 10)
     {
          if (p_X >= Width){ return; }
          if (p_Y >= Height){ return; }
          
          for (int cou_X=p_X;cou_X<(int (p_String.size()) + p_X);cou_X++)
          {
               if (cou_X >= Width){ return; }
               Canvas[cou_X] [p_Y] = p_String[(cou_X - p_X)];
               Canvas_Color[cou_X] [p_Y] = p_Color;
          }
     }
     
     //Update a portion of the canvas with a string.
     void plot_String(int p_X, int p_Y, char p_Char[], int p_Size, int p_Color = 10)
     {
          if (p_X >= Width){ return; }
          if (p_Y >= Height){ return; }
          
          for (int cou_X=p_X;cou_X<(p_Size + p_X);cou_X++)
          {
               if (cou_X >= Width){ return; }
               Canvas[cou_X] [p_Y] = p_Char[(cou_X - p_X)];
               Canvas_Color[cou_X] [p_Y] = p_Color;
          }
     }
     
     //Update a portion of the canvas with a character array.
     
     //Update a portion of the canvas with a character.
     void plot_Character(int p_X, int p_Y, char p_Char, int p_Color = 10)
     {
          if (p_X >= Width){ return; }
          if (p_Y >= Height){ return; }
          
          if (p_X < 0){ return; }
          if (p_Y < 0){ return; }
          
          Canvas[p_X] [p_Y] = p_Char;
          Canvas_Color[p_X] [p_Y] = p_Color;
     }
     
     
     //Outputs a line in the console window from the first set of coordinates to the second set of coordinates.
     void plot_Line(int p_X1, int p_Y1, int p_X2, int p_Y2, char p_Char, int p_Color = 10, float p_Percentage = 1.0)
     {
          if (p_X1 >= Width){ return; }
          if (p_Y1 >= Height){ return; }
          
          if (p_X2 >= Width){ return; }
          if (p_Y2 >= Height){ return; }
          
          //Get the rise over run.
		  float tmp_Rise = float(p_Y2 - p_Y1);
		  float tmp_Run = float(p_X2 - p_X1);
          
          //The increment for rise and run.
          float tmp_Rise_Inc = 0;
          float tmp_Run_Inc = 0;
          
          //The temporary positions.
          float tmp_X = 0;
          float tmp_Y = 0;
          
          if (absolutef(tmp_Rise) >= absolutef(tmp_Run))
          {
               //If the tmp_Rise is larger then reduce by the rise.
               tmp_Rise_Inc = tmp_Rise / absolutef(tmp_Rise);
               tmp_Run_Inc = tmp_Run / absolutef(tmp_Rise);
               
     //          cout << "\n ~Ri >= ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
               
               for (int cou_Y=0;cou_Y<(int (absolutef(tmp_Rise) * p_Percentage));cou_Y++)
               {
                    //Increase the current position.
                    tmp_X += tmp_Run_Inc;
                    tmp_Y += tmp_Rise_Inc;
                    
                    //Output the current position.
                    plot_Character(int(tmp_X + p_X1), int(tmp_Y + p_Y1), p_Char, p_Color);
               }
          }
          if (absolutef(tmp_Rise) < absolutef(tmp_Run))
          {
               //If the tmp_Run is larger then reduce by the tmp_Run.
               tmp_Rise_Inc = tmp_Rise / absolutef(tmp_Run);
               tmp_Run_Inc = tmp_Run / absolutef(tmp_Run);
               
     //          cout << "\n ~Ri < ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
               
               for (int cou_X=0;cou_X<absolute(int ((tmp_Run) * p_Percentage));cou_X++)
               {
                    //Increase the current position.
                    tmp_X += tmp_Run_Inc;
                    tmp_Y += tmp_Rise_Inc;
                    
                    //Output the current position.
                    plot_Character(int(tmp_X + p_X1), int(tmp_Y + p_Y1), p_Char, p_Color);
               }
          }
     }
     
     //==--      VIEWSCREEN MANIPULATION
     
     //Display Viewscreen.
     void display_Viewscreen(int p_DX = 0, int p_DY = 0)
     {
          for (int cou_Y=V_Y;cou_Y<(V_Y + V_Height);cou_Y++)
          {
               if (cou_Y >= Height){ ochr(0, 0, '?'); }
               if (cou_Y < 0){ ochr(0, 0, '?'); }
               
               xy(p_DX, (cou_Y + p_DY - V_Y));
               
               for (int cou_X=V_X;cou_X<(V_X + V_Width);cou_X++)
               {
                    if (cou_X >= Width || cou_X < 0)
                    { 
                         ochr(0, 0, '?'); 
                         continue; 
                    }
                    else
                    {
                         ochr(0, Canvas_Color[cou_X] [cou_Y], Canvas[cou_X] [cou_Y]);
                    }
                         
               }
          }
     }
     
     //Scrolls (the viewscreen in the given direction.
     //0 == Left, 1 == Right, 2 == Up, 3 == Down.
     int scroll_Viewscreen(int p_Direction = 0)
     {
          if (p_Direction == 0)
          {
               return move_Viewscreen((V_X - 1), V_Y);
          }
          if (p_Direction == 1)
          {
               return move_Viewscreen((V_X + 1), V_Y);
          }
          if (p_Direction == 2)
          {
               return move_Viewscreen(V_X, (V_Y - 1));
          }
          if (p_Direction == 3)
          {
               return move_Viewscreen(V_X, (V_Y + 1));
          }
          return 0;
     }
     
     //Move Viewscreen to given coordinates.
     int move_Viewscreen(int p_X, int p_Y)
     {
          if ((p_X + V_Width) >= Width || p_X < 0){ return 0; }
          if ((p_Y + V_Height) >= Height || p_Y < 0){ return 0; }
          V_X = p_X;
          V_Y = p_Y;
          return 1;
     }
     
     //==--      ARRAY MANAGMENT
     
     //Resize the canvas.
     void resize_Canvas(int p_Width, int p_Height)
     {
          for (int cou_X=0;cou_X<Width;cou_X++)
          {
               delete [] Canvas[cou_X]; Canvas[cou_X] = NULL;
               delete [] Canvas_Color[cou_X]; Canvas_Color[cou_X] = NULL;
          }
          
          delete [] Canvas; Canvas = NULL;
          delete [] Canvas_Color; Canvas_Color = NULL;
          
          Canvas = new char*[p_Width];
          Canvas_Color = new int*[p_Width];
          for (int cou_X=0;cou_X<p_Width;cou_X++)
          {
               Canvas[cou_X] = new char[p_Height];
               Canvas_Color[cou_X] = new int[p_Height];
               
               for (int cou_Y=0;cou_Y<p_Height;cou_Y++)
               {
                    Canvas[cou_X] [cou_Y] = ' ';
                    Canvas_Color[cou_X] [cou_Y] = 0;
               }
          }
          
          Width = p_Width;
          Height = p_Height;
     }
     
     //Resize the viewscreen.
     void resize_Viewscreen(int p_Width, int p_Height)
     {
          V_Width = p_Width;
          V_Height = p_Height;
     }
};

