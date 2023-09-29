

class c_Keno
{
public:

     int place_Bet(int p_Ticket, int p_Hits)
     {
          if (p_Ticket == 1)
          {
               if (p_Hits == 1){ return 2; }
          }
          if (p_Ticket == 2)
          {
               if (p_Hits == 2){ return 11; }
          }
          if (p_Ticket == 3)
          {
               if (p_Hits == 3){ return 27; }
               if (p_Hits == 2){ return 2; }
          }
          if (p_Ticket == 4)
          {
               if (p_Hits == 4){ return 72; }
               if (p_Hits == 3){ return 5; }
               if (p_Hits == 2){ return 1; }
          }
          if (p_Ticket == 5)
          {
               if (p_Hits == 5){ return 410; }
               if (p_Hits == 4){ return 18; }
               if (p_Hits == 3){ return 2; }
          }
          if (p_Ticket == 6)
          {
               if (p_Hits == 6){ return 1100; }
               if (p_Hits == 5){ return 57; }
               if (p_Hits == 4){ return 7; }
               if (p_Hits == 3){ return 1; }
          }
          if (p_Ticket == 7)
          {
               if (p_Hits == 7){ return 2000; }
               if (p_Hits == 6){ return 100; }
               if (p_Hits == 5){ return 11; }
               if (p_Hits == 4){ return 5; }
               if (p_Hits == 3){ return 1; }
          }
          if (p_Ticket == 8)
          {
               if (p_Hits == 8){ return 10000; }
               if (p_Hits == 7){ return 300; }
               if (p_Hits == 6){ return 50; }
               if (p_Hits == 5){ return 15; }
               if (p_Hits == 4){ return 2; }
          }
          if (p_Ticket == 9)
          {
               if (p_Hits == 9){ return 25000; }
               if (p_Hits == 8){ return 2000; }
               if (p_Hits == 7){ return 100; }
               if (p_Hits == 6){ return 20; }
               if (p_Hits == 5){ return 5; }
               if (p_Hits == 4){ return 2; }
          }
          if (p_Ticket == 10)
          {
               if (p_Hits == 10){ return 100000; }
               if (p_Hits == 9){ return 5000; }
               if (p_Hits == 8){ return 500; }
               if (p_Hits == 7){ return 50; }
               if (p_Hits == 6){ return 10; }
               if (p_Hits == 5){ return 2; }
               if (p_Hits == 0){ return 5; }
          }
          return -1;
     }
}KENO_TICKETS;


