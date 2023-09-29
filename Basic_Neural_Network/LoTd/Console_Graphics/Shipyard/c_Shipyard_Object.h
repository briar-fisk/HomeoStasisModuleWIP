//==--    OBJECTS

//This is a simple label that can be assigned to a variable address for updates.
class c_Label_String
{
public:
     
     //This is the data that updates whenever displayed.
     std::string * Data;
     
     //The Message preluding the data.
     std::string Label;
     
     //The Maximum characters shown.
     int Width;
     
     c_Label_String()
     {
          Data = NULL;
          Label = "String->";
          Width = 20;
     }
     
     //Returns the formatted string for submission to the canvas.
     std::string get_Plot()
     {
          std::string tmp_Plot = Label;
          std::string tmp_Formatted_Plot = "";
          
          if (Data == NULL)
          {
               
          }
          else
          {
               tmp_Plot += *Data;
          }
          
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               tmp_Formatted_Plot += " ";
          }
          
          for (int cou_Index=0;cou_Index<int (tmp_Plot.size());cou_Index++)
          {
               if (cou_Index >= Width){ break; }
               
               tmp_Formatted_Plot[cou_Index] = tmp_Plot[cou_Index];
          }
          
          return tmp_Formatted_Plot;
               
     }
     
};

