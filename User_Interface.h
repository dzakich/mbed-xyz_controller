#include "mbed.h"
#include "GLOBALS.h"
#include "XYZ_POSITION.h"

void TimerISR()
{
   UI_Clk = 1;
}

//enum UI_States{INIT,UI_Main_Menu,UI_Automated,UI_Manual,UI_Parameters,UI_Reset,UI_Check_Param,UI_Change_Param,UI_Start_Manual_Guidance,UI_Update_manual_Direction, UI_Set_Home_Position,UI_Return_Coordinates,UI_Go_to_Coordinate,UI_Raster,UI_Get_Coordinate,UI_Set_Home,UI_Enter_Coordinate,UI_Ready,UI_Execution,UI_Go_Home_Coordinate_Abort,UI_Check_Raster_Params,UI_Set_Raster_Params,UI_START_Raster,UI_Go_Home_Raster_Abort,UI_Raster_Complete} UI_State;

void User_Interface()
 {
 
   switch(UI_State)
    {                     // This Switch Defines Transition
      case INIT:
         UI_State = UI_Main_Menu;
         break;
         
         case UI_Main_Menu: 
         if (user_menu_input == 1)
        {
            UI_State = UI_Parameters;
         }
         else if (user_menu_input == 3)
          {
            UI_State = UI_Automated;
         }
         else if (user_menu_input == 2)
          {
            UI_State = UI_Manual;
         }
         else if ((user_menu_input != 1) && (user_menu_input != 2) && (user_menu_input != 3) && (user_menu_input != 4)) 
         {
            UI_State = UI_Main_Menu;
         }
         else if (user_menu_input == 4)
          {
            UI_State = UI_Reset;
         }
         break;
         
      case UI_Automated: 
         if (user_menu_input == 3)
          {
            UI_State = UI_Main_Menu;
         }
         else if ((user_menu_input != 1) && (user_menu_input != 2) && (user_menu_input != 3))
         {
            UI_State = UI_Automated;
         }
         else if (user_menu_input == 1)
          {
            UI_State = UI_Raster;
         }
         else if (user_menu_input == 2)
          {
            UI_State = UI_Go_to_Coordinate;
         }
         break;
         
      case UI_Manual: 
         if (user_menu_input == 1) 
         {
            UI_State = UI_Update_Manual_Direction;
            // Activates Step Clk On this transition
            //X_Rail.Step_Clock(user_X_Step_CLOCK_DIR);
            //Y_Rail.Step_Clock(user_Y_Step_CLOCK_DIR);
            //Z_Rail.Step_Clock(user_Z_Step_CLOCK_DIR);
            
            pc.printf("============== Manual Guidance is ACTIVE ============\n\n");
            pc.printf("============== Please use Joysticks =================\n");
            
         }
         else if (user_menu_input == 2)
          {
            UI_State = UI_Main_Menu;
         }
         else if ((user_menu_input != 1) && (user_menu_input != 2))
          {
            UI_State = UI_Manual;
         }
         
         break;
         
         
         case UI_Update_Manual_Direction:
         
         if( Manual_Latency_Counter <= user_Manual_Latency )
         {
             UI_State = UI_Update_Manual_Direction;
             
             //Update direction for Step_Clock() only if direction has changed
             
             if(new_user_X_Step_CLOCK_DIR != user_X_Step_CLOCK_DIR)
             {
                 X_Rail.Step_Clock(new_user_X_Step_CLOCK_DIR);
                 
                 user_X_Step_CLOCK_DIR = new_user_X_Step_CLOCK_DIR;
             }
             
             if(new_user_Y_Step_CLOCK_DIR != user_Y_Step_CLOCK_DIR)
             {
                 Y_Rail.Step_Clock(new_user_Y_Step_CLOCK_DIR);
                 user_Y_Step_CLOCK_DIR = new_user_Y_Step_CLOCK_DIR;
             }
             
             if(new_user_Z_Step_CLOCK_DIR != user_Z_Step_CLOCK_DIR)
             {
                 Z_Rail.Step_Clock(new_user_Z_Step_CLOCK_DIR);
                 user_Z_Step_CLOCK_DIR = new_user_Z_Step_CLOCK_DIR;
             }
             
             
         }
         
         if( Manual_Latency_Counter > user_Manual_Latency )
         { 
             UI_State = UI_Start_Manual_Guidance;
         }
         
         break;
         
         
      case UI_Parameters: 
         if (user_menu_input == 1)
          {
            UI_State = UI_Check_Param;
         }
         else if (user_menu_input == 2) 
         {
            UI_State = UI_Change_Param;
         }
         else if ((user_menu_input != 1) && (user_menu_input != 2) && (user_menu_input != 3))
          {
            UI_State = UI_Parameters;
         }
         else if (user_menu_input == 3)
         {
            UI_State = UI_Main_Menu;
         }
         break;
         
      case UI_Reset: 
         if (1) 
         {
            UI_State = UI_Main_Menu;
         }
         break;
         
      case UI_Check_Param: 
         if (user_menu_input == 1)
         {
            UI_State = UI_Parameters;
         }
         else if (user_menu_input != 1)
         {
            UI_State = UI_Check_Param;
         }
         break;
         
      case UI_Change_Param: 
         if (USER_INPUT_ACCEPTED != 1)
         {
            UI_State = UI_Change_Param;
         }
         else if (USER_INPUT_ACCEPTED == 1)
         {
            UI_State = UI_Parameters;
         }
         break;
         
      case UI_Start_Manual_Guidance: 
         if ((user_menu_input != 1) && (user_menu_input != 2) && (user_menu_input != 3)&& (user_menu_input != 4))
          {
            UI_State = UI_Start_Manual_Guidance;
         }
         else if (user_menu_input == 4)
          {
            UI_State = UI_Manual;
            // Deactivates step clk on this transition
            
            X_Rail.Move(FWD,0);
            Y_Rail.Move(FWD,0);
            Z_Rail.Move(FWD,0);
            
         }
         else if (user_menu_input == 1)
         {
            UI_State = UI_Return_Coordinates;
         }
         else if (user_menu_input == 2)
         {
            UI_State = UI_Set_Home_Position;
         }
         
         else if (user_menu_input == 3) 
         {
            UI_State = UI_Update_Manual_Direction;
            Manual_Latency_Counter = 0;                   //Reset Latency Counter
            
            pc.printf("============== Manual Guidance is ACTIVE ============\n\n");
            pc.printf("============== Please use Joysticks =================\n");
         }
         
         break;
         
      case UI_Set_Home_Position: 
         if (user_menu_input == 1)
          {
            UI_State = UI_Start_Manual_Guidance;
         }
         else if (user_menu_input != 1)
          {
            UI_State = UI_Set_Home_Position;
         }
         break;
         
      case UI_Return_Coordinates: 
         if (user_menu_input == 1)
         {
            UI_State = UI_Start_Manual_Guidance;
         }
         else if (user_menu_input != 1)
         {
            UI_State = UI_Return_Coordinates;
         }
         break;
         
      case UI_Go_to_Coordinate: 
         if (user_menu_input == 4)
         {
            UI_State = UI_Automated;
         }
         else if ((user_menu_input != 1) && (user_menu_input != 2) && (user_menu_input != 3) && (user_menu_input != 4))
         {
            UI_State = UI_Go_to_Coordinate;
         }
         else if (user_menu_input == 3)
         {
            UI_State = UI_Get_Coordinate;
         }
         else if (user_menu_input == 2)
         {
            UI_State = UI_Set_Home;
         }
         else if (user_menu_input == 1)
         {
            UI_State = UI_Enter_Coordinate;
         }
         break;
         
      case UI_Raster: 
         if ((user_menu_input != 1) && (user_menu_input != 2) && (user_menu_input != 3) && (user_menu_input != 4))
         {
            UI_State = UI_Raster;
         }
         else if (user_menu_input == 4)
         {
            UI_State = UI_Automated;
         }
         else if (user_menu_input == 3)
         {
            //UI_State = UI_START_Raster;
            UI_State = UI_is_MATLAB_Rdy;

            //Raster(Scans_per_Line(user_y_N_samples,user_y_length,user_division),Lines_per_Scan(user_z_N_samples,user_y_length,user_division,Aspect_Ratio_Length,Aspect_Ratio_Height),user_dwell_time,user_scan_option);

         }
         else if (user_menu_input == 2)
         {
            UI_State = UI_Set_Raster_Params;
         }
         else if (user_menu_input == 1)
         {
            UI_State = UI_Check_Raster_Params;
         }
         break;
         
         case UI_is_MATLAB_Rdy:
         
            if(is_MATLAB_RDY  == 0xEE)
            {
                UI_State = UI_START_Raster;
                Raster(Scans_per_Line(user_y_N_samples,user_y_length,user_division),Lines_per_Scan(user_z_N_samples,user_y_length,user_division,Aspect_Ratio_Length,Aspect_Ratio_Height),user_dwell_time,user_scan_option);
            }
            
            else if(is_MATLAB_RDY != 0xEE)
            {
                UI_State = UI_is_MATLAB_Rdy;
            }
            
         break;
         
      case UI_Get_Coordinate: 
         if (user_menu_input == 1)
         {
            UI_State = UI_Go_to_Coordinate;
         }
         else if (user_menu_input != 1)
         {
            UI_State = UI_Get_Coordinate;
         }
         break;
         
      case UI_Set_Home: 
         if (user_menu_input == 1)
         {
            UI_State = UI_Go_to_Coordinate;
         }
         else if (user_menu_input != 1)
         {
            UI_State = UI_Set_Home;
         }
         break;
         
      case UI_Enter_Coordinate: 
         if (USER_INPUT_ACCEPTED != 1)
          {
            UI_State = UI_Enter_Coordinate;
         }
         else if (USER_INPUT_ACCEPTED == 1)
         {
            UI_State = UI_Ready;
         }
         break;
         
      case UI_Ready: 
         if (user_menu_input == 2)
         {
            UI_State = UI_Go_to_Coordinate;
         }
         else if ((user_menu_input != 1) && (user_menu_input != 2))
         {
            UI_State = UI_Ready;
         }
         else if (user_menu_input == 1)
         {
            UI_State = UI_Execution;
            
            goTo_Coordinate(new_X_goto_coordinate,new_Y_goto_coordinate,new_Z_goto_coordinate);
            ///pc.printf("Please wait. Execution in progress...\r");
            
         }
         break;
         
      case UI_Execution: 
      
         if ((ABORT != 1) && ( X_Rail.isBusy() || Y_Rail.isBusy() || Z_Rail.isBusy() ))
         {
            UI_State = UI_Execution;
            
         }
         else if (ABORT == 1)
         {
            UI_State = UI_Go_Home_Coordinate_Abort;
            
         }
         else if ((ABORT != 1) && !( X_Rail.isBusy() || Y_Rail.isBusy() || Z_Rail.isBusy() ))
         {
            UI_State = UI_Go_to_Coordinate;
           
         }
         break;
         
      case UI_Go_Home_Coordinate_Abort: 
         if (ABORT == 1)
         {
            UI_State = UI_Go_Home_Coordinate_Abort;
         }
         else if (ABORT != 1)
         {
            UI_State = UI_Go_Home_Coordinate_Abort_execution;
            
            X_Rail.goHome();
            Y_Rail.goHome();
            Z_Rail.goHome();
         }
         break;
        //============================================== 
          case UI_Go_Home_Coordinate_Abort_execution: 
          
          if ( ( X_Rail.isBusy() || Y_Rail.isBusy() || Z_Rail.isBusy() ) && ABORT != 1 )
          {
              UI_State = UI_Go_Home_Coordinate_Abort_execution;
          }
          else if (!( X_Rail.isBusy() || Y_Rail.isBusy() || Z_Rail.isBusy() ) && ABORT != 1 )
         {
            UI_State = UI_Go_to_Coordinate;
         }
           else if (ABORT == 1)
         {
            UI_State = UI_Go_Home_Coordinate_Abort;
         }
      
         break;
     //=======================================================   
      case UI_Check_Raster_Params: 
         if (user_menu_input == 1)
         {
            UI_State = UI_Raster;
         }
         else if (user_menu_input != 1)
         {
            UI_State = UI_Check_Raster_Params;
         }
         break;
         
      case UI_Set_Raster_Params: 
         if (USER_INPUT_ACCEPTED != 1)
         {
            UI_State = UI_Set_Raster_Params;
         }
         else if (USER_INPUT_ACCEPTED == 1)
         {
            UI_State = UI_Raster;
         }
         break;
         
 
      case UI_START_Raster: 
         if ( ( ( X_Rail.isBusy() || Y_Rail.isBusy()  || Z_Rail.isBusy() ) == 1 ) && (ABORT != 1) && (RASTER_COMPLETE != 1) )
         {
            UI_State = UI_START_Raster;
         }
         else if (ABORT == 1)
         {
            UI_State = UI_Go_Home_Raster_Abort;
            
            X_Rail.goHome();
            Y_Rail.goHome();
            Z_Rail.goHome();
         }
         else if ((RASTER_COMPLETE == 1) && (ABORT != 1) && ( (X_Rail.isBusy() == 0) && (Y_Rail.isBusy() == 0) && (Z_Rail.isBusy() == 0) ))
         {
            UI_State = UI_Raster_Complete;
         }
         break;
         
      case UI_Go_Home_Raster_Abort: 
         if (ABORT == 1)
         {
            UI_State = UI_Go_Home_Raster_Abort;
         }
         else if (ABORT != 1)
         {
             
             UI_State = UI_Raster_Abort_Home_execution;
          }
         break;
         
          //===================================================
      case UI_Raster_Abort_Home_execution:
        if( ( X_Rail.isBusy() || Y_Rail.isBusy() || Z_Rail.isBusy() ) && ABORT != 1 )
        {
            UI_State = UI_Raster_Abort_Home_execution;
        }
        
        else if (!( X_Rail.isBusy() || Y_Rail.isBusy() || Z_Rail.isBusy() ) && ABORT != 1)
         {
            UI_State = UI_Raster;
         }
         else if (ABORT == 1)
         {
            UI_State = UI_Go_Home_Raster_Abort;
         }
        
        break;
         //===================================================
         
      case UI_Raster_Complete: 
         if (user_menu_input != 1)
         {
            UI_State = UI_Raster_Complete;
         }
         else if (user_menu_input == 1)
         {
            UI_State = UI_Raster;
         }
         break;
      default:
         UI_State = UI_Main_Menu;
         break;
   } // Transitions

   switch(UI_State)                                                 // State actions
    {                        
      case UI_Main_Menu:
         pc.printf("============MAIN MENU===============\n\n");
         pc.printf("Please select from the following and press Enter\n");
         pc.printf("1. Parameters\n");
         pc.printf("2. Manual Motion\n");
         pc.printf("3. Automated Motion\n");
         pc.printf("4. Reset Device\n");
         pc.scanf("%d",&user_menu_input);
         pc.printf("%d",user_menu_input);
         
         break;
         
      case UI_Automated:
         pc.printf("==============Automated Scan============\n\n");
         pc.printf("1. Raster Scan\n");
         pc.printf("2. Go to a coordinate\n");
         pc.printf("3. Back to Main Menu\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Manual:
         
         pc.printf("==============Manual Scan============\n\n");
         pc.printf("1. Start Manual Guidance\n");
         pc.printf("2. Return to Main Menu\n");
         pc.scanf("%d",&user_menu_input);
        
         break;
      case UI_Parameters:
         
          pc.printf("==============Parameters============\n\n");
          pc.printf("1. Check Parameters\n");
          pc.printf("2. Change Parameters\n");
          pc.printf("3. Return to Main Menu\n");
          pc.scanf("%d",&user_menu_input);
       
         break;
      case UI_Reset:
         pc.printf("==============The Device will Reset...============\n\n");
         break;
         
      case UI_Check_Param:
         
         //outputs all available Parameters
         pc.printf("Microstepping Mode: %d",user_division);
         pc.printf("\n");
         pc.printf("Samples per Line: %d",user_y_N_samples);
         pc.printf("\n");
         pc.printf("Lines per Scan: %d",user_z_N_samples);
         pc.printf("\n");
         pc.printf("Scan Length: %d",user_y_length);
         pc.printf("\n");
         pc.printf("Aspect Ratio Length: %f",Aspect_Ratio_Length);
         pc.printf("\n");
         pc.printf("Aspect Ratio Height: %f",Aspect_Ratio_Height);
         pc.printf("\n");
         pc.printf("Aspect Ratio : %f",(Aspect_Ratio_Length / Aspect_Ratio_Height));
         pc.printf("\n");
         pc.printf("Dwell Time: %f",user_dwell_time);
         pc.printf("\n");
         pc.printf("Raster Scan Option # [0-3]: %d",user_scan_option);
         pc.printf("\n");
         pc.printf("Please press 1 to continue...\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Change_Param:
         /*
         
         Waits user input to change available parameters
         */
         pc.printf("Enter Microstepping Mode: \n");
         pc.scanf("%d",&user_division);
         //===================================================
              X_Rail.setMicroSteps(user_division);
              Y_Rail.setMicroSteps(user_division);
              Z_Rail.setMicroSteps(user_division);
         //===================================================
         pc.printf("%d\n",user_division);
         
         pc.printf("Enter Samples per Line: \n");
         pc.scanf("%d",&user_y_N_samples);
         pc.printf("%d\n",user_y_N_samples);
         
         pc.printf("Enter Lines per Scan: \n");
         pc.scanf("%d",&user_z_N_samples);
         pc.printf("%d\n",user_z_N_samples);
         
         pc.printf("Enter Scan Area Length [in integer micrometers]: \n");
         pc.scanf("%d",&user_y_length);
         pc.printf("%d\n",user_y_length);
         
         pc.printf("Enter Aspect Ratio Length: \n");
         pc.scanf("%f",&Aspect_Ratio_Length);
         pc.printf("%f\n",Aspect_Ratio_Length);
         pc.printf("Enter Aspect Ratio Height: \n");
         pc.scanf("%f",&Aspect_Ratio_Height);
         pc.printf("%f\n",Aspect_Ratio_Height);
         
         pc.printf("Enter Dwell Time [in seconds]: \n");
         pc.scanf("%f",&user_dwell_time);
         pc.printf("%f\n",user_dwell_time);
         
         pc.printf("Enter Raster Scan Option [0-3]: \n");
         pc.scanf("%d",&user_scan_option);
         pc.printf("%d\n",user_scan_option);
         
         pc.printf("Thank you. All Parameters are stored. \n");
         
 
         break;
         
         
         case UI_Update_Manual_Direction:
        
        
        //Keep receiving DIR byte from Mbuino joystics and setting the globals for Step_Clock()
             
        Manual_Direction_Received = Mbuino.getc();
                       
        user_Manual_Stop_Run =  (Manual_Direction_Received & Mask_Stop_Run) >> 3;
        
        if(user_Manual_Stop_Run == 0)   //if the joystick is in neutral, keep incrementing latency timer
        {
            Manual_Latency_Counter ++;
        }
        
        if(user_Manual_Stop_Run != 0)  //if the joystick has moved, reset the  latency timer
        {
            Manual_Latency_Counter = 0;
        }
    
        new_user_X_Step_CLOCK_DIR = (Manual_Direction_Received & Mask_X) >> 2;

        new_user_Y_Step_CLOCK_DIR = (Manual_Direction_Received & Mask_Y) >> 1;

        new_user_Z_Step_CLOCK_DIR = (Manual_Direction_Received & Mask_Z);
   
         break;
         
      case UI_Start_Manual_Guidance:
        
        
          pc.printf("============== Manual Guidance is PAUSED. Enter (3) to Resume ============\n\n");
         
          pc.printf("1. Return Coordinates\n");
          pc.printf("2. Set Home Position\n");
          pc.printf("3. Resume Manual Guidance\n");
          pc.printf("4. Return to Manual Menu\n");
          
          
          pc.scanf("%d",&user_menu_input);

         
         break;
         
      case UI_Set_Home_Position:
         /*
         Sets home position based on current position using: resetPosition()
         */
         X_Rail.resetPosition();
         Y_Rail.resetPosition();
         Z_Rail.resetPosition();
        
         pc.printf("New HOME Position is set\n");
         pc.printf("Press 1 to continue...\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Return_Coordinates:
         /*
         Returns Current Position Using getPos()
         */
         pc.printf("The current coordinates is:\n");
         pc.printf("X: %ld\n",Position_Steps_to_Micrometers(X_Rail.getPos()));
         pc.printf("Y: %ld\n",Position_Steps_to_Micrometers(Y_Rail.getPos()));
         pc.printf("Z: %ld\n",Position_Steps_to_Micrometers(Z_Rail.getPos()));
         pc.printf("\n");
         pc.printf("Press 1 to continue...\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
         
      case UI_Go_to_Coordinate:
         pc.printf("==============GO TO A COORDINATE============\n\n");
         pc.printf("1. Enter the coordinate\n");
         pc.printf("2. Set HOME\n");
         pc.printf("3. Current Coordinate\n");
         pc.printf("4. Back to Automated Menu\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Raster:
         pc.printf("==============RASTERING============\n\n");
         pc.printf("1. Check Raster Parameters\n");
         pc.printf("2. Set Raster Parameters\n");
         pc.printf("3. START Raster\n");
         pc.printf("4. Return to Automated Menu\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Get_Coordinate:
         /* Returns coordinates by calling getPos() and translating using steps to micrometers()
         */
         pc.printf("The current coordinates is:\n");
         pc.printf("X: %ld\n",Position_Steps_to_Micrometers(X_Rail.getPos()));
         pc.printf("Y: %ld\n",Position_Steps_to_Micrometers(Y_Rail.getPos()));
         pc.printf("Z: %ld\n",Position_Steps_to_Micrometers(Z_Rail.getPos()));
         pc.printf("\n");
         pc.printf("Press 1 to continue...\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Set_Home:
         /* Sets home by calling resetPos()
         */
         
         X_Rail.resetPosition();
         Y_Rail.resetPosition();
         Z_Rail.resetPosition();
         
         pc.printf("New HOME position is SET\n\n");
     
         
         pc.printf("Press 1 to continue...\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
      case UI_Enter_Coordinate:
         /*
         Prompts user for the coordinates, accepts the coordinates
         */
         pc.printf("X Coordinate: \n");
         pc.scanf("%d",&new_X_goto_coordinate);
         pc.printf("%d\n",new_X_goto_coordinate);
         pc.printf("Y Coordinate: \n");
         pc.scanf("%d",&new_Y_goto_coordinate);
         pc.printf("%d\n",new_Y_goto_coordinate);
         pc.printf("Z Coordinate: \n");
         pc.scanf("%d",&new_Z_goto_coordinate);
         pc.printf("%d\n",new_Z_goto_coordinate);
         
         
         pc.printf("Coordinates Accepted\n");
         break;
         
      case UI_Ready:
         pc.printf("The GoTo is READY to START:\n\n");
         pc.printf("1. START Execution\n"); 
         pc.printf("2. ABORT to Coordinate Menu\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
         
      case UI_Execution:
         pc.printf("Please wait.Execution in Progress....\r");
         
         break;
         
      case UI_Go_Home_Coordinate_Abort:
      
         /* executes goHome() function and 
         displays "Return Home in progress...." 
         while in motion
         */
         pc.printf("Execution is ABORTED due to a FATAL ERROR\n");
         pc.printf("Returning to HOME\n");
         
         break;
      case UI_Check_Raster_Params:
         /*
         Return Raster Parameters: y_N_Samples z_N_Samples y_length Aspect Ratio dwell time
        */
         pc.printf("Microstepping Mode: %d",user_division);
         pc.printf("\n");
         pc.printf("Samples per Line: %d",user_y_N_samples);
         pc.printf("\n");
         pc.printf("Lines per Scan: %d",user_z_N_samples);
         pc.printf("\n");
         pc.printf("Scan Length: %d",user_y_length);
         pc.printf("\n");
         pc.printf("Aspect Ratio Length: %f",Aspect_Ratio_Length);
         pc.printf("\n");
         pc.printf("Aspect Ratio Height: %f",Aspect_Ratio_Height);
         pc.printf("\n");
         pc.printf("Aspect Ratio : %f",(Aspect_Ratio_Length / Aspect_Ratio_Height));
         pc.printf("\n");
         pc.printf("Dwell Time: %f",user_dwell_time);
         pc.printf("\n");
         pc.printf("Raster Scan Option # [0-3]: %d",user_scan_option);
         pc.printf("\n");
         pc.printf("Please press 1 to continue...\n");
         pc.scanf("%d",&user_menu_input);
         
         break;
         
      case UI_Set_Raster_Params:
      /* Prompts user to set the raster parameters
      */
         pc.printf("Enter Microstepping Mode: \n");
         pc.scanf("%d",&user_division);
          //===================================================
            //X_Rail.setMicroSteps(user_division);
              Y_Rail.setMicroSteps(user_division);
              Z_Rail.setMicroSteps(user_division);
         //===================================================
         pc.printf("%d\n",user_division);
         
         pc.printf("Enter Samples per Line: \n");
         pc.scanf("%d",&user_y_N_samples);
         pc.printf("%d\n",user_y_N_samples);
         
         pc.printf("Enter Lines per Scan: \n");
         pc.scanf("%d",&user_z_N_samples);
         pc.printf("%d\n",user_z_N_samples);
         
         pc.printf("Enter Scan Area Length [in integer micrometers]: \n");
         pc.scanf("%d",&user_y_length);
         pc.printf("%d\n",user_y_length);
         
         pc.printf("Enter Aspect Ratio Length: \n");
         pc.scanf("%f",&Aspect_Ratio_Length);
         pc.printf("%f\n",Aspect_Ratio_Length);
         pc.printf("Enter Aspect Ratio Height: \n");
         pc.scanf("%f",&Aspect_Ratio_Height);
         pc.printf("%f\n",Aspect_Ratio_Height);
         
         pc.printf("Enter Dwell Time [in seconds]: \n");
         pc.scanf("%f",&user_dwell_time);
         pc.printf("%f",user_dwell_time);
         
         pc.printf("Enter Raster Scan Option [0-3]: \n");
         pc.scanf("%d",&user_scan_option);
         pc.printf("%f",user_scan_option);
         
         pc.printf("Thank you. All Parameters are stored. Raster can Begin \n");
         break;
         
      case UI_START_Raster:
         pc.printf("Rastering in Progress... \n");
         break;
         
         
                 
      case UI_is_MATLAB_Rdy:
      
         is_MATLAB_RDY = pc.getc();
         
         break;
         
      case UI_Go_Home_Raster_Abort:
         /*
         Return to HOME while displaying 
         "In Progress...."
         */
         pc.printf("Execution is ABORTED due to a FATAL ERROR\n");
         pc.printf("Returning to HOME\n");
         
         break;
         
      case UI_Raster_Abort_Home_execution:
      
         pc.printf("Execution in Progress....\n");
         
         break;
         
        case  UI_Go_Home_Coordinate_Abort_execution:
        
         pc.printf("Execution in Progress....\n");
         
         break;
         
      case UI_Raster_Complete:
         
         pc.printf("Raster is COMPLETE!\n");
         pc.printf("Please enter 1 to continue...\n");
         pc.scanf ("%d",&user_menu_input);
        
         break;
      default: 
        pc.printf("Execution Failed");// ADD default behaviour below
      break;
   } // State actions

}
