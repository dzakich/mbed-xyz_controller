
/*
Register description and address location 

*/

#ifndef L6470_REGLIST_H
#define L6470_REGLIST_H


#define ABS_POS             0x01     /*Function: Current position       Leng: 22 bits   Reset Hex:  0x000000    Reset Value: 0                                          Remarks:   R,WS   */
#define EL_POS              0x02     /*Function: Electrical Position    Leng: 9  bits   Reset Hex:  0x000       Reset Value: 0                                          Remarks:   R,WS   */
#define MARK                0x03     /*Function: Mark Position          Leng: 22 bits   Reset Hex:  0x000000    Reset Value: 0                                          Remarks:   R,WR   */
#define SPEED               0x04     /*Function: Current Speed          Leng: 20 bits   Reset Hex:  0x00000     Reset Value: 0 step/tick (0 steps/sec)                  Remarks:   R only */
#define ACC                 0x05     /*Function: Acceleration           Leng: 12 bits   Reset Hex:  0x08A       Reset Value: 125.5e-12 step/tick^2 (2008 step/s^2)      Remarks:   R,WS   */
#define DEC                 0x06     /*Function: Deceleration           Leng: 12 bits   Reset Hex:  0x08A       Reset Value: 125.5e-12 step/tick^2 (2008 step/s^2)      Remarks:   R,WS   */
#define MAX_SPEED           0x07     /*Function: Maximum Speed          Leng: 10 bits   Reset Hex:  0x041       Reset Value: 248e-6 step/tick (991.8 step/s)            Remarks:   R,WR   */
#define MIN_SPEED           0x08     /*Function: Minimum Speed          Leng: 13 bits   Reset Hex:  0x000       Reset Value: 0 step/tick (0 step/s)                     Remarks:   R,WS   */
#define FS_SPD              0x15     /*Function: Full-Step Speed        Leng: 10 bits   Reset Hex:  0x027       Reset Value: 150.7e-6 step/tick (602.7 step/s)          Remarks:   R,WR   */
#define KVAL_HOLD           0x09     /*Function: Holding Kval           Leng: 8  bits   Reset Hex:  0x29        Reset Value: 0.16 x VS                                  Remarks:   R,WR   */
#define KVAL_RUN            0x0A     /*Function: Constant Speed Kval    Leng: 8  bits   Reset Hex:  0x29        Reset Value: 0.16 x VS                                  Remarks:   R,WR   */
#define KVAL_ACC            0x0B     /*Function: Acc. Starting Kval     Leng: 8  bits   Reset Hex:  0x29        Reset Value: 0.16 x VS                                  Remarks:   R,WR   */
#define KVAL_DEC            0x0C     /*Function: Dec. Starting Kval     Leng: 8  bits   Reset Hex:  0x29        Reset Value: 0.16 x VS                                  Remarks:   R,WR   */
#define INT_SPEED           0x0D     /*Function: Intersect Speed        Leng: 14 bits   Reset Hex:  0x0408      Reset Value: 15.4e-6 step/tick (61.5 step/s)            Remarks:   R,WH   */
#define ST_SLP              0x0E     /*Function: Start Slope            Leng: 8  bits   Reset Hex:  0x19        Reset Value: 0.038% s/step                              Remarks:   R,WH   */
#define FN_SLP_ACC          0x0F     /*Function: Acc. Final Slope       Leng: 8  bits   Reset Hex:  0x29        Reset Value: 0.063% s/step                              Remarks:   R,WH   */
#define FN_SLP_DEC          0x10     /*Function: Dec. Final Slope       Leng: 8  bits   Reset Hex:  0x29        Reset Value: 0.063% s/step                              Remarks:   R,WH   */
#define K_THERM             0x11     /*Function: Thermal Comp. Factor   Leng: 4  bits   Reset Hex:  0x0         Reset Value: 1.0                                        Remarks:   R,WR   */
#define ADC_OUT             0x12     /*Function: ADC Output             Leng: 5  bits   Reset Hex:  XX[*]       Reset Value: -----NA-----                               Remarks:   R only */
#define OCD_TH              0x13     /*Function: OCD Threshold          Leng: 4  bits   Reset Hex:  0x8         Reset Value: 3.38 A                                     Remarks:   R,WR   */   
#define STALL_TH            0x14     /*Function: STALL Threshold        Leng: 7  bits   Reset Hex:  0x40        Reset Value: 2.03 A                                     Remarks:   R,WR   */
#define STEP_MODE           0x16     /*Function: Step Mode              Leng: 8  bits   Reset Hex:  0x7         Reset Value: 128 microsteps                             Remarks:   R,WH   */
#define ALARM_EN            0x17     /*Function: Alarm Enable           Leng: 8  bits   Reset Hex:  0xFF        Reset Value: All alarmas Enabled                        Remarks:   R,WS   */
#define CONFIG              0x18     /*Function: IC Configuration       Leng: 16 bits   Reset Hex:  0x2E88      Reset Value: Internal oscillator, 2 MHz OSCOUT clock,
                                                                                                                             supply voltage compensation disabled, 
                                                                                                                             overcurrent shutdown enabled, 
                                                                                                                             slew rate = 290 V/µs,
                                                                                                                             PWM frequency = 15.6 kHz.                  Remarks:   R,WH   */
#define STATUS              0x19     /*Function: Status                 Leng: 16 bits   Reset Hex:  XXXX[*]    Reset Value: High impedance state, UVLO/Reset flag set  Remarks:   R only  */

 /*  NOTES: [*] : According to Start-Up conditions   
             R  : Readable 
             WH : Writable only when outputs are in High-Z
             WS : Writable only when motor is stopped
             WR : Always writable
             
             
 #To convert SPEED  to steps/s use: [steps/s] = (SPEED x 2^-28)/tick where tick is 250 ns (or 250e-9 sec). Range:     0 <= steps/s <= 15625 with resolution 0.015 steps/sec
 #To convert ACC[DEC] to steps/s^2 use: [steps/s^2] = (ACC[DEC] x 2^-40)/tick^2 where tick is 250 ns. Range :     14.55 <= steps/s^2 <= 59590 with resolutions 14.55 steps/s^2
 #To convert MIN_SPEED to steps/s use: [steps/s] = (MIN_SPEED x 2^-24)/tick.  Range: 0 <= steps/s <= 976.3 with resolution 0.238 steps/sec. Register's MSB Enbales LOW SPEED OPTIMIZATION LSPD_OPT 
 #To convert FS_SPD to step/s use: [steps/s] = 2^18 x(FS_SPD + 0.5)/tick.  Range:  7.63 <= steps/s <= 15625 with resolution 15.25 steps/sec   
 #To convert INT_SPEED to step/s use:  [steps/s] = (INT_SPEED x 2^-26)/tick.  Range:   0 <= steps/s <= 976.5 with resolution 0.0596 step/s
        
 */
 
 
/*====================== The following section contains definitions for bit-masking of various registers. When multiple definitions must be set withing a given register, please use bitwise | (OR) to combine ===========  */
/* Overcurrent Register Masks*/

#define OCD_TH_375mA   0x00   /* Overcurrent detection threshold X mA */
#define OCD_TH_750mA   0x01   
#define OCD_TH_1125mA  0x02   
#define OCD_TH_1500mA  0x03   
#define OCD_TH_1875mA  0x04   
#define OCD_TH_2250mA  0x05  
#define OCD_TH_2625mA  0x06  
#define OCD_TH_3000mA  0x07  
#define OCD_TH_3375mA  0x08   
#define OCD_TH_3750mA  0x09   
#define OCD_TH_4125mA  0x0A   
#define OCD_TH_4500mA  0x0B   
#define OCD_TH_4875mA  0x0C   
#define OCD_TH_5250mA  0x0D   
#define OCD_TH_5625mA  0x0E   
#define OCD_TH_6000mA  0x0F 


/* Step Mode Register  Masks */

#define SYNC_EN   0x80       /* !BUSY/SYNC bit:  forced low during command execution  and  when the SYNC_EN bit is set high, BUSY/SYNC output provides a clock signal according to the SYNC_SEL parameter. */

#define SYNC_SEL_1_2  0x00         /* SYNC output frequency is Full-Step Frequency / 2  */
#define SYNC_SEL_1    0x10         /* SYNC output frequency is Full-Step Frequency      */
#define SYNC_SEL_2    0x20         /* SYNC output frequency is Full-Step Frequency * 2  */
#define SYNC_SEL_4    0x30         /* SYNC output frequency is Full-Step Frequency * 4  */
#define SYNC_SEL_8    0x40         /* SYNC output frequency is Full-Step Frequency * 8  */
#define SYNC_SEL_16    0x50         /* SYNC output frequency is Full-Step Frequency * 16  */
#define SYNC_SEL_32    0x60         /* SYNC output frequency is Full-Step Frequency * 32  */
#define SYNC_SEL_64    0x70         /* SYNC output frequency is Full-Step Frequency * 64  */

#define STEP_SEL_FULL        0x00         /* Full  Step   */
#define STEP_SEL_HALF        0x01         /* Half  Step   */
#define STEP_SEL_1_4         0x02         /* 1/4   Step   */
#define STEP_SEL_1_8         0x03         /* 1/8   Step   */
#define STEP_SEL_1_16        0x04         /* 1/16  Step   */
#define STEP_SEL_1_32        0x05         /* 1/32  Step   */
#define STEP_SEL_1_64        0x06         /* 1/64  Step   */
#define STEP_SEL_1_128       0x07         /* 1/128 Step   */

/*  Alarm Register Masks  */

#define ALARM_EN_OC         0x01   /*Overcurrent  Alarm   */
#define ALARM_EN_TH_SWDWN   0x02   /*Thermal Shutdiwn     */
#define ALARM_EN_TH_WRN     0x04   /*Thermal Warning      */
#define ALARM_EN_UV         0x08   /*Undervoltage         */
#define ALARM_EN_STALL_A    0x10   /*Stall Bridge A       */
#define ALARM_EN_STALL_B    0x20   /*Stall Bridge B       */
#define ALARM_EN_SWTCH      0x40   /*Switcj turn-on event */
#define ALARM_EN_NC         0x80   /*Wrong or non-performable command  */


/* Configuration Register Masks  */

//Oscillator Management

#define CONFIG_INT_16MHZ               0x0000       /*Internal 16MHz, no output         */
#define CONFIG_INT_16MHZ_OSCOUT_2MHZ   0x0008       /*DFLT; internal 16MHz, 2MHz output */
#define CONFIG_INT_16MHZ_OSCOUT_4MHZ   0x0009       /*Internal 16MHz, 4MHz output       */
#define CONFIG_INT_16MHZ_OSCOUT_8MHZ   0x000A       /*Internal 16MHz, 8MHz output       */
#define CONFIG_INT_16MHZ_OSCOUT_16MHZ  0x000B       /*Internal 16MHz, 16MHz output      */

//External Switch Interrupt

#define CONFIG_SW_MODE_HARD       0x0000   /*  Sets External switch to act as HardStop Interrups  */
#define CONFIG_SW_MODE_USER       0x0010   /*  User Disposal                                      */

//Overcurrent Event

#define CONFIG_OC_SD   0x0080     /* Bridges Shut Down  */
#define CONFIG_OC_NSD  0x0000     /* Bridges Not Shut Down */

//Power bridge output slew rate values

#define CONFIG_POW_SR_320Vus   0x0000   /* Output Slew Rate 320 V/us  */
#define CONFIG_POW_SR_75Vus    0x0100   /* Output Slew Rate 75  V/us  */
#define CONFIG_POW_SR_110Vus   0x0200   /* Output Slew Rate 110  V/us  */
#define CONFIG_POW_SR_260Vus   0x0300   /* Output Slew Rate 260  V/us  */

//Motor supply voltage compensation

#define CONFIG_EN_VSCOMP_ON    0x0020   /*Compensation Enabled  */
#define CONFIG_EN_VSCOMP_OFF   0x0000   /*Copensation Disabled */

// Integer multipliers for PWM frequency generation

#define CONFIG_PWM_DEC_0_625           (0x00)<<10
#define CONFIG_PWM_DEC_0_75            (0x01)<<10
#define CONFIG_PWM_DEC_0_875           (0x02)<<10
#define CONFIG_PWM_DEC_1               (0x03)<<10
#define CONFIG_PWM_DEC_1_25            (0x04)<<10
#define CONFIG_PWM_DEC_1_5             (0x05)<<10
#define CONFIG_PWM_DEC_1_75            (0x06)<<10
#define CONFIG_PWM_DEC_2               (0x07)<<10

// Division for the PWM  frequency generation

#define CONFIG_PWM_INT_1               (0x00)<<13
#define CONFIG_PWM_INT_2               (0x01)<<13
#define CONFIG_PWM_INT_3               (0x02)<<13
#define CONFIG_PWM_INT_4               (0x03)<<13
#define CONFIG_PWM_INT_5               (0x04)<<13
#define CONFIG_PWM_INT_6               (0x05)<<13
#define CONFIG_PWM_INT_7               (0x06)<<13

/* Status Register Masks  */

#define STATUS_HiZ          0x0001    /*When 1, Bridges are in High-Impedance mode  */
#define STATUS_BUSY         0x0002    /*When 1, the device has finished performing command execution   */
#define STATUS_SW_F         0x0004    /*When 1, switch is closed; when 0, switch is open               */
#define STATUS_SW_ENV       0x0008    /*When 1, Switch turn-on event has occured   */
#define STATUS_DIR          0x0010    /*When 1, Forward; when 0 , reverse    */
#define STATUS_MOT_STOP     0x0000    /*When 00, Stopped; 01 - acceleration, 10 - deceleration, 11 - constant speed run */
#define STATUS_MOT_ACC      0x0020
#define STATUS_MOT_DEC      0x0040
#define STATUS_MOT_RUN      0x0060
#define STATUS_NOTPERF_CMD  0x0080    /*When 1, command from SPI cannot be performed flag. i.e. writing to read-only register.   */
#define STATUS_WRONG_CMD    0x0100    /*When 1, wrong command is issued   */
#define STATUS_UVLO         0x0200    /*When 0 (active low), undervoltage lockout or reset events (power-up included).  */
#define STATUS_TH_WRN       0x0400    /*When 0, thermal warning is issued   */
#define STATUS_TH_SD        0x0800    /*When 0, thermal shutdown occured    */
#define STATUS_OCD          0x1000    /*When 0, overcurrent detection triggered    */
#define STATUS_STEP_LOSS_A  0x2000    /*When 0, stall is detected on bridge A    */
#define STATUS_STEP_LOSS_B  0x4000    /*When 0, stall is detected on bridge B    */
#define STATUS_SCK_MOD      0x8000    /*When 1, device is operating is step-clock mode. Signal should be provided throughSTCK input. */


/* Command Byte Handler   */

#define NOP                  0x00     /*Nothing                 Used for padding to receive data over MISO     */
#define SET_PARAM            0x00     /*SetParam(PARAM,VALUE)   Writes VALUE in PARAM register    */
#define GET_PARAM            0x20     /*GetParam(PARAM)         Returns the stored value in PARAM register   */
#define RUN                  0x50     /*Run(DIR,SPD)            Sets the target speed and the motor direction */
#define STEP_CLOCK           0x58     /*StepClock(DIR)          Puts the device into Step-clock mode and imposes DIR direction  */
#define MOVE                 0x40     /*Move(DIR,N_STEP)        Makes N_STEP (micro)steps in DIR direction(Not performable when motor is running)  */
#define GOTO                 0x60     /*GoTo(ABS_POS)           Brings motor into ABS_POS position (minimum path) */
#define GOTO_DIR             0x68     /*GoTo_DIR(DIR,ABS_POS)   Brings motor into ABS_POS position forcing DIR direction  */
#define GO_UNTIL             0x82     /*GoUntil(ACT,DIR,SPD)    Performs a motion in DIR direction with speed SPD until SW is closed, the ACT action is executed then a SoftStop takes place   */
#define RELEASE_SW           0x92     /*ReleseSW(ACT, DIR)      Performs a motion in DIR direction at minimum speed until the SW is released (open), the ACT action is executed then a HardStop takes place.  */
#define GO_HOME              0x70     /*GoHome                  Brings the motor into HOME position    */
#define GO_MARK              0x78     /*GoMark                  Brings the motor into MARK position    */
#define RESET_POS            0xD8     /*ResetPos                Resets the ABS_POS register (set HOME position)   */
#define RESET_DEVICE         0xC0     /*ResetDevice             Device is reset to power-up conditions.    */
#define SOFT_STOP            0xB0     /*SoftStop                Stops motor with a deceleration phase     */
#define HARD_STOP            0xB8     /*HardStop                Stops motor immediately          */
#define SOFT_HIZ             0xA0     /*SoftHiZ                 Puts the bridges into high impedance status after a deceleration phase   */
#define HARD_HIZ             0xA8     /*HardHiZ                 Puts the bridges into high impedance status immediately    */
#define GET_STATUS           0xD0     /*GetStatus                Returns the STATUS register value      */

#define FWD                  0x01     /* Forward Diresction   */
#define REV                  0x00     /* Reverse Direction    */

#define ABS_POS_RST          0x00     /* the ABS_POS register is reset. Applies to goUntil() and releaseSW() functions   */
#define ABS_POS_COPY         0x08     /*  the ABS_POS register value is copied into the MARK register. Applies to goUntil() and releaseSW() functions   */

#endif