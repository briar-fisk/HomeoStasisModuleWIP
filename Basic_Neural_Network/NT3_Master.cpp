
//The data type used for internal manipulation.
union u_Data
{
    int I;
    float F;
    char C;
    bool B;
    void* NR;
    //char binary[16];
};

//Stolen from >>>/g/68965893
//Defaults to setting the first 8 bits to the given byte.
uint64_t u_Data_3_Set_Byte(uint64_t number, uint64_t the_byte, uint8_t offset = 56)
{
    uint64_t mask = ~(0xFFULL << offset);
    return (number & mask) + (the_byte << offset);
}

uint64_t u_Data_3_Get_Byte(uint64_t number, uint8_t offset = 56)
{
    return (number >> offset);
}

uint64_t u_Data_3_Get_Unsalted(uint64_t number)
{
    uint64_t tmp_Num = number << 8;
    tmp_Num = tmp_Num >> 8;
    return tmp_Num;
}

//The union holding the IO data for the NT3 constructs.
union u_Data_3
{
    void* NR;
    long long int I;
    uint64_t U;
    double F;
    char C;

    //The u_Data_3 should be 64 bits long.
    unsigned char B[8];
};

//This is the linked list for the buffer.
class c_LoTd_Linked_List
{
public:

    //The next node in the chain.
    c_LoTd_Linked_List* Next;
    c_LoTd_Linked_List* Previous;
    u_Data_3 ID;
    u_Data_3 Data;

    c_LoTd_Linked_List()
    {
        Next = NULL;
        Previous = NULL;
        ID.U = 0;
        Data.U = 0;
    }

    ~c_LoTd_Linked_List()
    {
        ID.U = 0;
        Data.U = 0;
        //delete Next; Handled by the handler.
    }

    //Outputs the linked list.
    //-- !WARNING WILL CAUSE STACK CORRUPTION ERROR WITH ENOUGH NODES!
    void output_LL()
    {
        //*std::cout << " [";
        std::cout << "[" << ID.U << " " << Data.U << "]  ";
        if (Next != NULL) { Next->output_LL(); }
    }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_LoTd_Linked_List_Handler
{
public:

    //The nodes in the linked list.
    c_LoTd_Linked_List* Root;
    c_LoTd_Linked_List** Current_LL;
    long long int Depth;

    c_LoTd_Linked_List_Handler()
    {
        Root = NULL;
        Current_LL = &Root;
        Depth = 0;
    }

    ~c_LoTd_Linked_List_Handler()
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
    c_LoTd_Linked_List* new_LL(u_Data_3 p_ID, u_Data_3 p_Data)
    {
        c_LoTd_Linked_List* tmp_LL = NULL;

        *Current_LL = new c_LoTd_Linked_List;

        (*Current_LL)->ID = p_ID;
        (*Current_LL)->Data = p_Data;

        tmp_LL = *Current_LL;

        (*Current_LL)->Previous = tmp_LL;

        Current_LL = &(*Current_LL)->Next;

        Depth++;

        return tmp_LL;
    }

    //Adds a node with a pointer as the data.
    c_LoTd_Linked_List* new_LL(__int64 p_ID, void* p_Data)
    {
        u_Data_3 tmp_ID;
        u_Data_3 tmp_Data;
        tmp_ID.I = p_ID;
        tmp_Data.NR = p_Data;
        return new_LL(tmp_ID, tmp_Data);
    }

    //Resets the linked list.
    void reset()
    {
        c_LoTd_Linked_List* tmp_LL = Root;
        Root = NULL;
        c_LoTd_Linked_List* tmp_LL_Next = NULL;

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
         c_LoTd_Linked_List * tmp_Prv = NULL;
         c_LoTd_Linked_List * tmp_Next = NULL;
         c_LoTd_Linked_List * tmp_Current = NULL;
         bool flg_Done = false;

         while(!flg_Done)
         {
              flg_Done = true;


         }
    }
     * */
};



//--     SMALL COMMOMLY USED FUNCTIONS.


//Gets the datatype of the std::string submitted.
std::string gdt(std::string p_Data)
{
    // > 57 = char
    // < 48 && !46 = char
    // = 46 = float
    bool flg_Int = true;
    bool flg_Float = false;

    for (unsigned int cou_Index = 0; cou_Index < (p_Data.size()); cou_Index++)
    {
        if (int(p_Data[cou_Index]) > 57) { return "string"; }
        if (int(p_Data[cou_Index]) < 48 && int(p_Data[cou_Index]) != 46 && int(p_Data[cou_Index]) != 45) { return "string"; }
        if (int(p_Data[cou_Index]) == 45 && cou_Index > 0) { return "string"; }
        if (int(p_Data[cou_Index]) == 46) { flg_Float = true; flg_Int = false; }
    }
    if (flg_Float) { return "float"; }
    if (flg_Int) { return "int"; }
    return "string";
}
//Gets the datatype and returns an integer for the type rather than a std::string as gdt().
int gdti(std::string p_Data)
{
    // > 57 = char
    // < 48 && !46 = char
    // = 46 = float
    bool flg_Int = true;
    bool flg_Float = false;

    for (unsigned int cou_Index = 0; cou_Index < (p_Data.size()); cou_Index++)
    {
        if (int(p_Data[cou_Index]) > 57) { return 2; }
        if (int(p_Data[cou_Index]) < 48 && int(p_Data[cou_Index]) != 46 && int(p_Data[cou_Index]) != 45) { return 2; }
        if (int(p_Data[cou_Index]) == 45 && cou_Index > 0) { return 2; }
        if (int(p_Data[cou_Index]) == 46) { flg_Float = true; flg_Int = false; }
    }
    if (flg_Float) { return 1; }
    if (flg_Int) { return 0; }
    return 2;
}


//Takes a std::string and converts it to an array of u_Data.
void str2data(std::string p_String, u_Data p_Converted_Data[])
{

    //Read the std::string into the data array.
    for (unsigned int cou_Index = 0; cou_Index < p_String.size(); cou_Index++)
    {
        p_Converted_Data[cou_Index].I = int(p_String[cou_Index]);
    }
}

//Finds a power
int pow(int p_Base, int p_Exponet)
{
    int tmp_Base = p_Base;

    if (p_Exponet == 0) { return 1; }
    if (p_Exponet == 1) { return p_Base; }

    for (int cou_Index = 1; cou_Index < p_Exponet; cou_Index++)
    {
        tmp_Base *= p_Base;
    }

    return tmp_Base;
}


//Takes a std::string and converts it to an integer.
int str2int(std::string p_String)
{
    if (gdt(p_String) != "float" && gdt(p_String) != "int") { return 0; }

    std::stringstream tmp_Cin;
    tmp_Cin.flush();
    tmp_Cin << p_String;
    int tmp_Int;
    tmp_Cin >> tmp_Int;
    return tmp_Int;
}


//Takes a std::string and converts it to an integer.
unsigned long long  str2ullint(std::string p_String)
{
    if (gdt(p_String) != "float" && gdt(p_String) != "int") { return 0; }

    std::stringstream tmp_Cin;
    tmp_Cin.flush();
    tmp_Cin << p_String;
    unsigned long long int tmp_Int;
    tmp_Cin >> tmp_Int;
    return tmp_Int;
}


float str2float(std::string p_String)
{
    if (gdt(p_String) != "float" && gdt(p_String) != "int") { return 0.0; }

    std::stringstream tmp_Cin;
    tmp_Cin.flush();
    tmp_Cin << p_String;
    float tmp_Float;
    tmp_Cin >> tmp_Float;
    return tmp_Float;
}

//Takes a std::string and returns a bool.
bool str2bool(std::string p_String)
{
    if (p_String[0] == '0') { return 0; }
    if (p_String[0] == '1') { return 1; }
    if (p_String == "false") { return 0; }
    if (p_String == "true") { return 1; }
    return 0;
}

//Takes an int and returns a string.
std::string int2str(int p_Integer)
{
    std::stringstream tmp_Cin;
    tmp_Cin.flush();
    tmp_Cin << p_Integer;
    return tmp_Cin.str();
}
//Takes an int and returns a string.
std::string int2str(unsigned long long int p_Integer)
{
    std::stringstream tmp_Cin;
    tmp_Cin.flush();
    tmp_Cin << p_Integer;
    return tmp_Cin.str();
}

//Takes a float and returns a string.
std::string float2str(double p_Float)
{
    std::stringstream tmp_Cin;
    tmp_Cin << p_Float;
    return tmp_Cin.str();
}

//Takes a float and returns a string.
std::string dbl2str(double p_Double)
{
    std::stringstream tmp_Cin;
    tmp_Cin << p_Double;
    return tmp_Cin.str();
}

//Takes a bool and returns a string.
std::string bool2str(bool p_Bool)
{
    std::stringstream tmp_Cin;
    tmp_Cin << p_Bool;
    return tmp_Cin.str();
}

//Takes a bool and returns a string.
u_Data_3 poi2x_Tmp_Data;
std::string poi2str(void* p_Void)
{
    poi2x_Tmp_Data.NR = p_Void;
    return int2str(poi2x_Tmp_Data.U);
}

//Takes an adress and returns its value in base ten.
unsigned long long int poi2int(void* p_Void)
{
    poi2x_Tmp_Data.NR = p_Void;
    return poi2x_Tmp_Data.U;
}


//Gets the first digit of a given float.
int gfdf(double p_Data)
{
    if (p_Data < 1 && p_Data > 0)
    {
        while (p_Data < 1)
        {
            p_Data *= 10;
        }
    }
    while (p_Data >= 10)
    {
        p_Data *= .1;
    }

    return int(p_Data);
}

//Gets the first two digits of a given float.
int gf2df(double p_Data)
{
    if (p_Data < 1 && p_Data > 0)
    {
        while (p_Data < 10)
        {
            p_Data *= 10;
        }
    }
    while (p_Data >= 100)
    {
        p_Data *= .1;
    }

    return int(p_Data);
}

//Gets the first two digits of a given float.
int gf3df(double p_Data)
{
    if (p_Data < 1 && p_Data > 0)
    {
        while (p_Data < 100)
        {
            p_Data *= 10;
        }
    }
    while (p_Data >= 1000)
    {
        p_Data *= .1;
    }

    return int(p_Data);
}

//Gets the first digit of a given int.
int gfdi(double p_Data)
{
    while (p_Data >= 10)
    {
        p_Data *= .1;
    }

    return int(p_Data);
}

//Gets the first two digits of a given int.
int gf2di(double p_Data)
{
    while (p_Data >= 100)
    {
        p_Data *= .1;
    }

    return int(p_Data);
}

//Gets the first two digits of a given int.
int gf3di(double p_Data)
{
    while (p_Data >= 1000)
    {
        p_Data *= .1;
    }

    return int(p_Data);
}

//Gets the last digit of a given int.
int gldi(long int p_Data)
{
    if (p_Data < 10) { return p_Data; }

    int tmp_Remainder = p_Data;
    int tmp_Takaway = int((p_Data * .1)) * 10;
    tmp_Remainder = tmp_Remainder - tmp_Takaway;
    return tmp_Remainder;
}

//Gets the last two digits of a given int.
int gl2di(long int p_Data)
{
    if (p_Data < 100) { return p_Data; }

    int tmp_Remainder = p_Data;
    int tmp_Takaway = int((p_Data * .01)) * 100;
    tmp_Remainder = tmp_Remainder - tmp_Takaway;
    return tmp_Remainder;
}

//Gets the last two digits of a given int.
int gl3di(long int p_Data)
{
    if (p_Data < 1000) { return p_Data; }

    int tmp_Remainder = p_Data;
    int tmp_Takaway = int((p_Data * .001)) * 1000;
    tmp_Remainder = tmp_Remainder - tmp_Takaway;
    //std::cout << "\n\t p_Data->" << p_Data << " tmp_R->" << tmp_Remainder << " tmp_T->" << tmp_Takaway;
    return tmp_Remainder;
}

//Gets the last digit of a given int.
u_Data gldd(u_Data p_Data)
{
    if (p_Data.I < 0) { p_Data.I *= -1; }

    if (p_Data.I < 10) { return p_Data; }

    u_Data tmp_Remainder;
    tmp_Remainder.I = p_Data.I;
    p_Data.I = int(p_Data.I * .1);
    p_Data.I = int(p_Data.I * 10);

    tmp_Remainder.I -= p_Data.I;
    if (tmp_Remainder.I > 0) { tmp_Remainder.I -= 1; }
    return tmp_Remainder;
}

//Gets the last two digits of a given int.
u_Data gl2dd(u_Data p_Data)
{
    if (p_Data.I < 0) { p_Data.I *= -1; }

    if (p_Data.I < 100) { return p_Data; }

    u_Data tmp_Remainder;
    tmp_Remainder.I = p_Data.I;
    p_Data.I = int(p_Data.I * .01);
    p_Data.I = int(p_Data.I * 100);
    tmp_Remainder.I -= p_Data.I;
    if (tmp_Remainder.I > 0) { tmp_Remainder.I -= 1; }
    return tmp_Remainder;
}

//Gets the last two digits of a given int.
u_Data gl3dd(u_Data p_Data)
{
    if (p_Data.I < 0) { p_Data.I *= -1; }

    if (p_Data.I < 1000) { return p_Data; }

    //std::cout << "\n ->" << p_Data.I;
    u_Data tmp_Remainder;
    tmp_Remainder.I = p_Data.I;
    p_Data.I = int(p_Data.I * .001);
    p_Data.I = int(p_Data.I * 1000);
    tmp_Remainder.I -= p_Data.I;
    //std::cout << " ->" << p_Data.I << " ->" << tmp_Remainder.I;
    if (tmp_Remainder.I > 0) { tmp_Remainder.I -= 1; }
    return tmp_Remainder;
}

//Generates a folder.
void make_Dir(std::string p_Dir)
{
    //Create the subfolder.
    std::string tmp_MD = "md " + p_Dir;
    system(tmp_MD.c_str());
}




















/*****************************************************************  
**                                                              **
**                                                              **
**     c_Console                                                **
**                                                              **
**     This class is for the console handling functions         **
**                                                              **
**                                                              **
**     Functions:                                               **
**                                                              **
**          /set_Console_Color -This functions takes two        **
**               arguments, one for background color and        **
**               one for foreground color                       **
**                                                              **
**                                                              **
**                                                              **
**     Classes:                                                 **
**                                                              **
*****************************************************************/



//console color thingy
HANDLE  hConsole;
CONSOLE_SCREEN_BUFFER_INFO csbi;
int bg;//current background
int fg;//current foreground

void init_Console()
{
     hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
}

void set_Console_Title(string p_Title)
{
     SetConsoleTitleA(p_Title.c_str());
	 
}

void set_Console_Color(int p_bg=0, int p_fg=0)
{
     
     // sets color
     bg = p_bg;
     fg = p_fg;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void output_Console_Character_Colored(int p_bg, int p_fg, char p_Char)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Char;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}


void output_Console_Int_Colored(int p_bg, int p_fg, int p_Int)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Int;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}

void output_Console_Float_Colored(int p_bg, int p_fg, float p_Float)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}

void output_Console_Float_Colored_With_Additional_Charge_For_Precision_Setting(int p_bg, int p_fg, float p_Float, float p_Precision = .0000001)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     p_Float += p_Precision;
     cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}


void output_Console_Float_Colored_With_Precision_5(int p_bg, int p_fg, float p_Float)
{
     //hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
     // sets color
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     int tmp = int (p_Float * 100000);
     p_Float = float (tmp * .000001);
     cout << p_Float;
     
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
     set_Console_Color(bg, fg);
}


void ostr(int p_bg, int p_fg, string p_String)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_String;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void ochr(int p_bg, int p_fg, char p_String)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_String;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void opchr(int p_bg, int p_fg, char p_String)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     if (int (p_String) == 0 || int (p_String) == 7 || int (p_String) == 8 || int (p_String) == 9 || int (p_String) == 10 || int (p_String) == 13 || int (p_String) == 255)
     {
          cout << "?";
     }
     else
     {
          cout << p_String;
     }
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void oull(int p_bg, int p_fg, unsigned long long int p_Int)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Int;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void oint(int p_bg, int p_fg, int p_Int)
{
	SetConsoleTextAttribute(hConsole, p_fg + (p_bg * 16));
	cout << p_Int;
	SetConsoleTextAttribute(hConsole, fg + (bg * 16));
}

void oint64(int p_bg, int p_fg, long long int p_Int)
{
	SetConsoleTextAttribute(hConsole, p_fg + (p_bg * 16));
	cout << p_Int;
	SetConsoleTextAttribute(hConsole, fg + (bg * 16));
}

void oflt(int p_bg, int p_fg, float p_Float)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Float;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}

void odbl(int p_bg, int p_fg, double p_Double)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_Double;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}



void opoi(int p_bg, int p_fg, void * p_NR)
{
     SetConsoleTextAttribute(hConsole, p_fg+(p_bg*16));
     cout << p_NR;
     SetConsoleTextAttribute(hConsole, fg+(bg*16));
}




void oindex(int p_Index, int p_Color = 13)
{
     cout << " ["; oint(0, p_Color, p_Index); cout << "] ";
}












void console_Move_Cursor_To_Given_X_And_Y( short int p_X, short int p_Y )
{
     //system("PAUSE > NULL");
    COORD tmp_Coordinate = { p_X, p_Y };
    SetConsoleCursorPosition( hConsole, tmp_Coordinate );
}

int get_Console_Cursor_X()
{
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return csbi.dwCursorPosition.X;
}

int get_Console_Cursor_Y()
{
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	return csbi.dwCursorPosition.Y;
}

void xy(short int p_X, short int p_Y = -1)
{
     if (p_X == -1){ p_X = get_Console_Cursor_X(); }
     if (p_Y == -1){ p_Y = get_Console_Cursor_Y(); }
     console_Move_Cursor_To_Given_X_And_Y(p_X, p_Y);
}

void sx(short int p_X)
{
     short int tmp_Y = get_Console_Cursor_Y();
     
     xy(p_X, tmp_Y);
}

void sy(short int p_Y)
{
     short int tmp_X = get_Console_Cursor_X();
     
     xy(tmp_X, p_Y);
}


void cls(int X = 0, int Y = 0, int Width = 4, int Height = 100)
{
     console_Move_Cursor_To_Given_X_And_Y(X, Y);
     for (int cou_Y=Y;cou_Y<(Height + Y);cou_Y++)
     {
          console_Move_Cursor_To_Given_X_And_Y(X, cou_Y);
          for (int cou_Index=0;cou_Index<Width;cou_Index++)
          {
               cout << "                         ";
          }
     }
     console_Move_Cursor_To_Given_X_And_Y(X, Y);
}

//Outputs a line in the console window from the first set of coordinates to the second set of coordinates.
void console_Draw_Line(int p_X1, int p_Y1, int p_X2, int p_Y2, int p_Color = 1, int p_Char = 177, float p_Percentage = 1.0)
{
     //Bounds checking for the console window.
     if (p_X1 > 9998 || p_Y1 > 9998 || p_X2 > 9998 || p_Y2 > 9998){ return; }
     
     //Get the rise over run.
	 float tmp_Rise = float(p_Y2 - p_Y1);
	 float tmp_Run = float(p_X2 - p_X1);
     
     //The increment for rise and run.
     float tmp_Rise_Inc = 0.0;
     float tmp_Run_Inc = 0.0;
     
     //The temporary positions.
     float tmp_X = 0.0;
     float tmp_Y = 0.0;
     
     if (abs(tmp_Rise) >= abs(tmp_Run))
     {
          //If the tmp_Rise is larger then reduce by the rise.
          tmp_Rise_Inc = tmp_Rise / abs(tmp_Rise);
          tmp_Run_Inc = tmp_Run / abs(tmp_Rise);
          
//          cout << "\n ~Ri >= ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
          
          for (int cou_Y=0;cou_Y<(int (abs(tmp_Rise) * p_Percentage));cou_Y++)
          {
               //Increase the current position.
               tmp_X += tmp_Run_Inc;
               tmp_Y += tmp_Rise_Inc;
               
               //Output the current position.
               console_Move_Cursor_To_Given_X_And_Y(tmp_X + p_X1, tmp_Y + p_Y1);
               output_Console_Character_Colored(0, p_Color, p_Char);
          }
     }
     if (abs(tmp_Rise) < abs(tmp_Run))
     {
          //If the tmp_Run is larger then reduce by the tmp_Run.
          tmp_Rise_Inc = tmp_Rise / abs(tmp_Run);
          tmp_Run_Inc = tmp_Run / abs(tmp_Run);
          
//          cout << "\n ~Ri < ~Ru " << tmp_Rise_Inc << "\\" << tmp_Run_Inc;
          
          for (int cou_X=0;cou_X<abs(int ((tmp_Run) * p_Percentage));cou_X++)
          {
               //Increase the current position.
               tmp_X += tmp_Run_Inc;
               tmp_Y += tmp_Rise_Inc;
               
               //Output the current position.
               console_Move_Cursor_To_Given_X_And_Y(int(tmp_X + p_X1), int(tmp_Y + p_Y1));
               output_Console_Character_Colored(0, p_Color, p_Char);
          }
     }
}






//Countdowns for the given amount of time displaying as it counts down.
void countdown(int p_Seconds)
{
     int tmp_X = get_Console_Cursor_X();
     int tmp_Y = get_Console_Cursor_Y();
     double tmp_Output = p_Seconds;
     
     for (int cou_Index=0;cou_Index<(p_Seconds * 10);cou_Index++)
     {
          xy(tmp_X, tmp_Y);
          cout << " <{ ";
          
          odbl(0, 12, tmp_Output);
          
          cout << "     ";
          
          Sleep(100);
          
          tmp_Output -= .1;
     }
}













/*
 *   This class holds the information for a console window that different objects ttach to. for example the one attached to the node network
 * would hold the postition for the graphical output of the individual polyu nodes to use when outputting to the screen using console coordinates.
 * used dynamic arrays of coordinates to be shared among different objects and referenced by all to allow the constroller to manipulate the posisions of 
 * its members.
*/

//For the lookup tree I will be using get() and set() functions to set and retrieve data as the array holding it will be dynamic.
class c_CMD_Variable_Node
{
     friend class c_Lookup_Tree; 
private:
     
     c_CMD_Variable_Node * Right;
     c_CMD_Variable_Node * Left;
     
     
     u_Data_3 * Data;
     int Data_Length;
     int Var_Type;
          
public:
     
     c_CMD_Variable_Node()
     {
          Data = NULL;
          Var_Type = 0;
          Data_Length = 0;
          Right = NULL;
          Left = NULL;
     }
     
     ~c_CMD_Variable_Node()
     {
          delete [] Data;
          Data = NULL;
          
          delete Right;
          Right = NULL;
          
          delete Left;
          Left = NULL;
     }
     
     string Var_Name;
     
     //--     SET FUNCTIONS FOR THE DATA
     
     //String
     void set_string(string p_Data)
     {
          delete [] Data;
		  if (p_Data.size() == 0) { Data = new u_Data_3[1]; Data[0].U = 0; return; }
          Data = new u_Data_3[p_Data.size()];
          
          //cout << "\n\n\t Setting Characters->";
		  if (p_Data.size() > 0)
		  {
			  for (unsigned int cou_Index = 0; cou_Index < p_Data.size(); cou_Index++)
			  {
				  Data[cou_Index].U = unsigned long long int (p_Data[cou_Index]);
				  //cout << "\n\t\t" << Data[cou_Index].I;
			  }
		  }
          //cout << "\n\t Finished Gathering...";
          Var_Type = 0;
          Data_Length = p_Data.size();
          
          //cout << "\n\t\t\t set_string(" << p_Data << ")";
     }
     void set_int(int p_Data)          { Data = new u_Data_3[1]; Data[0].I =  p_Data; Var_Type = 1; Data_Length = 1; }// cout << "\n\t\t\t set_int(" << p_Data << ")"; }
     void set_float(float p_Data)      { Data = new u_Data_3[1]; Data[0].F =  p_Data; Var_Type = 2; Data_Length = 1; }// cout << "\n\t\t\t set_float(" << p_Data << ")"; }
     void set_reference(void * p_Data) { Data = new u_Data_3[1]; Data[0].NR = p_Data; Var_Type = 3; Data_Length = 1; }// cout << "\n\t\t\t set_reference(" << p_Data << ")"; }
     void set_bool(bool p_Data)        { Data = new u_Data_3[1]; Data[0].U =  p_Data; Var_Type = 4; Data_Length = 1; }// cout << "\n\t\t\t set_bool(" << p_Data << ")"; }
	 
     void set_raw(string p_Var_Name, u_Data_3 p_Data[], int p_Data_Length, int p_Data_Type)
     {
          delete [] Data;
          Data = new u_Data_3[p_Data_Length];
          Data_Length = p_Data_Length; 
          Var_Type = p_Data_Type;
          Var_Name = p_Var_Name;
          
          for (int cou_Index=0;cou_Index<Data_Length;cou_Index++)
          {
               Data[cou_Index].NR = p_Data[cou_Index].NR;
          }
     }
     
     
     //--     GET FUNCTIONS FOR THE DATA
     
     string get_string()
     {    
          if (Data == NULL){ return "";   } 
          string tmp_String = ""; 
          
          //cout << "\n\n\t Gathering Characters->";
          for (int cou_Index=0;cou_Index<Data_Length;cou_Index++)
          {
               //cout << "\n\t\t" << Data[cou_Index].I;
               tmp_String += char (Data[cou_Index].I);
          }
          //cout << "\n\t Finished Gathering...";
          
          
          
          //cout << "\n\n\t [" << tmp_String << "]";
          
          
          return tmp_String;
     }
     
     long long int    get_int(){       if (Data == NULL){ return 0;    } return Data[0].I;}
     double  get_float(){     if (Data == NULL){ return 0.0;  } return Data[0].F;}
     void * get_reference(){ if (Data == NULL){ return NULL; } return Data[0].NR;}
     bool   get_bool(){      if (Data == NULL){ return 0;    } return Data[0].B;}
     
     //Outputs the current node.
     void output_Node()
     {
          cout << "\n\t\t " << this << " Var_Name->" << Var_Name << " Var_Type->" << Var_Type << "  Data_Length->" << Data_Length << "  Data->";
          if (Var_Type == 0){ cout << get_string(); }
          if (Var_Type == 1){ cout << get_int(); }
          if (Var_Type == 2){ cout << get_float(); }
          if (Var_Type == 3){ cout << get_reference(); }
          if (Var_Type == 4){ cout << get_bool(); }
     }
};

class c_Lookup_Tree
{
     private:
             
     public:
          c_CMD_Variable_Node * Root;
          c_CMD_Variable_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          c_Lookup_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_Lookup_Tree()
          {
               delete Root;
               //Current does not get deleted as it only points to other nodes that may not exist after root is deleted and the node chain is gone.
               //delete *Current;
          }
          
          int search(string p_Var_Name)
          {
               flg_Foundit = false;
               return query_Name(Root, p_Var_Name);
          }
          
          int query_Name(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    p_Node = new c_CMD_Variable_Node;
                    p_Node->Right = NULL;
                    p_Node->Left = NULL;
                    p_Node->Var_Name = p_Var_Name;
                    p_Node->set_int(-1);
                    Current = &p_Node;
                    //cout << "\n\n\t New Variable Created";
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    Current = &p_Node;  
                    flg_Foundit = true;
                    return p_Node->Var_Type;
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    query_Name(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    query_Name(p_Node->Right, p_Var_Name);
               }
                    
               return 0;
          }
          
          int exists(string p_Var_Name)
          {
               return exists_query_Name(Root, p_Var_Name);
          }
          
          int exists_query_Name(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    //cout << "\n\t Variable NOT Found->" << p_Var_Name;
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    //cout << "\n\t Variable Found->" << p_Var_Name;
                    return 1;
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    return exists_query_Name(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    return exists_query_Name(p_Node->Right, p_Var_Name);
               }
                    
               return 0;
          }
          
          
          void * get_Bin_Node_Reference(string p_Var_Name)
          {
               flg_Foundit = false;
               return get_Bin_Node_Reference(Root, p_Var_Name);
          }
          
          void * get_Bin_Node_Reference(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    return NULL;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    cout << "\n\n\t Returning Address " << p_Node;
                    return ((void*) p_Node);
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    return get_Bin_Node_Reference(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    return get_Bin_Node_Reference(p_Node->Right, p_Var_Name);
               }
                    
               return NULL;
          }
          
          int submit_Old_Node(c_CMD_Variable_Node *&p_Node)
          {
               return find_Spot_For_Old_Node(Root, p_Node);
          }
          
          int find_Spot_For_Old_Node(c_CMD_Variable_Node *&p_Node, c_CMD_Variable_Node *&p_Old_Node)
          {
               if (p_Node == NULL)
               {
                    p_Node = p_Old_Node;
                    Current = &p_Node;
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Old_Node->Var_Name)
               {
                    Current = &p_Node;  
                    flg_Foundit = true;
                    
                    cout << "\n\n\t During Replacement A Node With The Same Variable Name Was Found.";
                    
                    return p_Node->Var_Type;
               }
                    
               if (p_Old_Node->Var_Name < p_Node->Var_Name)
               {
                    find_Spot_For_Old_Node(p_Node->Left, p_Old_Node);
               }
                    
               if (p_Old_Node->Var_Name > p_Node->Var_Name)
               {
                    find_Spot_For_Old_Node(p_Node->Right, p_Old_Node);
               }
                    
               return 0;
          }
          
          int null_Leg(c_CMD_Variable_Node *&p_Node, string p_Var_Name)
          {
               if (p_Node == NULL)
               {
                    return 0;
               }
               
               if (p_Node->Var_Name == p_Var_Name)
               {
                    return 0;
               }
                    
               if (p_Var_Name < p_Node->Var_Name)
               {
                    if ((p_Node->Left)->Var_Name == p_Var_Name)
                    {
                         delete p_Node->Left;
                         p_Node->Left = NULL;
                    }
                    
                    null_Leg(p_Node->Left, p_Var_Name);
               }
                    
               if (p_Var_Name > p_Node->Var_Name)
               {
                    if ((p_Node->Right)->Var_Name == p_Var_Name)
                    {
                         delete p_Node->Right;
                         p_Node->Right = NULL;
                    }
                    
                    null_Leg(p_Node->Right, p_Var_Name);
               }
                    
               return 0;
          }
          
          void remove_Node(c_CMD_Variable_Node *&p_Node)
          {
               //Get the Right leg and set the passed nodes right leg to NULL to avoid deletion.
               c_CMD_Variable_Node * tmp_Right = p_Node->Right;
               p_Node->Right = NULL;
               
               //Get the left leg and set the given nodes to NULL.
               c_CMD_Variable_Node * tmp_Left = p_Node->Left;
               p_Node->Left = NULL;
               
               //NULL the leg containing the passed node.
               null_Leg(Root, p_Node->Var_Name);
               
               //Submits a node that is already created to the tree.
               //Make shure that it is allocated in the first place, pass a NULL and receive and error.
               if (tmp_Left){ submit_Old_Node(tmp_Left); }
               if (tmp_Right){ submit_Old_Node(tmp_Right); }
          }
          
          void remove_Node(string p_Var_Name)
          {
               search(p_Var_Name);
               remove_Node(*Current);
          }
          
          void output_Tree()
          {
               output_Node(Root);
          }
          
          void output_Node(c_CMD_Variable_Node *&p_Node)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left);
               p_Node->output_Node();
               output_Node(p_Node->Right);
          }
          
          void output_Current_Node()
          {
               if (Current == NULL){ return; }
               output_Node(*Current);
          }
          
          
          void funger_Tree(void (*p_Function(string)))
          {
               funger_Node(Root, p_Function);
          }
          
          void funger_Node(c_CMD_Variable_Node *&p_Node, void (*p_Function(string)))
          {
               if (p_Node == NULL){ return; }
               funger_Node(p_Node->Left, p_Function);
               (*p_Function)(p_Node->Var_Name);
               funger_Node(p_Node->Right, p_Function);
          }
          
          //Gets the data type from the current node after searching the tree.
          int get_Var_Type(string p_Var_Name)            { search(p_Var_Name); return get_Var_Type(*Current);    }
          int get_Var_Type(c_CMD_Variable_Node * &p_Node){ if (p_Node != NULL){ return p_Node->Var_Type; } else { return 0; } }
          
          
          //Changes the name of a registered variable, if it already exists then I dunno, don't do that.
          string change_name(string p_Var_Name, string p_Var_New_Name){ search(p_Var_Name); change_name(*Current, p_Var_New_Name); return "1"; }
          string change_name(c_CMD_Variable_Node * &p_Node, string p_Var_New_Name){ p_Node->Var_Name = p_Var_New_Name; return "1"; }
          
          
          //Gets the data from the current node after searching the tree.
          string get_string(string p_Var_Name)    { search(p_Var_Name); return get_string(*Current);    }
          int    get_int(string p_Var_Name)       { search(p_Var_Name); return get_int(*Current);       }
          float  get_float(string p_Var_Name)     { search(p_Var_Name); return get_float(*Current);     }
          void * get_reference(string p_Var_Name) { search(p_Var_Name); return get_reference(*Current); }
          bool   get_bool(string p_Var_Name)      { search(p_Var_Name); return get_bool(*Current);      }
          
          string get_string(c_CMD_Variable_Node * &p_Node)    { if (p_Node != NULL){ return p_Node->get_string();    } else { return "";    } }
          int    get_int(c_CMD_Variable_Node * &p_Node)       { if (p_Node != NULL){ return p_Node->get_int();       } else { return -1;     } }
          float  get_float(c_CMD_Variable_Node * &p_Node)     { if (p_Node != NULL){ return p_Node->get_float();     } else { return -1.0;   } }
          void * get_reference(c_CMD_Variable_Node * &p_Node) { if (p_Node != NULL){ return p_Node->get_reference(); } else { return NULL;  } }
          bool   get_bool(c_CMD_Variable_Node * &p_Node)      { if (p_Node != NULL){ return p_Node->get_bool();      } else { return false; } }
          
          
          //Searches for the variable name given, if not found a node is created for it. After establishing which one the node is the data is set.
          void set_string(string p_Var_Name, string p_Data)    { search(p_Var_Name); set_string(*Current, p_Data);    return; }
          void set_int(string p_Var_Name, int p_Data)          { search(p_Var_Name); set_int(*Current, p_Data);       return; }
          void set_float(string p_Var_Name, float p_Data)      { search(p_Var_Name); set_float(*Current, p_Data);     return; }
          void set_reference(string p_Var_Name, void * p_Data) { search(p_Var_Name); set_reference(*Current, p_Data); return; }
          void set_bool(string p_Var_Name, bool p_Data)        { search(p_Var_Name); set_bool(*Current, p_Data);      return; }
          
          void set_string(c_CMD_Variable_Node * &p_Node, string p_Data)    { if (p_Node != NULL){ p_Node->set_string(p_Data);    } }
          void set_int(c_CMD_Variable_Node * &p_Node, int p_Data)          { if (p_Node != NULL){ p_Node->set_int(p_Data);       } }
          void set_float(c_CMD_Variable_Node * &p_Node, float p_Data)      { if (p_Node != NULL){ p_Node->set_float(p_Data);     } }
          void set_reference(c_CMD_Variable_Node * &p_Node, void * p_Data) { if (p_Node != NULL){ p_Node->set_reference(p_Data); } }
          void set_bool(c_CMD_Variable_Node * &p_Node, bool p_Data)        { if (p_Node != NULL){ p_Node->set_bool(p_Data);      } }
          
                    
};

//The cell is the base unit for the table.

//The data types are; 0 = char, 1 = int, 2 = float, 3 = *void, 4 = bool
//These data types are mostly used for output.

class c_Cell_1D
{
protected:
     
     //The data in the cell. Is protected to avoid bounding errors.
     u_Data * Data;
     
     //The dimensions of the data in the cell.
     int Data_Depth;
     
     //The type of data containded within, used for output.
     int flg_Data_Type;
     
     //Whether or not the cell has been used.
     bool flg_Set;
     
     //The current position of the Push_Index.
     int Push_Index;
     
public:
     
     
     c_Cell_1D()
     {
          //Setup the array to use.
          Data = new u_Data[1];
          Data[0].I = 0;
          
          Push_Index = 0;
          Data_Depth = 1;
          
          //0 = character.
          flg_Data_Type = 1;
          
          flg_Set = 0;
     }
     
     ~c_Cell_1D()
     {
          delete [] Data;
          Data = NULL;
     }
     
     //--     MEMBERS FOR SETTING DATA
     
     
     //--     GETS A DATA BIT FROM THE CELL AT THE GIVEN INDEX
     
     //Character
     void set_Data_Bit_At_Given_Index_C(int p_Index, char p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].I = int (p_Data);
     }
     
     //Integer
     void set_Data_Bit_At_Given_Index_I(int p_Index, int p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].I = p_Data;
     }
     
     //Float
     void set_Data_Bit_At_Given_Index_F(int p_Index, float p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].F = p_Data;
     }
     
     //Node Reference
     void set_Data_Bit_At_Given_Index_NR(int p_Index, void * p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].NR = p_Data;
     }
     
     //Bool
     void set_Data_Bit_At_Given_Index_B(int p_Index, bool p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index].B = p_Data;
     }
     
     //u_Data
     void set_Data_Bit_At_Given_Index(int p_Index, u_Data p_Data)
     {
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Index);
          
          Data[p_Index] = p_Data;
     }
     
     
     //==--   SETTING DATA CHUNK
     
     //Sets the data in the cell.
     void set_C(char p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].I = int (p_Data[cou_Index]);
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 0);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_S(string p_Data)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data.size() + 1];
          
          //Copy the data into it.
          for (unsigned int cou_Index=0;cou_Index<(p_Data.size() + 1);cou_Index++)
          {
               tmp_Data[cou_Index].I = int (p_Data[cou_Index]);
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          //With .size() the +1 is not needed as we are not passing a NULL terminated sequence.
          set(tmp_Data, p_Data.size(), 0);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_I(int p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].I = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 1);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_F(float p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].F = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 2);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_NR(void * p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].NR = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 3);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set_B(bool p_Data[], int p_Data_Length)
     {
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[p_Data_Length];
          
          //Copy the data into it.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               tmp_Data[cou_Index].B = p_Data[cou_Index];
          }
          
          //Ship the data to the set function now that it is converted to the appropriate datatype.
          set(tmp_Data, p_Data_Length, 4);
          
          //Delete the tmp_Data.
          delete [] tmp_Data;
     }
     
     //Sets the data in the cell.
     void set(u_Data p_Data[], int p_Data_Length, int p_Data_Type)
     {
          //cout << "\n\t\t\t set p_Data_Type->" << p_Data_Type;
          //cout << "\n\t\t\t  p_Data_Length->" << p_Data_Length;
          
          //Wipe the cell first.
          reset_Cell();
          
          //Resize the cell to hold the data.
          expand_Cell_Data(p_Data_Length);
          
          //Read the data into the cell.
          for (int cou_Index=0;cou_Index<p_Data_Length;cou_Index++)
          {
               Data[cou_Index].I = p_Data[cou_Index].I;
          }
          
          //Sets the data type for the cell.
          flg_Data_Type = p_Data_Type;
          
          //Sets the flag for being set.
          flg_Set = 1;
     }
     
     //--     MEMBERS FOR PUSHING
     
     //Pushes a character.
     void push_C(char p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.I = int (p_Data);
          push(tmp_Data, 0);
     }
     
     //Pushes an int.
     void push_I(int p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.I = p_Data;
          push(tmp_Data, 1);
     }
     
     //Pushes a float.
     void push_F(float p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.F = p_Data;
          push(tmp_Data, 2);
     }
     
     //Pushes an address.
     void push_NR(void * p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.NR = p_Data;
          push(tmp_Data, 3);
     }
     
     //Pushes an address.
     void push_B(bool p_Data)
     {
          u_Data tmp_Data;
          tmp_Data.B = p_Data;
          push(tmp_Data, 4);
     }
     
     //Pushes a bit of data into the cell.
     void push(u_Data p_Data, int p_Data_Type)
     {
          //If there is no room in the cell then increase the cells size.
          if (Push_Index >= Data_Depth)
          {
               expand_Cell_Data(Data_Depth + 1);
          }
          
          //Assign the given value to the current index.
          Data[Push_Index].I = p_Data.I;
          
          //Increment the Input Pointer.
          Push_Index++;
          
          //Sets the data type for the cell.
          flg_Data_Type = p_Data_Type;
          
          //Sets the flag for being set.
          flg_Set = 1;
     }
     
     //--     MEMBERS FOR RETRIEVING DATA
     
     //Gets the current data_Depth;
     int get_Data_Depth()
     {
          return Data_Depth;
     }
     
     //Gets the set flag.
     bool get_Set_State()
     {
          return flg_Set;
     }
     
     //Gets the current push index.
     int get_Push_Index()
     {
          return Push_Index;
     }
     
     //--     GET DATA CHUNK FROM CELL BY PASSING AN ARRAY REFERENCE
     
     void get_Cell_Data_Chunk_Passed_S(string * p_String)
     {
          //Reset passed string.
          *p_String = "";
          
          //Read the data into the string as characters.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               *p_String += char (Data[cou_Index].I);
          }
     }
     
     void get_Cell_Data_Chunk_Passed_C(char * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new char[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = char (Data[cou_Index].I);
          }
     }
     
     void get_Cell_Data_Chunk_Passed_I(int * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new int[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].I;
          }
     }
     
     void get_Cell_Data_Chunk_Passed_F(float * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new float[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].F;
          }
     }
     
     void get_Cell_Data_Chunk_Passed_NR(void ** &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new void*[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].NR;
          }
     }
     
     void get_Cell_Data_Chunk_Passed_B(bool * &p_Passed_Array)
     {
          //Reset passed array.
          delete [] p_Passed_Array;
          p_Passed_Array = new bool[Data_Depth];
          
          //Read the data into the array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               p_Passed_Array[cou_Index] = Data[cou_Index].B;
          }
     }
     
     
     u_Data * get_Cell_Data()
     {
          return Data;
     }
     
     //--     GETS A DATA BIT FROM THE CELL AT THE GIVEN INDEX
     
     //Character
     char get_Data_Bit_From_Given_Index_C(int p_Index)
     {
          if (p_Index >= Data_Depth){ return char (0); }
          return char (Data[p_Index].I);
     }
     
     //Integer
     int get_Data_Bit_From_Given_Index_I(int p_Index)
     {
          if (p_Index >= Data_Depth){ return 0; cout << "\n\t\t\t\t returning 0;"; }
          return Data[p_Index].I;
     }
     
     //Float
     float get_Data_Bit_From_Given_Index_F(int p_Index)
     {
          if (p_Index >= Data_Depth){ return 0.0; }
          return Data[p_Index].F;
     }
     
     //Node Reference
     void * get_Data_Bit_From_Given_Index_NR(int p_Index)
     {
          if (p_Index >= Data_Depth){ return NULL; }
          return Data[p_Index].NR;
     }
     
     //Bool
     bool get_Data_Bit_From_Given_Index_B(int p_Index)
     {
          if (p_Index >= Data_Depth){ return 0; }
          return Data[p_Index].B;
     }
     
     //u_Data
     u_Data get_Data_Bit_From_Given_Index(int p_Index)
     {
          if (p_Index < Data_Depth){ return Data[0]; }
          return Data[p_Index];
     }
     
     //Gets the datatype for the cell.
     int get_flg_Data_Type()
     {
          return flg_Data_Type;
     }
     
     //Sets the flg_Data_Type.
     void set_flg_Data_Type(int p_flg_Data_Type)
     {
          flg_Data_Type = p_flg_Data_Type;
     }
     
     
     
     //--      ARRAY MANIPULATION MEMBERS
     
     //Expands the array in the given dimension by the given amount copying the data already held therein.
     void expand_Cell_Data(int p_Depth)
     {
          //If the cell already excedes the given depth then return.
          if (Data_Depth >= p_Depth){ return; }
          
          //Setup the temporary array.
          u_Data * tmp_Data = new u_Data[Data_Depth];
          
          //Copy the original data into the array.
          for (int cou_D=0;cou_D<Data_Depth;cou_D++)
          {
               //Copy the c_Data from Data to tmp_Data.
               tmp_Data[cou_D] = Data[cou_D];
          }
          
          //Reallocate the Data array.
          delete [] Data;
          Data = NULL;
          
          Data = new u_Data[p_Depth];
          
          //Copy the data back into the data array.
          for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
          {
               Data[cou_Index] = tmp_Data[cou_Index];
          }
          
          //Destroy tmp_Array;
          delete [] tmp_Data;
          tmp_Data = NULL;
          
          //Now create the u_Data for the new cells.
          for (int cou_Index=Data_Depth;cou_Index<p_Depth;cou_Index++)
          {
               Data[cou_Index].I = 0;
          }
          
          //Resize the data tracker to the appropriate amount.
          Data_Depth = p_Depth;
     }
     
     //Resets all of the cells data members.
     void reset_Cell()
     {
          //Erase the old data.
          delete [] Data;
          Data = NULL;
          
          //Setup the array to use.
          Data = new u_Data[1];
          
          Push_Index = 0;
          Data_Depth = 1;
     }
     
     //Output the cell data.
     void convert_Cell_To_String()
     {
          //cout << " ^" << flg_Data_Type;
          if (flg_Data_Type == 0)
          {
               return;
          }
          
          if (flg_Data_Type == 1)
          {
               set_S(int2str(Data[0].I));
               flg_Data_Type = 0;
               return;
          }
          
          if (flg_Data_Type == 2)
          {
               set_S(float2str(Data[0].F));
               flg_Data_Type = 0;
               return;
          }
          
          if (flg_Data_Type == 3)
          { 
               set_S(int2str(Data[0].I));
               flg_Data_Type = 0;
               return;
          }
          
          if (flg_Data_Type == 4)
          {
               set_S(bool2str(Data[0].B));
               flg_Data_Type = 0;
               return;
          }
     }
     
     //--     OUTPUT FUNCTIONS
     
     //Output the cell data.
     void output_Cell_Data()
     {
          //cout << " ^" << flg_Data_Type;
          if (flg_Data_Type == 0)
          {
               ochr(0, 8, '[');
               for (int cou_Index=0;cou_Index<Data_Depth;cou_Index++)
               {
                    if (char (Data[cou_Index].I) == '_')
                    { 
                         ochr(0, 9, char (Data[cou_Index].I));
                         continue;
                    }
                    ochr(0, 11, char (Data[cou_Index].I));
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 1)
          {
               ochr(0, 8, '[');
               ostr(0, 14, int2str(Data[0].I));
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    cout << " ";
                    ostr(0, 14, int2str(Data[cou_Index].I));
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 2)
          {
               ochr(0, 8, '[');
               ostr(0, 13, float2str(Data[0].F));
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    cout << " ";
                    ostr(0, 13, float2str(Data[cou_Index].F));
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 3)
          {
               ochr(0, 8, '[');
               cout << Data[0].NR;
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    cout << " " << Data[cou_Index].NR;                    
               }
               ochr(0, 8, ']'); cout << " ";
          }
          
          if (flg_Data_Type == 4)
          {
               ochr(0, 8, '[');
               ostr(0, 15, bool2str(Data[0].B));
               for (int cou_Index=1;cou_Index<Data_Depth;cou_Index++)
               {
                    ostr(0, 15, bool2str(Data[cou_Index].B));
               }
               ochr(0, 8, ']'); cout << " ";
          }
     }
     
     //Outputs the cell.
     void output_Cell()
     {
          cout << "\n\t\t\t c_Cell_1D->" << this;
          cout << "\n\t\t\t   Data_Depth->" << Data_Depth;
          cout << "\n\t\t\t   flg_Data_Type->" << flg_Data_Type;
          cout << "\n\t\t\t   flg_Set->" << flg_Set;
          cout << "\n\t\t\t   Push_Index->" << Push_Index;
          cout << "\n\t\t\t   Data->";
          output_Cell_Data();
     }
};
//This class holds and array of cells for the table rows.

class c_Row_1D
{
     
protected:
     
     //The cell array to use.
     c_Cell_1D ** Cells;
     
     //The current push index.
     int Push_Index;
     
     //The current cell count.
     int Cell_Count;
     
     
     
public:
     
     c_Row_1D()
     {
          Cells = new c_Cell_1D*[1];
          Cells[0] = new c_Cell_1D;
          
          Cell_Count = 1;
          Push_Index = 0;
          
     }
     
     ~c_Row_1D()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               delete Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          delete [] Cells;
          Cells = NULL;
          
     }
     
     //--     DATA SETTING FUNCTIONS
     
     void set_Data_Type_For_Current_Cell(int p_Data_Type)
     {
          Cells[Push_Index]->set_flg_Data_Type(p_Data_Type);
     }
     
     //--     PUSHING A SINGLE BIT OF DATA INTO THE CURRENT CELL WITHOUT POPPING, USED FOR BUILDING A PATTERN DURING BACKPROPAGATION FOR EXAMPLE
     
     //Pushes a bit of data into the current push cell. Used when doing backpropagation.
     void push_Data_Bit_Into_Current_Cell_C(char p_Data){    Cells[Push_Index]->push_C(p_Data); }
     void push_Data_Bit_Into_Current_Cell_I(int p_Data){     Cells[Push_Index]->push_I(p_Data); }
     void push_Data_Bit_Into_Current_Cell_F(float p_Data){   Cells[Push_Index]->push_F(p_Data); }
     void push_Data_Bit_Into_Current_Cell_NR(void * p_Data){ Cells[Push_Index]->push_NR(p_Data); }
     void push_Data_Bit_Into_Current_Cell_B(bool p_Data){    Cells[Push_Index]->push_B(p_Data); }
     void push_Data_Bit_Into_Current_Cell(u_Data p_Data, int p_Data_Type){ Cells[Push_Index]->push(p_Data, p_Data_Type); }
     
     //--     PUSHING A SINGLE BIT OF DATA ONTO THE CURRENT CELL AFTER POPPING
     
     void push_Data_Bit_F(float p_Data){    if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_F(p_Data); }
     void push_Data_Bit_I(int p_Data){      if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_I(p_Data); }
     void push_Data_Bit_NR(void * p_Data){  if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_NR(p_Data); }     
     void push_Data_Bit_B(bool p_Data){     if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->push_B(p_Data); }
     void push_Data_Bit_S(string p_Data){   if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } push_Data_Chunk_S(p_Data); }
     void push_Data_Bit(u_Data p_Data, int p_Data_Type){ if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } push_Data_Chunk(&p_Data, 0, p_Data_Type); }
     
     
     //--     PUSHING A DATA CHUNK ONTO THE CURRENT CELL AFTER POPPING IT
     
     void push_Data_Chunk_S(string p_Data)                       { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_S(p_Data); }
     void push_Data_Chunk_I(int p_Data[], int p_Data_Depth)      { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_I(p_Data, p_Data_Depth); }
     void push_Data_Chunk_F(float p_Data[], int p_Data_Depth)    { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_F(p_Data, p_Data_Depth); }
     void push_Data_Chunk_NR(void * p_Data[], int p_Data_Depth)  { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_NR(p_Data, p_Data_Depth); }
     void push_Data_Chunk_B(bool p_Data[], int p_Data_Depth)     { if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set_B(p_Data, p_Data_Depth); }
     void push_Data_Chunk(u_Data p_Data[], int p_Data_Depth, int p_Data_Type){ if (Cells[Push_Index]->get_Set_State()){ pop_Cell(); } Cells[Push_Index]->set(p_Data, p_Data_Depth, p_Data_Type); }
     
     //--     SETTING A GIVEN CELLS DATA TO THE GIVEN DATA CHUNK
     
     //Sets a given index to the given data set.
     void set_Given_Cell_Data_Chunk_S (int p_Cell, string p_Data)                     { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_S(p_Data); }
     void set_Given_Cell_Data_Chunk_C (int p_Cell, char p_Data[], int p_Data_Length)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_C(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_I (int p_Cell, int p_Data[], int p_Data_Length)   { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_I(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_F (int p_Cell, float p_Data[], int p_Data_Length) { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_F(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_NR(int p_Cell, void * p_Data[], int p_Data_Length){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_NR(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk_B (int p_Cell, bool p_Data[], int p_Data_Length)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_B(p_Data, p_Data_Length); }
     void set_Given_Cell_Data_Chunk   (int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set(p_Data, p_Data_Length, p_Data_Type); }
     
     
     //--     SETTING A GIVEN CELLS DATA TO THE GIVEN DATA BIT
     
     //Sets the given cells data to the given single value.
     void set_Given_Cell_Data_Bit_C (int p_Cell, char p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_C(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_S (int p_Cell, string p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_S(p_Data);}
     void set_Given_Cell_Data_Bit_I (int p_Cell, int p_Data)   { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_I(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_F (int p_Cell, float p_Data) { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_F(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_NR(int p_Cell, void * p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_NR(&p_Data, 1);}
     void set_Given_Cell_Data_Bit_B (int p_Cell, bool p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_B(&p_Data, 1);}
     void set_Given_Cell_Data_Bit   (int p_Cell, u_Data p_Data, int p_Data_Type){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set(&p_Data, 1, p_Data_Type);}
     
     
     //--     SETTING A GIVEN CELLS DATA TO THE GIVEN DATA BIT AT THE GIVEN INDEX
     
     //Sets the given cells data to the given single value.
     void set_Given_Cell_Data_Bit_At_Given_Index_C (int p_Cell, int p_Index, char p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_C(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_S (int p_Cell, int p_Index, string p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_C(p_Index, p_Data[0]); }
     void set_Given_Cell_Data_Bit_At_Given_Index_I (int p_Cell, int p_Index, int p_Data)   { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_I(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_F (int p_Cell, int p_Index, float p_Data) { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_F(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_NR(int p_Cell, int p_Index, void * p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_NR(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index_B (int p_Cell, int p_Index, bool p_Data)  { expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index_B(p_Index, p_Data); }
     void set_Given_Cell_Data_Bit_At_Given_Index   (int p_Cell, int p_Index, u_Data p_Data){ expand_Row_Depth(p_Cell + 1); Cells[p_Cell]->set_Data_Bit_At_Given_Index(p_Index, p_Data); }
     
     //Sets the flg_Data_Type for a given cell.
     //From here up it will be abstracted as Data_Type.
     int set_Given_Cells_Data_Type(int p_Cell, int p_Data_Type)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->set_flg_Data_Type(p_Data_Type);
          }
          return 0;
     }
     
     //--     GET FUNCTIONS
     
     //Gets the current cell count.
     int get_Cell_Count()
     {
          return Cell_Count;
     }
     
     //Gets the Data_Depth for a given cell.
     int get_Given_Cells_Data_Depth(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          return Cells[p_Cell]->get_Data_Depth();
     }
     
     //Gets the current cells data depth.
     int get_Current_Cells_Data_Depth()
     {
          return get_Given_Cells_Data_Depth(Push_Index);
     }
     
     //Gets the flg_Data_Type for a given cell.
     //From here up it will be abstracted as Data_Type.
     int get_Given_Cells_Data_Type(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          return Cells[p_Cell]->get_flg_Data_Type();
     }
     
     //Gets the set flag for the given cell.
     bool get_Given_Cells_Set_State(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          
          return Cells[p_Cell]->get_Set_State();
     }
          
     //Gets the set flag for the current cell.
     bool get_Current_Cells_Set_State()
     {
          return get_Given_Cells_Set_State(Push_Index);
     }
     
     //Gets the set flag for the given cell.
     bool get_Given_Cells_Push_Index(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          
          return Cells[p_Cell]->get_Push_Index();
     }
     
     //Gets the reference to a given cell.
     void * get_Cell_Reference(int p_Cell)
     {
          expand_Row_Depth(p_Cell + 1);
          
          return Cells[p_Cell];
     }
     
     //Gets the current push index.
     int get_Push_Index()
     {
          return Push_Index;
     }
     
     //--     GETS A GIVEN CELLS DATA CHUNK USING AN ARRAY REFERENCE PASSED
     
     //String
     void get_Given_Cells_Data_Chunk_Passed_S(int p_Cell, string * p_String)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_S(p_String);
               return;
          }
          *p_String = "NULL";
     }
     
     //Character
     void get_Given_Cells_Data_Chunk_Passed_C(int p_Cell, char * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_C(p_Passed_Data);
          }
     }
     
     //Integer
     void get_Given_Cells_Data_Chunk_Passed_I(int p_Cell, int * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_I(p_Passed_Data);
          }
     }
     
     //Float
     void get_Given_Cells_Data_Chunk_Passed_F(int p_Cell, float * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_F(p_Passed_Data);
          }
     }
     
     //Node Reference
     void get_Given_Cells_Data_Chunk_Passed_NR(int p_Cell, void ** &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_NR(p_Passed_Data);
          }
     }
     
     //Bool
     void get_Given_Cells_Data_Chunk_Passed_B(int p_Cell, bool * &p_Passed_Data)
     {
          if (p_Cell < Cell_Count)
          {
               Cells[p_Cell]->get_Cell_Data_Chunk_Passed_B(p_Passed_Data);
          }
     }
     
     //--     GETS THE GIVEN CELLS RAW DATA
     
     //Gets the data from the given cell assuming it is in the array.
     u_Data * get_Given_Cells_Data(int p_Cell)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Cell_Data();
          }
          return NULL;
     }
     
     //--    GETS THE GIVEN CELLS DATA FROM A GIVEN INDEX
     
     //Char
     char get_Data_Bit_At_Given_Index_In_Given_Cell_C(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_C(p_Index);
          }
          return char (0);
     }
     
     //Int
     int get_Data_Bit_At_Given_Index_In_Given_Cell_I(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_I(p_Index);
          }
          return 0;
     }
     
     //Float
     float get_Data_Bit_At_Given_Index_In_Given_Cell_F(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_F(p_Index);
          }
          return 0.0;
     }
     
     //Node Reference
     void * get_Data_Bit_At_Given_Index_In_Given_Cell_NR(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_NR(p_Index);
          }
          return NULL;
     }
     
     //Bool
     bool get_Data_Bit_At_Given_Index_In_Given_Cell_B(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index_B(p_Index);
          }
          return false;
     }
     
     //u_Data
     u_Data get_Data_Bit_At_Given_Index_In_Given_Cell(int p_Cell, int p_Index)
     {
          if (p_Cell < Cell_Count)
          {
               return Cells[p_Cell]->get_Data_Bit_From_Given_Index(p_Index);
          }
          u_Data tmp_Data;
          tmp_Data.I = -2;
          return tmp_Data;
     }
     
     
     //--     ARRAY MANIPULATIONS
     
     //Increases the push index.
     void pop_Cell()
     {
          Push_Index++;
          if (Push_Index >= Cell_Count){ expand_Row_Depth(Push_Index + 1); }
     }
     
     //Increases the row count to the given amount.
     void expand_Row_Depth(int p_Depth)
     {
          //If the given number is less than what already exists then return.
          if (p_Depth < Cell_Count){ return; }
          
          //Setup the temporary array.
          c_Cell_1D ** tmp_Cells = new c_Cell_1D*[Cell_Count];
          
          //Copy the old data into the temporary cells.
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               tmp_Cells[cou_Index] = Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          
          //Reallocate the old array.
          delete [] Cells;
          Cells = new c_Cell_1D*[p_Depth];
          
          //Copy the old cells back into the old array.
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               Cells[cou_Index] = tmp_Cells[cou_Index];
               tmp_Cells[cou_Index] = NULL;
          }
          
          //Ini the new cells.
          for (int cou_Index=Cell_Count;cou_Index<p_Depth;cou_Index++)
          {
               Cells[cou_Index] = new c_Cell_1D;
          }
          
          //Set the current cell count.
          Cell_Count = p_Depth;
          
          //Cleanup.
          delete [] tmp_Cells;
          
     }
     
     //Converts each cell in the row into strings.
     string convert_Row_To_String()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               Cells[cou_Index]->convert_Cell_To_String();
          }
          return "Converted";
     }
     
     //Resets the row.
     void reset_Row()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               delete Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          delete [] Cells;
          
          Cells = new c_Cell_1D*[1];
          Cells[0] = new c_Cell_1D;
          
          Cell_Count = 0;
          Push_Index = -1;
     }
     
     
     //--     OUTPUT MEMBERS
     
     //Outputs a single cells data.
     void output_Given_Cell(int p_Cell)
     {
          Cells[p_Cell]->output_Cell_Data();
     }
     
     //Outputs the row data.
     void output_Row_Data(int p_Limit)
     {
          //If p_Limit is -1 that means take the limits off and output everything.
          if (p_Limit == -1 || p_Limit > Cell_Count){ p_Limit = Cell_Count; }
          
          for (int cou_Index=0;cou_Index<p_Limit;cou_Index++)
          {
               cout << cou_Index;
               output_Given_Cell(cou_Index);
          }
     }
     
     //Outputs the row.
     void output_Row(int p_Limit)
     {
          cout << "\n\t ";
          ostr(0, 15, "Row");
          ostr(0, 7, "->");
          cout << this;
          ostr(0, 15, " Cell_Count");
          ostr(0, 7, "->");
          cout << Cell_Count;
          ostr(0, 7, " | ");
          output_Row_Data(p_Limit);
     }
     
     
     
     //Outputs a single cells data.
     void output_Given_Cell_Verbose(int p_Cell)
     {
          Cells[p_Cell]->output_Cell();
     }
     
     //Outputs the row data.
     void output_Row_Data_Verbose()
     {
          for (int cou_Index=0;cou_Index<Cell_Count;cou_Index++)
          {
               output_Given_Cell_Verbose(cou_Index);
          }
     }
     
     //Outputs the row.
     void output_Row_Verbose()
     {
          cout << "\n\t\t Row->" << this << " Cell_Count->" << Cell_Count << " | ";
          output_Row_Data_Verbose();
     }
     
};//This class is the table class, it holds the table rows and allows for their manipulation.

//Used for polymorphic referencing.
class c_Base_Table
{
public:
     
     virtual ~c_Base_Table(){};
     
     virtual void set_Table_ID(int p_Table_ID)=0;
     virtual void set_Table_Name(string p_Table_Name)=0;
     virtual string get_Table_Name()=0;
     virtual int get_Table_Dimension()=0;
     virtual int row_Is_Empty(int p_Row)=0;
     virtual int rowc_Is_Empty()=0;
     virtual int table_Is_Empty()=0;
     virtual string get_Table_Data_Type()=0;
     
     virtual void output_Table()=0;
     virtual void output_Table_Header()=0;
     virtual void output_Table_Verbose()=0;
     
     virtual void * get_Row_Reference(int p_Row)=0;
     virtual void * get_Cell_Reference(int p_Row, int p_Cell)=0;
     virtual void * get_Current_Cell_Reference(int p_Row)=0;
     
     virtual int get_Row_Cell_Count(int p_Row)=0;
     virtual int get_Current_Row_Cell_Count()=0;
     virtual int get_Row_Count()=0;
     virtual int get_Current_Row()=0;
     virtual int get_Current_Cell()=0;
     virtual int get_Current_Cell_In_Given_Row(int p_Row)=0;
     
     
     //Sets a given rows cell to the given data.
     virtual void set_string(   int p_Row, int p_Cell, string p_String)=0;
     virtual void set_int(      int p_Row, int p_Cell, int p_Int)=0;
     virtual void set_float(    int p_Row, int p_Cell, float p_Float)=0;
     virtual void set_reference(int p_Row, int p_Cell, void * p_Void)=0;
     virtual void set_bool(     int p_Row, int p_Cell, bool p_Bool)=0;
     
     //Pushes a data chunk onto the given row.
     virtual void push_string(   int p_Row, string p_String)=0;
     virtual void push_int(      int p_Row, int p_Int)=0;
     virtual void push_float(    int p_Row, float p_Float)=0;
     virtual void push_reference(int p_Row, void * p_Void)=0;
     virtual void push_bool(     int p_Row, bool p_Bool)=0;
     
     //Pushes a data chunk onto the current row.
     virtual void pushc_string(   string p_String)=0;
     virtual void pushc_int(      int p_Int)=0;
     virtual void pushc_float(    float p_Float)=0;
     virtual void pushc_reference(void * p_Void)=0;
     virtual void pushc_bool(     bool p_Bool)=0;
     
     //Pops the current row then pushes a data chunk onto the given row.
     virtual void pop_push_string(   string p_String)=0;
     virtual void pop_push_int(      int p_Int)=0;
     virtual void pop_push_float(    float p_Float)=0;
     virtual void pop_push_reference(void * p_Void)=0;
     virtual void pop_push_bool(     bool p_Bool)=0;
     
     //Pushes a data chunk onto the current row then pops it.
     virtual void push_pop_string(   string p_String)=0;
     virtual void push_pop_int(      int p_Int)=0;
     virtual void push_pop_float(    float p_Float)=0;
     virtual void push_pop_reference(void * p_Void)=0;
     virtual void push_pop_bool(     bool p_Bool)=0;
     
     //Gets a given cells data in a given row.
     virtual string get_string(   int p_Row, int p_Cell)=0;
     virtual int    get_int(      int p_Row, int p_Cell, int p_Index=0)=0;
     virtual float  get_float(    int p_Row, int p_Cell, int p_Index=0)=0;
     virtual void * get_reference(int p_Row, int p_Cell, int p_Index=0)=0;
     virtual bool   get_bool(     int p_Row, int p_Cell, int p_Index=0)=0;
     virtual u_Data get_data(     int p_Row, int p_Cell, int p_Index=0)=0;
     
     //Gets a given cells data in a given row.
     virtual string getc_string(   )=0;
     virtual int    getc_int(      )=0;
     virtual float  getc_float(    )=0;
     virtual void * getc_reference()=0;
     virtual bool   getc_bool(     )=0;
     virtual u_Data getc_data(     )=0;
     
     //Copies the data from the submitted cell into itself.
     virtual string copy_Cell(int p_Row, int p_Cell, void * p_Cell_To_Copy)=0;
     virtual string copy_Row(int p_Row, void * p_Cell_To_Copy)=0;
     virtual string copy_Row(void * p_Row_To_Overwrite, void * p_Row_To_Copy)=0;
     virtual string copy_Table(void * p_Table_To_Copy)=0;
     
     virtual string swap_Row_Reference(int p_Row_One, int p_Row_Two)=0;
     virtual string overlay_Row(int p_Row, int p_Index, void * p_Row_To_Copy)=0;
     virtual string shift_Column(int p_Column_To_Shift)=0;
     virtual string shift_All_Rows(int p_Index)=0;
     virtual string translate_Row(void * p_From_Table, int p_Row, int p_Column, int p_Offset)=0;
     virtual string translate_Column(void * p_From_Table, int p_Column, int p_Offset, int p_Depth, int p_Row)=0;
     virtual string rotate_Table()=0;
     
     virtual string convert_Table_To_String()=0;
     
     //Resets the table.
     virtual void reset_Table()=0;
     
     //Sorts the table.
     virtual void bubble_Sort()=0;
     virtual void bubble_Sort_String()=0;
     virtual void bubble_Sort_Int()=0;
     virtual void bubble_Sort_Float()=0;
     virtual void bubble_Sort_Raw_Int()=0;
     virtual void bubble_Sort_Raw_Float()=0;
     
};

class c_Table_1D: public c_Base_Table
{
protected:
     
     //The rows for the table.
     c_Row_1D ** Rows;
     
     //The counter for the rows.
     int Row_Count;
     
     //The current push index for the rows.
     int Push_Index;
     
     //The name of the table.
     string Table_Name;
     
     //The tables ID as an int.
     int Table_ID;
     
public:
     
     c_Table_1D(string p_Table_Name = "Generic_Table")
     {
          Table_Name = p_Table_Name;
          Rows = new c_Row_1D*[1];
          Rows[0] = new c_Row_1D;
          Row_Count = 1;
          Push_Index = 0;
          Table_ID = 0;
     }
     
     ~c_Table_1D()
     {
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               delete Rows[cou_Index];
               Rows[cou_Index] = NULL;
          }
          delete [] Rows;
          Rows = NULL;
     }
     
     //Sets the ID of the table.
     void set_Table_ID(int p_ID)
     {
          Table_ID = p_ID;
     }
     
     //Gets the ID of the table.
     int get_Table_ID()
     {
          return Table_ID;
     }
     
     //Gets whether or not the table is empty.
     int table_Is_Empty()
     {
          if (!Rows[0]->get_Given_Cells_Set_State(0) && Row_Count == 1 && Rows[0]->get_Cell_Count() == 1){ return 1; }
          return 0;
     }
     
     //Gets whether or not the table is empty.
     int row_Is_Empty(int p_Row)
     {
          if (!bounds(p_Row)){ return 1; }
          if (!Rows[p_Row]->get_Given_Cells_Set_State(0) && Rows[p_Row]->get_Cell_Count() == 1){ return 1; }
          return 0;
     }
     
     //Gets whether or not the table is empty.
     int rowc_Is_Empty()
     {
          if (!Rows[Push_Index]->get_Given_Cells_Set_State(0) && Rows[Push_Index]->get_Cell_Count() == 1){ return 1; }
          return 0;
     }
     
     //Gets the dimension of the table, hardcoded into the classes.
     int get_Table_Dimension(){ return 1; }
     
     //Sets the name of the table to the given name.
     void set_Table_Name(string p_Table_Name)
     {
          Table_Name = p_Table_Name;
     }
     
     //Gets the name of the table.
     string get_Table_Name()
     {
          return Table_Name;
     }
     
     //Sets the datatype for the current push index.
     void set_Data_Type_For_Current_Cell_In_Current_Row(int p_Data_Type)
     {
          Rows[Push_Index]->set_Data_Type_For_Current_Cell(p_Data_Type);
     }
     
     
     //--     PUSHING DATA BITS INTO THE CURRENT CELL ON THE CURRENT ROW
     
     
     //Pushes a data bit into the current cell in the current row.
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_S (string p_Data) { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_C(p_Data.at(0));  }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_C (char p_Data)   { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_C(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_I (int p_Data)    { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_I(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_F (float p_Data)  { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_F(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_NR(void * p_Data) { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_NR(p_Data);       }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row_B (bool p_Data)   { Rows[Push_Index]->push_Data_Bit_Into_Current_Cell_B(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Current_Row   (u_Data p_Data, int p_Data_Type = 0){ Rows[Push_Index]->push_Data_Bit_Into_Current_Cell(p_Data, p_Data_Type); }
     
     //Pushes a bit of data into the current cell on the given row.
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_S (int p_Row, string p_Data) { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_C(p_Data.at(0));  }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_C (int p_Row, char p_Data)   { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_C(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_I (int p_Row, int p_Data)    { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_I(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_F (int p_Row, float p_Data)  { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_F(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_NR(int p_Row, void * p_Data) { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_NR(p_Data);       }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row_B (int p_Row, bool p_Data)   { xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell_B(p_Data);        }
     void push_Data_Bit_Into_Current_Cell_On_Given_Row   (int p_Row, u_Data p_Data, int p_Data_Type = 0){ xTble(p_Row + 1); Rows[p_Row]->push_Data_Bit_Into_Current_Cell(p_Data, p_Data_Type); }
     
     //--     PUSHING DATA ONTO THE CURRENT ROW ONE PIECE AT A TIME
     
     //Pushes a string onto the current row.
     void push_Data_Bit_Onto_Current_Row_S(string p_Data)  { Rows[Push_Index]->push_Data_Bit_S(p_Data);  }
     void push_Data_Bit_Onto_Current_Row_I(int p_Data)     { Rows[Push_Index]->push_Data_Bit_I(p_Data);  }
     void push_Data_Bit_Onto_Current_Row_F(float p_Data)   { Rows[Push_Index]->push_Data_Bit_F(p_Data);  }
     void push_Data_Bit_Onto_Current_Row_NR(void * p_Data) { Rows[Push_Index]->push_Data_Bit_NR(p_Data); }
     void push_Data_Bit_Onto_Current_Row_B(bool p_Data)    { Rows[Push_Index]->push_Data_Bit_B(p_Data);  }
     
     
     //--     PUSHING DATASETS ONTO THE CURRENT ROW
     
     //Pushes a data set onto the current row.
     void push_Data_Chunk_Onto_Current_Row_S(string p_Data)                      { Rows[Push_Index]->push_Data_Chunk_S(p_Data); }
     void push_Data_Chunk_Onto_Current_Row_I(int p_Data[], int p_Data_Depth)     { Rows[Push_Index]->push_Data_Chunk_I(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row_F(float p_Data[], int p_Data_Depth)   { Rows[Push_Index]->push_Data_Chunk_F(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row_NR(void * p_Data[], int p_Data_Depth) { Rows[Push_Index]->push_Data_Chunk_NR(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row_B(bool p_Data[], int p_Data_Depth)    { Rows[Push_Index]->push_Data_Chunk_B(p_Data, p_Data_Depth); }
     void push_Data_Chunk_Onto_Current_Row(u_Data p_Data[], int p_Data_Depth, int p_Data_Type){ Rows[Push_Index]->push_Data_Chunk(p_Data, p_Data_Depth, p_Data_Type); }
     
     //--     PUSHING DATA ONTO THE CURRENT ROW THEN POPPING THE ROW
     
     //Pushes a data set onto the current row and then pops it.
     void push_Data_Set_S(string p_Data)                     { Rows[Push_Index]->push_Data_Chunk_S(p_Data);                  pop_Row(); }
     void push_Data_Set_I(int p_Data[], int p_Data_Depth)    { Rows[Push_Index]->push_Data_Chunk_I(p_Data, p_Data_Depth);    pop_Row(); }
     void push_Data_Set_F(float p_Data[], int p_Data_Depth)  { Rows[Push_Index]->push_Data_Chunk_F(p_Data, p_Data_Depth);    pop_Row(); }
     void push_Data_Set_NR(void * p_Data[], int p_Data_Depth){ Rows[Push_Index]->push_Data_Chunk_NR(p_Data, p_Data_Depth);   pop_Row(); }
     void push_Data_Set_B(bool p_Data[], int p_Data_Depth)   { Rows[Push_Index]->push_Data_Chunk_B(p_Data, p_Data_Depth);    pop_Row(); }
     void push_Data_Set(u_Data p_Data[], int p_Data_Depth, int p_Data_Type){ Rows[Push_Index]->push_Data_Chunk(p_Data, p_Data_Depth, p_Data_Type); pop_Row(); }
     
     //--     SETTING A GIVEN CELLS DATA IN A GIVEN ROW, SINGLE VALUE
     
     //Sets a given cells data to a given value in a given row.
     void set_Data_Bit_For_Given_Cell_In_Given_Row_S (int p_Row, int p_Cell,  string p_Data){ xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_S(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_C (int p_Row, int p_Cell, char p_Data)   { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_C(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_I (int p_Row, int p_Cell, int p_Data)    { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_I(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_F (int p_Row, int p_Cell, float p_Data)  { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_F(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_NR(int p_Row, int p_Cell, void * p_Data) { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_NR(p_Cell, p_Data); }
     void set_Data_Bit_For_Given_Cell_In_Given_Row_B (int p_Row, int p_Cell, bool p_Data)   { xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit_B(p_Cell, p_Data);  }
     void set_Data_Bit_For_Given_Cell_In_Given_Row   (int p_Row, int p_Cell, u_Data p_Data, int p_Data_Type){ xTble(p_Row + 1); Rows[p_Row]->set_Given_Cell_Data_Bit(p_Cell, p_Data, p_Data_Type); }
     
     //--     SETTING A GIVEN DATA SET FOR A GIVEN CELL IN A GIVEN ROW
     
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_S (int p_Row, int p_Cell, string p_Data){                      xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_S (p_Cell, p_Data); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_C (int p_Row, int p_Cell, char p_Data[],   int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_C (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_I (int p_Row, int p_Cell, int p_Data[],    int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_I (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_F (int p_Row, int p_Cell, float p_Data[],  int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_F (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_NR(int p_Row, int p_Cell, void * p_Data[], int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_NR(p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row_B (int p_Row, int p_Cell, bool p_Data[],   int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk_B (p_Cell, p_Data, p_Data_Length); }
     void set_Data_Chunk_For_Given_Cell_In_Given_Row   (int p_Row, int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type){ xTble(p_Row+1); Rows[p_Row]->set_Given_Cell_Data_Chunk(p_Cell, p_Data, p_Data_Length, p_Data_Type); }
     
     //--     PUSHES A Chunk ONTO CURRENT ROW
     
     void push_Data_Chunk_Onto_Given_Row_S (int p_Row, string p_Data){                      xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_S (p_Data); }
     void push_Data_Chunk_Onto_Given_Row_I (int p_Row, int p_Data[],    int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_I (p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row_F (int p_Row, float p_Data[],  int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_F (p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row_NR(int p_Row, void * p_Data[], int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_NR(p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row_B (int p_Row, bool p_Data[],   int p_Data_Length){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk_B (p_Data, p_Data_Length); }
     void push_Data_Chunk_Onto_Given_Row   (int p_Row, u_Data p_Data[], int p_Data_Length, int p_Data_Type){ xTble(p_Row+1); Rows[p_Row]->push_Data_Chunk(p_Data, p_Data_Length, p_Data_Type); }
     
     
     //--     PUSHES A BIT ONTO CURRENT ROW
     
     void push_Data_Bit_Onto_Given_Row_S (int p_Row, string p_Data){ xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_S (p_Data); }
     void push_Data_Bit_Onto_Given_Row_I (int p_Row, int p_Data)   { xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_I (p_Data); }
     void push_Data_Bit_Onto_Given_Row_F (int p_Row, float p_Data) { xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_F (p_Data); }
     void push_Data_Bit_Onto_Given_Row_NR(int p_Row, void * p_Data){ xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_NR(p_Data); }
     void push_Data_Bit_Onto_Given_Row_B (int p_Row, bool p_Data)  { xTble(p_Row+1); Rows[p_Row]->push_Data_Bit_B (p_Data); }
     void push_Data_Bit_Onto_Given_Row   (int p_Row, u_Data p_Data, int p_Data_Type){ xTble(p_Row+1); Rows[p_Row]->push_Data_Bit(p_Data, p_Data_Type); }
     
     
     //--     GET FUNCTIONS
     
     //Gets the number of rows in the table.
     int get_Row_Count()
     {
          return Row_Count;
     }
     
     //Gets the current number of cells in a given row.
     int get_Row_Cell_Count(int p_Row)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Cell_Count();
     }
     
     //Gets the current number of cells in the current row.
     int get_Current_Row_Cell_Count()
     {
          return get_Row_Cell_Count(Push_Index);
     }
     
     //Gets a reference to a row.
     void * get_Row_Reference(int p_Row)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row];
     }
     
     //Gets a reference to a cell.
     void * get_Cell_Reference(int p_Row, int p_Cell)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row]->get_Cell_Reference(p_Cell);
     }
     
     //Gets a reference to the current cell in the given row.
     void * get_Current_Cell_Reference(int p_Row)
     {
          return get_Cell_Reference(p_Row, get_Current_Cell_In_Given_Row(p_Row));
     }
     
     //Gets the current row.
     int get_Current_Row()
     {
          return Push_Index;
     }
     
     //Gets the current cells index in the current row.
     int get_Current_Cell()
     {
          return Rows[Push_Index]->get_Push_Index();
     }
     
     int get_Current_Cell_In_Given_Row(int p_Row)
     {
          if (!bounds(p_Row)){ return 0; }
          return Rows[p_Row]->get_Push_Index();
     }
     
     void * get_Current_Cell_Reference_In_Given_Row(int p_Row)
     {
          if (!bounds(p_Row)){ return NULL; }
          return get_Cell_Reference(p_Row, Rows[p_Row]->get_Push_Index());
     }
     
     //--    GETS THE DATA LENGTH
     
     //Gets the Data_Length for a given cell.
     int get_Data_Depth_For_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row]->get_Given_Cells_Data_Depth(p_Cell);
     }
     
     //Gets the data depth for the current cell in the given row.
     int get_Data_Depth_For_Current_Cell_In_Given_Row(int p_Row)
     {
          expand_Table(p_Row + 1);
          
          return Rows[p_Row]->get_Current_Cells_Data_Depth();
     }
     
     //Gets the data depth for the given cell in the current row.
     int get_Data_Depth_For_Given_Cell_In_Current_Row(int p_Cell)
     {
          return get_Data_Depth_For_Given_Cell_In_Given_Row(Push_Index, p_Cell);
     }
     
     //Gets the data depth for the current cell in the current row.
     int get_Data_Depth_For_Current_Cell_In_Current_Row()
     {
          return get_Data_Depth_For_Current_Cell_In_Given_Row(Push_Index);
     }     
     
     //--     GETS THE SET STATE FOR A CELL
     
     //Gets the Set_State for a given cell.
     int get_Set_State_For_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Given_Cells_Set_State(p_Cell);
     }
     
     //Gets the Set_State for the current cell in the given row.
     int get_Set_State_For_Current_Cell_In_Given_Row(int p_Row)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Current_Cells_Set_State();
     }
     
     //Gets the Set_State for the given cell in the current row.
     int get_Set_State_For_Given_Cell_In_Current_Row(int p_Cell)
     {
          return get_Set_State_For_Given_Cell_In_Given_Row(Push_Index, p_Cell);
     }
     
     //Gets the Set_State for the current cell in the current row.
     int get_Set_State_For_Current_Cell_In_Current_Row()
     {
          return get_Set_State_For_Current_Cell_In_Given_Row(Push_Index);
     }
     
     //Gets the Data_Type for a given cell.
     int get_Data_Type_For_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Given_Cells_Data_Type(p_Cell);
     }
     
     //--     GETS DATA FROM A GIVEN CELL IN A GIVEN ROW
     //Data aquired through these functions is by passing and array reference to it and setting the data.
     
     //String
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(int p_Row, int p_Cell, string * p_String)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_S(p_Cell, p_String);
     }
     
     //Character
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_C(int p_Row, int p_Cell, char * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_C(p_Cell, p_Passed_Data);
     }
     
     //Integer
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_I(int p_Row, int p_Cell, int * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_I(p_Cell, p_Passed_Data);
     }
     
     //Float
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_F(int p_Row, int p_Cell, float * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_F(p_Cell, p_Passed_Data);
     }
     
     //Node Reference
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_NR(int p_Row, int p_Cell, void ** &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_NR(p_Cell, p_Passed_Data);
     }
     
     //Bool
     void get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_B(int p_Row, int p_Cell, bool * &p_Passed_Data)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->get_Given_Cells_Data_Chunk_Passed_B(p_Cell, p_Passed_Data);
     }
     
     //--     GETS THE RAW u_Data FROM A GIVEN CELL IN A GIVEN ROW
     
     u_Data * get_Data_Chunk_From_Given_Cell_In_Given_Row(int p_Row, int p_Cell)
     {
          if (p_Row >= Row_Count){ return 0; }
          
          return Rows[p_Row]->get_Given_Cells_Data(p_Cell);
     }
     
     //--     GETS A DATA BIT FROM A GIVEN CELL IN A GIVEN ROW AT THE GIVEN INDEX
     
     //Char
     char get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_C(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_C(p_Cell, p_Index);
          }
          return char (0);
     }
     
     //Int
     int get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_I(p_Cell, p_Index);
          }
          return 0;
     }
     
     //Float
     float get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_F(p_Cell, p_Index);
          }
          return 0.0;
     }
     
     //Node Reference
     void * get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_NR(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_NR(p_Cell, p_Index);
          }
          return NULL;
     }
     
     //Bool
     bool get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell_B(p_Cell, p_Index);
          }
          return false;
     }
     
     //u_Data
     u_Data get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(int p_Row, int p_Cell, int p_Index)
     {
          if (bounds(p_Row))
          {
               return Rows[p_Row]->get_Data_Bit_At_Given_Index_In_Given_Cell(p_Cell, p_Index);
          }
          u_Data tmp_Data;
          tmp_Data.I = 0;
          return tmp_Data;
     }
     
     
     //==--     BUBBLE SORT
     
     string get_Table_Data_Type()
     {
          //0 = int
          //1 = float
          //2 = string
          
          int tmp_Datatype = 0;
          int tmp_gDatatype = 0;
          string tmp_Data;
          
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &tmp_Data);
               tmp_gDatatype = gdti(tmp_Data);
               
               if (tmp_gDatatype == 2){ return "string"; }
               if (tmp_Datatype < tmp_gDatatype)
               {
                    tmp_Datatype = tmp_gDatatype;
               }
          }
          
          if (tmp_Datatype == 0){ return "int"; }
          if (tmp_Datatype == 1){ return "float"; }
          
          return "string";
     }
     
     void bubble_Sort()
     {
          string tmp_Type = get_Table_Data_Type();
          
          if (tmp_Type == "string"){ bubble_Sort_String(); }
          if (tmp_Type == "int"){ bubble_Sort_Int(); }
          if (tmp_Type == "float"){ bubble_Sort_Float(); }
     }
     
     void bubble_Sort_String()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          string Row_Low_Index;
          string Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_Low_Index);
                     Rows[cou_Index + 1]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_High_Index);
                    
                    //Sorts High - Low
                    if (Row_Low_Index < Row_High_Index)
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     void bubble_Sort_Int()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          string Row_Low_Index;
          string Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_Low_Index);
                     Rows[cou_Index + 1]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_High_Index);
                    
                    //Sorts High - Low
                    if (str2int(Row_Low_Index) < str2int(Row_High_Index))
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     void bubble_Sort_Raw_Int()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          int Row_Low_Index;
          int Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Row_Low_Index = Rows[cou_Index]->get_Data_Bit_At_Given_Index_In_Given_Cell_I(0, 0);
                     Row_High_Index = Rows[cou_Index + 1]->get_Data_Bit_At_Given_Index_In_Given_Cell_I(0, 0);
                    
                    //Sorts High - Low
                    if (Row_Low_Index < Row_High_Index)
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     void bubble_Sort_Float()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          string Row_Low_Index;
          string Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Rows[cou_Index]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_Low_Index);
                     Rows[cou_Index + 1]->get_Given_Cells_Data_Chunk_Passed_S(0, &Row_High_Index);
                    
                    //Sorts High - Low
                    if (str2float(Row_Low_Index) < str2float(Row_High_Index))
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     void bubble_Sort_Raw_Float()
     {
          
          c_Row_1D tmp_Row;
          bool flg_Not_Done = 1;
          float Row_Low_Index;
          float Row_High_Index;
          
          while (flg_Not_Done)
          {
               flg_Not_Done = 0;
               
               for (int cou_Index=0;cou_Index<(Row_Count - 1);cou_Index++)
               {
                     Row_Low_Index = Rows[cou_Index]->get_Data_Bit_At_Given_Index_In_Given_Cell_F(0, 0);
                     Row_High_Index = Rows[cou_Index + 1]->get_Data_Bit_At_Given_Index_In_Given_Cell_F(0, 0);
                    
                    //Sorts High - Low
                    if (Row_Low_Index < Row_High_Index)
                    {
                         /*
                         copy_Row(&tmp_Row, Rows[cou_Index]);
                         
                         copy_Row(Rows[cou_Index], Rows[cou_Index + 1]);
                         copy_Row(Rows[cou_Index + 1], &tmp_Row);
                         */
                         
                         swap_Row_Reference(cou_Index, (cou_Index + 1));
                         
                         flg_Not_Done = 1;
                    }
               }
          }
     }
     
     

     
     //--    ARRAY MANIPULATION FUNCTIONS
     
     //Pops a cell in the given row.
     void pop_Cell(int p_Row)
     {
          if (p_Row >= Row_Count){ return; }
          
          Rows[p_Row]->pop_Cell();
     }
     
     //Pops a cell in the current push row.
     void pop_Cell_In_Current_Row()
     {
          Rows[Push_Index]->pop_Cell();
     }
     
     //Pops a row.
     void pop_Row()
     {
          Push_Index++;
          if (Push_Index >= Row_Count){ expand_Table(Push_Index + 1); }
     }
     
     void xTble()
     {
          expand_Table(Row_Count + 1);
     }
     
     void xTble(int p_Depth)
     {
          expand_Table(p_Depth);
     }
     
     //Resizes the table to the given dimension.
     void expand_Table()
     {
          expand_Table(Row_Count + 1);
     }
     
     //Resizes the table to the given dimension.
     void expand_Table(int p_Depth)
     {
          //If the given depth is less than the current depth then return as this would be a pointless operation.
          if (p_Depth <= Row_Count){ return; }
          
          //Setup the temporaray array to hold the rows.
          c_Row_1D ** tmp_Rows = new c_Row_1D*[Row_Count];
          
          //Copy the old row references into the temporary array.
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               tmp_Rows[cou_Index] = Rows[cou_Index];
               Rows[cou_Index] = NULL;
          }
          
          //Reallocate the rows array.
          delete [] Rows;
          Rows = new c_Row_1D*[p_Depth];
          
          //Copy the tmp into the reallocated row.
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               Rows[cou_Index] = tmp_Rows[cou_Index];
               tmp_Rows[cou_Index] = NULL;
          }
          
          //Ini the new rows.
          for (int cou_Index=Row_Count;cou_Index<p_Depth;cou_Index++)
          {
               Rows[cou_Index] = new c_Row_1D;
          }
          
          //Set the row count.
          Row_Count = p_Depth;
          
          //Cleanup
          delete [] tmp_Rows;
          
     }
     
     //resets the table.
     void reset_Table()
     {
          //cout << "\n\t\t Resetting Table->" << Table_Name;
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               delete Rows[cou_Index];
          }
          delete [] Rows;
          Rows = NULL;
          
          Rows = new c_Row_1D*[1];
          Rows[0] = new c_Row_1D;
          Row_Count = 1;
          Push_Index = 0;
          
          //cout << "    T_EOF->" << table_Is_Empty();
     }
     
     
     //--     FUNCTIONS FOR THE COMMAND PARSER TO USE
     
     
     //Pushes a data chunk onto the given row.
     void push_pop_string(   string p_String){ push_Data_Chunk_Onto_Given_Row_S(Push_Index, p_String); pop_Row(); }
     void push_pop_int(      int p_Int)      { push_Data_Bit_Onto_Given_Row_I(Push_Index, p_Int); pop_Row(); }
     void push_pop_float(    float p_Float)  { push_Data_Bit_Onto_Given_Row_F(Push_Index, p_Float); pop_Row(); }
     void push_pop_reference(void * p_Void)  { push_Data_Bit_Onto_Given_Row_NR(Push_Index, p_Void); pop_Row(); }
     void push_pop_bool(     bool p_Bool)    { push_Data_Bit_Onto_Given_Row_B(Push_Index, p_Bool); pop_Row(); }
     
     //Pushes a data chunk onto the given row.
     void pop_push_string(   string p_String){ pop_Row(); push_Data_Chunk_Onto_Given_Row_S(Push_Index, p_String); }
     void pop_push_int(      int p_Int)      { pop_Row(); push_Data_Bit_Onto_Given_Row_I(Push_Index, p_Int); }
     void pop_push_float(    float p_Float)  { pop_Row(); push_Data_Bit_Onto_Given_Row_F(Push_Index, p_Float); }
     void pop_push_reference(void * p_Void)  { pop_Row(); push_Data_Bit_Onto_Given_Row_NR(Push_Index, p_Void); }
     void pop_push_bool(     bool p_Bool)    { pop_Row(); push_Data_Bit_Onto_Given_Row_B(Push_Index, p_Bool); }
     
     //Pushes a data chunk onto the current row.
     void pushc_string(   string p_String){ push_Data_Chunk_Onto_Given_Row_S(Push_Index, p_String); }
     void pushc_int(      int p_Int)      { push_Data_Bit_Onto_Given_Row_I(Push_Index, p_Int); }
     void pushc_float(    float p_Float)  { push_Data_Bit_Onto_Given_Row_F(Push_Index, p_Float); }
     void pushc_reference(void * p_Void)  { push_Data_Bit_Onto_Given_Row_NR(Push_Index, p_Void); }
     void pushc_bool(     bool p_Bool)    { push_Data_Bit_Onto_Given_Row_B(Push_Index, p_Bool); }
     
     //Pushes a data chunk onto the given row.
     void push_string(   int p_Row, string p_String){ push_Data_Chunk_Onto_Given_Row_S(p_Row, p_String); }
     void push_int(      int p_Row, int p_Int)      { push_Data_Bit_Onto_Given_Row_I(p_Row, p_Int); }
     void push_float(    int p_Row, float p_Float)  { push_Data_Bit_Onto_Given_Row_F(p_Row, p_Float); }
     void push_reference(int p_Row, void * p_Void)  { push_Data_Bit_Onto_Given_Row_NR(p_Row, p_Void); }
     void push_bool(     int p_Row, bool p_Bool)    { push_Data_Bit_Onto_Given_Row_B(p_Row, p_Bool); }
     
     //Sets a given rows cell to the given data.
     void set_string(   int p_Row, int p_Cell, string p_String){ set_Data_Chunk_For_Given_Cell_In_Given_Row_S(p_Row, p_Cell, p_String); }
     void set_int(      int p_Row, int p_Cell, int p_Int)      { set_Data_Bit_For_Given_Cell_In_Given_Row_I(p_Row, p_Cell, p_Int); }
     void set_float(    int p_Row, int p_Cell, float p_Float)  { set_Data_Bit_For_Given_Cell_In_Given_Row_F(p_Row, p_Cell, p_Float); }
     void set_reference(int p_Row, int p_Cell, void * p_Void)  { set_Data_Bit_For_Given_Cell_In_Given_Row_NR(p_Row, p_Cell, p_Void); }
     void set_bool(     int p_Row, int p_Cell, bool p_Bool)    { set_Data_Bit_For_Given_Cell_In_Given_Row_B(p_Row, p_Cell, p_Bool); }
     
     //Gets a given cells data in a given row.
     string get_string(   int p_Row, int p_Cell){ string tmp_String = ""; get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(p_Row, p_Cell, &tmp_String); return tmp_String; }
     int    get_int(      int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(p_Row, p_Cell, p_Index); }
     float  get_float(    int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(p_Row, p_Cell, p_Index); }
     void * get_reference(int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_NR(p_Row, p_Cell, p_Index); }
     bool   get_bool(     int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(p_Row, p_Cell, p_Index); }
     u_Data get_data(     int p_Row, int p_Cell, int p_Index=0){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(p_Row, p_Cell, p_Index); }
     
     //Gets the current cells data in the current row.
     string getc_string(   ){ string tmp_String = ""; get_Data_Chunk_From_Given_Cell_In_Given_Row_Passed_S(Push_Index, Rows[Push_Index]->get_Push_Index(), &tmp_String); return tmp_String; }
     int    getc_int(      ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_I(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     float  getc_float(    ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_F(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     void * getc_reference(){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_NR(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     bool   getc_bool(     ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row_B(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     u_Data getc_data(     ){ return get_Data_Bit_At_Given_Index_In_Given_Cell_In_Given_Row(Push_Index, Rows[Push_Index]->get_Push_Index(), 0); }
     
     //Copies the data from the submitted cell into itself.
     string copy_Cell(int p_Row, int p_Cell, void * p_Cell_To_Copy)
     {
          if (p_Cell_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Cell_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Cell_To_Copy"; }
          
          expand_Table(p_Row + 1);
          
          //Gets a reference to the receiving cell.
          c_Cell_1D * tmp_Cell = (c_Cell_1D*) Rows[p_Row]->get_Cell_Reference(p_Cell);
          
          //Copy the cells.
          tmp_Cell->set(((c_Cell_1D*) p_Cell_To_Copy)->get_Cell_Data(), ((c_Cell_1D*) p_Cell_To_Copy)->get_Data_Depth(), ((c_Cell_1D*) p_Cell_To_Copy)->get_flg_Data_Type());
          
          
          return "CODE_4";
     }
     
     //Copies the data from the submitted row into itself.
     string copy_Row(int p_Row, void * p_Row_To_Copy)
     {
          if (p_Row_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; }
          
          expand_Table(p_Row + 1);
          
          //For readability cast the void to a row.
          c_Row_1D * tmp_Row_To_Copy = ((c_Row_1D*) p_Row_To_Copy);
          
          //Wipe the row that the data will be copied into.
          Rows[p_Row]->reset_Row();
          
          
          //For readability the data for the cell will be stored in the following temporary members.
          //void set_Given_Cell_Data_Chunk(int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type)
          u_Data * tmp_Data;
          int tmp_Data_Depth;
          int tmp_flg_Data_Type;
               
          //Copy the cells.
          for (int cou_Index=0;cou_Index<tmp_Row_To_Copy->get_Cell_Count();cou_Index++)
          {
               //Gets the data from the current cell.
               tmp_Data = tmp_Row_To_Copy->get_Given_Cells_Data(cou_Index);
               tmp_Data_Depth = tmp_Row_To_Copy->get_Given_Cells_Data_Depth(cou_Index);
               tmp_flg_Data_Type = tmp_Row_To_Copy->get_Given_Cells_Data_Type(cou_Index);
               
               Rows[p_Row]->set_Given_Cell_Data_Chunk(cou_Index, tmp_Data, tmp_Data_Depth, tmp_flg_Data_Type);
          }
          
          
          
          return "CODE_4";
     }
     
     //Copies the data from the submitted row into itself.
     string copy_Row(void * p_Row_To_Overwrite, void * p_Row_To_Copy)
     {
          if (p_Row_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; }
          if (p_Row_To_Overwrite == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Overwrite"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Overwrite"; }
          
          //For readability cast the void to a row.
          c_Row_1D * tmp_Row_To_Copy = ((c_Row_1D*) p_Row_To_Copy);
          c_Row_1D * tmp_Row_To_Overwrite = ((c_Row_1D*) p_Row_To_Overwrite);
          
          //Wipe the row that the data will be copied into.
          tmp_Row_To_Overwrite->reset_Row();
          
          
          //For readability the data for the cell will be stored in the following temporary members.
          //void set_Given_Cell_Data_Chunk(int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type)
          u_Data * tmp_Data;
          int tmp_Data_Depth;
          int tmp_flg_Data_Type;
               
          //Copy the cells.
          for (int cou_Index=0;cou_Index<tmp_Row_To_Copy->get_Cell_Count();cou_Index++)
          {
               //Gets the data from the current cell.
               tmp_Data = tmp_Row_To_Copy->get_Given_Cells_Data(cou_Index);
               tmp_Data_Depth = tmp_Row_To_Copy->get_Given_Cells_Data_Depth(cou_Index);
               tmp_flg_Data_Type = tmp_Row_To_Copy->get_Given_Cells_Data_Type(cou_Index);
               
               tmp_Row_To_Overwrite->set_Given_Cell_Data_Chunk(cou_Index, tmp_Data, tmp_Data_Depth, tmp_flg_Data_Type);
          }
          
          
          
          return "CODE_4";
     }
     
     string swap_Row_Reference(int p_Row_One, int p_Row_Two)
     {
          c_Row_1D * tmp_Row;
          if (!bounds(p_Row_One) || !bounds(p_Row_Two)){ return "OOB_ERROR"; }
          
          tmp_Row = Rows[p_Row_One];
          Rows[p_Row_One] = Rows[p_Row_Two];
          Rows[p_Row_Two] = tmp_Row;
          tmp_Row = NULL;
          return "SWAP_ROW__1";
     }
     
     //Copies a set of rows from one table into another table.
     string copy_Row_Set(void * p_Table_To_Copy_From, int p_Start, int p_End, void * p_Table_To_Copy_To, int p_Offset)
     {
          void * tmp_Current_Row = NULL;
          //Gets the two tables.
          c_Table_1D * tmp_Table_To_Copy_From;
          tmp_Table_To_Copy_From = ((c_Table_1D*) p_Table_To_Copy_From);
          
          c_Table_1D * tmp_Table_To_Copy_To;
          tmp_Table_To_Copy_To = ((c_Table_1D*) p_Table_To_Copy_To);
          
          for (int cou_Row=0;cou_Row<=(p_End - p_Start);cou_Row++)
          {
               //Get the row we will be copying from.
               tmp_Current_Row = tmp_Table_To_Copy_From->get_Row_Reference(cou_Row + p_Start);
               
               
               //Copy it to the appropiate table with the appropriate offset.
               copy_Row(tmp_Table_To_Copy_To->get_Row_Reference(cou_Row + p_Offset), tmp_Current_Row);
               
          } 
          
          return "success";
     }
     
     //Copys a table into this table.
     virtual string copy_Table(void * p_Table_To_Copy)
     {
          //If now table then return.
          if (p_Table_To_Copy == NULL){ return "NULL_TABLE"; }
          
          //Create the temporary table pointer to use.
          c_Table_1D * tmp_Table = (c_Table_1D*) p_Table_To_Copy;
          
          //Reset this table.
          reset_Table();
          
          for (int cou_Row=0;cou_Row<tmp_Table->get_Row_Count();cou_Row++)
          {
               copy_Row(cou_Row, tmp_Table->get_Row_Reference(cou_Row));
          }
          
          return "1";
     }
     
     string translate_Row(void * p_From_Table, int p_Row, int p_Column, int p_Offset)
     {
          //The current cell to copy from.
          void * tmp_Cell = NULL;
          
          //The table to gather from.
          c_Table_1D * tmp_From_Table = ((c_Table_1D*) p_From_Table);
          
          for (int cou_Index=0;cou_Index<tmp_From_Table->get_Row_Cell_Count(p_Row);cou_Index++)
          {
               //Holds the current cell in the row.
               tmp_Cell = tmp_From_Table->get_Cell_Reference(p_Row, cou_Index);
               //cout << "\n\n\t Current_Cell->";
               
               copy_Cell((p_Offset + cou_Index), p_Column, tmp_Cell); 
               
          }
          
          return "translate_Row";
     }
     
     //Translates a column into a row using push(). Starts at the offset and continues down until either p_Depth or end of Table is hit.
     virtual string translate_Column(void * p_From_Table, int p_Column, int p_Offset, int p_Depth, int p_Row)
     {
          //Bounds checking.
          if (!bounds(p_Row)){ return "OOB_ERROR_translate_Column"; }
          
          //Get the table to draw the column from.
          c_Table_1D * tmp_From_Table = ((c_Table_1D*) p_From_Table);
          
          //Get the ending point.
          int tmp_Limit = (p_Depth + p_Offset);
          
          if (tmp_Limit > tmp_From_Table->get_Row_Count()){ tmp_Limit = tmp_From_Table->get_Row_Count(); }
          
          //Read the from table column into the given row.
          for (int cou_Index=p_Offset;cou_Index<tmp_Limit;cou_Index++)
          {
               copy_Cell(p_Row, get_Current_Cell_In_Given_Row(p_Row), tmp_From_Table->get_Cell_Reference(cou_Index, p_Column));
          }
          return "translate_Row";
     }
     
     //Copies the data from a row into another at a given bit index. The receiving row is not wiped.
     string overlay_Row(int p_Row, int p_Index, void * p_Row_To_Copy)
     {
          if (p_Row_To_Copy == NULL){ cout << "\n\n\t\t ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; return "ARG_ERR_NULL_POINTER->TABLE_COPY->p_Row_To_Copy"; }
          
          expand_Table(p_Row + 1);
          
          //For readability cast the void to a row.
          c_Row_1D * tmp_Row_To_Copy = ((c_Row_1D*) p_Row_To_Copy);
          
          
          //For readability the data for the cell will be stored in the following temporary members.
          //void set_Given_Cell_Data_Chunk(int p_Cell, u_Data p_Data[], int p_Data_Length, int p_Data_Type)
          u_Data * tmp_Data;
               
          //Copy the cells.
          for (int cou_Index=0;cou_Index<tmp_Row_To_Copy->get_Cell_Count();cou_Index++)
          {
               //Gets the data from the current cell.
               tmp_Data = tmp_Row_To_Copy->get_Given_Cells_Data(cou_Index);
               
               Rows[p_Row]->set_Given_Cell_Data_Bit_At_Given_Index(cou_Index, p_Index, tmp_Data[0]);
          }
          
          
          
          return "CODE_4";
     }
     
     string shift_Column(int p_Column_To_Shift)
     {
          
          //The current cell to copy from.
          void * tmp_Cell = NULL;
          
          
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               //Holds the current cell in the row.
               tmp_Cell = Rows[cou_Index]->get_Cell_Reference(p_Column_To_Shift);
               
               //cout << "\n\n\t Current_Cell->";
               
               copy_Cell(cou_Index, (p_Column_To_Shift + 1), tmp_Cell); 
               
               set_string(cou_Index, p_Column_To_Shift, " ");
          }
          return "1";
     }
     
     string shift_All_Rows(int p_Index)
     {
          if (!bounds(p_Index)){ return "0"; }
          
          expand_Table();
          
          for (int cou_Index=(Row_Count - 1);cou_Index>p_Index;cou_Index--)
          {
               copy_Row(cou_Index, Rows[cou_Index-1]);
          }
          
          delete Rows[p_Index];
          Rows[p_Index] = new c_Row_1D;
          
          return "1";
     }
     
     //Rotates a tables data. Rows are translated to columns.
     string rotate_Table()
     {
          c_Table_1D tmp_Table;
          tmp_Table.reset_Table();
          
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               tmp_Table.translate_Row(this, cou_Index, cou_Index, 0);
          }
          
          reset_Table();
          
          copy_Table(&tmp_Table);
          
          cout << "\n\nRotated Table.";
          output_Table();
          
          return "Table_Rotated";
     }
     
     //Converts all indexes in a table to strings.
     string convert_Table_To_String()
     {
          for (int cou_Index=0;cou_Index<Row_Count;cou_Index++)
          {
               Rows[cou_Index]->convert_Row_To_String();
          }
          return "Converted";
     }
     
     //Bounds checking.
     bool bounds(int p_Row)
     {
          if (p_Row >= Row_Count || p_Row < 0){ return 0; }
          return 1;
     }
     
     //--     OUTPUT FUNCTIONS
     
     //Outputs a given row.
     void output_Row(int p_Row, int p_Limit)
     {
          if (Rows[p_Row] == NULL){ return; }
          if (row_Is_Empty(p_Row)){ cout << " Row Is Empty..."; return; }
          Rows[p_Row]->output_Row_Data(p_Limit);
     }
     
     //Outputs all the rows in the table.
     void output_All_Rows(int p_Limit = 100)
     {
          for (int cou_Index=0;cou_Index<Row_Count - 1;cou_Index++)
          {
               cout << "\n\t    ";
               ochr(0, 8, char (195));
               ostr(0, 7, "<");
               cout << cou_Index;
               ostr(0, 7, ">-");
               output_Row(cou_Index, p_Limit);
          }
          
          cout << "\n\t    ";
          ochr(0, 8, char (192));
          ostr(0, 7, "<");
          cout << Row_Count - 1;
          ostr(0, 7, ">-");
          
          output_Row(Row_Count - 1, p_Limit);
     }
     
     //Outputs a row verbose.
     void output_Row_Verbose(int p_Row)
     {
          Rows[p_Row]->output_Row_Verbose();
     }
     
     //Output all the rows in verbose mode.
     void output_All_Rows_Verbose()
     {
          for (int cou_Index=0;cou_Index<Row_Count - 1;cou_Index++)
          {
               cout << "\n\t    " << char (195) << "<" << cou_Index << ">-";
               output_Row_Verbose(cou_Index);
          }
          cout << "\n\t    " << char (192) << "<" << (Row_Count - 1) << ">-";
          output_Row_Verbose(Row_Count - 1);
     }
     
     //Outputs the table.
     void output_Table()
     {
          cout << "\n\n\t ";
          ostr(0, 15, "Table");
          ostr(0, 7, "[");
          cout << Table_ID;
          ostr(0, 7, "]->");
          ostr(0, 15, Table_Name);
          cout << "\n\t   ";
          if (table_Is_Empty()){ cout << " Table Is Empty..."; return; }
          cout << Row_Count;
          ostr(0, 7, "->");
          cout << Row_Count;
          output_All_Rows();
     }
     
     //Outputs the table in verbose mode.
     void output_Table_Verbose()
     {
          cout << "\n\n\t Table->" << Table_Name;
          cout << "\n\t   Table_ID->" << Table_ID;
          cout << "\n\t   Row_Count->" << Row_Count;
          cout << "\n\t   Push_Index->" << Push_Index;
          output_All_Rows_Verbose();
     }
     
     //Outputs the information for the table without its contained data.
     void output_Table_Header()
     {
          cout << " #Rows" << Row_Count << " " << Table_Name;
     }
     
     //Saves a table int the given mode in the given file, otherwise defaults to current table ID.Table_Name.svc;
     void save_Table(string p_Mode = "csv", string p_Name = "Default")
     {
		 cout << "\n Saving Table: " << p_Name << "." << p_Mode;
          if (p_Name == "Default")
          {
               p_Name = "Tables\\" + int2str(Table_ID) + "." + Table_Name + "." + p_Mode;
          }

          output_Table();
          
          //Open the file to write to.
          ofstream SF;
          SF.open(p_Name.c_str());
          
          //Save the table.
          if (p_Mode == "ssv")
          {
               for (int cou_Row=0;cou_Row<Row_Count;cou_Row++)
               {
                    for (int cou_Index=0;cou_Index<Rows[cou_Row]->get_Cell_Count();cou_Index++)
                    {
                         switch (get_Data_Type_For_Given_Cell_In_Given_Row(cou_Row, cou_Index))
                         {
                              //String
                              case 0:
                                   SF << get_string(cou_Row, cou_Index) << " ";
                                   break;
                                   
                              //Int
                              case 1:
                                   SF << get_int(cou_Row, cou_Index) << " ";
                                   break;
                                   
                              //Float
                              case 2:
                                   SF << get_float(cou_Row, cou_Index) << " ";
                                   break;
                         }
                         
                    }
                    SF << "\n";
               }
          }
          
          if (p_Mode == "csv")
          {
               for (int cou_Row=0;cou_Row<Row_Count;cou_Row++)
               {
                    for (int cou_Index=0;cou_Index<Rows[cou_Row]->get_Cell_Count();cou_Index++)
                    {
                         if (cou_Index > 0){ SF << ", "; }
                         switch (get_Data_Type_For_Given_Cell_In_Given_Row(cou_Row, cou_Index))
                         {
                              //String
                              case 0:
                                   SF << get_string(cou_Row, cou_Index);
                                   break;
                                   
                              //Int
                              case 1:
                                   SF << get_int(cou_Row, cou_Index);
                                   break;
                                   
                              //Float
                              case 2:
                                   SF << get_float(cou_Row, cou_Index);
                                   break;
                         }
                         
                    }
                    SF << ";\n";
               }
          }
          
          //Close the file.
          SF.close();
          
     }

     //Loads a table into the current table wiping out any existing data.
     bool load_Table(string p_Filename, int p_Type = 0)
     {
          //Setup the tmp variables.
          int tmp_Int;
          //float tmp_Float;
          string tmp_String;
		  string tmp_Data;
          string tmp_File_Type;
          
          p_Filename = "Tables\\" + p_Filename;
          cout << "\n\t load_Table: " << p_Filename;
          
          //Open the file to write to.
          ifstream SF;
          SF.open(p_Filename.c_str());
          
          if (!SF.is_open())
          {
			  cout << "\n Failed To Load Table";
               return 0;
          }
          
          //p_Types
          //0: Read checking each entry for type.
          //1: String
          //2: Int
          //3: Float
          //4: NR
          //5: Bool
          
          tmp_Int = p_Filename.size();
          tmp_File_Type = "";
          for (int cou_Index=(tmp_Int-3);cou_Index<(tmp_Int);cou_Index++)
          {
               tmp_File_Type += p_Filename[cou_Index];
          }

		  ifstream tmp_SF;
		  tmp_SF.open(p_Filename);
          
		  if (!tmp_SF.is_open())
		  {
			  cout << "\n File Failed To Open: " << p_Filename;
		  }

          if (tmp_File_Type == "csv")
          {
			  cout << "\n Comma Separated Volume Found...";
			  return 1;
          }
          
		  if (tmp_File_Type == "ssv")
		  {
			  cout << "\n Space Separated Volume Found...";

			  while (!tmp_SF.eof())
			  {
				  tmp_String = "";
				  getline(tmp_SF, tmp_String);
				  cout << "\n ->" << tmp_String << "<-";
				  if ((!table_Is_Empty()) && (tmp_String != "")) { pop_Row(); }

				  int flg_Var_Pos = 0;
				  int flg_String = 0;

				  //Loop through the command text finding variable identifiers and extracting them.
				  for (unsigned int cou_Index = 0; cou_Index < tmp_String.size(); cou_Index++)
				  {
					  //If a space is encountered then start on the next token.
					  if (tmp_String[cou_Index] != ' ')
					  {
						  tmp_Data += tmp_String[cou_Index];
						  continue;
					  }
					  if (tmp_String[cou_Index] == ' ')
					  {
						  if (tmp_Data != "")
						  {
							  pushc_string(tmp_Data);
							  tmp_Data = "";
						  }
					  }

				  }
			  }
			  return 1;
		  }

		  cout << "\n Unrecognized Filetype: " << p_Filename;


          /*
                         switch (get_Data_Type_For_Given_Cell_In_Given_Row(cou_Row, cou_Index))
                         {
                              //String
                              case 0:
                                   SF << get_string(cou_Row, cou_Index);
                                   break;
                                   
                              //Int
                              case 1:
                                   SF << get_int(cou_Row, cou_Index);
                                   break;
                                   
                              //Float
                              case 2:
                                   SF << get_float(cou_Row, cou_Index);
                                   break;
                         }
          */
		  return 1;
     }
};//Handles the tables. Allows for one manager for registering, tracking, and manipulating tables.
//Currently only tables with 1 dimensional data are implemented. 

//Handles all dimensions of tables.
class c_Table_Handler
{
protected:

	//The tables to be handled.
	//The reason for the c_Base_Table is that in the future tables that accept more than 1 dimensional data 
	// will be implemented and a polymorphic base class is useful here.
	c_Base_Table** Tables;

	//The current table count.
	int Table_Count;

	//A pointer to the first table in the table array used to track all registered tables.
	c_Table_1D* Registry_Table;

	//The table registry.
	c_Lookup_Tree Table_Registry;

public:

	c_Table_Handler()
	{
		//The first table in the array contains information on all the tables the handler currently has registered.
		//Due to the fact that the default table ID is 0 in the case of nonallocated tables someone may alter the
		//registry, however, it does not actually affect anything other than display data.

		//Allocate the memory for the registry table and create it.
		Tables = new c_Base_Table * [1];
		Tables[0] = new c_Table_1D;

		//Use the table members to assign the ID and name. 
		//Then in the lookup tree set the variable labeled "Table_Registry" to 0.
		Tables[0]->set_Table_ID(0);
		Tables[0]->set_Table_Name("Table_Registry");
		Table_Registry.set_int("Table_Registry", 0);

		//Assign the registry variable to the table just created. Then set the first cell in the first row to represent the registry.
		Registry_Table = (c_Table_1D*)Tables[0];
		Registry_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S(0, 0, "Table_Registry");

		//There is now the one table, the registry table.
		Table_Count = 1;
	}

	~c_Table_Handler()
	{
		if (Tables != NULL)
		{
			for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
			{
				if (Tables[cou_Index] != NULL)
				{
					delete Tables[cou_Index];
					Tables[cou_Index] = NULL;
				}
			}
		}
		if (Tables != NULL)
		{
			delete[] Tables;
			Tables = NULL;
		}
	}

	//Creates a new table and returns the table address.
	c_Table_1D* register_New_Table_1D(string p_Table_Name)
	{
		//Search for a NULL index, if found register the table there.
		//If a NULL index is found and the table created then the handle is returned.
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			if (Tables[cou_Index] == NULL)
			{
				Tables[cou_Index] = new c_Table_1D;
				Tables[cou_Index]->set_Table_ID(cou_Index);
				Tables[cou_Index]->set_Table_Name(p_Table_Name);
				Registry_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((cou_Index), 0, p_Table_Name);

				Table_Registry.set_int(p_Table_Name, cou_Index);
				return (c_Table_1D*)Tables[cou_Index];
			}
		}
		
		//Make room for the new table.
		increment_Table_Array();

		Tables[Table_Count - 1] = new c_Table_1D;
		Tables[Table_Count - 1]->set_Table_ID(Table_Count - 1);
		Tables[Table_Count - 1]->set_Table_Name(p_Table_Name);

		//Add the table to the lookup tree.
		Table_Registry.set_int(p_Table_Name, (Table_Count - 1));

		//Add the current table to the registry.
		Registry_Table->set_Data_Chunk_For_Given_Cell_In_Given_Row_S((Table_Count - 1), 0, p_Table_Name);


		return (c_Table_1D*)Tables[Table_Count - 1];

	}


	//Gets the number of tables.
	int get_Table_Count()
	{
		return Table_Count;
	}

	//Removes a table from the registry.
	void delete_Table(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		//cout << "\n\n\t Deleting Table[" << p_TID << "]";

		//Remove the table from the registry.
		Table_Registry.remove_Node(Tables[p_TID]->get_Table_Name());

		//Delete the table and set to NULL.
		delete Tables[p_TID];
		Tables[p_TID] = NULL;
		Registry_Table->set_string(p_TID, 0, " ");
	}

	//Removes a table using the table name.
	void delete_Table(string p_Table_Name)
	{
		delete_Table(get_Table_ID(p_Table_Name));
	}

	//Gets a reference to the current table.
	void* get_Table_Reference(int p_TID = 0)
	{
		if (p_TID < 0) { return NULL; }

		if (!bounds_TID(p_TID)) { return Tables[p_TID]; }

		return Tables[p_TID];
	}
	void* get_Table_Reference(string p_TName)
	{
		return get_Table_Reference(get_Table_ID(p_TName));
	}

	//Gets the ID of a given table from the given table name.
	int get_Table_ID(string p_Table_Name)
	{
		return Table_Registry.get_int(p_Table_Name);
	}

	//Gets the name of a given table.
	string get_Table_Name(int p_TID)
	{
		if (bounds_TID(p_TID)) { return Tables[p_TID]->get_Table_Name(); }
		return "Table_ID_Not_Found";
	}

	//Updates the name of a table in the registry.
	void set_Table_Name(int p_TID, string p_Table_Name)
	{
		if (!bounds_TID(p_TID)) { return; }

		//Update the registry lookup tree.
		Table_Registry.change_name(Tables[p_TID]->get_Table_Name(), p_Table_Name);

		Tables[p_TID]->set_Table_Name(p_Table_Name);
		Registry_Table->set_string(p_TID, 0, p_Table_Name);
	}
	void set_Table_Name(string p_TName, string p_Table_Name)
	{
		set_Table_Name(get_Table_ID(p_TName), p_Table_Name);
	}

	//Gets a reference to a given row in a given table.
	void* get_Table_Row_Reference(int p_TID, int p_Row)
	{
		if (!bounds_TID(p_TID)) { return NULL; }

		return Tables[p_TID]->get_Row_Reference(p_Row);
	}
	void* get_Table_Row_Reference(string p_TName, int p_Row)
	{
		return get_Table_Row_Reference(get_Table_ID(p_TName), p_Row);
	}


	//Gets a reference to a given cell in a given row in a given table.
	void* get_Table_Cell_Reference(int p_TID, int p_Row, int p_Cell)
	{
		if (!bounds_TID(p_TID)) { return NULL; }

		return Tables[p_TID]->get_Cell_Reference(p_Row, p_Cell);
	}
	void* get_Table_Cell_Reference(string p_TName, int p_Row, int p_Cell)
	{
		return get_Table_Cell_Reference(get_Table_ID(p_TName), p_Row, p_Cell);
	}

	//Gets the number of rows in the given table.
	int get_Row_Count(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return 0; }

		return Tables[p_TID]->get_Row_Count();
	}
	int get_Row_Count(string p_Table)
	{
		return get_Row_Count(get_Table_ID(p_Table));
	}

	//Gets the cell count of the given row.
	int get_Row_Cell_Count(int p_TID, int p_Row)
	{
		if (!bounds_TID(p_TID)) { return 0; }

		return Tables[p_TID]->get_Row_Cell_Count(p_Row);
	}
	int get_Row_Cell_Count(string p_Table, int p_Row)
	{
		return get_Row_Cell_Count(get_Table_ID(p_Table), p_Row);
	}

	//Gets whether or not the table is empty.
	int table_Is_Empty(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return 0; }

		return Tables[p_TID]->table_Is_Empty();
	}
	int table_Is_Empty(string p_Table)
	{
		return table_Is_Empty(get_Table_ID(p_Table));
	}

	//Resets a table.
	void reset_Table(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		Tables[p_TID]->reset_Table();
	}
	void reset_Table(string p_Table)
	{
		reset_Table(get_Table_ID(p_Table));
	}

	//--     TABLE MANIPULATIONS     --//

	//==--     Table ID Submission

	//Sets a given rows cell to the given data.
	void set_string(int p_TID, int p_Row, int p_Cell, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_string(p_Row, p_Cell, p_String); }
	void set_int(int p_TID, int p_Row, int p_Cell, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_int(p_Row, p_Cell, p_Int); }
	void set_float(int p_TID, int p_Row, int p_Cell, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_float(p_Row, p_Cell, p_Float); }
	void set_reference(int p_TID, int p_Row, int p_Cell, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_reference(p_Row, p_Cell, p_Void); }
	void set_bool(int p_TID, int p_Row, int p_Cell, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->set_bool(p_Row, p_Cell, p_Bool); }

	//Pushes a given rows cell to the given data.
	void push_string(int p_TID, int p_Row, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_string(p_Row, p_String); }
	void push_int(int p_TID, int p_Row, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_int(p_Row, p_Int); }
	void push_float(int p_TID, int p_Row, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_float(p_Row, p_Float); }
	void push_reference(int p_TID, int p_Row, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_reference(p_Row, p_Void); }
	void push_bool(int p_TID, int p_Row, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_bool(p_Row, p_Bool); }


	//Pushes a data onto the curren row.
	void pushc_string(int p_TID, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_string(p_String); }
	void pushc_int(int p_TID, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_int(p_Int); }
	void pushc_float(int p_TID, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_float(p_Float); }
	void pushc_reference(int p_TID, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_reference(p_Void); }
	void pushc_bool(int p_TID, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pushc_bool(p_Bool); }


	//Pushes a given rows cell to the given data.
	void pop_push_string(int p_TID, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_string(p_String); }
	void pop_push_int(int p_TID, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_int(p_Int); }
	void pop_push_float(int p_TID, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_float(p_Float); }
	void pop_push_reference(int p_TID, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_reference(p_Void); }
	void pop_push_bool(int p_TID, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->pop_push_bool(p_Bool); }


	//Pushes a given rows cell to the given data.
	void push_pop_string(int p_TID, string p_String) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_string(p_String); }
	void push_pop_int(int p_TID, int p_Int) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_int(p_Int); }
	void push_pop_float(int p_TID, float p_Float) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_float(p_Float); }
	void push_pop_reference(int p_TID, void* p_Void) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_reference(p_Void); }
	void push_pop_bool(int p_TID, bool p_Bool) { if (!bounds_TID(p_TID)) { return; } Tables[p_TID]->push_pop_bool(p_Bool); }

	//Gets a given cells data in a given row.
	string get_string(int p_TID, int p_Row, int p_Cell) { if (!bounds_TID(p_TID)) { return "Foul"; } return Tables[p_TID]->get_string(p_Row, p_Cell); }
	int    get_int(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return 0; }    return Tables[p_TID]->get_int(p_Row, p_Cell, p_Index); }
	float  get_float(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return 0.0; }  return Tables[p_TID]->get_float(p_Row, p_Cell, p_Index); }
	void* get_reference(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return NULL; } return Tables[p_TID]->get_reference(p_Row, p_Cell, p_Index); }
	bool   get_bool(int p_TID, int p_Row, int p_Cell, int p_Index = 0) { if (!bounds_TID(p_TID)) { return 0; }    return Tables[p_TID]->get_bool(p_Row, p_Cell, p_Index); }

	//Gets a given cells data in a given row.
	string getc_string(int p_TID) { if (!bounds_TID(p_TID)) { return "Foul"; } return Tables[p_TID]->getc_string(); }
	int    getc_int(int p_TID) { if (!bounds_TID(p_TID)) { return 0; }      return Tables[p_TID]->getc_int(); }
	float  getc_float(int p_TID) { if (!bounds_TID(p_TID)) { return 0.0; }    return Tables[p_TID]->getc_float(); }
	void* getc_reference(int p_TID) { if (!bounds_TID(p_TID)) { return NULL; }   return Tables[p_TID]->getc_reference(); }
	bool   getc_bool(int p_TID) { if (!bounds_TID(p_TID)) { return 0; }      return Tables[p_TID]->getc_bool(); }


	//==--    Table Name Submission

	//Sets a given rows cell to the given data.
	void set_string(string p_TName, int p_Row, int p_Cell, string p_String) { set_string(get_Table_ID(p_TName), p_Row, p_Cell, p_String); }
	void set_int(string p_TName, int p_Row, int p_Cell, int p_Int) { set_int(get_Table_ID(p_TName), p_Row, p_Cell, p_Int); }
	void set_float(string p_TName, int p_Row, int p_Cell, float p_Float) { set_float(get_Table_ID(p_TName), p_Row, p_Cell, p_Float); }
	void set_reference(string p_TName, int p_Row, int p_Cell, void* p_Void) { set_reference(get_Table_ID(p_TName), p_Row, p_Cell, p_Void); }
	void set_bool(string p_TName, int p_Row, int p_Cell, bool p_Bool) { set_bool(get_Table_ID(p_TName), p_Row, p_Cell, p_Bool); }

	//Sets a given rows cell to the given data.
	void push_string(string p_TName, int p_Row, string p_String) { push_string(get_Table_ID(p_TName), p_Row, p_String); }
	void push_int(string p_TName, int p_Row, int p_Int) { push_int(get_Table_ID(p_TName), p_Row, p_Int); }
	void push_float(string p_TName, int p_Row, float p_Float) { push_float(get_Table_ID(p_TName), p_Row, p_Float); }
	void push_reference(string p_TName, int p_Row, void* p_Void) { push_reference(get_Table_ID(p_TName), p_Row, p_Void); }
	void push_bool(string p_TName, int p_Row, bool p_Bool) { push_bool(get_Table_ID(p_TName), p_Row, p_Bool); }

	//Pushes a given rows cell to the given data.
	void pop_push_string(string p_TName, string p_String) { pop_push_string(get_Table_ID(p_TName), p_String); }
	void pop_push_int(string p_TName, int p_Int) { pop_push_int(get_Table_ID(p_TName), p_Int); }
	void pop_push_float(string p_TName, float p_Float) { pop_push_float(get_Table_ID(p_TName), p_Float); }
	void pop_push_reference(string p_TName, void* p_Void) { pop_push_reference(get_Table_ID(p_TName), p_Void); }
	void pop_push_bool(string p_TName, bool p_Bool) { pop_push_bool(get_Table_ID(p_TName), p_Bool); }


	//Pushes a given rows cell to the given data.
	void push_pop_string(string p_TName, string p_String) { push_pop_string(get_Table_ID(p_TName), p_String); }
	void push_pop_int(string p_TName, int p_Int) { push_pop_int(get_Table_ID(p_TName), p_Int); }
	void push_pop_float(string p_TName, float p_Float) { push_pop_float(get_Table_ID(p_TName), p_Float); }
	void push_pop_reference(string p_TName, void* p_Void) { push_pop_reference(get_Table_ID(p_TName), p_Void); }
	void push_pop_bool(string p_TName, bool p_Bool) { push_pop_bool(get_Table_ID(p_TName), p_Bool); }

	//Gets a given cells data in a given row.
	string get_string(string p_TName, int p_Row, int p_Cell) { return get_string(get_Table_ID(p_TName), p_Row, p_Cell); }
	int    get_int(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_int(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }
	float  get_float(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_float(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }
	void* get_reference(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_reference(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }
	bool   get_bool(string p_TName, int p_Row, int p_Cell, int p_Index = 0) { return get_bool(get_Table_ID(p_TName), p_Row, p_Cell, p_Index); }

	//Gets a the current cells first data bit.
	string getc_string(string p_TName) { return getc_string(get_Table_ID(p_TName)); }
	int    getc_int(string p_TName) { return getc_int(get_Table_ID(p_TName)); }
	float  getc_float(string p_TName) { return getc_float(get_Table_ID(p_TName)); }
	void* getc_reference(string p_TName) { return getc_reference(get_Table_ID(p_TName)); }
	bool   getc_bool(string p_TName) { return getc_bool(get_Table_ID(p_TName)); }


	//Copies the data from the submitted cell into itself.
	string copy(int p_Table, int p_Row, int p_Cell, void* p_Cell_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->copy_Cell(p_Row, p_Cell, p_Cell_To_Copy);

		return "CODE 4";
	}

	string copy(string p_TName, int p_Row, int p_Cell, void* p_Cell_To_Copy)
	{
		return copy(get_Table_ID(p_TName), p_Row, p_Cell, p_Cell_To_Copy);
	}

	//Copies a row.
	string copy_Row(int p_Table, int p_Row, void* p_Row_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->copy_Row(p_Row, p_Row_To_Copy);

		return "CODE 4";
	}
	string copy_Row(string p_TName, int p_Row, void* p_Row_To_Copy)
	{
		return copy_Row(get_Table_ID(p_TName), p_Row, p_Row_To_Copy);
	}

	//Copies a row from given table IDs
	string copy_Row(int p_Table, int p_Row, int p_Table_To, int p_Row_To)
	{
		if (!bounds_TID(p_Table) || !bounds_TID(p_Table_To)) { return "TABLE_NO_EXISTY"; }

		return copy_Row(p_Table_To, p_Row_To, Tables[p_Table]->get_Row_Reference(p_Row));
	}

	//Copies a whole damn table.
	string copy_Table(int p_Table, int p_Table_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->copy_Table(Tables[p_Table_To_Copy]);

		return "CODE 4";
	}
	string copy_Table(string p_TName, string p_TTCName)
	{
		return copy_Table(get_Table_ID(p_TName), get_Table_ID(p_TTCName));
	}

	//Copies a whole damn table.
	string rotate_Table(int p_Table)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->rotate_Table();

		return "CODE 4";
	}
	string rotate_Table(string p_TName)
	{
		return rotate_Table(get_Table_ID(p_TName));
	}


	string translate_Row(string p_From_Table, int p_Row, string p_To_Table, int p_Offset, int p_Column = 0)
	{
		int tmp_Table_ID = get_Table_ID(p_To_Table);

		if (!bounds_TID(tmp_Table_ID)) { cout << "\n\n\t Table[" << tmp_Table_ID << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[tmp_Table_ID]->translate_Row(get_Table_Reference(p_From_Table), p_Row, p_Offset, p_Column);
	}
	string translate_Row(int p_From_Table, int p_Row, int p_To_Table, int p_Offset, int p_Column = 0)
	{
		if (!bounds_TID(p_To_Table)) { cout << "\n\n\t Table[" << p_To_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[p_To_Table]->translate_Row(get_Table_Reference(p_From_Table), p_Row, p_Offset, p_Column);
	}

	//Overlays a row onto another row at the given index.
	string overlay_Row(int p_Table, int p_Row, int p_Index, void* p_Row_To_Copy)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		Tables[p_Table]->overlay_Row(p_Row, p_Index, p_Row_To_Copy);

		return "CODE 4";
	}
	string overlay_Row(string p_TName, int p_Row, int p_Index, void* p_Row_To_Copy)
	{
		return overlay_Row(get_Table_ID(p_TName), p_Row, p_Index, p_Row_To_Copy);
	}

	string shift_Column(int p_Table, int p_Column_To_Shift)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[p_Table]->shift_Column(p_Column_To_Shift);
	}
	string shift_Column(string p_TName, int p_Column_To_Shift)
	{
		return shift_Column(get_Table_ID(p_TName), p_Column_To_Shift);
	}

	string shift_All_Rows(int p_Table, int p_Index)
	{
		if (!bounds_TID(p_Table)) { cout << "\n\n\t Table[" << p_Table << "] does not exist."; return "BOUNDING_ERROR->Table_Handler->copy->p_TID"; }

		return Tables[p_Table]->shift_All_Rows(p_Index);
	}
	string shift_All_Rows(string p_TName, int p_Index)
	{
		return shift_All_Rows(get_Table_ID(p_TName), p_Index);
	}

	//Expands the table array by one.
	void increment_Table_Array()
	{
		expand_Table_Array(Table_Count + 1);
	}

	//Expands the table array.
	void expand_Table_Array(int p_Depth)
	{
		if (p_Depth <= Table_Count) { return; }

		//Setup the tmp_tables
		c_Base_Table** tmp_Tables = new c_Base_Table * [Table_Count];

		//Copy the current tables into the temporary tables.
		for (int cou_Table = 0; cou_Table < Table_Count; cou_Table++)
		{
			tmp_Tables[cou_Table] = Tables[cou_Table];
			Tables[cou_Table] = NULL;
		}

		//Reallocate the Table array.
		delete[] Tables;
		Tables = NULL;

		Tables = new c_Base_Table * [p_Depth];

		//Read the original tables back into the reallocated array.
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			Tables[cou_Index] = tmp_Tables[cou_Index];
			tmp_Tables[cou_Index] = NULL;
		}

		//NULL the newly minted indexes.
		for (int cou_Index = Table_Count; cou_Index < p_Depth; cou_Index++)
		{
			Tables[cou_Index] = NULL;
		}

		//Set the table count.
		Table_Count = p_Depth;

		//Cleanup the tmp array.
		delete[] tmp_Tables;

	}

	//Checks for bounding errors.
	bool bounds_TID(int p_TID)
	{
		if (p_TID >= Table_Count) { return 0; }
		if (p_TID < 0) { return 0; }
		if (Tables[p_TID] == NULL) { return 0; }
		return 1;
	}

	//Outputs a given table.
	void output_Table(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }
		ostr(0, 12, "\n\n\t __________");
		for (unsigned int cou_Index = 0; cou_Index < ((Tables[p_TID]->get_Table_Name()).size()); cou_Index++)
		{
			ostr(0, 12, "_");
		}
		Tables[p_TID]->output_Table();
	}
	void output_Table(string p_TName)
	{
		output_Table(get_Table_ID(p_TName));
	}

	//Outputs all of the tables.
	void output_All_Tables()
	{
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			output_Table(cou_Index);
		}
	}

	//Outputs a tables header.
	void output_Table_Header(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		cout << "\n\t TID[" << p_TID << "]";
		Tables[p_TID]->output_Table_Header();
	}
	void output_Table_Header(string p_TName)
	{
		output_Table_Header(get_Table_ID(p_TName));
	}

	//Outputs every table header.
	void output_All_Table_Headers()
	{
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			output_Table_Header(cou_Index);
		}
	}

	//Outputs a tables header.
	void output_Table_Verbose(int p_TID)
	{
		if (!bounds_TID(p_TID)) { return; }

		cout << "\n\t TID[" << p_TID << "]";
		Tables[p_TID]->output_Table_Verbose();
	}
	void output_Table_Verbose(string p_TName)
	{
		output_Table_Verbose(get_Table_ID(p_TName));
	}

	//Outputs every table header.
	void output_All_Tables_Verbose()
	{
		for (int cou_Index = 0; cou_Index < Table_Count; cou_Index++)
		{
			output_Table_Verbose(cou_Index);
		}
	}

	//Outputs the headers for registered tables.
	void output_Table_Registry()
	{
		output_Table(0);
	}

	//Outputs the registry tree.
	void output_Table_Registry_Tree()
	{
		Table_Registry.output_Tree();
	}
	
	//Saves all the tables.
	void save_Tables()
	{
		
	}
};




//The class encapsulating the union to ini it.
class c_Data_3
{
public:

     //The data.
     u_Data_3 D;
     
     c_Data_3()
     {
          D.I = 0;
     }
};

//The raw table cell class for the 1d table.
class c_Raw_Table_Cell_1D
{
public:
     
     //The data held in the cell.
     u_Data_3 * Data;
     
     //The length of the current cell.
     int Length;
     
     //The max length of the data in the current cell.
     int MAX_Length;
     
     //The Increment which to increase the max length by.
     int MAX_Length_I;
     
     c_Raw_Table_Cell_1D()
     {
          Data = NULL;
          Length = 0;
          MAX_Length = 0;
          MAX_Length_I = 10;
     }
     
     ~c_Raw_Table_Cell_1D()
     {
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          delete [] Data;
          Data = NULL;
     }
     
     //Initializes the cells data.
     void initialize()
     {
          Data = NULL;
          Length = 0;
          MAX_Length = 0;
          MAX_Length_I = 10;
     }
     
     //Resets the cell.
     void reset()
     {
          delete [] Data;
          Data = NULL;
          Length = 0;
          MAX_Length = 0;
          MAX_Length_I = 10;
     }
     
     //====----
     //==---     ACCEPTS A SUBMISSION AND SETS THE CELLS DATA ACCORDINGLY
     //====----
          
     //Takes a submission.
     void submit_String(string p_Submission)
     {
          resize(p_Submission.size());
          
          for (unsigned int cou_C=0;cou_C<p_Submission.size();cou_C++)
          {
               Data[cou_C].C = p_Submission[cou_C];
          }
     }
          
     //Takes a submission.
     void submit_UData_String(u_Data_3 * p_Submission, int p_Size)
     {
          resize(p_Size);
          
          for (int cou_C=0;cou_C<p_Size;cou_C++)
          {
               Data[cou_C] = p_Submission[cou_C];
          }
     }
     
     //Takes a submission.
     void submit_Int(long long int p_Submission)
     {
          resize(1);
          
          Data[0].I = p_Submission;
     }
     
     //Takes a submission.
     void submit_UInt(unsigned long long int p_Submission)
     {
          resize(1);
          
          Data[0].U = p_Submission;
     }
     
     //Takes a submission.
     void submit_Float(double p_Submission)
     {
          resize(1);
          
          Data[0].F = p_Submission;
     }
          
     //Takes a submission.
     void submit_NR(void * p_Submission)
     {
          resize(1);
          
          Data[0].NR = p_Submission;
     }
     
     
     
     //Takes a submission.
     void submit_Int(int p_Index, long long int p_Submission)
     {
          soft_resize(p_Index + 1);
          
          Data[p_Index].I = p_Submission;
     }
     
     //Takes a submission.
     void submit_UInt(int p_Index, unsigned long long int p_Submission)
     {
          soft_resize(p_Index + 1);
          
          Data[p_Index].U = p_Submission;
     }
     
     //Takes a submission.
     void submit_Float(int p_Index, double p_Submission)
     {
          soft_resize(p_Index + 1);
          
          Data[p_Index].F = p_Submission;
     }

	 //Takes a submission.
	 void submit_NR(int p_Index, void* p_Submission)
	 {
		 soft_resize(p_Index + 1);

		 Data[p_Index].NR = p_Submission;
	 }

	 //Takes a submission.
	 void submit_U_Data_3(int p_Index, u_Data_3 p_Submission)
	 {
		 soft_resize(p_Index + 1);

		 Data[p_Index] = p_Submission;
	 }
     
     
     
     //====----
     //==---     ADDS A BIT OF DATA TO CELLS DATA ARRAY
     //====----
     
     //Adds a bit to the current set.
     void add_Data(u_Data_3 p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1] = p_Submission;
     }
     
     //Adds a bit to the current set.
     void add_Data_Int(long long int p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1].I = p_Submission;
     }
          
     //Adds a bit to the current set.
     void add_Data_Float(double p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1].F = p_Submission;
     }
     
     //Adds a bit to the current set.
     void add_Data_NR(void * p_Submission)
     {
          soft_resize(Length + 1);
          
          Data[Length - 1].NR = p_Submission;
     }
     
     
     
     
     
     
     
     //Gets teh contents of the cell as a string.
     string get_String()
     {
          string tmp_Out = "";
          
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               if (Data[cou_Index].I == 0){ tmp_Out += " "; continue; }
               tmp_Out += Data[cou_Index].C;
          }
          
          return tmp_Out;
     }
     
     //Resizes the row to hold a submission.
     void resize(int p_Size)
     {
          if (p_Size <= Length){ return; }
          
          if (p_Size < MAX_Length)
          {
               for (int cou_Index=0;cou_Index<Length;cou_Index++)
               {
                    Data[cou_Index].U = 0;
               }
               Length = p_Size;
               return;
          }
          
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          delete [] Data;
          Data = NULL;
          //**--ga_Cell_Alloc++;
          int tmp_MAX_Length = p_Size + MAX_Length_I;
          Data = new u_Data_3[tmp_MAX_Length];
          for (int cou_Index=0;cou_Index<tmp_MAX_Length;cou_Index++)
          {
               Data[cou_Index].U = 0;
          }
          Length = p_Size;
          MAX_Length = tmp_MAX_Length;
     }
     
     //Resizes the row to hold a submission.
     void soft_resize(int p_Size)
     {
          if (p_Size <= Length){ return; }
          
          if (p_Size < MAX_Length)
          {
               for (int cou_Index=Length;cou_Index<p_Size;cou_Index++)
               {
                    Data[cou_Index].U = 0;
               }
               Length = p_Size;
               return;
          }
          
          u_Data_3 * tmp_Data;
          tmp_Data = new u_Data_3[Length];
          
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               tmp_Data[cou_Index] = Data[cou_Index];
          }
          
          //**--if (Data != NULL){ ga_Cell_Alloc--; }
          delete [] Data;
          Data = NULL;
          //**--ga_Cell_Alloc++;
          int tmp_MAX_Length = p_Size + MAX_Length_I;
          Data = new u_Data_3[tmp_MAX_Length];
          
          for (int cou_Index=0;cou_Index<tmp_MAX_Length;cou_Index++)
          {
               Data[cou_Index].U = 0;
          }
          
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               Data[cou_Index] = tmp_Data[cou_Index];
          }
          
          delete [] tmp_Data;
          tmp_Data = NULL;
          
          Length = p_Size;
          MAX_Length = tmp_MAX_Length;
     }
     
     //Outputs the cell as characters.
     void output_C()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               char tmp_State = char (Data[cou_Index].I);
               if (tmp_State == char (0)){ tmp_State = '?'; }
               if (tmp_State == char (7)){ tmp_State = '?'; }
               if (tmp_State == char (8)){ tmp_State = '?'; }
               if (tmp_State == char (9)){ tmp_State = '?'; }
               if (tmp_State == char (10)){ tmp_State = '?'; }
               if (tmp_State == char (13)){ tmp_State = '?'; }
               cout << tmp_State;
          }
     }
     
     //Outputs the cell as integer.
     void output_I()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               oint64(0, 8, Data[cou_Index].I);
          }
     }
     
     //Outputs the cell as integer.
     void output_U()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               oull(0, 8, Data[cou_Index].U);
          }
     }
     
     //Outputs the cell as *void.
     void output_NR()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               opoi(0, 8, Data[cou_Index].NR);
          }
     }
     
     //Outputs the cell as float.
     void output_F()
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               ostr(0, 1, " ");
               odbl(0, 8, Data[cou_Index].F);
          }
     }
     
     
     
     
     
     //==--     SAVING TO FILE
     
     void save_C(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               if (Data[cou_Index].I == 0){ *p_File << char(254); if ((cou_Index + 1) < Length){ *p_File << " "; } continue; }
               *p_File << Data[cou_Index].C;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
     
     void save_I(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               *p_File << Data[cou_Index].I;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
     
     void save_F(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               *p_File << Data[cou_Index].F;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
     
     void save_NR(ofstream * p_File)
     {
          for (int cou_Index=0;cou_Index<Length;cou_Index++)
          {
               *p_File << Data[cou_Index].NR;
               if ((cou_Index + 1) < Length){ *p_File << " "; }
          }
     }
};


//The raw table row class for the 1d table.
class c_Raw_Table_Row_1D
{
public:
     
     //The data held in the row.
     c_Raw_Table_Cell_1D ** Cells;
     
     //The depth of the current row.
     int Depth;
     
     //The max depth.
     int MAX_Depth;
     
     //The number to increase the MAX_Depth by whenever enlarging.
     int MAX_Depth_Inc;
     
     c_Raw_Table_Row_1D()
     {
          Cells = NULL;
          MAX_Depth = 0;
          MAX_Depth_Inc = 1000;
          Depth = 0;
     }
     
     ~c_Raw_Table_Row_1D()
     {
          reset();
     }
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
     
     //Takes a submission.
     void submit_String(string p_Submission)
     {
          //ostr(0, 12, "\n Depth_B:"); cout << Depth;
          resize(Depth + 1);
          Cells[Depth - 1]->submit_String(p_Submission);
          //ostr(0, 12, "\n Depth_A:"); cout << Depth;
     }
     
     //Takes a submission.
     void submit_UData_String(u_Data_3 * p_Submission, int p_Size)
     {
          //ostr(0, 12, "\n Depth_B:"); cout << Depth;
          resize(Depth + 1);
          Cells[Depth - 1]->submit_UData_String(p_Submission, p_Size);
          //ostr(0, 12, "\n Depth_A:"); cout << Depth;
     }
     
     //Takes a submission.
     void submit_Int(long long int p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_Int(p_Submission);
     }
     
     //Takes a submission to a given cell.
     void submit_UInt(unsigned long long int p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_UInt(p_Submission);
     }
     
     //Takes a submission.
     void submit_Float(double p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_Float(p_Submission);
     }
     
     //Takes a submission.
     void submit_NR(void * p_Submission)
     {
          resize(Depth + 1);
          Cells[Depth - 1]->submit_NR(p_Submission);
     }
     
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
     
     //Takes a submission to a given cell.
     void set_String(int p_Cell, string p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_String(p_Submission);
     }
     
     //Takes a submission to a given cell.
     void set_Int(int p_Cell, long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Int(p_Submission);
     }
     
     //Takes a submission to a given cell.
     void set_UInt(int p_Cell, unsigned long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_UInt(p_Submission);
     }
          
     //Takes a submission to a given cell.
     void set_Float(int p_Cell, double p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Float(p_Submission);
     }

	 //Takes a submission to a given cell.
	 void set_NR(int p_Cell, void* p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_NR(p_Submission);
	 }


	 //Takes a submission to a given cell.
	 void set_U_Data_3(int p_Cell, u_Data_3 p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_UData_String(&p_Submission, 1);
	 }
     
     

	 //Takes a submission to a given cell.
	 void set_Int(int p_Cell, int p_Index, long long int p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_Int(p_Index, p_Submission);
	 }

	 //Takes a submission to a given cell.
	 void set_U_Data_3(int p_Cell, int p_Index, u_Data_3 p_Submission)
	 {
		 resize(p_Cell + 1);
		 Cells[p_Cell]->submit_U_Data_3(p_Index, p_Submission);
	 }
     
     //Takes a submission to a given cell.
     void set_UInt(int p_Cell, int p_Index, unsigned long long int p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_UInt(p_Index, p_Submission);
     }
          
     //Takes a submission to a given cell.
     void set_Float(int p_Cell, int p_Index, double p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_Float(p_Index, p_Submission);
     }
          
     //Takes a submission to a given cell.
     void set_NR(int p_Cell, int p_Index, void * p_Submission)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->submit_NR(p_Index, p_Submission);
     }
     
     
     
     //====----
     //==---     ADDS A BIT OF DATA TO THE GIVEN/CURRENT CELL
     //====----
     
     //Adds a bit of data onto the current cell.
     void add_Data(u_Data_3 p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Int(long long int p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Float(double p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data_Float(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_NR(void * p_Data)
     {
          if (Depth == 0){ resize(1); }
          Cells[Depth - 1]->add_Data_NR(p_Data);
     }
     
     //Adds a bit of data onto the current cell.
     void add_Data(int p_Cell, u_Data_3 p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Int(int p_Cell, long long int p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Float(int p_Cell, double p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data_Float(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_NR(int p_Cell, void * p_Data)
     {
          resize(p_Cell + 1);
          Cells[p_Cell]->add_Data_NR(p_Data);
     }
     
     //==--     RESIZE AND RESET
     
     
     //Flattens all the cells in the row into one.
     void flatten_Row()
     {
          c_Raw_Table_Cell_1D * tmp_Cell = new c_Raw_Table_Cell_1D;
          
          //Read each cell in the row into the first cell.
          for (int cou_Cell=0;cou_Cell<Depth;cou_Cell++)
          {
               for (int cou_Data=0;cou_Data<Cells[cou_Cell]->Length;cou_Data++)
               {
                    tmp_Cell->add_Data(Cells[cou_Cell]->Data[cou_Data]);
               }
          }
          
          resize_With_Reset(1);
          
          delete Cells[0];
          Cells[0] = NULL;
          Cells[0] = tmp_Cell;
     }
     
     
     //==--     RESIZE AND RESET
     
     
     //Resizes the row to hold a new submission.
     void resize_With_Reset(int p_Size)
     {
          reset();
          
          int tmp_MAX_Depth = p_Size + MAX_Depth_Inc;
          
          
          //**--if (Cells != NULL){ ga_Row_Alloc--; }
          delete [] Cells;
          Cells = NULL;
          //**--ga_Row_Alloc++;
          Cells = new c_Raw_Table_Cell_1D*[tmp_MAX_Depth];
          
          for (int cou_Index=0;cou_Index<tmp_MAX_Depth;cou_Index++)
          {
               //**--ga_Row_Alloc++;
               Cells[cou_Index] = new c_Raw_Table_Cell_1D;
          }
          
          Depth = p_Size;
          MAX_Depth = tmp_MAX_Depth;
          
          //cout << " ND->" << Depth << " NMX->" << MAX_Depth << " MX_I->" << tmp_MAX_Depth;
     }
     
     //Resizes the row to hold a new submission.
     void resize(int p_Size)
     {
          //cout << "\n resize(" << p_Size << ") D->" << Depth << " MX->" << MAX_Depth; cout.flush();
          
          if (p_Size <= Depth){ return; }
          
          //When it hits (MAX_Depth - 1) it resizes the array.
          if (p_Size < MAX_Depth){ Depth = p_Size; return; }
          
          
          //Setup the tmp and transfer the old cells to it while reallocating the row and transfering the old cells back before destroying the tmp.
          c_Raw_Table_Cell_1D ** tmp_Row;
          //**--ga_Row_Alloc++;
          tmp_Row = new c_Raw_Table_Cell_1D*[MAX_Depth];
          int tmp_MAX_Depth = p_Size + MAX_Depth_Inc;
          
          for (int cou_Index=0;cou_Index<MAX_Depth;cou_Index++)
          {
               tmp_Row[cou_Index] = Cells[cou_Index];
               Cells[cou_Index] = NULL;
          }
          
          //**--if (Cells != NULL){ ga_Row_Alloc--; }
          delete [] Cells;
          Cells = NULL;
          //**--ga_Row_Alloc++;
          Cells = new c_Raw_Table_Cell_1D*[tmp_MAX_Depth];
          
          for (int cou_Index=0;cou_Index<MAX_Depth;cou_Index++)
          {
               Cells[cou_Index] = tmp_Row[cou_Index];
               tmp_Row[cou_Index] = NULL;
          }
          
          //**--if (tmp_Row != NULL){ ga_Row_Alloc--; }
          delete [] tmp_Row;
          tmp_Row = NULL;
          
          for (int cou_Index=MAX_Depth;cou_Index<tmp_MAX_Depth;cou_Index++)
          {
               //**--ga_Row_Alloc++;
               Cells[cou_Index] = new c_Raw_Table_Cell_1D;
          }
          
          Depth = p_Size;
          MAX_Depth = tmp_MAX_Depth;
          
          //cout << " ND->" << Depth << " NMX->" << MAX_Depth << " MX_I->" << tmp_MAX_Depth;
     }
     
     //Resets the cells back to nothing.
     void reset()
     {
          if (Cells == NULL){ return; }
          if (MAX_Depth > 0)
          {
               for (int cou_Index=0;cou_Index<MAX_Depth;cou_Index++)
               {
                    //**--if (Cells[cou_Index] != NULL){ ga_Row_Alloc--; }
                    delete Cells[cou_Index];
                    Cells[cou_Index] = NULL;
               }
               //**--if (Cells != NULL){ ga_Row_Alloc--; }
               delete [] Cells;
               Cells = NULL;
               
               Depth = 0;
               MAX_Depth = 0;
               return;
          }
          
          cout << "\n\n\n CELLS IN ROW " << this << " ARE NOT NULL YET DEPTH == 0, NOT GOOD!! WTF!!";
          //**--if (Cells != NULL){ ga_Row_Alloc--; }
          delete [] Cells;
          Cells = NULL;
          
          Depth = 0;
     }   
     
     //Resets the cells back to nothing.
     void reset_End()
     {
          cout << " reset_End() " << this; cout.flush();
          
          if (Cells == NULL){ return; }
          
          cout << " Depth->" << Depth; cout.flush();
          if (Depth != 0)
          {
               cout << " delete Cells->" << Cells; cout.flush();
               for (int cou_Index=0;cou_Index<Depth;cou_Index++)
               {
                    cout << " Cell[" << cou_Index << "]->" << Cells[cou_Index]; cout.flush();
                    delete Cells[cou_Index];
                    Cells[cou_Index] = NULL;
               }
               cout << " delete [] Cells->" << Cells; cout.flush();
               delete [] Cells;
               Cells = NULL;
          }
          
          //cout << " delete [] Cells->" << Cells; cout.flush();
          //delete [] Cells;
          //Cells = NULL;
          
          Depth = 0;
          cout << " Done(" << this << ")";
     }   
     
     
     //==--     OUTPUTS THE DATA IN THE ROW
     
     
     //Outputs the row as characters.
     void output_C()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "} ";
               Cells[cou_Index]->output_C();
          }
     }
     
     //Outputs the row as integer.
     void output_I()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_I();
          }
     }
     
     //Outputs the row as integer.
     void output_U()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_U();
          }
     }
     
     //Outputs the row as *void.
     void output_NR()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_NR();
          }
     }
     
     //Outputs the row as float.
     void output_F()
     {
          cout << " {";
          if (Depth != 0){ oint(0, 13, Depth); }
          cout << "} ";
          
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               cout << " {";
               oint(0, 12, cou_Index);
               cout << "}";
               Cells[cou_Index]->output_F();
          }
     }
     
     
     
     
     
     //==--     SAVES TO GIVEN FILE
        
     
     //Outputs the row as characters.
     void save_C(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_C(p_File);
          }
     }
     
     //Outputs the row as integer.
     void save_I(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_I(p_File);
          }
     }
     
     //Outputs the row as *void.
     void save_NR(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_NR(p_File);
          }
     }
     
     //Outputs the row as float.
     void save_F(ofstream * p_File)
     {
          *p_File << " " << Depth;
          for (int cou_Index=0;cou_Index<Depth;cou_Index++)
          {
               *p_File << " .. " << cou_Index << " ";
               Cells[cou_Index]->save_F(p_File);
          }
     }
};

//The class for the 1D raw table.
class c_Raw_Table_1D
{
public:
     
     //The rows of data in the table.
     c_Raw_Table_Row_1D ** Rows;
     
     //The number of rows in the table.
     int Number_Of_Rows;
     
     //Whether or not a row is hotlinked
     bool * Hotlinked;
     
     //The name of the table.
     string Name;
     
     c_Raw_Table_1D()
     {
          Rows = NULL;
          Hotlinked = NULL;
          Number_Of_Rows = 0;
          Name = "Table_1D";
     }
     
     ~c_Raw_Table_1D()
     {
          
          cout << "\n    ~c_Raw_Table_1D " << Name << " " << Number_Of_Rows << " "; cout.flush();
          reset_End();
          cout << "\n    ~~~c_Raw_Table_1D " << Name << " " << Number_Of_Rows << " "; cout.flush();
     }
     
     //====----
     //==---     CREATES A NEW ROW AND SUBMITS THE SUBMISSION TO THE FIRST CELL
     //====----
          
     //Makes a submission to the table adding a new row.
     void submit_String(string p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_String(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell.
     void submit_Int(long long int p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Int(p_Submission);
     }
          
     //Makes a submission to the table adding a new cell.
     void submit_Float(double p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Float(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell.
     void submit_NR(void * p_Submission)
     {
          //Make room for the new submission.
          resize(Number_Of_Rows + 1);
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_NR(p_Submission);
     }
     
     
     
     //====----
     //==---     ADDS SUBMISSION TO NEW CELL IN CURRENT/GIVEN ROW
     //====----
          
     //Makes a submission to the table adding a new cell to the current row.
     void add_String(string p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_String(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the current row.
     void add_Int(long long int p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Int(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the current row.
     void add_Float(double p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_Float(p_Submission);
     }
          
     //Makes a submission to the table adding a new cell to the current row.
     void add_NR(void * p_Submission)
     {
          //If there are no rows then make the first one.
          if (Number_Of_Rows == 0){ resize(1); }
          
          //Submit it.
          Rows[Number_Of_Rows - 1]->submit_NR(p_Submission);
     }
          
     //Makes a submission to the table adding a new cell to the given row.
     void add_String(int p_Row, string p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_String(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the given row.
     void add_Int(int p_Row, long long int p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_Int(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the given row.
     void add_Float(int p_Row, double p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_Float(p_Submission);
     }
     
     //Makes a submission to the table adding a new cell to the given row.
     void add_NR(int p_Row, void * p_Submission)
     {
          resize(p_Row + 1);
          
          //Submit it.
          Rows[p_Row]->submit_NR(p_Submission);
     }
     
     
     
     //====----
     //==---     SETS A GIVEN CELLS DATA TO THE GIVEN SUBMISSION
     //====----
          
     //Sets a given cell cell to the given string.
     void set_String(int p_Row, int p_Cell, string p_Submission)
     {
          //IF there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_String(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given integer.
     void set_Int(int p_Row, int p_Cell, long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Int(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given integer.
     void set_UInt(int p_Row, int p_Cell, unsigned long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_UInt(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given float.
     void set_Float(int p_Row, int p_Cell, double p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Float(p_Cell, p_Submission);
     }
     
     //Sets a given cell to the given float.
     void set_NR(int p_Row, int p_Cell, void * p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_NR(p_Cell, p_Submission);
     }
     
     
     //Sets a given cell to the given float.
     void set_U_Data_3(int p_Row, int p_Cell, u_Data_3 p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_U_Data_3(p_Cell, p_Submission);
     }
     
     
     
     //Sets a given cell to the given integer.
     void set_Int(int p_Row, int p_Cell, int p_Index, long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Int(p_Cell, p_Index, p_Submission);
     }
     
     //Sets a given cell to the given integer.
     void set_UInt(int p_Row, int p_Cell, int p_Index, unsigned long long int p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_UInt(p_Cell, p_Index, p_Submission);
     }
          
     //Sets a given cell to the given float.
     void set_Float(int p_Row, int p_Cell, int p_Index, double p_Submission)
     {
          //If there are not enough rows then create them.
          if (Number_Of_Rows <= p_Row){ resize(p_Row + 1); }
          
          //Set the string.
          Rows[p_Row]->set_Float(p_Cell, p_Index, p_Submission);
     }

	 //Sets a given cell to the given float.
	 void set_NR(int p_Row, int p_Cell, int p_Index, void* p_Submission)
	 {
		 //If there are not enough rows then create them.
		 if (Number_Of_Rows <= p_Row) { resize(p_Row + 1); }

		 //Set the string.
		 Rows[p_Row]->set_NR(p_Cell, p_Index, p_Submission);
	 }

	 //Sets a given cell to the given float.
	 void set_U_Data_3(int p_Row, int p_Cell, int p_Index, u_Data_3 p_Submission)
	 {
		 //If there are not enough rows then create them.
		 if (Number_Of_Rows <= p_Row) { resize(p_Row + 1); }

		 //Set the string.
		 Rows[p_Row]->set_U_Data_3(p_Cell, p_Index, p_Submission);
	 }
     
     //Sets a row to the given row.
     void set_Row(int p_Row, c_Raw_Table_Row_1D * p_Row_Reference)
     {
          resize(p_Row + 1);
          
          //**--ga_Table_Alloc--;
          if (!Hotlinked[p_Row]){ delete Rows[p_Row]; }
          Rows[p_Row] = NULL;
          
          Rows[p_Row] = p_Row_Reference;
          Hotlinked[p_Row] = 1;
     }
     
     //====----
     //==---     ADDS A BIT OF DATA TO THE GIVEN/CURRENT CELL IN THE CURRENT/GIVEN ROW
     //====----
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data(u_Data_3 p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Int(long long int p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_Float(double p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data_Float(p_Data);
     }
          
     //Adds a bit of data to the current cell in the current row.
     void add_Data_NR(void * p_Data)
     {
          if (Number_Of_Rows == 0){ resize(1); }
          Rows[Number_Of_Rows - 1]->add_Data_NR(p_Data);
     }
     
     //Adds a bit of data to the current cell in the given row.
     void add_Data(int p_Row, u_Data_3 p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data(p_Data);
     }
     
     //Adds a bit of data to the current cell in the given row.
     void add_Data_Int(int p_Row, long long int p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Int(p_Data);
     }
          
     //Adds a bit of data to the current cell in the given row.
     void add_Data_Float(int p_Row, double p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Float(p_Data);
     }
     
     //Adds a bit of data to the current cell in the given row.
     void add_Data_NR(int p_Row, void * p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_NR(p_Data);
     }
     
     //Adds a bit of data to the given cell in the given row.
     void add_Data(int p_Row, int p_Cell, u_Data_3 p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data(p_Cell, p_Data);
     }
     
     //Adds a bit of data to the given cell in the given row.
     void add_Data_Int(int p_Row, int p_Cell, long long int p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Int(p_Cell, p_Data);
     }
          
     //Adds a bit of data to the given cell in the given row.
     void add_Data_Float(int p_Row, int p_Cell, double p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_Float(p_Cell, p_Data);
     }
     
     //Adds a bit of data to the given cell in the given row.
     void add_Data_NR(int p_Row, int p_Cell, void * p_Data)
     {
          resize(p_Row + 1);
          Rows[p_Row]->add_Data_NR(p_Cell, p_Data);
     }
     
     
     
     
     //====----
     //==---      CREATES NEW CELLS AND ROWS
     //====----
     
     //Creates a new cell in the current row to submit data to, return the Cell ID.
     int new_Cell()
     {
          resize(1);
          Rows[Number_Of_Rows - 1]->resize(Rows[Number_Of_Rows - 1]->Depth + 1);
          
          //Return the new cells ID.
          return Rows[Number_Of_Rows - 1]->Depth - 1;
     }
     
     //Creates a new cell in the given row to submit data to.
     int new_Cell(int p_Row)
     {
          resize(p_Row + 1);
          Rows[p_Row]->resize(Rows[p_Row]->Depth + 1);
          
          //Return the new cells ID.
          return Rows[p_Row]->Depth - 1;
     }
     
     int new_Row()
     {
          //IF there are not enough rows then create them.
          resize(Number_Of_Rows + 1);
          
          //Return the new Rows ID.
          return Number_Of_Rows - 1;
     }
     
     void new_Row(int p_Row)
     {
          resize(p_Row + 1);
     }
          
     //Resets the table.
     void reset()
     {
          if (Number_Of_Rows != 0)
          {
               for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
               {
                    //**--if (Rows[cou_Index] != NULL){ ga_Table_Alloc--; }
                    if (Hotlinked[cou_Index]){ Rows[cou_Index] = NULL; continue; }
                    delete Rows[cou_Index];
                    Rows[cou_Index] = NULL;
               }
               //**--if (Rows != NULL){ ga_Table_Alloc--; }
               delete [] Rows;
               Rows = NULL;
          }
          Number_Of_Rows = 0;
     }
          
     //Resets the table.
     void reset_End()
     {
          cout << "\n    " << this << " Rows->" << Rows << "  ";
          if (Number_Of_Rows != 0)
          {
               cout << " (Number_Of_Rows->" << Number_Of_Rows << " != 0)"; cout.flush();
               for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
               {
                    cout << " delete Rows[" << cou_Index << "]->" << Rows[cou_Index]; cout.flush();
                    if (Hotlinked[cou_Index]){ Rows[cou_Index] = NULL; cout << "REEEEE"; continue; }
                    delete Rows[cou_Index];
                    
                    Rows[cou_Index] = NULL;
                    cout << " Roe"; cout.flush();
               }
               
               cout << " delete [] Rows->" << Rows; cout.flush();
               
               delete [] Rows;
               Rows = NULL;
          }
          Number_Of_Rows = 0;
          cout << " Done"; cout.flush();
     }
     
     //Resets the data in the rows, however, the row itself is not destroyed.
     void soft_Reset()
     {

          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               if (Rows[cou_Index] == NULL){ continue; }
               Rows[cou_Index]->reset();
          }
     }
     
     //Resizes the table to hold submissions.
     void resize(int p_Size)
     {
          if (Number_Of_Rows >= p_Size){ return; }
          
          //Setup the tmp rows array.
          c_Raw_Table_Row_1D ** tmp_Rows;
          bool * tmp_Hotlinked;
          
          //**--ga_Table_Alloc++;
          tmp_Rows = new c_Raw_Table_Row_1D*[Number_Of_Rows];
          tmp_Hotlinked = new bool[Number_Of_Rows];
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               tmp_Hotlinked[cou_Index] = Hotlinked[cou_Index];
               tmp_Rows[cou_Index] = Rows[cou_Index];
               Rows[cou_Index] = NULL;
          }
          
          //**--if (Rows != NULL){ ga_Table_Alloc--; }
          delete [] Rows;
          Rows = NULL;
          delete Hotlinked;
          Hotlinked = NULL;
          
          //**--ga_Table_Alloc++;
          Rows = new c_Raw_Table_Row_1D*[p_Size];
          Hotlinked = new bool[p_Size];
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               Hotlinked[cou_Index] = tmp_Hotlinked[cou_Index];
               Rows[cou_Index] = tmp_Rows[cou_Index];
               tmp_Rows[cou_Index] = NULL;
          }
          
          //**--if (tmp_Rows != NULL){ ga_Table_Alloc--; }
          delete [] tmp_Rows;
          tmp_Rows = NULL;
          delete [] tmp_Hotlinked;
          tmp_Hotlinked = NULL;
          
          for (int cou_Index=Number_Of_Rows;cou_Index<p_Size;cou_Index++)
          {
               Rows[cou_Index] = NULL;
               //**--ga_Table_Alloc++;
               Rows[cou_Index] = new c_Raw_Table_Row_1D;
               Hotlinked[cou_Index] = 0;
          }
          
          Number_Of_Rows = p_Size;
     }
     
     
     
     //====----
     //==---      GETS DATA FROM THE TABLE
     //====----
     
     // !!! NO BOUNDS CHECKING, ASSUMED DATA REQUESTED ALREADY EXISTS!
     
     //Gets a data bit from the given row, cell, position.
     u_Data_3 get(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ u_Data_3 tmp_Data; tmp_Data.U = 0; return tmp_Data; }
          
          return Rows[p_Row]->Cells[p_Cell]->Data[p_Position];
     }
     
     //Gets a data bit from the given row, cell, position.
     long long int get_I(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return 0; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).U;
     }
     
     //Gets a data bit from the given row, cell, position.
     char get_C(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return char (0); }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return char (0); }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).C;
     }
     
     //Gets a data bit from the given row, cell, position.
     double get_F(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return 0.0f; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).F;
     }
     
     //Gets a data bit from the given row, cell, position.
     unsigned long long int get_U(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return 0.0f; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return 0.0f; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).U;
     }
     
     //Gets a data bit from the given row, cell, position.
     void * get_NR(int p_Row, int p_Cell, int p_Position)
     {
          if (!bounds_C(p_Row, p_Cell)){ return NULL; }
          if (Rows[p_Row]->Cells[p_Cell]->Length <= p_Position){ return NULL; }
          
          return (Rows[p_Row]->Cells[p_Cell]->Data[p_Position]).NR;
     }
     
     
     //Gets a cells reference.
     c_Raw_Table_Cell_1D * get_Cell_Ref(int p_Row, int p_Cell)
     {
          if (!bounds_C(p_Row, p_Cell)){ return NULL; }
          
          return Rows[p_Row]->Cells[p_Cell];
     }
     
     //Gets a row reference.
     c_Raw_Table_Row_1D * get_Row_Ref(int p_Row)
     {
          if (!bounds_R(p_Row)){ return NULL; }
          
          return Rows[p_Row];
     }
     
     //Gets a cells reference.
     c_Raw_Table_Cell_1D * get_Cell_Ref_Hard(int p_Row, int p_Cell)
     {
          if (!bounds_R(p_Row)){ resize(p_Row + 1); }
          if (!bounds_C(p_Row, p_Cell)){ Rows[p_Row]->resize(p_Cell + 1); }
          
          return Rows[p_Row]->Cells[p_Cell];
     }
     
     //Gets a row reference.
     c_Raw_Table_Row_1D * get_Row_Ref_Hard(int p_Row)
     {
          if (!bounds_R(p_Row)){ resize(p_Row + 1); }
          Rows[p_Row]->reset();
          return Rows[p_Row];
     }
     
     //Bounds checking.
     bool bounds_R(int p_Row)
     {
          if (Number_Of_Rows <= p_Row){ return 0; }
          
          return 1;
     }
     
     //Bounds for a cell
     bool bounds_C(int p_Row, int p_Cell)
     {
          if (!bounds_R(p_Row)){ return 0; }
          
          if (Rows[p_Row]->Depth <= p_Cell){ return 0; }
          
          return 1;
     }
     
     //====----
     //==---      OUTPUTS
     //====----
     
     
     void output_C(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_C();
          }
     }
     
     void output_I(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_I();
          }
     }
     
     void output_U(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_U();
          }
     }
     
     void output_NR(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_NR();
          }
     }
     
     void output_F(int p_Tab = 0)
     {
          string tmp_Tabl = "";
          for (int cou_Index=0;cou_Index<p_Tab;cou_Index++){ tmp_Tabl += "\t"; }
          
          cout << "\n\n" << tmp_Tabl << " --== ";
          ostr(0, 8, Name);
          cout << " ==--";
          
          cout << "\n" << tmp_Tabl << "  Number_Of_Rows->";
          oint(0, 7, Number_Of_Rows);
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               cout << "\n" << tmp_Tabl << "  --[";
               oint(0, 15, cou_Index);
               cout << "]";
               Rows[cou_Index]->output_F();
          }
     }
     
     
     
     
     
     
     
     //====----
     //==---      SAVES
     //====----
     
     
     void save_C(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_C(SF);
          }
     }
     
     void save_I(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_I(SF);
          }
     }
     
     void save_NR(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_NR(SF);
          }
     }
     
     void save_F(ofstream * SF)
     {
          *SF << "\n\nR " << Number_Of_Rows;
          
          for (int cou_Index=0;cou_Index<Number_Of_Rows;cou_Index++)
          {
               *SF << "\n" << cou_Index << " ";
               Rows[cou_Index]->save_F(SF);
          }
     }
     
     
     
     
     void save_C(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_C(&SF);
          
          SF.close();
     }
     
     void save_I(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_I(&SF);
          
          SF.close();
     }
     
     void save_NR(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_NR(&SF);
          
          SF.close();
     }
     
     void save_F(string p_File, string p_DIR = "NO_DIR")
     {
          ofstream SF;
          string tmp_File;
          if (p_DIR != "NO_DIR"){ tmp_File = p_DIR + "\\" + p_File; } else { tmp_File = p_File; }
          
          SF.open(tmp_File);
          if (!SF.is_open()){ cout << "\n FILE " << tmp_File << " COULD NOT BE OPENED!"; return; }
          
          save_F(&SF);
          
          SF.close();
     }
};

//The basic polymorphic node to use for the node network.
class c_NT3_Base_Node_1D
{
public:
     
     virtual ~c_NT3_Base_Node_1D(){};
     
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
     
     //Member Functions.
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
     
     virtual void reinforce()=0; //Reinforces the node.
     virtual double get_RC_Score()=0; //Returns the Reinforcement score of the node. double (RC_Lvl + (RC_XP / RC_Lvl))
     virtual double get_RC_Lvl()=0; //Returns the current reinforcement Lvl.
     
     virtual void set_State(u_Data_3 p_State)=0; //Sets the state if the node is a state node, otherwise it does nothing.
     virtual long long int get_State()=0; //Returns the state if it is a state node otherwise it does nothing.
     
     virtual int get_Type()=0; //Returns the type of node, 0 == State node, 1 == Normal Node, 2 == Treetop state node, 3 == Treetop node.
     virtual void set_Type(int p_Type)=0; //Sets the type of node.
     
     virtual void bp_O()=0; //Initiates a backpropagation that outputs to the given table with the given index.
     virtual void bp_L()=0; //bp_Output the left node.
     virtual void bp_R()=0; //bp_Output the right node.
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
     void reinforce()
     {
          RC_Lvl += double (1.0f / int (RC_Lvl));
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
     void reinforce()
     {
          RC_Lvl += double (1.0f / int (RC_Lvl));
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
};



























/*
		When I was building the networks I ran into the problem of searching them for existing state nodes. A state
	node is the node that associates an input with a node. State nodes allows for storing information in the networks and
	subsequently retrieving information from them. With the higher networks that used node IDs (NID)s from the lower
	constructs this became an issue because the number of unique nodes input into the network could get into the millions.
		In building a network you have to first take an input and search the network to see if that state was already
	associated with a node. Originally this was done with binary trees. The millions of unique inputs into these binary
	trees caused long search times. When it came to reading the nodes in there was another issue with the NIDs being
	sequential leading to extremely long branches in the binary tree that stored almost all of the nodes. This led to
	stack corruption with recusive searches.
		The first thing done was changing the recursive searches to an iterative one. This solved the issue of stack
	corruption but did nothing to solve the long search times. So then an optimization function was created which
	redistributed the nodes evenly along the branches. This function helped but did not fix the issue as the optimization
	had to be done frequently and was costly in and of itself.
		In the end I used a trinary tree combined with bit shifting the query. This allowed for a tree that has branches
	with fixed lengths. With the branches not getting excessive it does not run into the problem of stack corruptions
	even with millions of entries.
*/


//This class forms the basic node on which the fractal tree is built.
class c_NT3_Fractal_State_Node
{
	friend class c_NT3_Fractal_State_Tree;
private:

	//Legs of the node.
	c_NT3_Fractal_State_Node* Right;
	c_NT3_Fractal_State_Node* Center;
	c_NT3_Fractal_State_Node* Left;

public:

	//This stores the state. It is a union.
	u_Data_3 Data;

	//Pointer to the actual node.
	c_NT3_Base_Node_1D* NID;

	c_NT3_Fractal_State_Node()
	{
		Right = NULL;
		Center = NULL;
		Left = NULL;
		Data.I = 0;
	}

	~c_NT3_Fractal_State_Node()
	{
		if (Right != NULL) { delete Right; Right = NULL; }
		if (Center != NULL) { delete Center; Center = NULL; }
		if (Left != NULL) { delete Left; Left = NULL; }
	}

};

//The fractal state tree is a trinary tree that uses a bitshifted input to dtermine which nodes to assign.
class c_NT3_Fractal_State_Tree
{
private:


public:

	//The root node.
	c_NT3_Fractal_State_Node* Root;

	//the current node that will allow referencing outside of the search function.
	c_NT3_Fractal_State_Node** Current;

	//the flag that set when a node is found already set
	bool flg_Foundit;

	//Holds the bitshifted data to compare to the nodes data.
	u_Data_3 tmp_Data;

	c_NT3_Fractal_State_Tree()
	{
		Root = NULL;
		Current = NULL;
		flg_Foundit = false;

		tmp_Data.U = 0;
	}

	~c_NT3_Fractal_State_Tree()
	{
		if (Root != NULL) { delete Root; Root = NULL; }
	}
	
	//Resets the tree.
	void reset()
	{
		if (Root != NULL) { delete Root; Root = NULL; }
	}

	//Search for a state.
	void search(u_Data_3 p_Data)
	{
		//Set the flag to indicate that the data was not found.
		flg_Foundit = false;

		tmp_Data.U = 0;

		//The union is 64 bits deep. So start the shift at 63.
		query_Node(Root, p_Data, 63);
	}

	//Queries a node. 
	//p_Node is the node to check against the query value.
	//p_Data is the data that is checked against.
	//p_Shift is the number of bits to extract for comparison.
	int query_Node(c_NT3_Fractal_State_Node*& p_Node, u_Data_3 p_Data, int p_Shift)
	{
		//Shift the data to get the temporary value.
		tmp_Data.U = p_Data.U >> p_Shift;

		//If the passed node is NULL create it.
		if (p_Node == NULL)
		{
			p_Node = new c_NT3_Fractal_State_Node;
			p_Node->Right = NULL;
			p_Node->Center = NULL;
			p_Node->Left = NULL;
			p_Node->NID = NULL;


			if (p_Shift >= 1)
			{
				//If the query is not at the end set the data to tmp_Data rather than p_Data.
				p_Node->Data = tmp_Data;
			}
			else
			{
				//If the query has reached the end set the data to the full query data.
				p_Node->Data = p_Data;

				//Set the current node to the newly created node.
				Current = &p_Node;

				//The node was not found, one had to be created.
				flg_Foundit = false;

				//Return as we are done here.
				return 0;
			}
		}

		//If the current data matches the tmp data then another depth is explored.
		if (p_Node->Data.U == tmp_Data.U && p_Shift >= 1)
		{
			if (p_Shift == 1) { query_Node(p_Node->Center, p_Data, 0); return 0; }
			query_Node(p_Node->Center, p_Data, (p_Shift - 2));
			return 0;
		}

		//If the node data matches the given data exactly the node has been found.
		//Set Current to the current node.
		//Set the flg_Foundit to true as the node was found without creating it.
		if (p_Node->Data.U == p_Data.U)
		{
			Current = &p_Node;
			if (p_Node->NID != NULL) { flg_Foundit = true; }
			return 0;
		}

		//If the tmp_Data is less than the nodes data then look on the left leg.
		if (tmp_Data.U < p_Node->Data.U)
		{
			query_Node(p_Node->Left, p_Data, p_Shift);
			return 0;
		}

		//If the tmp_Data is more than the nodes data then look on the right leg.
		if (tmp_Data.U > p_Node->Data.U)
		{
			query_Node(p_Node->Right, p_Data, p_Shift);
			return 0;
		}
		return 0;
	}

	//Outputs the tree starting at the root node.
	void output_Tree()
	{
		output_Node(Root, 0);
	}

	//Outputs a given node. p_Tab determines an offset during output.
	void output_Node(c_NT3_Fractal_State_Node * &p_Node, int p_Tab)
	{
		if (p_Node == NULL) { return; }
		output_Node(p_Node->Left, (p_Tab));
		std::cout << "\n";
		for (int cou_Index = 0; cou_Index < p_Tab; cou_Index++)
		{
			std::cout << "  ";
		}
		std::cout << (p_Node->Data).U << "->";
		if (p_Node->NID != NULL) 
		{ std::cout << (p_Node->NID); }
		else { std::cout << "NULL"; }
		output_Node(p_Node->Center, (p_Tab + 1));
		output_Node(p_Node->Right, (p_Tab));
	}

	//Outputs the tree with the backpropagation from the nodes starting at the root node.
	void output_Tree_BP()
	{
		std::cout << "\n\t Fractal Tree Backpropagation Station";
		output_Node_BP(Root);
	}

	//Outputs a given node and backpropagates the node it is linked to.
	void output_Node_BP(c_NT3_Fractal_State_Node * &p_Node)
	{
		if (p_Node == NULL) { return; }
		output_Node_BP(p_Node->Left);
		if ((p_Node->NID) != NULL) { std::cout << "\n\t" << (p_Node->Data).I << "->" << (p_Node->NID); (p_Node->NID)->bp_O(); }
		output_Node_BP(p_Node->Center);
		output_Node_BP(p_Node->Right);
	}

	//Retrieves the NID from a given node.
	c_NT3_Base_Node_1D* get_Node_NID(c_NT3_Fractal_State_Node * &p_Node)
	{
		if (p_Node != NULL)
		{
			return p_Node->NID;
		}
		else {
			return NULL;
		}
	}

	//Sets a node NID.
	void set_Node_NID(c_NT3_Fractal_State_Node * &p_Node, c_NT3_Base_Node_1D * p_NID)
	{
		if (p_Node != NULL)
		{
			p_Node->NID = p_NID;
		}
	}

	//Sets the current nodes NID.
	c_NT3_Base_Node_1D* set_Current_Node_NID(c_NT3_Base_Node_1D * p_NID)
	{
		set_Node_NID(*Current, p_NID);
		return p_NID;
	}

	//Gets the current nodes NID.
	c_NT3_Base_Node_1D* get_Current_Node_NID()
	{
		return get_Node_NID(*Current);
	}
};


//The node network that handles the nodes.
class c_NT3_Node_Network_1D
{
private:
     
public:
     
     //The one that started it all.
     c_NT3_Base_Node_1D * Root;
     
     //The current node to attach to.
     c_NT3_Base_Node_1D ** Current_Node;
     
     //The Fractional tree for handling state queries.
     c_NT3_Fractal_State_Tree State_Tree;
     
     //The tree to hold the Treetops for backpropagation.
     c_NT3_Fractal_State_Tree Treetops_Tree;
     
     //This tracks the current Nodes ID to be entered into the node tracking tree for saving and loading.
     u_Data_3 CNID;
     
     //Tree to track the nodes and their IDs.
     //Only used during saving and loading, then it is destroyed to save memory.
     c_NT3_Fractal_State_Tree NID_Tree;
     
     c_NT3_Node_Network_1D()
     {
          CNID.I = 0;
          Root = NULL;
          Current_Node = &Root;
     }
     
     ~c_NT3_Node_Network_1D()
     {
          std::cout << "\n ~c_NT3_Node_Network_1D " << CNID.I << " " << this << ".........."; std::cout.flush();
          //delete Root;
          
          c_NT3_Base_Node_1D * tmp_LL = Root;
          Root = NULL;
          c_NT3_Base_Node_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          
          std::cout << "\n ~~~c_NT3_Node_Network_1D " << CNID.I << " " << this << ".........."; std::cout.flush();
     }
     
     //Resets the tree.
     void reset()
     {
          delete Root;
     }
     
     
     ////==------------------+
     //==--   NODE CREATION
     ////==------------------+
     
     //Creates a new state node.
     c_NT3_Base_Node_1D * new_State_Node(u_Data_3 p_State, int p_A_L = 0, int p_A_R = 0)
     {
          //Create the state node.
          *Current_Node = new c_NT3_State_Node_1D;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon std::couts are given expand the axon arrays.
          if (p_A_L){ (*Current_Node)->expand_Axon_L(p_A_L); }
          if (p_A_R){ (*Current_Node)->expand_Axon_R(p_A_R); }
          
          //Add it to the state tree, assuming that the current state has already been queried.
          State_Tree.set_Current_Node_NID(*Current_Node);
          
          //Set the current nodes state.
          (*Current_Node)->set_State(p_State);
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
                    
          //Return the created state node using the state tree as Current_Node is changed.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Creates a new node.
     c_NT3_Base_Node_1D * new_Node(int p_A_L = 0, int p_A_R = 0)
     {
          //A tmp Node because we iterate the Current_Node.
          c_NT3_Base_Node_1D * tmp_Return_Node;
          
          //Create the state node.
          *Current_Node = new c_NT3_Node_1D;
          tmp_Return_Node = *Current_Node;
          
          //Sets the new nodes ID.
          (*Current_Node)->NID.I = CNID.I;
          
          //Increment the node tracker.
          CNID.I++;
          
          //If axon std::couts are given expand the axon arrays.
          if (p_A_L){ (*Current_Node)->expand_Axon_L(p_A_L); }
          if (p_A_R){ (*Current_Node)->expand_Axon_R(p_A_R); }
          
          //Set the Current node to the next one in the chain.
          Current_Node = &(*Current_Node)->Next;
          
          //Return the node that was created.
          return tmp_Return_Node;
     }
     
     //Creates a treetop state node for cases where input has only one tier.
     void convert_To_Treetop_Node(c_NT3_Base_Node_1D * p_Node)
     {
          
          if (p_Node->get_Type() == 0){ p_Node->set_Type(2); } //If the node is a state node then set it to a state treetop node.
          if (p_Node->get_Type() == 1){ p_Node->set_Type(3); } //IF the node is a normal node then set the type to treetop.
          
          //Search for the node in the treetop tree.
          Treetops_Tree.search(p_Node->NID);
          
          //If the node has not been found then add it to the tree.
          if (Treetops_Tree.get_Current_Node_NID() == NULL)
          {
               Treetops_Tree.set_Current_Node_NID(p_Node);
          }
     }
     
     
     ////==------------------------------------+
     //==--   CONNECTION TESTING AND CREATION
     ////==------------------------------------+
     
     //Creates a connection from p_From to p_To.
     void create_Connection_L(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Axon_L(p_To);
          p_To->set_Dendrite_L(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Connection_R(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Axon_R(p_To);
          p_To->set_Dendrite_R(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Predefined_Connection_L(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Predefined_Axon_L(p_To);
          p_To->set_Dendrite_L(p_From);
     }
     
     //Creates a connection from p_From to p_To.
     void create_Predefined_Connection_R(c_NT3_Base_Node_1D * p_From, c_NT3_Base_Node_1D * p_To)
     {
          p_From->add_Predefined_Axon_R(p_To);
          p_To->set_Dendrite_R(p_From);
     }
     
     //Checks for an upper tier connection.
     c_NT3_Base_Node_1D * does_Upper_Tier_Connection_Exist(c_NT3_Base_Node_1D * p_L, c_NT3_Base_Node_1D * p_R)
     {
          if (p_L == NULL || p_R == NULL){ return NULL; }
          return p_L->does_Upper_Tier_Connection_Exist(p_R);
     }
     
     
     
     ////==---------------------+
     //==--   RETRIEVING NODES
     ////==---------------------+
     
     //Assigns a given node to a state, used for loading.
     c_NT3_Base_Node_1D * assign_State_Node(u_Data_3 p_State, c_NT3_Base_Node_1D * p_Node, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State, p_A_L, p_A_R);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node.
     c_NT3_Base_Node_1D * get_State_Node(u_Data_3 p_State, int p_A_L = 0, int p_A_R = 0)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //If the node has not been found then create it.
          if (State_Tree.get_Current_Node_NID() == NULL)
          {
               new_State_Node(p_State, p_A_L, p_A_R);
          }
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Get a state node for a query, if none exist return NULL.
     c_NT3_Base_Node_1D * get_State_Node_Query(u_Data_3 p_State)
     {
          //Search for the node.
          State_Tree.search(p_State);
          
          //Return the current node NID.
          return State_Tree.get_Current_Node_NID();
     }
     
     //Gets an upper tier connection even if one has to be created.
     c_NT3_Base_Node_1D * get_Upper_Tier_Connection(c_NT3_Base_Node_1D * p_L, c_NT3_Base_Node_1D * p_R)
     {
          //If either submitted node is NULL then return NULL.
          if (p_L == NULL || p_R == NULL){ return NULL; }
          
          //A tmp var to hold the upper tier node for returning.
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          //Check for an upper tier connection already, if one exists then return it.
          tmp_Node = p_L->does_Upper_Tier_Connection_Exist(p_R);
          if (tmp_Node != NULL)
          {
               return tmp_Node;
          }
          
          //If one does not exist then create it.
          tmp_Node = new_Node();
          
          tmp_Node->set_Dendrite_L(p_L);
          tmp_Node->set_Dendrite_R(p_R);
          
          p_L->add_Axon_L(tmp_Node);
          p_R->add_Axon_R(tmp_Node);
          
          return tmp_Node;
     }
     
     //Gets a treetop connection.
     c_NT3_Base_Node_1D * get_Treetop_Connection(c_NT3_Base_Node_1D * p_L, c_NT3_Base_Node_1D * p_R)
     {
          c_NT3_Base_Node_1D * tmp_Node = get_Upper_Tier_Connection(p_L, p_R);
          
          convert_To_Treetop_Node(tmp_Node);
          
          return tmp_Node;
     }
     
     
     ////==----------------------------+
     //==--   BACKPROPAGATION STATION
     ////==----------------------------+
     
     //Starts the Backprop procedures for output only.
     void bp_Output_Only(c_NT3_Base_Node_1D * p_Node, int p_Type = 0)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          ochr(0, 12, '<');
          bp_Output_Only_L(p_Node, p_Type);
          ochr(0, 12, '>');
     }
     
     //bp_Output the left node.
     void bp_Output_Only_L(c_NT3_Base_Node_1D * p_Node, int p_Type)
     {
          char tmp_State = ' ';
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_L != NULL)
          {
               bp_Output_Only_L(p_Node->Dendrite_L, p_Type);
               bp_Output_Only_R(p_Node->Dendrite_R, p_Type);
          }
          else
          {
               if (p_Type == 0)
               {
                    tmp_State = char (p_Node->get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    std::cout << tmp_State;
               }
               if (p_Type == 1)
               {
                    std::cout << p_Node->get_State() << " ";
               }
          }
     }
     
     //bp_Output the right node.
     void bp_Output_Only_R(c_NT3_Base_Node_1D * p_Node, int p_Type)
     {
          char tmp_State = ' ';
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrite_R != NULL)
          {
               bp_Output_Only_R(p_Node->Dendrite_R, p_Type);
          }
          else
          {
               if (p_Type == 0)
               {
                    tmp_State = char (p_Node->get_State());
                    if (tmp_State == char (0)){ tmp_State = '0'; }
                    if (tmp_State == char (7)){ tmp_State = '7'; }
                    if (tmp_State == char (8)){ tmp_State = '8'; }
                    if (tmp_State == char (9)){ tmp_State = '9'; }
                    if (tmp_State == char (10)){ tmp_State = 'a'; }
                    if (tmp_State == char (13)){ tmp_State = 'd'; }
                    std::cout << tmp_State;
               }
               if (p_Type == 1)
               {
                    std::cout << p_Node->get_State();
               }
          }
     }
     
     
     //Starts the Backprop procedures for output only.
     void bp(c_Raw_Table_1D * p_Pattern_Output, int p_Flat_Output, int p_Input, c_NT3_Base_Node_1D * p_Node)
     {
          //For when directly output the dendrites of a nodes on the base tier.
          if (p_Node == NULL){ return; }
          
          if (p_Flat_Output)
          {
               p_Pattern_Output->new_Cell(p_Input);
          }
          else
          {
               p_Pattern_Output->new_Cell(0);
          }
          bp_L(p_Pattern_Output, p_Input, p_Node);
     }
     
     //bp_Output the left node.
     void bp_L(c_Raw_Table_1D * p_Pattern_Output, int p_Input, c_NT3_Base_Node_1D * p_Node)
     {
          //If a left leg exists then initiate a backpropagation along it, then along the right side.
          if (p_Node->Dendrite_L != NULL)
          {
               bp_L(p_Pattern_Output, p_Input, p_Node->Dendrite_L);
               bp_R(p_Pattern_Output, p_Input, p_Node->Dendrite_R);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
     }
     
     //bp_Output the right node.
     void bp_R(c_Raw_Table_1D * p_Pattern_Output, int p_Input, c_NT3_Base_Node_1D * p_Node)
     {
          //If a right leg exists then initiate a backpropagation.
          if (p_Node->Dendrite_R != NULL)
          {
               bp_R(p_Pattern_Output, p_Input, p_Node->Dendrite_R);
          }
          else
          {
               p_Pattern_Output->add_Data_Int(p_Input, p_Node->get_State());
          }
     }
     
     
     //Backpropagates a given Treetop. Returns the treetops address.
     c_NT3_Base_Node_1D * bp_Treetop(c_Raw_Table_1D * p_Pattern_Output,int p_Flat_Output, int p_Input, u_Data_3 p_NID)
     {
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          //Search for the node.
          Treetops_Tree.search(p_NID);
          
          //Return the current node NID.
          tmp_Node = Treetops_Tree.get_Current_Node_NID();
          
          //Backpropagate the treetop node. It is assumed this will always be a valid treetop, however, the bp() will catch a NULL with no error.
          bp(p_Pattern_Output, p_Flat_Output, p_Input, tmp_Node);
          
          return tmp_Node;
     }
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Saves the node network.
     void save(ofstream * p_SF)
     {
          //Create the file.
          std::cout << "\n\n Node Network Saving..........";
          if (p_SF->is_open()){ std::cout << "Authorized"; } else { std::cout << "Denied"; return; }
          
          //Save the nodes of the nodes.
          std::cout << "\n Saving Nodes.........." << CNID.U << ".Found.....";
          
          *p_SF << "\nNode_Count " << CNID.U;
          
          c_NT3_Base_Node_1D * tmp_Node = Root;
          
          if ((tmp_Node == NULL) && (CNID.U > 0)){ ostr(0, 12, "\n\n ERROR IN SAVING, NODES FOUND BUT ROOT IS NULL!!!"); }
          
          while(tmp_Node != NULL)
          {
               if (tmp_Node->Type == 0)
               {
                    *p_SF << "\nSN " << tmp_Node->get_State();
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
               }
               if (tmp_Node->Type == 1)
               {
                    *p_SF << "\nN ";
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
                    *p_SF << " " << tmp_Node->Dendrite_L->NID.I;
                    *p_SF << " " << tmp_Node->Dendrite_R->NID.I;
               }
               if (tmp_Node->Type == 2)
               {
                    *p_SF << "\nSTN " << tmp_Node->get_State();
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
               }
               if (tmp_Node->Type == 3)
               {
                    *p_SF << "\nTN ";
                    *p_SF << " " << tmp_Node->NID.I;
                    *p_SF << " " << tmp_Node->Axon_Count_L;
                    *p_SF << " " << tmp_Node->Axon_Count_R;
                    *p_SF << " " << tmp_Node->Dendrite_L->NID.I;
                    *p_SF << " " << tmp_Node->Dendrite_R->NID.I;
               }
               *p_SF << " " << tmp_Node->RC_Lvl;
               
               tmp_Node = tmp_Node->Next;
          }
          
          std::cout << "Complete";
     }
     
     //Loads the node network.
     void load_O(ifstream * p_LF)
     {
          //Create the file.
          std::cout << "\n\n Node Network Loading..........";
          if (p_LF->is_open()){ std::cout << "Authorized"; } else { std::cout << "Denied"; return; }
          
          u_Data_3 Node_Count;
          Node_Count.U = 0;
          
          c_NT3_Fractal_State_Tree tmp_Scaffold;
          
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          c_NT3_Base_Node_1D * tmp_D_L = NULL;
          c_NT3_Base_Node_1D * tmp_D_R = NULL;
          
          string tmp_Node_Type = "";
          u_Data_3 tmp_State;
          tmp_State.I = 0;
          u_Data_3 tmp_NID;
          tmp_NID.I = 0;
          u_Data_3 tmp_L;
          tmp_L.I = 0;
          u_Data_3 tmp_R;
          tmp_R.I = 0;
          u_Data_3 tmp_RC_Lvl;
          tmp_RC_Lvl.F = 0.0f;
          
          
          *p_LF >> tmp_Node_Type;
          tmp_Node_Type = "";
          *p_LF >> Node_Count.U;
          std::cout << "\n " << Node_Count.U << " Nodes Found..........";
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          u_Data_3 Node_Counter;
          Node_Counter.U = 1;
          int tmp_Div = int(Node_Count.I) / 1000;
          if (tmp_Div == 0){ tmp_Div = 1; }
          
          while(!(*p_LF).eof())
          {
               if (!(Node_Counter.U % tmp_Div))
               { 
                    xy(tmp_X, tmp_Y); 
                    std::cout << ((double (Node_Counter.U) / double (Node_Count.U)) * 100) << "%      "; 
               }
               Node_Counter.U++;
               
               *p_LF >> tmp_Node_Type;
               //*std::cout << "\n\n" << tmp_Node_Type;
               if (tmp_Node_Type == "SN")
               {
                    *p_LF >> tmp_State.I;
                    //*std::cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    tmp_Node = get_State_Node(tmp_State);
                    
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    
               }
               if (tmp_Node_Type == "N")
               {
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    *p_LF >> tmp_L.I;
                    //*std::cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*std::cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node();
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    
                    //Gather the dendrite references from the tree.
                    tmp_Scaffold.search(tmp_L);
                    tmp_D_L = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*std::cout << "  D_L->" << tmp_D_L << " L->" << tmp_L.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    tmp_Scaffold.search(tmp_R);
                    tmp_D_R = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*std::cout << "  D_R->" << tmp_D_R << " R->" << tmp_R.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    //Create the lower connections.
                    create_Connection_L(tmp_D_L, tmp_Node);
                    create_Connection_R(tmp_D_R, tmp_Node);
                    
                    
                    //*std::cout << " tmp_Node->NID->" << tmp_Node->NID.I;
               }
                              
               if (tmp_Node_Type == "STN")
               {
                    *p_LF >> tmp_State.I;
                    //*std::cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    tmp_Node = get_State_Node(tmp_State);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    
               }
               if (tmp_Node_Type == "TN")
               {
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    *p_LF >> tmp_L.I;
                    //*std::cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*std::cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node();
                    tmp_Scaffold.search(tmp_NID);
                    tmp_Scaffold.set_Current_Node_NID(tmp_Node);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Gather the dendrite references from the tree.
                    tmp_Scaffold.search(tmp_L);
                    tmp_D_L = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*std::cout << "  D_L->" << tmp_D_L << " L->" << tmp_L.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    tmp_Scaffold.search(tmp_R);
                    tmp_D_R = tmp_Scaffold.get_Current_Node_NID();
                    
                    //*std::cout << "  D_R->" << tmp_D_R << " R->" << tmp_R.I << " " << tmp_Scaffold.flg_Foundit;
                    
                    //Create the lower connections.
                    create_Connection_L(tmp_D_L, tmp_Node);
                    create_Connection_R(tmp_D_R, tmp_Node);
                    
                    tmp_Node->set_Type(3);
                    
                    //*std::cout << " tmp_Node->NID->" << tmp_Node->NID.I;
               }
               
               *p_LF >> tmp_RC_Lvl.F;
               //*std::cout << " tmp_RC_Lvl " << tmp_RC_Lvl.F;
               
               tmp_Node->RC_Lvl = tmp_RC_Lvl.F;
               
               //*std::cout << " " << tmp_Node << " ";
               //tmp_Node->bp_O();
          }
          xy(tmp_X, tmp_Y); std::cout << "100.00%      ";
          std::cout << "\n Resetting Scaffold.....";
          tmp_Scaffold.reset();
          std::cout << "Done\n";
     }
     
     
     //Loads the node network, new faster method, dendrites only.
     void load(ifstream * p_LF)
     {
          //Create the file.
          std::cout << "\n\n Node Network Loading..........";
          if (p_LF->is_open()){ std::cout << "Authorized"; } else { std::cout << "Denied"; return; }
          
          u_Data_3 Node_Count;
          Node_Count.U = 0;
          
          //The new scaffold.
          c_NT3_Base_Node_1D ** tmp_NScaffold = NULL;
          
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          string tmp_Node_Type = "";
          
          u_Data_3 tmp_State;
          tmp_State.I = 0;
          
          u_Data_3 tmp_NID;
          tmp_NID.I = 0;
          
          u_Data_3 tmp_L;
          tmp_L.I = 0;
          u_Data_3 tmp_R;
          tmp_R.I = 0;
          
          u_Data_3 tmp_RC_Lvl;
          tmp_RC_Lvl.F = 0.0f;
          
          int tmp_Axon_Count_L = 0;
          int tmp_Axon_Count_R = 0;
          
          *p_LF >> tmp_Node_Type;
          tmp_Node_Type = "";
          *p_LF >> Node_Count.U;
          std::cout << "\n " << Node_Count.U << " Nodes Found..........";
          
          //Setup the NScaffold
          if (Node_Count.U == 0){ return; }
          tmp_NScaffold = new c_NT3_Base_Node_1D*[unsigned int (Node_Count.U)];
          for (unsigned long long int cou_Index=0;cou_Index<Node_Count.U;cou_Index++)
          {
               tmp_NScaffold[cou_Index] = NULL;
          }
          
          int tmp_X = get_Console_Cursor_X();
          int tmp_Y = get_Console_Cursor_Y();
          u_Data_3 Node_Counter;
          Node_Counter.U = 1;
          int tmp_Div = int(Node_Count.I) / 1000;
          if (tmp_Div == 0){ tmp_Div = 1; }
          
          while(!(*p_LF).eof())
          {
               if (!(Node_Counter.U % tmp_Div))
               { 
                    xy(tmp_X, tmp_Y); 
                    std::cout << ((double (Node_Counter.U) / double (Node_Count.U)) * 100) << "%      "; 
               }
               Node_Counter.U++;
               
               *p_LF >> tmp_Node_Type;
               //*std::cout << "\n\n" << tmp_Node_Type;
               if (tmp_Node_Type == "SN")
               {
                    *p_LF >> tmp_State.I;
                    //*std::cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    tmp_Node = get_State_Node(tmp_State, tmp_Axon_Count_L, tmp_Axon_Count_R);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
               }
               if (tmp_Node_Type == "N")
               {
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    *p_LF >> tmp_L.I;
                    //*std::cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*std::cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node(tmp_Axon_Count_L, tmp_Axon_Count_R);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
                    //Create the lower connections.
                    create_Predefined_Connection_L(tmp_NScaffold[tmp_L.I], tmp_Node);
                    create_Predefined_Connection_R(tmp_NScaffold[tmp_R.I], tmp_Node);
               }
                              
               if (tmp_Node_Type == "STN")
               {
                    *p_LF >> tmp_State.I;
                    //*std::cout << " St " << tmp_State.I;
                    
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    tmp_Node = get_State_Node(tmp_State, tmp_Axon_Count_L, tmp_Axon_Count_R);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
               }
               if (tmp_Node_Type == "TN")
               {
                    *p_LF >> tmp_NID.I;
                    //*std::cout << " NID " << tmp_NID.I;
                    
                    //Get the axon counts.
                    *p_LF >> tmp_Axon_Count_L;
                    *p_LF >> tmp_Axon_Count_R;
                    
                    *p_LF >> tmp_L.I;
                    //*std::cout << " L " << tmp_L.I;
                    
                    *p_LF >> tmp_R.I;
                    //*std::cout << " R " << tmp_R.I;
                    
                    //Create the node and add it to the state tree with the NID as an identifier.
                    tmp_Node = new_Node(tmp_Axon_Count_L, tmp_Axon_Count_R);
                    convert_To_Treetop_Node(tmp_Node);
                    
                    //Add the tmp_Node to the tmp_NScaffold
                    tmp_NScaffold[tmp_NID.I] = tmp_Node;
                    
                    //Create the lower connections.
                    create_Predefined_Connection_L(tmp_NScaffold[tmp_L.I], tmp_Node);
                    create_Predefined_Connection_R(tmp_NScaffold[tmp_R.I], tmp_Node);
                    
                    tmp_Node->set_Type(3);
                    
                    //*std::cout << " tmp_Node->NID->" << tmp_Node->NID.I;
               }
               
               *p_LF >> tmp_RC_Lvl.F;
               //*std::cout << " tmp_RC_Lvl " << tmp_RC_Lvl.F;
               
               tmp_Node->RC_Lvl = tmp_RC_Lvl.F;
               
               //*std::cout << " " << tmp_Node << " ";
               //tmp_Node->bp_O();
          }
          xy(tmp_X, tmp_Y); std::cout << "100.00%      ";
          std::cout << "\n Resetting Scaffold.....";
          
          //Derete the tmp_NScaffold
          for (unsigned long long int cou_Index=0;cou_Index<Node_Count.U;cou_Index++)
          {
               /*
               std::cout << "\n ["; oint(0, 13, cou_Index); std::cout << "] ";
               if (tmp_NScaffold[cou_Index] != NULL)
               { 
                    oint(0, 8, tmp_NScaffold[cou_Index]->NID.U);
                    std::cout << " ";
                    tmp_NScaffold[cou_Index]->bp_O(); 
                    
                    if (tmp_NScaffold[cou_Index]->Dendrite_L != NULL)
                    {
                         std::cout << " ";
                         tmp_NScaffold[cou_Index]->Dendrite_L->bp_O();
                    }
                    if (tmp_NScaffold[cou_Index]->Dendrite_R != NULL)
                    {
                         std::cout << " ";
                         tmp_NScaffold[cou_Index]->Dendrite_R->bp_O();
                    }
               }
               */
               tmp_NScaffold[cou_Index] = NULL;
          }
          delete [] tmp_NScaffold;
          
          std::cout << "Done\n";
     }
     

     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     void output_Treetops()
     {
          Treetops_Tree.output_Tree();
     }
     
     //Outputs the treetops as backpropagated nodes.
     void output_Treetops_BP()
     {
          Treetops_Tree.output_Tree_BP();
     }
     
     
     //Oututs all of the nodes.
     void output_Nodes()
     {
          c_NT3_Base_Node_1D * tmp_LL = Root;
          //Root = NULL;
          //Root = NULL;
          c_NT3_Base_Node_1D * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               std::cout << "\n  ";
               tmp_LL->bp_O();
               tmp_LL = tmp_LL_Next;
          }
     }
};





















































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
          //*cout << " [";
          if (NID != NULL){ NID->bp_O(); }
          cout << " $" << Charge.I << "]  ";
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
               
               
               
               //--cout << "\n\n SEARCHING ->" << tmp_SData.U;
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
               //--cout << "\n\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               
               
               //--cout << "\n\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //--cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //--cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //--cout << "\n\t   tmp_DData.U !> 1";
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
                    //--cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //--cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               if (p_Node->Data.U == p_Data.U)
               {
                    //--cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
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
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, p_Shift);
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
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
               //*cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //*cout << "  ";
               }
               //*cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->NID); }
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = long long int(tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = long long int((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = long long int((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               if (p_Left_Leg)
               {
                    for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
                    {
                         submit(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                         //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                         //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
                    }
               }
               
               if (p_Right_Leg)
               {
                    for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
                    {
                         submit(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                         //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                         //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
                    }
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
          c_NT3_Linked_List * tmp_LL = Output.Root;
          
          Previous_Highest_Charge = Current_Highest_Charge;
          Current_Highest_Charge = 0;
          
          long long int tmp_Charge = 0;
          float tmp_Charge_Percentage = 0.0f;
          
          while(tmp_LL != NULL)
          {
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = long long int (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = long long int (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = long long int ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
                    //*tmp_LL->NID->Axons_L[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_L;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_L[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_L[cou_A];
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aR->" << tmp_LL->NID->Axon_Count_R;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_R;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_R[cou_A], tmp_Charge);
                    //*cout << " A->" << tmp_LL->NID->Axons_R[cou_A];
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
          cout << "\n Charge_LL->" << Current_Charge.Depth << " ->";
          output_Charge_LL();
          cout << "\n Output_LL->" << Output.Depth << " ->";
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
          //*cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          //*cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ cout << "empty..."; return; }
          Output.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          //*cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ cout << "empty..."; return; }
          Treetops.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
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
          cout << "\n[ " << NID << " ";
          if (NID != NULL){ NID->bp_0(); } else { ostr(0, 12, "NULL"); }
          cout << " $"; oint(0, 14, Charge.I); cout << "]  ";
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
               
               
               
               //--cout << "\n\n SEARCHING ->" << tmp_SData.U;
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
               //--cout << "\n\n\n tmp_DData.U = " << tmp_DData.U << " p_Shift = " << p_Shift;
               
               
               //--cout << "\n\n\t __QN__ tmp_DData.U->" << tmp_DData.U << " p_Data.U->" << p_Data.U;
               
               
               if (p_Node == NULL)
               {      
                    //--cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Buffer_Node_2D;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //--cout << "\n\t   tmp_DData.U > 1";
                         p_Node->Data = tmp_DData;
                    }
                    else 
                    {
                         //--cout << "\n\t   tmp_DData.U !> 1";
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
                    //--cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_DData.U (" << tmp_DData.U << ")";
                    
                    //--cout << "\t tmp_DData.U = " << tmp_DData.U;
                    
                    return query_Node(p_Node->Center, p_Data, (p_Shift - 1));
               }
               
               //If the node data matches the given data exactly the node has been found.
               //This is not locked because even if the Current & flg_Foundit are overwritten they will not be
               //used by the threads, but will work with the standard single thread.
               if (p_Node->Data.U == p_Data.U)
               {
                    //--cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
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
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    return query_Node(p_Node->Left, p_Data, p_Shift);
               }
               if (tmp_DData.U > p_Node->Data.U)
               {
                    //--cout << "\n\t    tmp_DData.U (" << tmp_DData.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
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
               //*cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    //*cout << "  ";
               }
               //*cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->NID); }
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = ((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_L;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_0;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_0[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[0])));
                    //*cout << " A->" << tmp_LL->NID->Axons_0[cou_A];
                    //*tmp_LL->NID->Axons_0[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_1;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_1[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_1[cou_A]->flg_Double_Legged[1])));
                    //*cout << " A->" << tmp_LL->NID->Axons_1[cou_A];
                    //*tmp_LL->NID->Axons_1[cou_A]->bp_O();
               }               
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_2;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_2[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_2[cou_A]->flg_Double_Legged[2])));
                    //*cout << " A->" << tmp_LL->NID->Axons_2[cou_A];
                    //*tmp_LL->NID->Axons_2[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_3;cou_A++)
               {
                    submit(tmp_LL->NID->Axons_3[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_3[cou_A]->flg_Double_Legged[3])));
                    //*cout << " A->" << tmp_LL->NID->Axons_3[cou_A];
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
               if (tmp_Charge < (Base_Charge * Action_Potential_Threshold)){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " aL->" << tmp_LL->NID->Axon_Count_0;
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_0;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_0[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[0])));
                    //*cout << " A->" << tmp_LL->NID->Axons_0[cou_A];
                    //*tmp_LL->NID->Axons_0[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_1;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_1[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[1])));
                    //*cout << " A->" << tmp_LL->NID->Axons_1[cou_A];
                    //*tmp_LL->NID->Axons_1[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_2;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_2[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[2])));
                    //*cout << " A->" << tmp_LL->NID->Axons_2[cou_A];
                    //*tmp_LL->NID->Axons_2[cou_A]->bp_O();
               }
               
               for (int cou_A=0;cou_A<tmp_LL->NID->Axon_Count_3;cou_A++)
               {
                    submit_RC(tmp_LL->NID->Axons_3[cou_A], (tmp_Charge / (tmp_LL->NID->Axons_0[cou_A]->flg_Double_Legged[3])));
                    //*cout << " A->" << tmp_LL->NID->Axons_3[cou_A];
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " (((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               
               tmp_Charge_Percentage = ((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               if (p_Use_Spike == 2){ tmp_Charge = ((tmp_Charge_Percentage + ((1 - tmp_Charge_Percentage) * .1)) * Base_Charge); }
               
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
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
               //*cout << "\n " << tmp_LL->NID << " T->" << tmp_LL->NID->get_Type() << " ";
               //*tmp_LL->NID->bp_O();
               
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               
               //*cout << " tmp_Charge = " << " ((((" << tmp_LL->Charge.I << " * " << Modifier_Charge << ") * " << tmp_LL->NID->get_RC_Score() << ") / " << Previous_Highest_Charge << ") * " << Base_Charge << ")";
               //tmp_Charge = (((tmp_LL->Charge.I * Modifier_Charge) / Previous_Highest_Charge) * Base_Charge);
               
               tmp_Charge_Percentage = (((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge);
               if (p_Use_Spike == 0){ tmp_Charge = (tmp_Charge_Percentage * Base_Charge); }
               if (p_Use_Spike == 1){ tmp_Charge = ((tmp_Charge_Percentage * tmp_Charge_Percentage) * Base_Charge); }
               //tmp_Charge = ((((tmp_LL->Charge.I * Modifier_Charge) * tmp_LL->NID->get_RC_Score()) / Previous_Highest_Charge) * Base_Charge);
               
               //*cout << " c->" << tmp_Charge;
               
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
          cout << "\n    Charge_LL->" << Current_Charge.Depth << " ->";
          output_Charge_LL();
          cout << "\n    Output_LL->" << Output.Depth << " ->";
          output_Output_LL();
          cout << "\n    Output_Tt->" << Treetops.Depth << " ->";
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
          //*cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          //*cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ cout << "empty..."; return; }
          Output.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          //*cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ cout << "empty..."; return; }
          Treetops.Root->output_LL();
          //*cout << " ~Fing"; cout.flush();
     }
};
*///This is the linked list for the buffer.
class c_NT3_Linked_Data_List
{
public:
     
     //The next node in the chain.
     c_NT3_Linked_Data_List * Next;
     u_Data_3 Charge;
     u_Data_3 NID;
     
     c_NT3_Linked_Data_List()
     {
          Next = NULL;
          Charge.I = 0;
          NID.U = 0;
     }
     
     ~c_NT3_Linked_Data_List()
     {
          Charge.I = 0;
          NID.U = 0;
          //delete Next; Handled by the handler.
     }
     
     //Outputs the linked list.
     void output_LL()
     {
          cout << " [";
          cout << NID.U << " ";
          cout << NID.C;
          cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Linked_Data_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Linked_Data_List * Root;
     c_NT3_Linked_Data_List ** Current_LL;
     
     c_NT3_Linked_Data_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
     }
     
     ~c_NT3_Linked_Data_List_Handler()
     {
          reset();
     }
     
     //Adds a node.
     c_NT3_Linked_Data_List * new_LL(u_Data_3 p_NID, long long int p_Charge)
     {
          c_NT3_Linked_Data_List * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Linked_Data_List;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->NID = p_NID;
          
          tmp_LL = *Current_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Linked_Data_List * tmp_LL = Root;
          Root = NULL;
          c_NT3_Linked_Data_List * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          delete Root;
          Root = NULL;
          Current_LL = &Root;
     }
     
};


class c_NT3_Data_Buffer_Node
{
     friend class c_NT3_Data_Buffer_Tree; 
     private:
          c_NT3_Data_Buffer_Node * Right;
          c_NT3_Data_Buffer_Node * Center;
          c_NT3_Data_Buffer_Node * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Linked_Data_List * LL;
          
          c_NT3_Data_Buffer_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Data_Buffer_Node()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Data_Buffer_Tree
{
private:
             
             
     public:
          c_NT3_Data_Buffer_Node * Root;
          c_NT3_Data_Buffer_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Data_Buffer_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Data_Buffer_Tree()
          {
               delete Root;
          }
          
          void search(u_Data_3 p_Data)
          {
               flg_Foundit = false;
               
               tmp_Data.U = 0;
               
               query_Node(Root, p_Data, 63);
          }
          
          int query_Node(c_NT3_Data_Buffer_Node *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               tmp_Data.U = p_Data.U >> p_Shift;
               //cout << "\n\n\n tmp_Data.U = " << tmp_Data.U << " p_Shift = " << p_Shift;
               
               
               //cout << "\n\n\t __QN__ tmp_Data.U->" << tmp_Data.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Data_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //cout << "\n\t   tmp_Data.U > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //cout << "\n\t   tmp_Data.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_Data.U && p_Shift > 0)
               {
                    //cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_Data.U (" << tmp_Data.U << ")";
                    
                    //cout << "\t tmp_Data.U = " << tmp_Data.U;
                    
                    query_Node(p_Node->Center, p_Data, (p_Shift - 2));
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.U == p_Data.U)
               {
                    //cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    return 1;
               }
               
               
               if (tmp_Data.U < p_Node->Data.U)
               {
                    //cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Left, p_Data, p_Shift);
                    return 0;
               }
               if (tmp_Data.U > p_Node->Data.U)
               {
                    //cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Right, p_Data, p_Shift);
                    return 0;
               }
               return 0;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Data_Buffer_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    cout << "  ";
               }
               cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->NID.U); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Linked_Data_List * get_Node_LL(c_NT3_Data_Buffer_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Data_Buffer_Node * &p_Node, c_NT3_Linked_Data_List * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Linked_Data_List * set_Current_Node_LL(c_NT3_Linked_Data_List * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Linked_Data_List * get_Current_Node_LL()
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
class c_NT3_Data_Buffer
{
private:
     
public:
     
     //The search tree.
     c_NT3_Data_Buffer_Tree Tree;
     
     //The linked lists.
     c_NT3_Linked_Data_List_Handler Data_Buffer;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
     
     //Settings & trackers for the settings.
     long long int Base_Charge;
     long long int Current_Highest_Charge;
     long long int Previous_Highest_Charge;
     
     double Modifier_Charge;
     double Action_Potential_Threshold;
     
     
     c_NT3_Data_Buffer()
     {
          flg_Not_Done = 0;
          Input_Position = 0;
          
          Current_Highest_Charge = 0;
          Previous_Highest_Charge = 0;
          Base_Charge = 0;
          Modifier_Charge = 0.0f;
          Action_Potential_Threshold = 0.0f;
     }
     
     ~c_NT3_Data_Buffer()
     {
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(u_Data_3 p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
               if ((Tree.get_Current_Node_LL())->Charge.I > Current_Highest_Charge){ Current_Highest_Charge = (Tree.get_Current_Node_LL())->Charge.I; }
          }
          
          if (!Tree.flg_Foundit)
          {
               if (p_Charge > Current_Highest_Charge){ Current_Highest_Charge = p_Charge; }
               
               Tree.set_Current_Node_LL(Data_Buffer.new_LL(p_Node, p_Charge));
          }
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          output_Charge_LL();
     }
     
     //Outputs the Data_Buffer_LL.
     void output_Charge_LL()
     {
          cout << "\n\t Data_Buffer_LL->";
          if (Data_Buffer.Root == NULL){ cout << "empty..."; return; }
          Data_Buffer.Root->output_LL();
     }
};//This is the linked list for the buffer.
class c_NT3_Vision_Linked_List
{
public:
     
     //The next node in the chain.
     c_NT3_Vision_Linked_List * Next;
     u_Data_3 Charge;
     u_Data_3 State;
     
     c_NT3_Vision_Linked_List()
     {
          Next = NULL;
          Charge.I = 0;
          State.I = 0;
     }
     
     ~c_NT3_Vision_Linked_List()
     {
          Charge.I = 0;
          State.I = 0;
     }
     
     //Outputs the linked list.
     void output_LL()
     {
          cout << " [";
          cout << State.C;
          cout << " $" << Charge.I << "]  ";
          if (Next != NULL){ Next->output_LL(); }
     }
};

//This handles the linked list creation and deletion so that recursive destruction does not crash the stack.
class c_NT3_Vision_Linked_List_Handler
{
public:
     
     //The nodes in the linked list.
     c_NT3_Vision_Linked_List * Root;
     c_NT3_Vision_Linked_List ** Current_LL;
     
     c_NT3_Vision_Linked_List_Handler()
     {
          Root = NULL;
          Current_LL = &Root;
     }
     
     ~c_NT3_Vision_Linked_List_Handler()
     {
          reset();
     }
     
     //Adds a node.
     c_NT3_Vision_Linked_List * new_LL(u_Data_3 p_State, long long int p_Charge)
     {
          c_NT3_Vision_Linked_List * tmp_LL = NULL;
          
          *Current_LL = new c_NT3_Vision_Linked_List;
          
          (*Current_LL)->Charge.I = p_Charge;
          (*Current_LL)->State = p_State;
          
          tmp_LL = *Current_LL;
          
          Current_LL = &(*Current_LL)->Next;
          
          return tmp_LL;
     }
     
     //Resets the linked list.
     void reset()
     {
          c_NT3_Vision_Linked_List * tmp_LL = Root;
          Root = NULL;
          c_NT3_Vision_Linked_List * tmp_LL_Next = NULL;
          
          while (tmp_LL != NULL)
          {
               tmp_LL_Next = tmp_LL->Next;
               tmp_LL->Next = NULL;
               delete tmp_LL;
               tmp_LL = NULL;
               tmp_LL = tmp_LL_Next;
          }
          
          Root = NULL;
          Current_LL = &Root;
     }
     
};



class c_NT3_Vision_Buffer_Node
{
     friend class c_NT3_Vision_Buffer_Tree; 
     private:
          c_NT3_Vision_Buffer_Node * Right;
          c_NT3_Vision_Buffer_Node * Center;
          c_NT3_Vision_Buffer_Node * Left;
          
     public:
          u_Data_3 Data;
          
          c_NT3_Vision_Linked_List * LL;
          
          c_NT3_Vision_Buffer_Node()
          {
               Right = NULL;
               Center = NULL;
               Left = NULL;
               LL = NULL;
               Data.I = 0;
          }
          
          ~c_NT3_Vision_Buffer_Node()
          {
               LL = NULL;
               delete Right; Right = NULL;
               delete Center; Center = NULL;
               delete Left; Left = NULL;
          }
                
};

class c_NT3_Vision_Buffer_Tree
{
private:
             
             
     public:
          c_NT3_Vision_Buffer_Node * Root;
          c_NT3_Vision_Buffer_Node ** Current;//the current node that will allow referencing outside of the search function
          bool flg_Foundit;//the flag that set when a node is found already set
          
          //For handling the current search.
          u_Data_3 tmp_Data;
          u_Data_3 tmp_SData;
          
          c_NT3_Vision_Buffer_Tree()
          {
               Root = NULL;
               Current = NULL;
               flg_Foundit = false;
          }
          
          ~c_NT3_Vision_Buffer_Tree()
          {
               delete Root;
          }
          
          void search(u_Data_3 p_Data)
          {
               flg_Foundit = false;
               
               tmp_Data.U = 0;
               
               query_Node(Root, p_Data, 63);
          }
          
          int query_Node(c_NT3_Vision_Buffer_Node *&p_Node, u_Data_3 p_Data, int p_Shift)
          {
               tmp_Data.U = p_Data.U >> p_Shift;
               //cout << "\n\n\n tmp_Data.U = " << tmp_Data.U << " p_Shift = " << p_Shift;
               
               
               //cout << "\n\n\t __QN__ tmp_Data.U->" << tmp_Data.U << " p_Data.U->" << p_Data.U;
               
               if (p_Node == NULL)
               {      
                    //cout << "\n\t   Node is NULL";
                    p_Node = new c_NT3_Vision_Buffer_Node;
                    p_Node->Right = NULL;
                    p_Node->Center = NULL;
                    p_Node->Left = NULL;
                    if (p_Shift > 1)
                    {
                         //cout << "\n\t   tmp_Data.U > 1";
                         p_Node->Data = tmp_Data;
                    }
                    else 
                    {
                         //cout << "\n\t   tmp_Data.U !> 1";
                         p_Node->Data = p_Data;
                         Current = &p_Node; 
                         flg_Foundit = false;
                         return 0;
                    }
               }
               
               //If the current data matches the tmp data then another depth is explored.
               if (p_Node->Data.U == tmp_Data.U && p_Shift > 0)
               {
                    //cout << "\n\t   p_Node->Data.U (" << p_Node->Data.U << ") == tmp_Data.U (" << tmp_Data.U << ")";
                    
                    //cout << "\t tmp_Data.U = " << tmp_Data.U;
                    
                    query_Node(p_Node->Center, p_Data, (p_Shift - 4));
                    return 0;  
               }
               
               //If the node data matches the given data exactly the node has been found.
               if (p_Node->Data.U == p_Data.U)
               {
                    //cout << "\n\t    p_Node->Data.NR(" << p_Node->Data.NR << ") == p_Data.NR(" << p_Data.NR << ")";
                    Current = &p_Node;  
                    flg_Foundit = true;
                    return 1;
               }
               
               
               if (tmp_Data.U < p_Node->Data.U)
               {
                    //cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") < p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Left, p_Data, p_Shift);
                    return 0;
               }
               if (tmp_Data.U > p_Node->Data.U)
               {
                    //cout << "\n\t    tmp_Data.U (" << tmp_Data.U << ") > p_Node->Data.U(" << p_Node->Data.U << ")";
                    query_Node(p_Node->Right, p_Data, p_Shift);
                    return 0;
               }
               return 0;
          }
          
          void output_Tree()
          {
               output_Node(Root, 0);
          }
          
          void output_Node(c_NT3_Vision_Buffer_Node *&p_Node, int p_Tab)
          {
               if (p_Node == NULL){ return; }
               output_Node(p_Node->Left, (p_Tab));
               cout << "\n";
               for (int cou_Index=0;cou_Index<p_Tab;cou_Index++)
               {
                    cout << "  ";
               }
               cout << (p_Node->Data).I;
               if (p_Node->LL != NULL){ cout << "->" << (p_Node->LL->State.C); }
               output_Node(p_Node->Center, (p_Tab + 1));
               output_Node(p_Node->Right, (p_Tab));
          }
          
          c_NT3_Vision_Linked_List * get_Node_LL(c_NT3_Vision_Buffer_Node * &p_Node)
          {
               if (p_Node != NULL){     
                    return p_Node->LL;      
                    } else {
                        return NULL;
                    }
          }
          
          void set_Node_LL(c_NT3_Vision_Buffer_Node * &p_Node, c_NT3_Vision_Linked_List * p_LL)
          {
               if (p_Node != NULL)
               {
                    p_Node->LL = p_LL;
               }
          }
          
          c_NT3_Vision_Linked_List * set_Current_Node_LL(c_NT3_Vision_Linked_List * p_LL)
          {
               set_Node_LL(*Current, p_LL);
               return p_LL;
          }
          
          c_NT3_Vision_Linked_List * get_Current_Node_LL()
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
class c_NT3_Vision_Buffer
{
private:
     
public:
     
     //The search tree.
     c_NT3_Vision_Buffer_Tree Tree;
     
     //The linked lists.
     c_NT3_Vision_Linked_List_Handler Current_Charge;
     
     c_NT3_Vision_Linked_List_Handler Output;
     
     c_NT3_Vision_Linked_List_Handler Treetops;
     
     //Flags
     bool flg_Not_Done;
     
     int Input_Position;
          
     //Used for the gathering of the strongest state.
     u_Data_3 SState;
     u_Data_3 SCharge;
     
     c_NT3_Vision_Buffer()
     {
          Current_Charge.reset();
          
          Treetops.reset();
          
          Output.reset();
     }
     
     ~c_NT3_Vision_Buffer()
     {
          Current_Charge.reset();
          
          Treetops.reset();
          
          Output.reset();
     }
     
     //Accepts a submission of nodes to charge, used for CAN.
     void submit(u_Data_3 p_Node, long long int p_Charge = 10000)
     {
          //Search for the submitted node, if found add the charge.
          Tree.search(p_Node);
          
          if (Tree.flg_Foundit)
          {
               (Tree.get_Current_Node_LL())->Charge.I += p_Charge;
          }
          
          if (!Tree.flg_Foundit)
          {
               Tree.set_Current_Node_LL(Current_Charge.new_LL(p_Node, p_Charge));
          }
     }
     
     //Gathers the output nodes and resets the tree for the next iteration.
     void gather()
     {
          //Delete the old Output.
          Output.reset();
          
          //Reassign the linked lists.
          Output.Root = Current_Charge.Root;
          Current_Charge.Root = NULL;
          Current_Charge.reset();
          
          //Delete the tree.
          Tree.reset();
          
          //Finds the strongest state and charge.
          find_SState();
     }
     
     //Finds the strongest state in the output buffer.
     void find_SState()
     {
          c_NT3_Vision_Linked_List * tmp_LL = Output.Root;
          
          SState.I = 0;
          SCharge.I = 0;
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->Charge.I > SCharge.I)
               {
                    SState = tmp_LL->State;
                    SCharge = tmp_LL->Charge;
               }
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Outputs the buffer.
     void output_Buffer()
     {
          Tree.output_Tree();
          output_Charge_LL();
          output_Output_LL();
     }
     
     //Outputs the Current_Charge_LL.
     void output_Charge_LL()
     {
          cout << "\n\t Current_Charge_LL->";
          if (Current_Charge.Root == NULL){ cout << "empty..."; return; }
          Current_Charge.Root->output_LL();
     }
     
     //Outputs the Output_LL.
     void output_Output_LL()
     {
          cout << "\n\t Output_LL->";
          if (Output.Root == NULL){ cout << "empty..."; return; }
          Output.Root->output_LL();
     }
     
     //Outputs the Treetops.
     void output_Treetops()
     {
          cout << "\n\t Treetops_LL->";
          if (Treetops.Root == NULL){ cout << "empty..."; return; }
          Treetops.Root->output_LL();
     }
};

//The class for the 1D CAN.
class c_NT3_CAN_1D
{
private:
     
     
public:
     
     //Reference to the input table to use for building.
     c_Raw_Table_Cell_1D * Input;
     
     //Reference to the node network to use for the node handling.
     c_NT3_Node_Network_1D * Nodes;
     
     //The nodes that make up the CAN.
     c_NT3_Base_Node_1D *** CAN;
     
     //The current number of tiers in the CAN.
     int Number_Of_Tiers;
     
     //The treetop node.
     c_NT3_Base_Node_1D * Treetop;
     
     c_NT3_CAN_1D()
     {
          Input = NULL;
          Nodes = NULL;
          CAN = NULL;
          Treetop = NULL;
          Number_Of_Tiers = 0;
     }
     
     ~c_NT3_CAN_1D()
     {
          reset();
     }
     
     //Initializes the CAN with an input cell and the node netowrk.
     void init(c_NT3_Node_Network_1D * p_Nodes)
     {
          Nodes = p_Nodes;
     }
     
     //Sets the current input cell.
     void set_Input(c_Raw_Table_Cell_1D * p_Input)
     {
          Input = p_Input;
     }
     
     //Builds full.
     void Full()
     {
          resize();
          fill_State();
          build_Tiers_Full();
     }
     
     //Builds full with RC reinforcing.
     void RC()
     {
          resize();
          fill_State();
          build_Tiers_Full();
          reinforce();
     }
     
     //Builds for Query.
     void Query()
     {
          resize();
          query_State();
          build_Tiers_Query();
     }
     
     //Fills out the State tier.
     void fill_State()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               CAN[0] [cou_Index] = Nodes->get_State_Node(Input->Data[cou_Index]);
          }
     }
          
     //Fills out the State tier.
     void query_State()
     {
          for (int cou_Index=0;cou_Index<Number_Of_Tiers;cou_Index++)
          {
               CAN[0] [cou_Index] = Nodes->get_State_Node_Query(Input->Data[cou_Index]);
          }
     }
     
     //Builds the Tiers full.
     void build_Tiers_Full()
     {
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 2);cou_T++)
          {
               for (int cou_Index=0;cou_Index<((Number_Of_Tiers - cou_T) - 1);cou_Index++)
               {
                    CAN[cou_T + 1] [cou_Index] = Nodes->get_Upper_Tier_Connection(CAN[cou_T] [cou_Index], CAN[cou_T] [cou_Index + 1]);
               }
          }
          
          //If there are not enough tiers to create a treetop connection then return NULL.
          if (Number_Of_Tiers < 1){ Treetop = NULL; return; }
          if (Number_Of_Tiers == 1){ Nodes->convert_To_Treetop_Node(CAN[0] [0]); Treetop = CAN[0] [0]; return; }
          
          //Gets the treetop node.
          CAN[Number_Of_Tiers - 1] [0] = Nodes->get_Treetop_Connection(CAN[Number_Of_Tiers - 2] [0], CAN[Number_Of_Tiers - 2] [1]);
          
          //Gather treetop node.
          Treetop = CAN[Number_Of_Tiers - 1] [0];
     }
     
     //Builds the Tiers full.
     void build_Tiers_Query()
     {
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<(Number_Of_Tiers - 1);cou_T++)
          {
               for (int cou_Index=0;cou_Index<((Number_Of_Tiers - cou_T) - 1);cou_Index++)
               {
                    CAN[cou_T + 1] [cou_Index] = Nodes->does_Upper_Tier_Connection_Exist(CAN[cou_T] [cou_Index], CAN[cou_T] [cou_Index + 1]);
               }
          }
     }
     
     //Reinforces the nodes in the CAN.
     void reinforce()
     {
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    if (CAN[cou_T] [cou_Index] == NULL){ continue; }
                    CAN[cou_T] [cou_Index]->reinforce();
               }
          }
     }
     
     //Resets the CAN.
     void reset()
     {
          if (Number_Of_Tiers != 0)
          {
               for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
               {
                    if (CAN[cou_T] == NULL){ continue; }
                    for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
                    {
                         CAN[cou_T] [cou_Index] = NULL;
                    }
                    delete [] CAN[cou_T];
                    CAN[cou_T] = NULL;
               }
               delete [] CAN;
               CAN = NULL;
          }
          Number_Of_Tiers = 0;
          
     }
     
     //Resizes the CAN to build the current input set.
     void resize()
     {
          //Reset the CAN before we begin.
          reset();
          
          //Gather the number of tiers to build.
          Number_Of_Tiers = Input->Length;
          
          //Setup the tiers.
          CAN = new c_NT3_Base_Node_1D**[Number_Of_Tiers];
          
          //Init the tiers and setup the indexes NULLing them along the way.
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               CAN[cou_T] = new c_NT3_Base_Node_1D*[(Number_Of_Tiers - cou_T)];
               
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    CAN[cou_T] [cou_Index] = NULL;
               }
          }
     }
     
     //Outputs the CAN.
     void output_CAN()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index] << "}";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_RC()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index]->get_RC_Score() << "}";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_State()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {";
                    Nodes->bp_Output_Only(CAN[cou_T] [cou_Index]);
                    cout << "}";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Type()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << (CAN[cou_T] [cou_Index]->get_Type()) << "}";
               }
          }
     }
          
     //Outputs the CAN.
     void output_CAN_Axons()
     {
          cout << "\n\n";
          for (int cou_T=0;cou_T<Number_Of_Tiers;cou_T++)
          {
               cout << "\n" << cou_T << "->";
               for (int cou_Index=0;cou_Index<(Number_Of_Tiers - cou_T);cou_Index++)
               {
                    cout << " {" << CAN[cou_T] [cou_Index] << " (" << (CAN[cou_T] [cou_Index]->Axon_Count_L) << ", " << (CAN[cou_T] [cou_Index]->Axon_Count_R) << ")}";
               }
          }
     }

};//This contains the base construct polymorphic class.
class c_NT3_Base_Construct: public c_NT3_Base
{
private:
     
public:
     
     //The ID of the construct.
     u_Data_3 CID;
     
     int X;
     int Y;
     int Dimension;
     
     //Tables
     c_Raw_Table_1D tbl_Input;
     
     c_Raw_Table_1D tbl_Input_Aux;
     c_Raw_Table_1D tbl_Input_Charges;
     c_Raw_Table_1D tbl_Input_Position;
     c_Raw_Table_1D tbl_Input_Depths;
     
     c_Raw_Table_1D tbl_Pattern_Output;
     c_Raw_Table_1D tbl_Salt_Output;
     c_Raw_Table_1D tbl_Charge_Output;
     c_Raw_Table_1D tbl_Position_Output;
     c_Raw_Table_1D tbl_RC_Output;
     
     c_Raw_Table_1D tbl_Vision_Output;
     c_Raw_Table_1D tbl_Vision_Charge_Output;
     
     c_Raw_Table_1D tbl_Treetops_Output;
     
     //Settings
     
     //--  Settings
     
     //Charging legs.
     bool setting_Charge_L;
     bool setting_Charge_R;
     
     //The tier to start charging.
     int settings_Tier;
     
     //Fixed vrs floating tier depth. If floating then Tier = (Input_Depth * Tier_Depth)
     int settings_Tier_Is_Floating;
     
     //The floating tier modifier.
     double settings_Tier_Depth;
     
     //The Action Potential Threshold as percentage of the base charge.
     double settings_Action_Potential_Threshold;
     
     //The base charge to apply from the CAN.
     int settings_Base_Charge;
     
     //The multiple to reduce each recursive charge by.
     double settings_Modifier_Charge;
     
     //Whether or not to use the input charges for charging.
     bool setting_Use_Input_Charges;
     
     //Build a vision or do not build a vision.
     bool settings_Build_Vision;
     
     //Sort the tables by charge.
     bool settings_Bubble_Sort_Output;
     
     //The percentage of the input string to charge starting at index 0.
     double settings_Input_Charging_Depth;
     
     //Cross CAN Charging, when charging a leg all nodes are charged in the CAN to the (L||R) of the current,
     //rather than just charging a column of the CAN.
     // 0 = Normal, 1 = Normal + TNT, 2 = Full Charge
     int settings_Cross_CAN_Charging;
     
     //Determines whether or not to stack outputs when discharging treetops from foreign constructs.
     int settings_Stack_Output_From_Foreign_Treetops;
     
     //The flag for using the charging spike in the buffer charging.
     int setting_Charge_Spike;
     
     //Whether or not to make a new cell in the output tables.
     int settings_Flat_Output;
     
     //This determines the method for handling the inputs and outputs from the tables.
     //0: Build each cell as a single input and reads the treetop into the corresponding output cell.
     //1: Buids all the cells together as a single input, [1] [2] [3] would be read in as [123]. Used for MSC.
     //2: Build each column as a single input, useful for multiple construct treetop input to MSC. 
     // [a] [b] [c]
     // [q] [w] [e]
     //Read in as aq, bw, ce and output as [Tt:aq] [Tt:bw] [Tt:ce]
     int settings_Build_Method;
     //0: Evals each cell as a single input and appends the outputs to the same row as the input row.
     //1: Evals a whole row as a single input, [1] [2] [3] would be read in as [123]. Used for MSC.
     int settings_Eval_Method;
     
     //Whether or not to use RC.
     int settings_RC;
     
     virtual ~c_NT3_Base_Construct(){};
     
     ////==-----------------------+
     //==--   API FUNCTIONS
     ////==-----------------------+
     
     virtual c_Raw_Table_1D * get_Table_1D(string p_Table)=0;
     
     virtual void set_Name(string p_Name)=0;
     virtual void set_CID(unsigned long long int p_CID)=0;
     
	 virtual int first_Run(string p_Dir, string p_Name)=0;
	 virtual int init(string p_Dir, string p_Name)=0;

     //Used to reset the IO tables of the construct.
     virtual void reset_IO()=0;
     
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
     
     virtual void Eval(int p_Discharge)=0; //Eval with no RC.
     
     ////==-----------------------+
     //==--   BUILDING FUNCTIONS
     ////==-----------------------+
     
     virtual void Build()=0; //Builds full CAN.
     virtual void Build_Query()=0; //Builds for query.
     
     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges a given treetop.
     //virtual void discharge_Treetop_By_NID_L(int p_Input, u_Data_3 p_NID, u_Data_3 p_Charge)=0;
     
     //Discharges the treetops in the treetops_output table. Used for discharging treetops backpropagated from higher constructs.
     virtual void Discharge()=0;
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     virtual void output_Input_Tables()=0; //Outputs the input tables.
     virtual void output_Output_Tables()=0; //Outputs the output tables.
     virtual void output_CAN()=0; //Outputs the CAN.
     virtual long long int get_Node_Count()=0; //Returns the number of nodes in the network.
     virtual void output_Treetops()=0; //Outputs the treetops tree in the node network.
     virtual void output_Treetops_BP()=0; //Outputs the treetops tree in a compressed BP format.
     virtual void output_Nodes_As_HTML(string p_Dir, string p_File, int p_X_Padd, int p_Y_Padd)=0; //Outputs the node network to a HTML for visual output.
      
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     virtual void Save(string p_DIR, string p_Name)=0; //Saves the construct.
     virtual void Load(string p_DIR, string p_Name)=0; //Loads a construct.     
};


//The construct class.
class c_NT3_Construct_1D: public c_NT3_Base_Construct
{
private:
     
     //The node network to use.
     c_NT3_Node_Network_1D Nodes;
     
     //The CAN used for building the memory trees.
     c_NT3_CAN_1D CAN;
     
     //The Charging_Buffers. One for each input.
     c_NT3_Buffer * Charging_Buffers;
     
     //The Vision. One for each input.
     c_NT3_Vision_Buffer * Vision_Buffers;
     
     //The current input depth.
     int Input_Depth;
     
     //The current input depth to charge using the settings_Charging_Depth
     int Input_Charging_Depth;
     
     //The current input being evaluated.
     int Current_Input;
     
public:
     
     
     //The name of the Construct.
     string Name;
     
     //The input tables.
     c_Raw_Table_1D tbl_Input;
     
     c_Raw_Table_1D tbl_Input_Aux_L;
     c_Raw_Table_1D tbl_Input_Charges_L;
     c_Raw_Table_1D tbl_Input_Position_L;
     c_Raw_Table_1D tbl_Input_Depths_L;
     
     c_Raw_Table_1D tbl_Input_Charges_R;
     c_Raw_Table_1D tbl_Input_Position_R;
     c_Raw_Table_1D tbl_Input_Aux_R;
     c_Raw_Table_1D tbl_Input_Depths_R;
     
     
     //Output Tables.
     c_Raw_Table_1D tbl_Pattern_Output_L;
     c_Raw_Table_1D tbl_Charge_Output_L;
     c_Raw_Table_1D tbl_Position_Output_L;
     c_Raw_Table_1D tbl_RC_Output_L;
     c_Raw_Table_1D tbl_Treetops_Output_L;
     
     c_Raw_Table_1D tbl_Pattern_Output_R;
     c_Raw_Table_1D tbl_Charge_Output_R;
     c_Raw_Table_1D tbl_Position_Output_R;
     c_Raw_Table_1D tbl_RC_Output_R;
     c_Raw_Table_1D tbl_Treetops_Output_R;
     
     c_Raw_Table_1D tbl_Pattern_Output_C;
     c_Raw_Table_1D tbl_Charge_Output_C;
     c_Raw_Table_1D tbl_Position_Output_C;
     c_Raw_Table_1D tbl_RC_Output_C;
     c_Raw_Table_1D tbl_Treetops_Output_C;
     
     c_Raw_Table_1D tbl_Treetops;
     
     c_Raw_Table_1D tbl_Vision_Output;
     c_Raw_Table_1D tbl_Vision_Charge_Output;
     
     c_Raw_Table_1D tbl_Depths_L;
     c_Raw_Table_1D tbl_Depths_R;
     
     //--  Settings
     
     //Charging legs.
     bool setting_Charge_L;
     bool setting_Charge_R;
     
     //The tier to start charging.
     int settings_Tier;
     
     //Fixed vrs floating tier depth. If floating then Tier = (Input_Depth * Tier_Depth)
     int settings_Tier_Is_Floating;
     
     //The floating tier modifier.
     double settings_Tier_Depth;
     
     //The Action Potential Threshold as percentage of the base charge.
     double settings_Action_Potential_Threshold;
     
     //The base charge to apply from the CAN.
     int settings_Base_Charge;
     
     //The multiple to reduce each recursive charge by.
     double settings_Modifier_Charge;
     
     //Whether or not to use the input charges for charging.
     bool setting_Use_Input_Charges;
     
     //Build a vision or do not build a vision.
     bool settings_Build_Vision;
     
     //Sort the tables by charge.
     bool settings_Bubble_Sort_Output;
     
     //The percentage of the input string to charge starting at index 0.
     double settings_Input_Charging_Depth;
     
     //Cross CAN Charging, when charging a leg all nodes are charged in the CAN to the (L||R) of the current,
     //rather than just charging a column of the CAN.
     // 0 = Normal, 1 = Normal + TNT, 2 = Full Charge
     int settings_Cross_CAN_Charging;
     
     //Determines whether or not to stack outputs when discharging treetops from foreign constructs.
     int settings_Stack_Output_From_Foreign_Treetops;
     
     //The flag for using the charging spike in the buffer charging.
     int setting_Charge_Spike;
     
     //Whether or not to make a new cell in the output tables.
     int settings_Flat_Output;
     
     //==--  SETTINGS FOR THE AUTOCALIBRATION
     
     //The type of auto to use, Numerical or Alphabetical for example.
     int sauto_Type;
     
     //Whether or not the RC is being evaluated.
     int sauto_Use_RC;
     
     c_NT3_Construct_1D()
     {
          u_Data_3 tmp_Name;
          tmp_Name.NR = this;
          Name = "C__" + int2str(tmp_Name.U);
          
          set_Name(Name);
          
          Current_Input = 0;
          
          setting_Charge_L = 1;
          setting_Charge_R = 1;
          
          settings_Tier = 0;
          settings_Tier_Is_Floating = 0;
          settings_Tier_Depth = 0;
          
          settings_Action_Potential_Threshold = 0;
          settings_Base_Charge = 10000;
          
          setting_Use_Input_Charges = 0;
          
          settings_Modifier_Charge = .5;
          
          settings_Bubble_Sort_Output = 0;
          
          settings_Build_Vision = 1;
          
          settings_Input_Charging_Depth = 1;
          
          settings_Cross_CAN_Charging = 0;
          
          settings_Stack_Output_From_Foreign_Treetops = 0;
          
          setting_Charge_Spike = 1;
          
          settings_Flat_Output = 0;
          
          CAN.init(&Nodes);
          Charging_Buffers = NULL;

          Vision_Buffers = NULL;
          Input_Depth = 0;
          Input_Charging_Depth = 0;
          
          sauto_Use_RC = 0;
          sauto_Type = 0;
     }
     
     ~c_NT3_Construct_1D()
     {
          cout << "\n  ~c_NT3_Construct_1D " << Name << " " << this << ".........."; cout.flush();
          
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Input_Depth = 0;
          cout << "\n  ~~~c_NT3_Construct_1D " << Name << " " << this << ".........."; cout.flush();
     }
     
     //Sets the name of the construct and the tables.
     void set_Name(string p_Name)
     {
          Name = p_Name;
          
          //The input tables.
          tbl_Input.Name = Name + "___Input";
          
          tbl_Input_Aux_L.Name = Name + "___Input_Aux_L";
          tbl_Input_Charges_L.Name = Name + "___Input_Charges_L";
          tbl_Input_Position_L.Name = Name + "___Input_Position_L";
          tbl_Input_Depths_L.Name = Name + "___Input_Depths_L";
          
          tbl_Input_Aux_R.Name = Name + "___Input_Aux_R";
          tbl_Input_Charges_R.Name = Name + "___Input_Charges_R";
          tbl_Input_Position_R.Name = Name + "___Input_Position_R";
          tbl_Input_Depths_R.Name = Name + "___Input_Depths_R";
          
          //Output Tables.
          tbl_Pattern_Output_L.Name = Name + "___Pattern_Output_L";
          tbl_Charge_Output_L.Name = Name + "___Charge_Output_L";
          tbl_Position_Output_L.Name = Name + "___Position_Output_L";
          tbl_RC_Output_L.Name = Name + "___RC_Output_L";
          tbl_Treetops_Output_L.Name = Name + "___Treetops_Output_L";
          
          tbl_Pattern_Output_R.Name = Name + "___Pattern_Output_R";
          tbl_Charge_Output_R.Name = Name + "___Charge_Output_R";
          tbl_Position_Output_R.Name = Name + "___Position_Output_R";
          tbl_RC_Output_R.Name = Name + "___RC_Output_R";
          tbl_Treetops_Output_R.Name = Name + "___Treetops_Output_R";
          
          tbl_Pattern_Output_C.Name = Name + "___Pattern_Output_C";
          tbl_Charge_Output_C.Name = Name + "___Charge_Output_C";
          tbl_Position_Output_C.Name = Name + "___Position_Output_C";
          tbl_RC_Output_C.Name = Name + "___RC_Output_C";
          tbl_Treetops_Output_C.Name = Name + "___Treetops_Output_C";
          
          tbl_Treetops.Name = Name + "___Treetops";
          
          tbl_Vision_Output.Name = Name + "___Vision_Output";
          tbl_Vision_Charge_Output.Name = Name + "___Vision_Charge_Output";
          
          tbl_Depths_L.Name = Name + "___Depths_L";
          tbl_Depths_R.Name = Name + "___Depths_R";
     }
     
     c_Raw_Table_1D * get_Table_1D(string p_Table)
     {
          if (p_Table == "tbl_Input"){ return &tbl_Input; }
     
          if (p_Table == "tbl_Input_Aux_L"){ return &tbl_Input_Aux_L; }
          if (p_Table == "tbl_Input_Charges_L"){ return &tbl_Input_Charges_L; }
          if (p_Table == "tbl_Input_Position_L"){ return &tbl_Input_Position_L; }
          if (p_Table == "tbl_Input_Depths_L"){ return &tbl_Input_Depths_L; }
     
          if (p_Table == "tbl_Input_Charges_R"){ return &tbl_Input_Charges_R; }
          if (p_Table == "tbl_Input_Position_R"){ return &tbl_Input_Position_R; }
          if (p_Table == "tbl_Input_Aux_R"){ return &tbl_Input_Aux_R; }
          if (p_Table == "tbl_Input_Depths_R"){ return &tbl_Input_Depths_R; }
     
     
          //Output Tables.
          if (p_Table == "tbl_Pattern_Output_L"){ return &tbl_Pattern_Output_L; }
          if (p_Table == "tbl_Charge_Output_L"){ return &tbl_Charge_Output_L; }
          if (p_Table == "tbl_Position_Output_L"){ return &tbl_Position_Output_L; }
          if (p_Table == "tbl_RC_Output_L"){ return &tbl_RC_Output_L; }
          if (p_Table == "tbl_Treetops_Output_L"){ return &tbl_Treetops_Output_L; }
     
          if (p_Table == "tbl_Pattern_Output_R"){ return &tbl_Pattern_Output_R; }
          if (p_Table == "tbl_Charge_Output_R"){ return &tbl_Charge_Output_R; }
          if (p_Table == "tbl_Position_Output_R"){ return &tbl_Position_Output_R; }
          if (p_Table == "tbl_RC_Output_R"){ return &tbl_RC_Output_R; }
          if (p_Table == "tbl_Treetops_Output_R"){ return &tbl_Treetops_Output_R; }
     
          if (p_Table == "tbl_Pattern_Output_C"){ return &tbl_Pattern_Output_C; }
          if (p_Table == "tbl_Charge_Output_C"){ return &tbl_Charge_Output_C; }
          if (p_Table == "tbl_Position_Output_C"){ return &tbl_Position_Output_C; }
          if (p_Table == "tbl_RC_Output_C"){ return &tbl_RC_Output_C; }
          if (p_Table == "tbl_Treetops_Output_C"){ return &tbl_Treetops_Output_C; }
     
          if (p_Table == "tbl_Treetops"){ return &tbl_Treetops; }
     
          if (p_Table == "tbl_Vision_Output"){ return &tbl_Vision_Output; }
          if (p_Table == "tbl_Vision_Charge_Output"){ return &tbl_Vision_Charge_Output; }
     
          if (p_Table == "tbl_Depths_L"){ return &tbl_Depths_L; }
          if (p_Table == "tbl_Depths_R"){ return &tbl_Depths_R; }
          return NULL;
     }
               
     ////==-----------------------+
     //==--   EVAL FUNCTIONS
     ////==-----------------------+
          
     //Eval with no RC.
     void Eval_OldSchool()
     {
          reset_Outputs();
          Current_Input = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               Build_Input_Query(cou_Cell);
               
               if (settings_Tier_Is_Floating)
               {
                    settings_Tier = (Input_Depth * settings_Tier_Depth);
                    //*cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
               }
               
               init_Charging_Buffer_C();
               charge_Buffer_C();
               discharge_Treetops_C();
               
               
               //gather_Vision();
               
               /*
               if (tbl_Vision_Output.Rows != NULL)
               {
                    cout << "\n-<{";
                    if (tbl_Vision_Output.Rows[0]->Cells != NULL)
                    {
                         tbl_Vision_Output.Rows[0]->Cells[0]->output_C();
                    }
                    cout << "}>-";
               }
               */
               //tbl_Vision_Charge_Output.output_I();
               
          }
     }
     
     
     //Eval with no RC.
     void Eval()
     {
          reset_Outputs();
          Current_Input = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               Build_Input_Query(cou_Cell);
               
               if (settings_Tier_Is_Floating)
               {
                    settings_Tier = (Input_Depth * settings_Tier_Depth);
                    //*cout << "\n " << Name << " settings_Tier->" << settings_Tier << " = Input_Depth->" << Input_Depth << " * " << settings_Tier_Depth;
               }
               
               if (setting_Charge_L)
               {
                    init_Charging_Buffers();
                    charge_Buffers_L();
                    discharge_All_Treetops_L();
               }
               
               if (setting_Charge_R)
               {
                    init_Charging_Buffers();
                    charge_Buffers_R();
                    discharge_All_Treetops_R();
               }
               
               
               //gather_Vision();
               
               /*
               if (tbl_Vision_Output.Rows != NULL)
               {
                    cout << "\n-<{";
                    if (tbl_Vision_Output.Rows[0]->Cells != NULL)
                    {
                         tbl_Vision_Output.Rows[0]->Cells[0]->output_C();
                    }
                    cout << "}>-";
               }
               */
               //tbl_Vision_Charge_Output.output_I();
               
          }
     }
     
     //Eval with RC.
     void Eval_RC()
     {
          reset_Outputs();
          Current_Input = 0;
          
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Current_Input = cou_Cell;
               
               Build_Input_Query(cou_Cell);
               
               if (settings_Tier_Is_Floating)
               {
                    settings_Tier = (Input_Depth * settings_Tier_Depth);
               }
               
               if (setting_Charge_L)
               {
                    init_Charging_Buffers();
                    charge_Buffers_RC_L();
                    discharge_All_Treetops_L();
               }
               
               if (setting_Charge_R)
               {
                    init_Charging_Buffers();
                    charge_Buffers_RC_R();
                    discharge_All_Treetops_R();
               }
               
               //gather_Vision();
               
               /*
               if (tbl_Vision_Output.Rows != NULL)
               {
                    cout << "\n-<{";
                    if (tbl_Vision_Output.Rows[0]->Cells != NULL)
                    {
                         tbl_Vision_Output.Rows[0]->Cells[0]->output_C();
                    }
                    cout << "}>-";
               }
               */
               
          }
     }
     
     //Eval with no RC.
     void Eval_Aux_Inputs()
     {
          reset_Outputs();
          Current_Input = 0;
          
          //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
          //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
          //will be submitted to the charging buffer along with the charge from the charge table.
          if (setting_Charge_L && !(tbl_Input_Aux_L.Number_Of_Rows == 0))
          {
               //*cout << "\n LEFT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_L.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_L.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    //*cout << "\n   gather_Aux_Input_L()";
                    gather_Aux_Input_L();
                    //*cout << "\n   charge_Buffers_L()";
                    charge_Aux_Buffers_L();
                    //*cout << "\n   discharge_All_Treetops_L()";
                    discharge_All_Treetops_L();
               }
               //*cout << "\n Discharge_Left.....Output_Tables";
               //output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
          if (setting_Charge_R && !(tbl_Input_Aux_R.Number_Of_Rows == 0))
          {
               //*cout << "\n RIGHT -------===========--------";
               for (int cou_Cell=0;cou_Cell<tbl_Input_Aux_R.Rows[0]->Depth;cou_Cell++)
               {
                    Current_Input = cou_Cell;
                    //*cout << "\n\n cou_Cell->" << cou_Cell;
                    
                    Input_Depth = (tbl_Input_Depths_R.Rows[0]->Cells[cou_Cell]->Data[0]).U;
                    Input_Charging_Depth = Input_Depth;
                    //*cout << "\n   Input_Charging_Depth->" << Input_Charging_Depth;
                    //*cout << "\n   init_Charging_Buffers()";
                    init_Charging_Buffers();
                    //*cout << "\n   gather_Aux_Input_R()";
                    gather_Aux_Input_R();
                    //*cout << "\n   charge_Buffers_R()";
                    charge_Aux_Buffers_R();
                    //*cout << "\n   discharge_All_Treetops_R()";
                    discharge_All_Treetops_R();
               }
               //*cout << "\n Discharge_Right.....Output_Tables";
               //*output_Output_Tables();
               //*cout << "\n DONE ------------===========-------------";
          }
          
     }
     
     
          
     //Sets the charging buffers up for each input.
     void init_Charging_Buffer_C()
     {
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Charging_Buffers = new c_NT3_Buffer[1];
          
          Charging_Buffers[0].Base_Charge = settings_Base_Charge;
          Charging_Buffers[0].Modifier_Charge = settings_Modifier_Charge;
          Charging_Buffers[0].Action_Potential_Threshold = settings_Action_Potential_Threshold;
          //Charging_Buffers[0].resize_Opp((Input_Depth - 0));
     }     
     
     //Sets the charging buffers up for each input.
     void init_Charging_Buffers()
     {
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Charging_Buffers = new c_NT3_Buffer[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Charging_Buffers[cou_Index].Base_Charge = settings_Base_Charge;
               Charging_Buffers[cou_Index].Modifier_Charge = settings_Modifier_Charge;
               Charging_Buffers[cou_Index].Action_Potential_Threshold = settings_Action_Potential_Threshold;
               //Charging_Buffers[cou_Index].resize_Opp((Input_Depth - cou_Index));
          }
     }     
     
     //Sets the charging buffers up for each input.
     void init_Charging_Buffers(int p_Input_Depth)
     {
          Input_Depth = p_Input_Depth;
          Input_Charging_Depth = Input_Depth;
          
          delete [] Charging_Buffers;
          Charging_Buffers = NULL;
          Charging_Buffers = new c_NT3_Buffer[Input_Depth];
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Charging_Buffers[cou_Index].Base_Charge = settings_Base_Charge;
               Charging_Buffers[cou_Index].Modifier_Charge = settings_Modifier_Charge;
               Charging_Buffers[cou_Index].Action_Potential_Threshold = settings_Action_Potential_Threshold;
          }
     }
     
     //Charges a single buffer.
     void charge_Aux_Buffer_L(int p_Input)
     {
          //*cout << "\n\t     charge_Aux_Buffer_L(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].gather();
               
               //*Charging_Buffers[p_Input].output_Buffer();
          }
     }
     
     //Charges a single buffer.
     void charge_Aux_Buffer_R(int p_Input)
     {
          //*cout << "\n\t     charge_Aux_Buffer_R(" << p_Input <<  ")";
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               //*Charging_Buffers[p_Input].output_Buffer();
               
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_R(cou_Index);
          }
     }
     
     //RC CHARGING FOR AUX INPUTS
     //Charges a single buffer.
     void charge_Aux_Buffer_RC_L(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Aux_Buffer_RC_R(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          Charging_Buffers[p_Input].gather();
          
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_RC_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_RC_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Aux_Buffers_RC_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               charge_Aux_Buffer_RC_R(cou_Index);
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_C()
     {
          Charging_Buffers[0].Input_Position = 0;
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {    
               Charging_Buffers[0].charge_Outputs(setting_Charge_Spike);
               
               for (int cou_Index=0;cou_Index<(Input_Depth - cou_T);cou_Index++)
               {
                    if (CAN.CAN[cou_T] [cou_Index] != NULL)
                    {
                         Charging_Buffers[0].submit(CAN.CAN[cou_T] [cou_Index], (settings_Base_Charge));
                    }
               }
               
               Charging_Buffers[0].gather();
               
               if (!Charging_Buffers[0].flg_Not_Done){ break; }
               
          }
          
          while(Charging_Buffers[0].flg_Not_Done)
          {
               Charging_Buffers[0].charge_Outputs(setting_Charge_Spike);
               
               Charging_Buffers[0].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_L(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          //Cross CAN Charging.
          if (settings_Cross_CAN_Charging == 1 && settings_Tier == 0)
          {
               for (int cou_Index=p_Input;cou_Index<Input_Depth;cou_Index++)
               {
                    if ((CAN.CAN[0] [cou_Index] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[0] [cou_Index], settings_Base_Charge); }
               }
          }
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {    
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               if (((Input_Depth - cou_T) - p_Input) > 0 && (CAN.CAN[cou_T] [p_Input] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[cou_T] [p_Input], settings_Base_Charge); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
               
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_R(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          
          //Cross CAN Charging.
          if (settings_Cross_CAN_Charging == 1 && settings_Tier == 0)
          {
               for (int cou_Index=0;cou_Index<=p_Input;cou_Index++)
               {
                    if ((CAN.CAN[0] [cou_Index] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[0] [cou_Index], settings_Base_Charge); }
               }
          }
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               if ((p_Input - cou_T) >= 0 && (CAN.CAN[cou_T] [p_Input - cou_T] != NULL)){ Charging_Buffers[p_Input].submit(CAN.CAN[cou_T] [p_Input - cou_T], settings_Base_Charge); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
               
               //cout << "\n\t   Tier[" << cou_T << "] ->" << Charging_Buffers[p_Input].Current_Highest_Charge;
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_R(cou_Index);
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_RC_L(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               if (((Input_Depth - cou_T) - p_Input) > 0 && (CAN.CAN[cou_T] [p_Input] != NULL)){ Charging_Buffers[p_Input].submit_RC(CAN.CAN[cou_T] [p_Input]); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_RC_L(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges a single buffer.
     void charge_Buffer_RC_R(int p_Input)
     {
          Charging_Buffers[p_Input].Input_Position = p_Input;
          
          for (int cou_T=settings_Tier;cou_T<Input_Depth;cou_T++)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               if ((p_Input - cou_T) >= 0 && (CAN.CAN[cou_T] [p_Input - cou_T] != NULL)){ Charging_Buffers[p_Input].submit_RC(CAN.CAN[cou_T] [p_Input - cou_T]); }
               
               Charging_Buffers[p_Input].gather();
               
               if (!Charging_Buffers[p_Input].flg_Not_Done){ break; }
          }
          while(Charging_Buffers[p_Input].flg_Not_Done)
          {
               Charging_Buffers[p_Input].charge_Outputs_R(setting_Charge_Spike);
               
               Charging_Buffers[p_Input].destroy_Output_Buffers();
               
               Charging_Buffers[p_Input].gather();
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_RC_L()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_RC_L(cou_Index);
          }
     }
     
     //Charges the buffers with the CAN until all inputs are finished.
     void charge_Buffers_RC_R()
     {
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               charge_Buffer_RC_R(cou_Index);
          }
     }
     

     
     
     //Gathers the output for the left vision table.
     void gather_Vision_L()
     {
          //The position.
          int tmp_Position = 0;
          u_Data_3 tmp_Charge;
          int tmp_Length = 0;
          c_Raw_Table_Row_1D * tmp_Row = NULL;
          
          //*cout << "\n\n\nGV__L #R " << tbl_Pattern_Output_L.Number_Of_Rows;
          
          for (int cou_R=0;cou_R<tbl_Pattern_Output_L.Number_Of_Rows;cou_R++)
          {
               //*cout << "\n\n Row[" << cou_R << "]\t";
               //*tbl_Pattern_Output_L.Rows[cou_R]->output_C();
               //*cout << "\n";
               
               tmp_Row = tbl_Pattern_Output_L.Rows[cou_R];
               tmp_Position = tmp_Row->Cells[0]->Data[0].I;
               tmp_Charge.I = tmp_Row->Cells[1]->Data[0].I;
               tmp_Length = tmp_Row->Cells[2]->Length;
               
               //*cout << "\t P " << tmp_Position << "  C " << tmp_Charge.I << "  L " << tmp_Length << "\n";
               
               for (int cou_D=0;cou_D<tmp_Length;cou_D++)
               {
                    //*cout << "\n\t     cou_D->" << cou_D << " tfp->" << (cou_D + tmp_Position);
                    if ((cou_D + tmp_Position) >= Input_Depth){ continue; }
                    //*cout << "  [" << cou_D << "]->" << tmp_Row->Cells[2]->Data[cou_D].C << " $" << tmp_Charge.I << " p" << (cou_D + tmp_Position);
                    Vision_Buffers[(cou_D + tmp_Position)].submit(tmp_Row->Cells[2]->Data[cou_D], tmp_Charge.I);
               }
               //*cout << "\n\n";
          }
     }

     //Gathers the output for the left vision table.
     void gather_Vision_R()
     {
          //The position.
          int tmp_Position = 0;
          u_Data_3 tmp_Charge;
          int tmp_Length = 0;
          c_Raw_Table_Row_1D * tmp_Row = NULL;
          
          //*cout << "\n\n\nGV__R #R " << tbl_Pattern_Output_R.Number_Of_Rows;
          
          for (int cou_R=0;cou_R<tbl_Pattern_Output_R.Number_Of_Rows;cou_R++)
          {
               //*cout << "\n\n Row[" << cou_R << "]\t";
               //*tbl_Pattern_Output_R.Rows[cou_R]->output_C();
               //*cout << "\n";
               
               tmp_Row = tbl_Pattern_Output_R.Rows[cou_R];
               tmp_Position = tmp_Row->Cells[0]->Data[0].I;
               tmp_Charge.I = tmp_Row->Cells[1]->Data[0].I;
               tmp_Length = tmp_Row->Cells[2]->Length;
               
               //*cout << "\t P " << tmp_Position << "  C " << tmp_Charge.I << "  L " << tmp_Length << "\n";
               
               for (int cou_Data=0;cou_Data<tmp_Length;cou_Data++)
               {
                    //*cout << "\n\t     cou_Data->" << cou_Data << " tfp->" << (cou_Data + tmp_Position - (tmp_Length - 1));
                    if ((cou_Data + tmp_Position - (tmp_Length - 1)) < 0){ continue; }
                    //*cout << "  [" << cou_Data << "]->{" << tmp_Row->Cells[2]->Data[cou_Data].C << "} $" << tmp_Charge.I << " p" << (cou_Data + tmp_Position - (tmp_Length - 1));
                    
                    Vision_Buffers[(cou_Data + tmp_Position - (tmp_Length - 1))].submit(tmp_Row->Cells[2]->Data[cou_Data], tmp_Charge.I);
               }
          }
     }
     
     //Gathers the outputs into the vision table.
     void gather_Vision()
     {
          //If the settings say no then return;
          if (!settings_Build_Vision){ return; }
          
          //Setup the vision buffers.
          delete [] Vision_Buffers;
          Vision_Buffers = NULL;
          Vision_Buffers = new c_NT3_Vision_Buffer[Input_Depth];
          
          //The two cells that will hold the vision output.
          //c_Raw_Table_Cell_1D * tmp_V;
          //c_Raw_Table_Cell_1D * tmp_VC;
          
          //Read the left side into the vision buffer.
          if (setting_Charge_L)
          {
               //*cout << "\n\n Gather_Vision_L";
               gather_Vision_L();
          }
          
          //Read the right side into the vision buffer.
          if (setting_Charge_R)
          {
               //*cout << "\n\n Gather_Vision_R";
               gather_Vision_R();
          }
          
          for (int cou_Index=0;cou_Index<Input_Depth;cou_Index++)
          {
               Vision_Buffers[cou_Index].gather();
               //*cout << "\n\t V[" << cou_Index << "] {" << Vision_Buffers[cou_Index].SState.C << "} " << Vision_Buffers[cou_Index].SCharge.I;
               tbl_Vision_Output.add_Data(Vision_Buffers[cou_Index].SState);
               tbl_Vision_Charge_Output.add_Data_Int(Vision_Buffers[cou_Index].SCharge.I);
          }
          
          //*system("PAUSE > NULL");
     }
     
     ////==-----------------------+
     //==--   BUILDING FUNCTIONS
     ////==-----------------------+
     
     //Builds full CAN.
     void Build()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops.reset();
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Build_Input(cou_Cell);
          }
     }
     
     //Builds RC CAN.
     void Build_RC()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops.reset();
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Build_Input_RC(cou_Cell);
          }
     }
     
     //Builds for query.
     void Build_Query()
     {
          if (tbl_Input.Number_Of_Rows == 0){ return; }
          
          tbl_Treetops.reset();
          
          for (int cou_Cell=0;cou_Cell<tbl_Input.Rows[0]->Depth;cou_Cell++)
          {
               Build_Input_Query(cou_Cell);
          }
     }
     
     //Builds full CAN.
     void Build_Input(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.Full();
          if (CAN.Treetop == NULL){ tbl_Treetops.add_Int(Current_Input, 0); return; }
          tbl_Treetops.add_Int(Current_Input, (CAN.Treetop->NID.U + 1));
     }
     
     //Builds RC CAN.
     void Build_Input_RC(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.RC();
          if (CAN.Treetop == NULL){ tbl_Treetops.add_Int(Current_Input, 0); return; }
          tbl_Treetops.add_Int(Current_Input, (CAN.Treetop->NID.U + 1));
     }
     
     //Builds for query.
     void Build_Input_Query(int p_Cell = 0)
     {
          gather_CAN_Input(0, p_Cell);
          CAN.Query();
     }
     
     /*
     //Creates a connection between two nodes.
     void create_Connection(long long int p_L, long long int p_R)
     {
          c_NT3_Base_Node_1D * tmp_L = Nodes.get_Node_By_NID(p_L);
          c_NT3_Base_Node_1D * tmp_R = Nodes.get_Node_By_NID(p_R);
          
          Nodes.get_Upper_Tier_Connection(tmp_L, tmp_R);
     }
      * */
     
     //Gathers the first cell of the input table to build. This is neccesary as the cells are dynamically
     //allocated and addresses may change.
     void gather_CAN_Input(int p_Row = 0, int p_Cell = 0)
     {
          Input_Depth = 0;
          
          if (tbl_Input.Number_Of_Rows <= p_Row){ return; }
          if (tbl_Input.Rows[0]->Depth <= p_Cell){ return; }
          
          Input_Depth = tbl_Input.Rows[p_Row]->Cells[p_Cell]->Length;
          CAN.set_Input(tbl_Input.Rows[p_Row]->Cells[p_Cell]);
          
          
          Input_Charging_Depth = (Input_Depth * settings_Input_Charging_Depth);
          if (Input_Charging_Depth < 1){ Input_Charging_Depth = 1; }
          //if (Input_Charging_Depth == 0 && Input_Depth == 1){ Input_Charging_Depth = 1; }
     }
     
     //Gathers the aux input into the buffers.a
     //For each cell the input pattern will be gathered. This pattern will be distributed to the appropriate charging
     //buffers designated by the pattern input table. The state will be gathered from the node network and the resultant node
     //will be submitted to the charging buffer along with the charge from the charge table.
     void gather_Aux_Input_L()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge);
          }
     }
     
     //Gathers the aux input into the buffers.
     void gather_Aux_Input_R()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit(tmp_Node, tmp_Charge);
          }
     }
     
     void gather_Aux_Input_RC_L()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_L.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit_RC(tmp_Node, tmp_Charge);
          }
     }
     
     //Gathers the aux input into the buffers.
     void gather_Aux_Input_RC_R()
     {
          u_Data_3 tmp_State;
          tmp_State.U = 0;
          c_NT3_Base_Node_1D * tmp_Node = 0;
          long long int tmp_Charge = 0;
          int tmp_Position = 0;
          
          for (int cou_Data=0;cou_Data<tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Length;cou_Data++)
          {
               tmp_State.U = (tbl_Input_Aux_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Node = Nodes.get_State_Node(tmp_State);
               tmp_Position = (tbl_Input_Position_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               tmp_Charge = (tbl_Input_Charges_R.Rows[0]->Cells[Current_Input]->Data[cou_Data]).U;
               Charging_Buffers[tmp_Position].submit_RC(tmp_Node, tmp_Charge);
          }
     }
     


     ////==--------------------------+
     //==--   DISCHARGING FUNCTIONS
     ////==--------------------------+
     
     //Discharges the treetops for a given input.
     void discharge_Treetops_C()
     {
          Charging_Buffers[0].gather_Treetops();
          c_NT3_Linked_List * tmp_LL = Charging_Buffers[0].Treetops.Root;
                    
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               //*cout << "\n";
               //*tmp_LL->NID->bp_O();
               //*cout << " "; odbl(0, 12, (settings_Base_Charge * settings_Action_Potential_Threshold));
               //*cout << " "; oint(0, 13, (tmp_LL->Charge.I));
               if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){  } else { tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_LL->NID == NULL)
               {                    
                    tbl_Pattern_Output_C.new_Cell(Current_Input);
                    tbl_Pattern_Output_C.add_String("NULL");
                    tbl_Position_Output_C.add_Int(Current_Input, 0);
                    tbl_Charge_Output_C.add_Int(Current_Input, tmp_LL->Charge.I);
                    tbl_RC_Output_C.add_Float(Current_Input, 0);
                    tbl_Treetops_Output_C.add_Int(Current_Input, 0);
                    
                    tmp_LL = tmp_LL->Next;
                    continue;
               }
               
               Nodes.bp(&tbl_Pattern_Output_C, settings_Flat_Output, Current_Input, tmp_LL->NID);
               
               tbl_Position_Output_C.add_Int(Current_Input, 0);
               tbl_Charge_Output_C.add_Int(Current_Input, tmp_LL->Charge.I);
               tbl_RC_Output_C.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());
               tbl_Treetops_Output_C.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Discharges the treetops for a given input.
     void discharge_Treetops_L(int p_Input)
     {
          Charging_Buffers[p_Input].gather_Treetops();
          c_NT3_Linked_List * tmp_LL = Charging_Buffers[p_Input].Treetops.Root;
                    
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){} else { tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_LL->NID == NULL)
               {                    
                    tbl_Pattern_Output_L.new_Cell(Current_Input);
                    tbl_Pattern_Output_L.add_String("NULL");
                    tbl_Position_Output_L.add_Int(Current_Input, p_Input);
                    tbl_Charge_Output_L.add_Int(Current_Input, tmp_LL->Charge.I);
                    tbl_RC_Output_L.add_Float(Current_Input, 0);
                    tbl_Treetops_Output_L.add_Int(Current_Input, 0);
                    
                    tmp_LL = tmp_LL->Next;
                    continue;
               }
               
               Nodes.bp(&tbl_Pattern_Output_L, settings_Flat_Output, Current_Input, tmp_LL->NID);
               
               tbl_Position_Output_L.add_Int(Current_Input, p_Input);
               tbl_Charge_Output_L.add_Int(Current_Input, tmp_LL->Charge.I);
               tbl_RC_Output_L.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());
               tbl_Treetops_Output_L.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);
               
               tmp_LL = tmp_LL->Next;
          }
     }
          
     //Discharges the treetops for a given input.
     void discharge_Treetops_R(int p_Input)
     {
          Charging_Buffers[p_Input].gather_Treetops();
          c_NT3_Linked_List * tmp_LL = Charging_Buffers[p_Input].Treetops.Root;
          
          while(tmp_LL != NULL)
          {
               if (tmp_LL->NID == NULL){ tmp_LL = tmp_LL->Next; continue; }
               if (tmp_LL->Charge.I > (settings_Base_Charge * settings_Action_Potential_Threshold)){} else { tmp_LL = tmp_LL->Next; continue; }
               
               if (tmp_LL->NID == NULL)
               {                    
                    tbl_Pattern_Output_R.new_Cell(Current_Input);
                    tbl_Pattern_Output_R.add_String("NULL");
                    tbl_Position_Output_R.add_Int(Current_Input, p_Input);
                    tbl_Charge_Output_R.add_Int(Current_Input, tmp_LL->Charge.I);
                    tbl_RC_Output_R.add_Float(Current_Input, 0);
                    tbl_Treetops_Output_R.add_Int(Current_Input, 0);
                    
                    tmp_LL = tmp_LL->Next;
                    continue;
               }
               
               Nodes.bp(&tbl_Pattern_Output_R, settings_Flat_Output, Current_Input, tmp_LL->NID);
               
               tbl_Position_Output_R.add_Int(Current_Input, p_Input);
               tbl_Charge_Output_R.add_Int(Current_Input, tmp_LL->Charge.I);
               tbl_RC_Output_R.add_Float(Current_Input, tmp_LL->NID->get_RC_Score());
               tbl_Treetops_Output_R.add_Int(Current_Input, tmp_LL->NID->NID.U + 1);
               
               tmp_LL = tmp_LL->Next;
          }
     }
     
     //Discharges the treetops in the L buffer.
     void discharge_All_Treetops_L()
     {          
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               discharge_Treetops_L(cou_Index);
          }
          tbl_Depths_L.add_Int(Current_Input, Input_Charging_Depth);
     }
     
     //Discharges the treetops in the L buffer.
     void discharge_All_Treetops_R()
     {          
          //Loop through the inputs charging them each separately and iteratively.
          for (int cou_Index=0;cou_Index<Input_Charging_Depth;cou_Index++)
          {
               discharge_Treetops_R(cou_Index);
          }
          tbl_Depths_R.add_Int(Current_Input, Input_Charging_Depth);
     }
     
     //Discharges a given treetop.
     void discharge_Treetop_By_NID_L(int p_Position, u_Data_3 p_NID, u_Data_3 p_Charge, int p_Current_Input)
     {
          //cout << "\n (Pos " << p_Position << ", p_NID " << p_NID.U << ", p_Charge " << p_Charge.I << ", p_Current_Input " << p_Current_Input << ")";
                
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          if (p_NID.U == 0)
          { 
               return;
               tbl_Pattern_Output_L.new_Cell(Current_Input);
               tbl_Pattern_Output_L.add_String("NULL");
               tbl_Position_Output_L.add_Int(p_Current_Input, p_Position);
               tbl_Charge_Output_L.add_Int(p_Current_Input, p_Charge.I);
               tbl_RC_Output_L.add_Float(p_Current_Input, 0);
               tbl_Treetops_Output_L.add_Int(p_Current_Input, 0);
               return; 
          }
          
          //Because it is incremented before submission.
          p_NID.U--;
          
          tmp_Node = Nodes.bp_Treetop(&tbl_Pattern_Output_L, settings_Flat_Output, p_Current_Input, p_NID);
          
          tbl_Position_Output_L.add_Int(p_Current_Input, p_Position);
          tbl_Charge_Output_L.add_Int(p_Current_Input, p_Charge.I);
          tbl_RC_Output_L.add_Float(p_Current_Input, tmp_Node->get_RC_Score());
          tbl_Treetops_Output_L.add_Int(p_Current_Input, tmp_Node->NID.U);
          
          if (tbl_Pattern_Output_L.Rows[p_Current_Input]->Depth != tbl_Charge_Output_L.Rows[p_Current_Input]->Depth)
          { 
               cout << "\n      //==-- po->" << tbl_Pattern_Output_L.Rows[p_Current_Input]->Depth << " != co->" << tbl_Charge_Output_L.Rows[p_Current_Input]->Depth;
               cout << "\n discharge_Treetop_By_NID_L(int p_Position " << p_Position << ", u_Data_3 p_NID " << p_NID.U << ", u_Data_3 p_Charge " << p_Charge.I << ", int p_Current_Input " << p_Current_Input << ")";
               cout << "\n po->";
               tbl_Pattern_Output_L.output_I();
               cout << "\n co->";
               tbl_Charge_Output_L.output_I();
               system("PAUSE > NULL");
          }
     }
          
     //Discharges a given treetop.
     void discharge_Treetop_By_NID_R(int p_Position, u_Data_3 p_NID, u_Data_3 p_Charge, int p_Current_Input)
     {
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          if (p_NID.U == 0)
          { 
               tbl_Pattern_Output_R.new_Cell(Current_Input);
               tbl_Pattern_Output_R.add_String("NULL");
               tbl_Position_Output_R.add_Int(p_Current_Input, p_Position);
               tbl_Charge_Output_R.add_Int(p_Current_Input, p_Charge.I);
               tbl_RC_Output_R.add_Float(p_Current_Input, 0);
               tbl_Treetops_Output_R.add_Int(p_Current_Input, 0);
               return; 
          }
          
          //Because it is incremented before submission.
          p_NID.U--;
          
          tmp_Node = Nodes.bp_Treetop(&tbl_Pattern_Output_R, settings_Flat_Output, p_Current_Input, p_NID);
          
          tbl_Position_Output_R.add_Int(p_Current_Input, p_Position);
          tbl_Charge_Output_R.add_Int(p_Current_Input, p_Charge.I);
          tbl_RC_Output_R.add_Float(p_Current_Input, tmp_Node->get_RC_Score());
          tbl_Treetops_Output_R.add_Int(p_Current_Input, tmp_Node->NID.U);
          
     }
     
     //Discharges a given treetop into the given tables.
     void discharge_Treetop_By_NID_Into_Given_Tables_L(int p_Position, u_Data_3 p_NID, u_Data_3 p_Charge, int p_Current_Input, c_Raw_Table_1D * p_PTbl, c_Raw_Table_1D * p_PosTbl, c_Raw_Table_1D * p_CTbl, c_Raw_Table_1D * p_RCTbl, c_Raw_Table_1D * p_TtTbl)
     {
          //cout << "\n (Pos " << p_Position << ", p_NID " << p_NID.U << ", p_Charge " << p_Charge.I << ", p_Current_Input " << p_Current_Input << ")";
                
          c_NT3_Base_Node_1D * tmp_Node = NULL;
          
          if (p_NID.U == 0)
          { 
               return;
               p_PTbl->new_Cell(Current_Input);
               p_PTbl->add_String("NULL");
               p_PosTbl->add_Int(p_Current_Input, p_Position);
               p_CTbl->add_Int(p_Current_Input, p_Charge.I);
               p_RCTbl->add_Float(p_Current_Input, 0);
               p_TtTbl->add_Int(p_Current_Input, 0);
               return; 
          }
          
          //Because it is incremented before submission.
          p_NID.U--;
          
          tmp_Node = Nodes.bp_Treetop(p_PTbl, settings_Flat_Output, p_Current_Input, p_NID);
          
          p_PosTbl->add_Int(p_Current_Input, p_Position);
          p_CTbl->add_Int(p_Current_Input, p_Charge.I);
          p_RCTbl->add_Float(p_Current_Input, tmp_Node->get_RC_Score());
          p_TtTbl->add_Int(p_Current_Input, tmp_Node->NID.U);
          
          if (p_PTbl->Rows[p_Current_Input]->Depth != p_CTbl->Rows[p_Current_Input]->Depth)
          { 
               cout << "\n      //==-- po->" << p_PTbl->Rows[p_Current_Input]->Depth << " != co->" << p_CTbl->Rows[p_Current_Input]->Depth;
               cout << "\n discharge_Treetop_By_NID_L(int p_Position " << p_Position << ", u_Data_3 p_NID " << p_NID.U << ", u_Data_3 p_Charge " << p_Charge.I << ", int p_Current_Input " << p_Current_Input << ")";
               cout << "\n po->";
               p_PTbl->output_I();
               cout << "\n co->";
               p_CTbl->output_I();
               system("PAUSE > NULL");
          }
     }
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Resets the input tables.
     void reset_Inputs()
     {
          tbl_Input.reset();
          
          tbl_Input_Aux_L.reset();
          tbl_Input_Charges_L.reset();
          tbl_Input_Depths_L.reset();
          tbl_Input_Position_L.reset();
          
          tbl_Input_Aux_R.reset();
          tbl_Input_Charges_R.reset();
          tbl_Input_Depths_R.reset();
          tbl_Input_Position_R.reset();
     }
     
     //Resets the output tables.
     void reset_Outputs()
     {
          tbl_Pattern_Output_L.reset();
          tbl_Charge_Output_L.reset();
          tbl_Position_Output_L.reset();
          tbl_RC_Output_L.reset();
          tbl_Treetops_Output_L.reset();
     
          tbl_Pattern_Output_R.reset();
          tbl_Charge_Output_R.reset();
          tbl_Position_Output_R.reset();
          tbl_RC_Output_R.reset();
          tbl_Treetops_Output_R.reset();
     
          tbl_Pattern_Output_C.reset();
          tbl_Charge_Output_C.reset();
          tbl_Position_Output_C.reset();
          tbl_RC_Output_C.reset();
          tbl_Treetops_Output_C.reset();
     
          tbl_Treetops.reset();
     
          tbl_Vision_Output.reset();
          tbl_Vision_Charge_Output.reset();
     
          tbl_Depths_L.reset();
          tbl_Depths_R.reset();
          
     }
     
     
     
     
     ////==---------------------+
     //==--   OUTPUT FUNCTIONS
     ////==---------------------+
     
     //Outputs the node network.
     void output_Nodes()
     {
          cout << "\n\n " << Name;
          Nodes.output_Nodes();
     }
     
     //Outputs the input table.
     void output_Input_Table()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_C(1);
          
          cout << "\n --===+===--\n";
     }
     void output_Input_Table_I()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_I(1);
          
          cout << "\n --===+===--\n";
     }
     void output_Input_Table_F()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_F(1);
          
          cout << "\n --===+===--\n";
     }
     void output_Input_Table_NR()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_NR(1);
          
          cout << "\n --===+===--\n";
     }
     
     //Outputs the input tables.
     void output_Input_Tables()
     {
          cout << "\n\n ---===+===---";
          cout << "\n " << Name << " Inputs Tables ->" << this;
          tbl_Input.output_C(1);
          
          tbl_Input_Aux_L.output_I(1);
          tbl_Input_Charges_L.output_I(1);
          tbl_Input_Position_L.output_I(1);
          tbl_Input_Depths_L.output_I(1);
          
          tbl_Input_Aux_R.output_I(1);
          tbl_Input_Charges_R.output_I(1);
          tbl_Input_Position_R.output_I(1);
          tbl_Input_Depths_R.output_I(1);
          cout << "\n --===+===--\n";
     }

     
     //Returns the number of nodes in the network.
     long long int get_Node_Count()
     {
          return Nodes.CNID.I;
     }
     
     //Outputs the treetops tree in the node network.
     void output_Treetops()
     {
          Nodes.output_Treetops();
     }
     
     //Outputs the treetops tree in a compressed BP format.
     void output_Treetops_BP()
     {
          Nodes.output_Treetops_BP();
     }
     
     //Outputs the tables of the construct.
     void output_Output_Tables()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output Tables ->" << this;
          tbl_Pattern_Output_L.output_C(2);
          tbl_Charge_Output_L.output_I(2);
          tbl_Position_Output_L.output_I(2);
          tbl_RC_Output_L.output_F(2);
          tbl_Treetops_Output_L.output_I(2);
          tbl_Depths_L.output_I(2);
     
          
          tbl_Pattern_Output_R.output_C(2);
          tbl_Charge_Output_R.output_I(2);
          tbl_Position_Output_R.output_I(2);
          tbl_RC_Output_R.output_F(2);
          tbl_Treetops_Output_R.output_I(2);
          tbl_Depths_R.output_I(2);
     
          
          tbl_Pattern_Output_C.output_C(2);
          tbl_Charge_Output_C.output_I(2);
          tbl_Position_Output_C.output_I(2);
          tbl_RC_Output_C.output_F(2);
          tbl_Treetops_Output_C.output_I(2);
          
          tbl_Treetops.output_I(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_I()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          tbl_Pattern_Output_L.output_I(2);
          tbl_Charge_Output_L.output_I(2);
          tbl_Position_Output_L.output_I(2);
          tbl_RC_Output_L.output_F(2);
          tbl_Treetops_Output_L.output_I(2);
          tbl_Depths_L.output_I(2);
     
          
          tbl_Pattern_Output_R.output_I(2);
          tbl_Charge_Output_R.output_I(2);
          tbl_Position_Output_R.output_I(2);
          tbl_RC_Output_R.output_F(2);
          tbl_Treetops_Output_R.output_I(2);
     
          
          tbl_Pattern_Output_C.output_I(2);
          tbl_Charge_Output_C.output_I(2);
          tbl_Position_Output_C.output_I(2);
          tbl_RC_Output_C.output_F(2);
          tbl_Treetops_Output_C.output_I(2);
          
          tbl_Treetops.output_I(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the tables of the construct with the patterns as int.
     void output_Output_Tables_F()
     {
          cout << "\n\n\t ---===+===---";
          cout << "\n\t " << Name << " Output_Tables_I ->" << this;
          tbl_Pattern_Output_L.output_F(2);
          tbl_Charge_Output_L.output_F(2);
          tbl_Position_Output_L.output_F(2);
          tbl_RC_Output_L.output_F(2);
          tbl_Treetops_Output_L.output_F(2);
          tbl_Depths_L.output_F(2);
     
          
          tbl_Pattern_Output_R.output_F(2);
          tbl_Charge_Output_R.output_F(2);
          tbl_Position_Output_R.output_F(2);
          tbl_RC_Output_R.output_F(2);
          tbl_Treetops_Output_R.output_F(2);
          tbl_Depths_R.output_F(2);
     
          
          tbl_Pattern_Output_C.output_F(2);
          tbl_Charge_Output_C.output_F(2);
          tbl_Position_Output_C.output_F(2);
          tbl_RC_Output_C.output_F(2);
          tbl_Treetops_Output_C.output_F(2);
          
          tbl_Treetops.output_F(2);
          cout << "\n\t ---===+===---\n";
     }
     
     //Outputs the CANs.
     void output_CAN()
     {
          CAN.output_CAN();
     }
          
     //Outputs the CAN state.
     void output_CAN_State()
     {
          CAN.output_CAN_State();
     }
          
     //Outputs the CAN state.
     void output_CAN_RC()
     {
          CAN.output_CAN_RC();
     }
          
     
     //Outputs the settings of the current Construct.
     void output_Settings()
     {
          cout << "\n\n --  Settings For "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          //cout << "\n  setting_Charge_L.................... "; oint(0, 11, setting_Charge_L);
          //cout << "\n  setting_Charge_R.................... "; oint(0, 11, setting_Charge_R);
          //cout << "\n  settings_Tier....................... "; oint(0, 11, settings_Tier);
          //cout << "\n  settings_Tier_Is_Floating........... "; oint(0, 11, settings_Tier_Is_Floating);
          cout << "\n  settings_Tier_Depth................. "; odbl(0, 11, settings_Tier_Depth);
          cout << "\n  settings_Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          cout << "\n  settings_Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          cout << "\n  settings_Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
     }
     
     //Outputs the settings of the current Construct.
     void output_Settings_Full()
     {
          cout << "\n\n --  Settings For "; ostr(0, 13, Name);
          
          //Use in the expanded form.
          cout << "\n  Charge_L.................... "; oint(0, 11, setting_Charge_L);
          cout << "\n  Charge_R.................... "; oint(0, 11, setting_Charge_R);
          cout << "\n  Tier....................... "; oint(0, 11, settings_Tier);
          cout << "\n  Tier_Is_Floating........... "; oint(0, 11, settings_Tier_Is_Floating);
          cout << "\n  Tier_Depth................. "; odbl(0, 11, settings_Tier_Depth);
          cout << "\n  Action_Potential_Threshold. "; odbl(0, 11, settings_Action_Potential_Threshold);
          cout << "\n  Base_Charge................ "; oint(0, 11, settings_Base_Charge);
          cout << "\n  Modifier_Charge............ "; odbl(0, 11, settings_Modifier_Charge);
          cout << "\n  Use_Input_Charges............ "; odbl(0, 11, setting_Use_Input_Charges);
          cout << "\n  Build_Vision............ "; odbl(0, 11, settings_Build_Vision);
          cout << "\n  Bubble_Sort_Output............ "; odbl(0, 11, settings_Bubble_Sort_Output);
          cout << "\n  Input_Charging_Depth............ "; odbl(0, 11, settings_Input_Charging_Depth);
          cout << "\n  Cross_CAN_Charging............ "; odbl(0, 11, settings_Cross_CAN_Charging);
          cout << "\n  Stack_Output_From_Foreign_Treetops............ "; odbl(0, 11, settings_Stack_Output_From_Foreign_Treetops);
          cout << "\n  sauto_Type............ "; odbl(0, 11, sauto_Type);
          cout << "\n  sauto_Use_RC............ "; odbl(0, 11, sauto_Use_RC);
     }
     
     
     
     ////==--------------+
     //==--   INTERFACE
     ////==--------------+
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, string p_Submission)
     {
          tbl_Input.set_String(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, int p_Submission)
     {
          tbl_Input.set_Int(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, double p_Submission)
     {
          tbl_Input.set_Float(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void in(int p_Row, int p_Cell, void * p_Submission)
     {
          tbl_Input.set_NR(p_Row, p_Cell, p_Submission);
     }
     
     
     //==--   Adds the submission to the given cell.
     //Accepts an input to the inputs motherfucker.
     void ina(int p_Row, int p_Cell, int p_Submission)
     {
          tbl_Input.add_Data_Int(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void ina(int p_Row, int p_Cell, double p_Submission)
     {
          tbl_Input.add_Data_Float(p_Row, p_Cell, p_Submission);
     }
     
     //Accepts an input to the inputs motherfucker.
     void ina(int p_Row, int p_Cell, void * p_Submission)
     {
          tbl_Input.add_Data_NR(p_Row, p_Cell, p_Submission);
     }
     
     
     
     
     //Accepts an array to be input into a row.
     void ina_A(int p_Row, int p_Cell, int * p_Submission, int p_Length)
     {
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               ina(p_Row, p_Cell, p_Submission[cou_Index]);
          }
     }
     
     //Accepts an array to be input into a row.
     void ina_A(int p_Row, int p_Cell, double * p_Submission, int p_Length)
     {
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               ina(p_Row, p_Cell, p_Submission[cou_Index]);
          }
     }
     
     //Accepts an array to be input into a row.
     void ina_A(int p_Row, int p_Cell, void ** p_Submission, int p_Length)
     {
          for (int cou_Index=0;cou_Index<p_Length;cou_Index++)
          {
               ina(p_Row, p_Cell, p_Submission[cou_Index]);
          }
     }
     
     
     
     
     ////==---------------------------------+
     //==--   SAVING AND LOADING FUNCTIONS
     ////==---------------------------------+
     
     //Saves the construct.
     void Save(string p_Name = "NO_NAME", string p_Dir = "NT3.Save/")
     {
          if (p_Name == "NO_NAME"){ p_Name = Name; }
          
          //The save file.
          ofstream SF;
          
          string tmp_SF_FN = p_Dir + p_Name;
          
          SF.open(tmp_SF_FN, ios::trunc);
          
          if (!SF.is_open()){ cout << "\n\t SAVE FILE " << p_Name << " CANNOT BE OPENED!!!"; return; }
          
          //-- Read the settings into the file.
          
          //The name of the construct.
          SF << "Name " << Name;
          
          //Charging legs.
          SF << "\nsetting_Charge_L " << setting_Charge_L;
          SF << "\nsetting_Charge_R " << setting_Charge_R;
          
          //The tier to start charging.
          SF << "\nsettings_Tier " << settings_Tier;
          
          //The tier to start charging.
          SF << "\nsettings_Tier_Is_Floating " << settings_Tier_Is_Floating;
          
          //The tier to start charging.
          SF << "\nsettings_Tier_Depth " << settings_Tier_Depth;
          
          //The Action Potential Threshold as percentage of the base charge.
          SF << "\nsettings_Action_Potential_Threshold " << settings_Action_Potential_Threshold;
          
          //The base charge to apply from the CAN.
          SF << "\nsettings_Base_Charge " << settings_Base_Charge;
          
          //The multiple to reduce each recursive charge by.
          SF << "\nsettings_Modifier_Charge " << settings_Modifier_Charge;
          
          //Whether or not to use the input charges for charging.
          SF << "\nsetting_Use_Input_Charges " << setting_Use_Input_Charges;
          
          //Build a vision or do not build a vision.
          SF << "\nsettings_Build_Vision " << settings_Build_Vision;
          
          //Sort the tables by charge.
          SF << "\nsettings_Bubble_Sort_Output " << settings_Bubble_Sort_Output;
          
          //The percentage depth of the input to charge starting at index 0.
          SF << "\nsettings_Input_Charging_Depth " << settings_Input_Charging_Depth;
          
          //Whether or not to stack outputs.
          SF << "\nsettings_Stack_Output_From_Foreign_Treetops " << settings_Stack_Output_From_Foreign_Treetops;
          
          SF << "\nplaceholder_For_Future_Settings " << 0;
          
          SF << "\nplaceholder_For_Future_Settings " << 0;
          
          SF << "\nplaceholder_For_Future_Settings " << 0;
          
          SF << "\nsauto_Type " << sauto_Type;
          
          //Whether or not the RC is being used for the autocalibration.
          SF << "\nsauto_Use_RC " << sauto_Use_RC;
          
          //Now that the settings are read in its time for the node network.
          Nodes.save(&SF);
          
          SF.close();
     }
     
     //Loads a construct.     
     void Load(string p_Name, string p_Dir = "NT3.Save/")
     {
          //The save file.
          ifstream LF;
          
          string tmp_Setting_Name = "";
          string tmp_Setting_Placeholder = "";
          
          string tmp_LF_FN = p_Dir + p_Name;
          
          LF.open(tmp_LF_FN);
          
          if (!LF.is_open()){ cout << "\n\t SAVE FILE " << p_Name << " CANNOT BE OPENED!!!"; system("PAUSE > NULL"); return; }
          
          //-- Read the settings into the file.
          
          //The constructs name.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> Name;
          cout << " " << Name;
          set_Name(Name);
          
          //Charging legs.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_L;
          cout << " " << setting_Charge_L;
          
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Charge_R;
          cout << " " << setting_Charge_R;
          
          //The tier to start charging.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier;
          cout << " " << settings_Tier;
          
          //Floating vrs fixed tier depth
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier_Is_Floating;
          cout << " " << settings_Tier_Is_Floating;
          
          //Floating tier modifier.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Tier_Depth;
          cout << " " << settings_Tier_Depth;
          
          //The Action Potential Threshold as percentage of the base charge.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Action_Potential_Threshold;
          cout << " " << settings_Action_Potential_Threshold;
          
          //The base charge to apply from the CAN.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Base_Charge;
          cout << " " << settings_Base_Charge;
          
          //The multiple to reduce each recursive charge by.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Modifier_Charge;
          cout << " " << settings_Modifier_Charge;
          
          //Whether or not to use the input charges for charging.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> setting_Use_Input_Charges;
          cout << " " << setting_Use_Input_Charges;
          
          //Build a vision or do not build a vision.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Build_Vision;
          cout << " " << settings_Build_Vision;
          
          //Sort the tables by charge.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Bubble_Sort_Output;
          cout << " " << settings_Bubble_Sort_Output;
          
          //settings_Input_Charging_Depth
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Input_Charging_Depth;
          cout << " " << settings_Input_Charging_Depth;
          
          //Whether or not to stack outputs when discharging foreign nodes.
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> settings_Stack_Output_From_Foreign_Treetops;
          cout << " " << settings_Stack_Output_From_Foreign_Treetops;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> tmp_Setting_Placeholder;
          cout << " " << tmp_Setting_Placeholder;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> tmp_Setting_Placeholder;
          cout << " " << tmp_Setting_Placeholder;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> tmp_Setting_Placeholder;
          cout << " " << tmp_Setting_Placeholder;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> sauto_Type;
          cout << " " << sauto_Type;
          
          //Placeholder For Future Settings
          LF >> tmp_Setting_Name;
          cout << "\n" << tmp_Setting_Name;
          LF >> sauto_Use_RC;
          cout << " " << sauto_Use_RC;
          
          //Now that the settings are read in its time for the node network.
          Nodes.load(&LF);
          
          LF.close();
     }
     
};