#ifndef XYZ_POS
#define XYZ_POS

#include "L6470_REGList.h"
#include "L6470_CONTROL_LIB.h"
#include "GLOBALS.h"

void Stage_Initialization()
{   

   X_Rail.Init();
   Y_Rail.Init();
   Z_Rail.Init();

   X_Rail.setAcc(x_acceleration);
   Y_Rail.setAcc(y_acceleration);
   Z_Rail.setAcc(z_acceleration);
   
   X_Rail.setDec(x_deceleration);
   Y_Rail.setDec(y_deceleration);
   Z_Rail.setDec(z_deceleration);
   
   X_Rail.setMaxSpeed(x_max_speed);
   Y_Rail.setMaxSpeed(y_max_speed);
   Z_Rail.setMaxSpeed(z_max_speed);
   
   X_Rail.setMinSpeed(x_min_speed);
   Y_Rail.setMinSpeed(y_min_speed);
   Z_Rail.setMinSpeed(z_min_speed);
   
   X_Rail.setThresholdSpeed(x_ThresholdSpeed);
   Y_Rail.setThresholdSpeed(y_ThresholdSpeed);
   Z_Rail.setThresholdSpeed(z_ThresholdSpeed);
   
   X_Rail.setOverCurrent(x_OverCurrent);
   Y_Rail.setOverCurrent(y_OverCurrent);
   Z_Rail.setOverCurrent(z_OverCurrent);
   
   X_Rail.setStallCurrent(x_StallCurrent);
   Y_Rail.setStallCurrent(y_StallCurrent);
   Z_Rail.setStallCurrent(z_StallCurrent); 
   
   X_Rail.setMicroSteps(user_division);
   Y_Rail.setMicroSteps(user_division);
   Z_Rail.setMicroSteps(user_division);
    
    
}

long Position_Steps_to_Micrometers(long Position_Steps)   //Function to be used with getPos() to obtain micrometers
{
    long Micrometers = (long)((float)(Position_Steps) * (( 128.0000 / (float)(user_division)) / 16.125984) + 0.5);
    
    return Micrometers;
}


//The function below used for communication with MATLAB during Raster()
void Matlab_Channel()
{
     
     
     V_Read = ((unsigned int) ( VOLTAGE_SCALER * V_Reading.read() ) & 0xFFFF); // 16-bit normalized Voltage reading in the range 0 to 3300. Assigtned to pin 20
     
     L_VRead = (char)( V_Read );          //Extract Low  and High bytes for transfer and later reconstruction
     H_VRead = (char)( V_Read >> 8 );
    
     //L_X = (char)( Position_Steps_to_Micrometers( X_Rail.getPos() ) & 0xFFFF );
     //H_X = (char)( ( Position_Steps_to_Micrometers( X_Rail.getPos() ) & 0xFFFF ) >> 8 );
     
     
     L_Y = (char)( Position_Steps_to_Micrometers( Y_Rail.getPos() ) & 0xFFFF );
     H_Y = (char)( ( Position_Steps_to_Micrometers( Y_Rail.getPos() ) & 0xFFFF ) >> 8 );
     L_Z = (char)( Position_Steps_to_Micrometers( Z_Rail.getPos() ) & 0xFFFF );
     H_Z = (char)( ( Position_Steps_to_Micrometers( Z_Rail.getPos() ) & 0xFFFF ) >> 8 );
     
     
     pc.putc(L_Y);
     pc.putc(H_Y);
     pc.putc(L_Z);
     pc.putc(H_Z);
     pc.putc(L_VRead);
     pc.putc(H_VRead);
}


//The function below translates desired scan length (in micrometers, integer multiples of 1 micron) into steps that the 
//motor needs to move @ selecter microsperring profile. The function returns an integer value of steps.
//This fuction is operating on Y-axis

    long Scan_Size(int axis_scan_length, int division)   //arguements: axis_scan_length is desired scan length in micrometers, division is desired microstepping profile (2,4,8,16,32,64, 128)
{
    /*
       Determine microstepping constant: 3200 full_steps per 24500 micrometers = 0.125984 full_steps per 1 micrometer which @ 128 microstepping per full step results in 0.125984 * 128 = 16.125984 microsteps per 1 micrometer
    */
    
    // where to set axial microstepping profile to be revised and decided
  
    float steps_per_micrometer = ((float)(division) / 128.0) * 16.125984;
    
    long steps = (long)(steps_per_micrometer * (float)(axis_scan_length) + 0.5);  //to make sure that the rounding in taken place acording to the midpoint i.e 100.3 = 100 and 100.6 = 101
    
    return steps;
     
}

long Aspect_Ratio(int axis_scan_length, int division, float length, float height)   //arguements: axis_scan_length is desired scan length in micrometers, division is desired microstepping profile (2,4,8,16,32,64, 128)
                                                                                    // length defines the ratio on top side of the rectangle, height defines the ratio on the height of the rectangle 
{
    /*
       Determine microstepping constant: 16.125984 microsteps per 1 micrometer
    */
    
    // where to set axial microstepping profile to be revised and decided
    
    float aspect  = length / height ;
    
    int scan_height = (int)(aspect * (float)(axis_scan_length));
  
    float steps_per_micrometer = ((float)(division) / 128.0) * 16.125984;
    
    long steps = (long)(steps_per_micrometer * (float)(scan_height) + 0.5);  //to make sure that the rounding in taken place acording to the midpoint i.e 100.3 = 100 and 100.6 = 101
    
    return steps;
     
}

// THESE ARE 2 MAIN FUCTIONS THAT UTILIZE FUNCTIONS ABOVE. THEY CAN FULLY SPECIFY ANY RECTANGULAR WORKING SURFACE

long Scans_per_Line(long y_N_samples,int axis_scan_length, int division )    // this fuction determines the number of steps per each of the scan intervals the y-axis is broken down to. 
                                                                            // It returns the number of steps per interval. The Dwell_Time() function must operate between scan intervals
{
    
    long Line_Scan_Interval = Scan_Size(axis_scan_length, division) / (y_N_samples - 1);
    
    return Line_Scan_Interval;
}

long Lines_per_Scan(long z_N_samples,int axis_scan_length, int division, float aspect_length, float aspect_height)    // this fuction determines the number of steps per each of the scan intervals the z-axis is broken down to. 
                                                                                                      // It returns the number of steps per interval. Next interval is moven when Y-axis is finished scanning
{
    long Lines_per_Scan_Interval = Aspect_Ratio(axis_scan_length, division, aspect_length, aspect_height) / (z_N_samples - 1);
    
    return Lines_per_Scan_Interval;
}


void Raster(long dline_segment, long dscan_segment, float dwell_time, char scan_option)
{
    /*  Scan Options
                                                                  Y-                       Y+
    0.       ^               1.          ^            2.       <------.           3.    .------>  
             |                           |                            |                 |
             |Z+                         |Z+                          |Z-               |Z-
             |                           |                            |                 |
             .--------->        <--------.                           \/                 \/
                 Y+                 Y-
    */
    
    
/* Taken care in GLOBALS
   
char y_dir;       //Initialize directional variables
char z_dir;

char y_dir_option[4] = {FWD,REV,REV,FWD};
char z_dir_option[4] = {FWD,FWD,REV,REV};

*/

if(scan_option > 3) //Illegal scan option check
{scan_option = 3; }
if(scan_option < 0)
{scan_option = 0; }


y_dir = y_dir_option[scan_option];
z_dir = z_dir_option[scan_option];

Y_Rail.resetPosition();    //Initialize the pre-rostering coordinates to (0,0) {HOME}
Z_Rail.resetPosition();

RASTER_COMPLETE = 0;

//========================Send to MATLAB the size of output data buffer
pc.putc( (char)( user_y_N_samples & 0xFFFF ) );
pc.putc( (char)(( user_y_N_samples & 0xFFFF ) >> 8 ) );

pc.putc( (char)( user_z_N_samples & 0xFFFF ) );
pc.putc( (char)(( user_z_N_samples & 0xFFFF ) >> 8 ) );

//=====================================================================


//Rastering Logic Core

    Z_Rail.Move((z_dir^ 0x01),dscan_segment);    //Initial Offset off HOME
    while((Z_Rail.isBusy())){};

for(int z = 0; z < user_z_N_samples; z++)   //this for-loop controls the Z-axis progression
{
    Z_Rail.Move(z_dir,dscan_segment);
    while((Z_Rail.isBusy())){};
    Matlab_Channel();                               //Transfer coordinate and Analog_read data to MATLAB
    
    wait(dwell_time);
         
    
    for(int y = 0; y < user_y_N_samples - 1; y++)   //this for-loop controls the Y-axis progression
    {
               
        Y_Rail.Move(y_dir,dline_segment);
        while((Y_Rail.isBusy())){};
        Matlab_Channel();                           //Transfer coordinate and Analog_read data to MATLAB
        
        wait(dwell_time);                   //wait for data aquisition before proceeding to a new sample's location
          
    }
    
    
      y_dir = y_dir ^ 0x01;                //Reversve the Y_RAIL direction for the next pass.

}
    
    Y_Rail.goHome();
    //while((Y_Rail.isBusy())){};
    Z_Rail.goHome();
    //while((Z_Rail.isBusy())){};
    
    RASTER_COMPLETE = 1;
       
}

void goTo_Coordinate(long x_coordinate,long y_coordinate, long z_coordinate)
{

//=================X-Axis Cooridinate Selection======================================================================
    if(x_coordinate < 0)                                            // Check for negative x-coordinate
    {
        x_coordinate = x_coordinate * (-1);                         // Make long Positive for um to steps function
        x_coordinate = Scan_Size(x_coordinate,user_division);       // Compute steps of |x-coordinate| using Scan_Size
        x_coordinate =  x_coordinate * (-1);                        // Multiply back the negative
  
    }
    else
    {
        x_coordinate = Scan_Size(x_coordinate,user_division);       // Compute steps of postive x-coordinate using Scan_Size

    }
//=================Y-Axis Cooridinate Selection=======================================================================    
    if(y_coordinate < 0)                                            // Check for negative y_coordinate
    {
        y_coordinate = y_coordinate * (-1);                         // Make long Positive for um to steps function
        y_coordinate = Scan_Size(y_coordinate,user_division);       // Compute steps of |y_coordinate| using Scan_Size
        y_coordinate =  y_coordinate * (-1);                        // Multiply back the negative 
    
    }
    else
    {
        y_coordinate = Scan_Size(y_coordinate,user_division);       // Compute steps of postive y-coordinate using Scan_Size

    }
//=================Z-Axis Cooridinate Selection=======================================================================   
    if(z_coordinate < 0)                                            // Check for negative z-coordinate
    {
        z_coordinate = z_coordinate * (-1);                         // Make long Positive for um to steps function
        z_coordinate = Scan_Size(z_coordinate,user_division);       // Compute steps of |z-coordinate| using Scan_Size
        z_coordinate =  z_coordinate * (-1);                        // Multiply back the negative

    }
    else
    {
        z_coordinate = Scan_Size(z_coordinate,user_division);        // Compute steps of postive z-coordinate using Scan_Size

    }
    
    X_Rail.goTo(x_coordinate);                                  // Apply goTo command for X-axis
    Y_Rail.goTo(y_coordinate);                                  // Apply goTo command for Y-axis
    Z_Rail.goTo(z_coordinate);                                  // Apply goTo command for Z-axis

}
//=================END goTo_Coordinate=================================================================================



#endif