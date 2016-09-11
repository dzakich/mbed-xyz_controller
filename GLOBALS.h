/*
The following file contains all the GLOBAL definitions and cariables used for control.
*/

#ifndef GLOBALS_H
#define GLOBALS_H

#include "L6470_CONTROL_LIB.h"

Serial pc(USBTX,USBRX);                                       /*Initialise Serial to PC GLOBAL for testing,debuging  and user interface interface*/
Serial Mbuino(NC,p27);
Ticker  timer;
AnalogIn V_Reading(p20);

enum UI_States{INIT,UI_Main_Menu,UI_Automated,UI_Manual,UI_Parameters,UI_Reset,UI_Check_Param,UI_Change_Param,UI_Start_Manual_Guidance,UI_Update_Manual_Direction,UI_Set_Home_Position,UI_Return_Coordinates,UI_Go_to_Coordinate,UI_Raster,UI_Get_Coordinate,UI_Set_Home,UI_Enter_Coordinate,UI_Ready,UI_Execution,UI_Go_Home_Coordinate_Abort,UI_Go_Home_Coordinate_Abort_execution,UI_Raster_Abort_Home_execution,UI_Check_Raster_Params,UI_Set_Raster_Params,UI_is_MATLAB_Rdy,UI_START_Raster,UI_Go_Home_Raster_Abort,UI_Raster_Complete} UI_State;

//============================CS Assignment================================

L6470_IC X_Rail(p17);
L6470_IC Y_Rail(p15);
L6470_IC Z_Rail(p16);

//============================End CS Assignment============================
//-------------------------------------------------------------------------
//============================Timing Variables=============================

float user_dwell_time = 1;


//============================End CS Assignment=============================
//--------------------------------------------------------------------------
//============================Motor Constants===============================

const unsigned long x_KVAL_RUN = 250;
const unsigned long x_KVAL_ACC = 250;
const unsigned long x_KVAL_DEC = 250;
const unsigned long x_KVAL_HOLD = 120;

const unsigned long y_KVAL_RUN = 250;
const unsigned long y_KVAL_ACC = 250;
const unsigned long y_KVAL_DEC = 250;
const unsigned long y_KVAL_HOLD = 120;

const unsigned long z_KVAL_RUN = 250;
const unsigned long z_KVAL_ACC = 250;
const unsigned long z_KVAL_DEC = 250;
const unsigned long z_KVAL_HOLD = 120;

//============================End Motor Constants===========================
//--------------------------------------------------------------------------
//============================Speed Profile Globals=========================

float x_acceleration = 50;
float y_acceleration = 50;
float z_acceleration = 50;


float x_deceleration = 50;
float y_deceleration = 50;
float z_deceleration = 50;


float x_max_speed = 120;
float y_max_speed = 120;
float z_max_speed = 120;


float x_min_speed = 5;
float y_min_speed = 5;
float z_min_speed = 5;

float x_ThresholdSpeed = 350;
float y_ThresholdSpeed = 350;
float z_ThresholdSpeed = 350;

//===========================End Speed Profile Globals======================
//--------------------------------------------------------------------------
//===========================Safety Globals=================================

unsigned int x_OverCurrent = 1500;
unsigned int y_OverCurrent = 1500;
unsigned int z_OverCurrent = 1500;
  
float x_StallCurrent = 3000;
float y_StallCurrent = 3000;
float z_StallCurrent = 3000;

//===========================End Safety Globals=============================
//--------------------------------------------------------------------------
//===========================Positioning Variables==========================
int user_division = 128;

long user_x_N_samples = 5;
long user_y_N_samples = 5;
long user_z_N_samples = 5;

long user_dline_segment = 1;
long user_dscan_segment = 1;
long user_y_length = 1;

char x_dir = FWD;
char y_dir = FWD;     
char z_dir = FWD;

char y_dir_option[4] = {FWD,REV,REV,FWD};
char z_dir_option[4] = {FWD,FWD,REV,REV};

long new_X_goto_coordinate = 0;  //These variables used in Go_To Menu 
long new_Y_goto_coordinate = 0;
long new_Z_goto_coordinate = 0;

char user_scan_option = 0;

float Aspect_Ratio_Height = 1;
float Aspect_Ratio_Length = 1;

char user_Manual_Stop_Run = 1;

char user_X_Step_CLOCK_DIR = FWD;
char user_Y_Step_CLOCK_DIR = FWD;
char user_Z_Step_CLOCK_DIR = FWD;

char new_user_X_Step_CLOCK_DIR = FWD;
char new_user_Y_Step_CLOCK_DIR = FWD;
char new_user_Z_Step_CLOCK_DIR = FWD;

long user_Manual_Latency = 1000;
long Manual_Latency_Counter = 0;

char Mask_Stop_Run = 0x08;
char Mask_X = 0x04;
char Mask_Y = 0x02;
char Mask_Z = 0x01;

char Manual_Direction_Received;

//============================== Voltage Aquisition Block===================

unsigned int V_Read = 0;    //Normalized to integer voltage reading from a sensor (0 - 3.3 V to 0 - 3300)
const float VOLTAGE_SCALER = 1000.0;
char L_X, H_X, L_Y, H_Y, L_Z, H_Z, L_VRead, H_VRead;     //Low-High bytes for 16-bit data transfer to Matlab over serial.
//==========================================================================

//===========================End Positioning Variables======================
//--------------------------------------------------------------------------
//===========================User Interface Variables=======================
unsigned char UI_Clk;
int user_menu_input = 0;

char ABORT = 0;
char RASTER_COMPLETE = 0;
char USER_INPUT_ACCEPTED = 1;
char is_MATLAB_RDY = 0;       //0xEE is ready
//==========================End User Interface VAaiables====================
#endif