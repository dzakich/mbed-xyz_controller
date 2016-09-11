#include "mbed.h"

#include "GLOBALS.h"
#include "User_Interface.h"
#include "L6470_REGList.h"
#include "L6470_CONTROL_LIB.h"
#include "XYZ_POSITION.h"

int main ()

{

    Stage_Initialization();
    timer.attach_us(&TimerISR,10);
    UI_State = INIT;

   wait(5);
   
   while(1) 
   {
    
      User_Interface();
      while(!UI_Clk);
      UI_Clk = 0;
   }
   

}