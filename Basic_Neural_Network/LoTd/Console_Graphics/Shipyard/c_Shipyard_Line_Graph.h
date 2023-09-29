//Create the graphs to use.
class c_Shipyard_Line_Graph
{
public:
     //The name of the graph.
    std::string Name;

     //The names of the datasets.
     std::string * Data_Set_Name;

     //The plot field.
     c_Shipyard_Canvas Canvas;

     //The Data sets to plot.
     double ** Data;

     //The background controlling dataset.
     double * Background_Data;

     //The number of data sets to plot.
     int Number_Of_Data_Sets;

     //The depth of the data sets.
     int Data_Set_Depth;

     //The Colors to use for the data sets.
     //int * Data_Set_Color;

     //The characters to plot for the data sets.
     //char * Data_Set_Characters;


     //These values are used for the plotting of the data sets.
     //All of the data points must be formatted to fit the window.

     //Take the Highest Value - The Lowest to get the range. Then divide the data by the range to get the percentage.
     double Lowest_Value;
     double Highest_Value;
     double Range;

     //The height and width of the canvas.
     //The height is used in modifying the values to fit into the given space.
     int Height;
     int Width;

     c_Shipyard_Line_Graph()
     {
          Name = "Line_Graph";
          Data = NULL;
          Data_Set_Name = NULL;

          //Data_Set_Color = NULL;
          //Data_Set_Characters = NULL;
          
          Background_Data = NULL;
          Background_Data = new double[1];
          Background_Data[0] = 0.0f;

          Number_Of_Data_Sets = 0;
          Data_Set_Depth = 0;
          Height = 50;
     }

     c_Shipyard_Line_Graph(std::string p_Name)
     {
          Name = p_Name;
          Data = NULL;
          Data_Set_Name = NULL;

          //Data_Set_Color = NULL;
          //Data_Set_Characters = NULL;
          
          Background_Data = NULL;
          Background_Data = new double[1];
          Background_Data[0] = 0.0f;

          Number_Of_Data_Sets = 0;
          Data_Set_Depth = 0;
          Height = 200;
     }

     ~c_Shipyard_Line_Graph()
     {
          //Copy the old ones over.
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               delete [] Data[cou_Index];
               Data[cou_Index] = NULL;
          }
          
          //delete [] Data_Set_Color; Data_Set_Color = NULL;
          //delete [] Data_Set_Characters; Data_Set_Characters = NULL;
          delete [] Data; Data = NULL;
          delete [] Background_Data; Background_Data = NULL;
          delete [] Data_Set_Name; Data_Set_Name = NULL;
          
     }

     //Sets the name.
     void set_Name(std::string p_Name)
     {
          Name = p_Name;
     }
          
	//Gets the name.
     std::string get_Name()
     {
          return Name;
     }

     //Sets the height.
     void set_Height(int p_Height)
     {
     	Height = p_Height;
     }

     //Submits a name for a dataset.
     void submit_Data_Set_Name(int p_Data_Set, std::string p_Name)
     {
          increase_Number_Of_Data_Sets(p_Data_Set + 1);

          Data_Set_Name[p_Data_Set] = p_Name;
     }

     //Submits a point to a dataset.
     void submit_Data_Point(int p_Data_Set, int p_X, double p_Data)
     {
          increase_Number_Of_Data_Sets(p_Data_Set + 1);
          increase_Data_Set_Depth(p_X + 1);

          Data[p_Data_Set] [p_X] = p_Data;
     }

     //Submits a point to the background dataset.
     void submit_Background_Data(int p_X, double p_Data)
     {
          increase_Data_Set_Depth(p_X + 1);

          Background_Data[p_X] = p_Data;
     }

     //Outputs the graph.
     void output(int p_X = 0, int p_Y = 10, int p_Height = 250, int p_Width = 10)
     {
          int B_X = 55 + ((Number_Of_Data_Sets / p_Height) * 55);
          int B_Y = 10;

          int B_S = p_Width;

          int B_H = 10;
          
          Width = Data_Set_Depth * B_S;

		  Height = p_Height;

          //--  Gather the data necessary to label the Y axis and to plot the points.

          //Get the range.
          Lowest_Value = 0.0f;
          Highest_Value = 0.0f;

          if (Number_Of_Data_Sets > 0)
          {
               if (Data_Set_Depth > 0)
               {
                    Lowest_Value = Data[0] [0];
                    Highest_Value = Data[0] [0];
               }
          }
          Range = 0.0f;

          //Get the high and low values to calculate the range.
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    if (Data[cou_Index] [cou_D] > Highest_Value){ Highest_Value = Data[cou_Index] [cou_D]; }
                    if (Data[cou_Index] [cou_D] < Lowest_Value){ Lowest_Value = Data[cou_Index] [cou_D]; }
               }
          }

          Range = Highest_Value - Lowest_Value;


          //Compute the Y positions.
          double tmp_Raw_Y = 0.0f;
          double tmp_Y = 0.0f;

          double tmp_Y_Prev = 0.0f;
          double tmp_X_Prev = 0.0f;




          //--  Set the canvas size.
          Canvas.resize_Canvas(Width + B_X + 1, Height + B_Y + 1);

          //--  Add the accessories to the canvas.
          //Canvas Border.
          Canvas.plot_Character(0, 0, char (201), 15);
          Canvas.plot_Character(0, (Height + B_Y), char (200), 15);

          for (int cou_Index=1;cou_Index<(Height + B_Y);cou_Index++)
          {
               Canvas.plot_Character(0, cou_Index, char (186), 15);
               Canvas.plot_Character(Width + B_X, cou_Index, char (186), 15);
          }

          Canvas.plot_Character(Width + B_X, 0, char (187), 15);
          Canvas.plot_Character(Width + B_X, (Height + B_Y), char (188), 15);

          for (int cou_Index=1;cou_Index<(Width + B_X);cou_Index++)
          {
               Canvas.plot_Character(cou_Index, 0, char (205), 15);
               Canvas.plot_Character(cou_Index, (Height + B_Y), char (205), 15);
          }

          Canvas.plot_String(1, 0, Name, 160);

          //The graph border
          Canvas.plot_Character((B_X - 3), 1, char (201), 8);
          Canvas.plot_Character((B_X - 3), (Height + 3), char (200), 8);

          Canvas.plot_Character(Width + (B_X - 1), 1, char (187), 8);
          Canvas.plot_Character(Width + (B_X - 1), (Height + 3), char (188), 8);


          double tmp_Y_Axis = 0;
          std::string tmp_Y_Axis_Str;

          for (int cou_Index=2;cou_Index<(Height + 3);cou_Index++)
          {
               Canvas.plot_Character((B_X - 3), cou_Index, char (186), 8);
               Canvas.plot_Character(Width + (B_X - 1), cou_Index, char (186), 8);

               if (!((cou_Index - 2) % B_H))
               {
                    Canvas.plot_Character((B_X - 3), cou_Index, char (215), 8);

                    tmp_Y_Axis = Height - (cou_Index - 2);

                    tmp_Y_Axis = (((tmp_Y_Axis / Height) * Range) + Lowest_Value);

                    tmp_Y_Axis_Str = float2str(tmp_Y_Axis);


                    for (unsigned int cou_C=0;cou_C<tmp_Y_Axis_Str.size();cou_C++)
                    {
                         Canvas.plot_Character((B_X - 5 - tmp_Y_Axis_Str.size() + cou_C), cou_Index, tmp_Y_Axis_Str[cou_C], 10);
                    }
               }
          }


          //Plot the X axis labels and the graph borders, top and bottom.
          int tmp_X_Axis = 0;
          std::string tmp_X_Axis_Str;

          for (int cou_Index=(B_X - 2);cou_Index<(Width + (B_X - 1));cou_Index++)
          {
               Canvas.plot_Character(cou_Index, 1, char (205), 8);
               Canvas.plot_Character(cou_Index, (Height + 3), char (205), 8);



               if (!((cou_Index - (B_X - 2)) % B_S))
               {
                    //Plot the columns for each set.
                    for (int cou_Column=2;cou_Column<(Height + 3);cou_Column++)
                    {
                         Canvas.plot_Character(cou_Index, cou_Column, char (179), (8 + int(Background_Data[tmp_X_Axis])));
                         if (!((cou_Column - 2) % B_H)){ Canvas.plot_Character(cou_Index, cou_Column, char (197), (8 + int(Background_Data[tmp_X_Axis]))); }
                    }

                    Canvas.plot_Character(cou_Index, 1, char (209), 8);
                    Canvas.plot_Character(cou_Index, (Height + 3), char (216), 8);

                    tmp_X_Axis_Str = int2str(tmp_X_Axis);

                    for (unsigned int cou_C=0;cou_C<tmp_X_Axis_Str.size();cou_C++)
                    {
                         Canvas.plot_Character(((cou_Index - (tmp_X_Axis_Str.size() / 2)) + cou_C), (Height + 5), tmp_X_Axis_Str[cou_C], 10);
                    }

                    tmp_X_Axis++;
               }


          }

          //Plot the names of the datasets.
          //For if the names overrun the extent of the graph.
          int tmp_Offset = 0;
          int tmp_Counter = Height;
          int tmp_Y_DName = 2;
          int tmp_X_Dash = 0;
          std::string tmp_Name = "";

          double tmp_High = 0.0f;
          double tmp_Low = 0.0f;
          std::string tmp_High_Str = "";
          std::string tmp_Low_Str = "";

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               //Find the high and lows.
               if (Data_Set_Depth){ tmp_High = Data[cou_Index] [0]; tmp_Low = Data[cou_Index] [0]; }

               for (int cou_D=1;cou_D<Data_Set_Depth;cou_D++)
               {
                    if (Data[cou_Index] [cou_D] > tmp_High){ tmp_High = Data[cou_Index] [cou_D]; }
                    if (Data[cou_Index] [cou_D] < tmp_Low){ tmp_Low = Data[cou_Index] [cou_D]; }
               }

               tmp_High_Str = float2str(tmp_High);
               tmp_Low_Str = float2str(tmp_Low);

               tmp_Name = int2str(cou_Index) + "~" + Data_Set_Name[cou_Index];

               if (!(cou_Index % tmp_Counter) && cou_Index != 0){ tmp_Offset += tmp_Name.size() + 25; tmp_Y_DName = 2; }
               Canvas.plot_String((2 + tmp_Offset), tmp_Y_DName, tmp_Name, 10);

               tmp_X_Dash = tmp_Name.size() + 2 + tmp_Offset;

               Canvas.plot_Character(tmp_X_Dash, tmp_Y_DName, '-', (cou_Index + 2));
               Canvas.plot_Character(tmp_X_Dash + 1, tmp_Y_DName, '-', (cou_Index + 2));
               Canvas.plot_Character(tmp_X_Dash + 2, tmp_Y_DName, '-', (cou_Index + 2));


               Canvas.plot_String(tmp_X_Dash + 4, tmp_Y_DName, tmp_High_Str, int ((tmp_High * 10) + 2));
               Canvas.plot_String(tmp_X_Dash + 15, tmp_Y_DName, tmp_Low_Str, int ((tmp_Low * 10) + 2));

               tmp_Y_DName++;
          }

          //--  Plot the points onto the canvas.

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               //cout << "\n";
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    tmp_Y = 0.0f;
                    tmp_Raw_Y = 0.0f;

                    if (Range == 0){ continue; }
                    
                    tmp_Raw_Y = (((Data[cou_Index] [cou_D] - Lowest_Value)) / Range);
                    if (isnan(Data[cou_Index] [cou_D])){ tmp_Raw_Y = 0; }
                         
                    tmp_Y = int (tmp_Raw_Y * Height);
                    tmp_Y = ((Height - tmp_Y) + 2);

                    //cout << "   D:" << Data[cou_Index] [cou_D] << " Raw_Y:" << tmp_Raw_Y << " Y:" << tmp_Y;

                    if (cou_D)
                    {
                         //if (tmp_Y_Prev == tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (178), (cou_Index + 2)); }
                         //if (tmp_Y_Prev < tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (177), (cou_Index + 2)); }
                         //if (tmp_Y_Prev > tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (176), (cou_Index + 2)); }

                         Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (196), (cou_Index + 2));

                         //Canvas.plot_String(((cou_D * B_S) + B_X + 2), tmp_Y, ")", 14);


                         tmp_X_Prev = ((cou_D * B_S) + B_X - 2);
                         tmp_Y_Prev = tmp_Y;
                    }
                    else
                    {
                         Canvas.plot_Character(((cou_D * B_S) + B_X - 2), tmp_Y, '+', (cou_Index + 2));
                         tmp_X_Prev = ((cou_D * B_S) + B_X - 2);
                         tmp_Y_Prev = tmp_Y;
                    }
               }
          }

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               //cout << "\n";
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    tmp_Y = 0.0f;
                    tmp_Raw_Y = 0.0f;

                    if (Range == 0){ continue; }

                    tmp_Raw_Y = ((Data[cou_Index] [cou_D] - Lowest_Value) / Range);
                    if (isnan(tmp_Raw_Y)){ tmp_Raw_Y = 0; }
                    tmp_Y = int (tmp_Raw_Y * Height);
                    tmp_Y = ((Height - tmp_Y) + 2);

                    //cout << "   D:" << Data[cou_Index] [cou_D] << " Raw_Y:" << tmp_Raw_Y << " Y:" << tmp_Y;

                    if (cou_D)
                    {
                         //if (tmp_Y_Prev == tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (178), (cou_Index + 2)); }
                         //if (tmp_Y_Prev < tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (177), (cou_Index + 2)); }
                         //if (tmp_Y_Prev > tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (176), (cou_Index + 2)); }


                        std::string tmp_Value = float2str(Data[cou_Index] [cou_D]);
                         int tmp_Value_Size = tmp_Value.size();
                         if (tmp_Value_Size > 5){ tmp_Value_Size = 5; }

                         for (int cou_Char=0;cou_Char<tmp_Value_Size;cou_Char++)
                         {
                              Canvas.plot_Character(((cou_D * B_S) + B_X - 2 + cou_Char - (int (tmp_Value_Size / 2))), tmp_Y, tmp_Value[cou_Char], 240);
                         }

                         Canvas.plot_String(((cou_D * B_S) + B_X - 1 + tmp_Value_Size - (int (tmp_Value_Size / 2))), tmp_Y, int2str(cou_Index), 169);

                         tmp_X_Prev = ((cou_D * B_S) + B_X - 2);
                         tmp_Y_Prev = tmp_Y;
                    }
                    else
                    {
                         Canvas.plot_Character(((cou_D * B_S) + B_X - 2), int(tmp_Y), '+', (cou_Index + 2));
                         tmp_X_Prev = ((cou_D * B_S) + B_X - 2);
                         tmp_Y_Prev = tmp_Y;
                    }
               }
          }

          //Display the canvas.
          Canvas.display_Raw_Canvas(p_X, p_Y);
     }

     //Outputs the graph.
     void output_Minimal(int p_X = 0, int p_Y = 10, int p_Height = 250, int p_BS = 10)
     {
          int B_S = p_BS;

          Width = (Data_Set_Depth * B_S);

          Height = p_Height;

          //--  Gather the data necessary to label the Y axis and to plot the points.

          //Get the range.
          Lowest_Value = 0.0f;
          Highest_Value = 0.0f;

          if (Number_Of_Data_Sets > 0)
          {
               if (Data_Set_Depth > 0)
               {
                    Lowest_Value = Data[0] [0];
                    Highest_Value = Data[0] [0];
               }
          }
          Range = 0.0f;

          //Get the high and low values to calculate the range.
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    if (Data[cou_Index] [cou_D] > Highest_Value){ Highest_Value = Data[cou_Index] [cou_D]; }
                    if (Data[cou_Index] [cou_D] < Lowest_Value){ Lowest_Value = Data[cou_Index] [cou_D]; }
               }
          }

          Range = Highest_Value - Lowest_Value;


          //Compute the Y positions.
          double tmp_Raw_Y = 0.0f;
          double tmp_Y = 0.0f;

          double tmp_Y_Prev = 0.0f;
          double tmp_X_Prev = 0.0f;




          //--  Set the canvas size.
          Canvas.resize_Canvas(Width + 1, Height + 1);

          //--  Add the accessories to the canvas.
          //Canvas Border.
          Canvas.plot_Character(0, 0, char (201), 15);
          Canvas.plot_Character(0, (Height), char (200), 15);

          for (int cou_Index=1;cou_Index<(Height);cou_Index++)
          {
               Canvas.plot_Character(0, cou_Index, char (186), 15);
               Canvas.plot_Character(Width, cou_Index, char (186), 15);
          }

          Canvas.plot_Character(Width, 0, char (187), 15);
          Canvas.plot_Character(Width, (Height), char (188), 15);

          for (int cou_Index=1;cou_Index<(Width);cou_Index++)
          {
               Canvas.plot_Character(cou_Index, 0, char (205), 15);
               Canvas.plot_Character(cou_Index, (Height), char (205), 15);
          }

          Canvas.plot_String(1, 0, Name, 160);


          //--  Plot the points onto the canvas.

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               //cout << "\n";
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    tmp_Y = 0.0f;
                    tmp_Raw_Y = 0.0f;

                    if (Range == 0){ continue; }

                    tmp_Raw_Y = ((Data[cou_Index] [cou_D] - Lowest_Value) / Range);
                    tmp_Y = int (tmp_Raw_Y * Height);
                    tmp_Y = ((Height - tmp_Y));

                    //cout << "   D:" << Data[cou_Index] [cou_D] << " Raw_Y:" << tmp_Raw_Y << " Y:" << tmp_Y;

                    if (cou_D)
                    {
                         //if (tmp_Y_Prev == tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (178), (cou_Index + 2)); }
                         //if (tmp_Y_Prev < tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (177), (cou_Index + 2)); }
                         //if (tmp_Y_Prev > tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (176), (cou_Index + 2)); }

                         Canvas.plot_Line(int(tmp_X_Prev), int(tmp_Y_Prev), ((cou_D * B_S)), int(tmp_Y), char (196 + cou_Index), (cou_Index + 2));

                         Canvas.plot_Character(((cou_D * B_S)), int(tmp_Y), '+', (cou_Index + 2));
                         Canvas.plot_String(((cou_D * B_S)), int(tmp_Y), float2str(Data[cou_Index] [cou_D]), 14 + cou_Index);

                         tmp_X_Prev = ((cou_D * B_S));
                         tmp_Y_Prev = tmp_Y;
                    }
                    else
                    {
                         Canvas.plot_Character(((cou_D * B_S)), int(tmp_Y), '+', (cou_Index + 2));
                         Canvas.plot_String(((cou_D * B_S)), int(tmp_Y), float2str(Data[cou_Index] [cou_D]), 14 + cou_Index);
                         tmp_X_Prev = ((cou_D * B_S));
                         tmp_Y_Prev = tmp_Y;
                    }
               }
          }
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               //cout << "\n";
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    tmp_Y = 0.0f;
                    tmp_Raw_Y = 0.0f;

                    if (Range == 0){ continue; }

                    tmp_Raw_Y = ((Data[cou_Index] [cou_D] - Lowest_Value) / Range);
                    tmp_Y = int (tmp_Raw_Y * Height);
                    tmp_Y = ((Height - tmp_Y));

                    //cout << "   D:" << Data[cou_Index] [cou_D] << " Raw_Y:" << tmp_Raw_Y << " Y:" << tmp_Y;

                    if (cou_D)
                    {
                         //if (tmp_Y_Prev == tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (178), (cou_Index + 2)); }
                         //if (tmp_Y_Prev < tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (177), (cou_Index + 2)); }
                         //if (tmp_Y_Prev > tmp_Y){ Canvas.plot_Line(tmp_X_Prev, tmp_Y_Prev, ((cou_D * B_S) + B_X - 2), tmp_Y, char (176), (cou_Index + 2)); }

                         Canvas.plot_Character(((cou_D * B_S)), int(tmp_Y), '+', (cou_Index + 2));
                         Canvas.plot_String(((cou_D * B_S) + 1), int(tmp_Y), float2str(Data[cou_Index] [cou_D]), 14 + cou_Index);

                         tmp_X_Prev = ((cou_D * B_S));
                         tmp_Y_Prev = tmp_Y;
                    }
                    else
                    {
                         Canvas.plot_Character(((cou_D * B_S)), int(tmp_Y), '+', (cou_Index + 2));
                         Canvas.plot_String(((cou_D * B_S) + 1), int(tmp_Y), float2str(Data[cou_Index] [cou_D]), 14 + cou_Index);
                         tmp_X_Prev = ((cou_D * B_S));
                         tmp_Y_Prev = tmp_Y;
                    }
               }
          }


          //Display the canvas.
          Canvas.display_Raw_Canvas(p_X, p_Y);
     }


     //Outputs the raw data array.
     void output_Raw_Data_Array()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               std::cout << "\n\t[";
               oint(0, (cou_Index + 2), cou_Index);
               std::cout << "]";
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    std::cout << " ";
                    odbl(0, (cou_Index + 2), Data[cou_Index] [cou_D]);
               }
          }
     }

     //==--  Array management functions.

     //Increase the depth of a data set.
     void increase_Data_Set_Depth(int p_Depth)
     {
          if (p_Depth <= Data_Set_Depth){ return; }

          //Setup the temporary arrays.
          double ** tmp_Data;
          tmp_Data = new double*[Number_Of_Data_Sets];

          double * tmp_Background_Data;
          tmp_Background_Data = new double[Data_Set_Depth];

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               tmp_Data[cou_Index] = new double[Data_Set_Depth];
          }

          //Copy the old ones over.
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    tmp_Data[cou_Index] [cou_D] = Data[cou_Index] [cou_D];
               }
               delete [] Data[cou_Index];
               Data[cou_Index] = NULL;
          }

          //Copy the old background data.
          for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
          {
               tmp_Background_Data[cou_D] = Background_Data[cou_D];
          }

          //Reallocate the old array.
          delete [] Data;
          Data = NULL;

          Data = new double*[Number_Of_Data_Sets];

          delete [] Background_Data;
          Background_Data = NULL;

          Background_Data = new double[p_Depth];

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               Data[cou_Index] = new double[p_Depth];

               for (int cou_D=0;cou_D<p_Depth;cou_D++)
               {
                    Data[cou_Index] [cou_D] = 0.0f;
               }
          }

          for (int cou_D=0;cou_D<p_Depth;cou_D++)
          {
               Background_Data[cou_D] = 0.0f;
          }

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    Data[cou_Index] [cou_D] = tmp_Data[cou_Index] [cou_D];
               }

               delete [] tmp_Data[cou_Index];
               tmp_Data[cou_Index] = NULL;
          }

          for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
          {
               Background_Data[cou_D] = tmp_Background_Data[cou_D];
          }

          delete [] tmp_Background_Data;
          tmp_Background_Data = NULL;

          delete [] tmp_Data;
          tmp_Data = NULL;

          Data_Set_Depth = p_Depth;
     }

     //Increase the number of data sets.
     void increase_Number_Of_Data_Sets(int p_Depth)
     {
          if (p_Depth <= Number_Of_Data_Sets){ return; }

          //Setup the temporary arrays.
          double ** tmp_Data;
          tmp_Data = new double*[Number_Of_Data_Sets];

          std::string * tmp_Name;
          tmp_Name = new std::string[Number_Of_Data_Sets];

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               tmp_Data[cou_Index] = new double[Data_Set_Depth];
          }

          //Copy the old ones over.
          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               //Copy the names.
               tmp_Name[cou_Index] = Data_Set_Name[cou_Index];

               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    tmp_Data[cou_Index] [cou_D] = Data[cou_Index] [cou_D];
               }
               delete [] Data[cou_Index];
               Data[cou_Index] = NULL;
          }

          //Reallocate the old array.
          delete [] Data;
          Data = NULL;

          delete [] Data_Set_Name;
          Data_Set_Name = NULL;

          Data = new double*[p_Depth];
          Data_Set_Name = new std::string[p_Depth];

          for (int cou_Index=0;cou_Index<p_Depth;cou_Index++)
          {
               Data[cou_Index] = new double[Data_Set_Depth];

               Data_Set_Name[cou_Index] = "";

               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    Data[cou_Index] [cou_D] = 0.0f;
               }
          }

          for (int cou_Index=0;cou_Index<Number_Of_Data_Sets;cou_Index++)
          {
               Data_Set_Name[cou_Index] = tmp_Name[cou_Index];

               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    Data[cou_Index] [cou_D] = tmp_Data[cou_Index] [cou_D];
               }

               delete [] tmp_Data[cou_Index];
               tmp_Data[cou_Index] = NULL;
          }

          delete [] tmp_Data;
          tmp_Data = NULL;
          
          delete [] tmp_Name;
          tmp_Name = NULL;

          for (int cou_Index=Number_Of_Data_Sets;cou_Index<p_Depth;cou_Index++)
          {
               for (int cou_D=0;cou_D<Data_Set_Depth;cou_D++)
               {
                    Data[cou_Index] [cou_D] = 0.0f;
               }
          }

          Number_Of_Data_Sets = p_Depth;
     }
};
