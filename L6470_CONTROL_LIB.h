#ifndef CONTROL_H
#define CONTROL_H

#include "mbed.h"
#include "L6470_REGList.h"

class L6470_IC

{
    public:
    
    L6470_IC(PinName pin);                  /* Contructor for L6470_IC object  that defines Slave Select */
    void Init();                            /* Initialize the SPI perifferal and check for connectivity with L6470 */
    unsigned long RegHandler(char param, unsigned long value);   /*    */ 
    
    
    void resetDevice();
    
    bool isBusy();                          /*  The BUSY flag is low when a constant speed, positioning or motion command is under execution
                                                and is released (high) after the command has been completed. isBusy() is Positive Logic  */
                                  
    void Run(char dir, float speed);        /*  Sets the target speed and the motor direction   */
    void Step_Clock(char dir);              /*  Puts the device into Step-clock mode and imposes DIR direction. Motor must be STOPPED before issuing the command. 
                                                Motion commands (RUN, MOVE, etc) will cause the device to exit step-clocking mode */
    void Move(char dir, long n_steps);      /*  Makes N_STEP (micro)steps in DIR direction (Not performable when motor is running) */
    void goTo(long ABS_pos);                /*  Brings motor into ABS_POS position (minimum path). Similar to Move()  */
    void goTo_DIR(char dir, long ABS_pos);  /*  Brings motor into ABS_POS position forcing DIR direction  */
    void goUntil(char action, char dir, unsigned long speed);  /*Performs a motion in DIR direction with speed SPD until SW is closed, the ACT action is executed then a SoftStop takes place.   */  
    void releaseSW(char action, char dir);  /*  Performs a motion in DIR direction at minimum speed until the SW is released (open), the ACT action is executed then a HardStop takes place.     */
        
    void goHome();                          /*  Brings the motor into HOME position (zero position) via the shortest path. */     
    void goMark();                          /*  Brings the motor into MARK position   */
    
    void resetPosition();                   /*  Resets the ABS_POS register (set HOME position)   */
    void softStop();                        /*  Stops motor with a deceleration phase. When the motor is in high impedance state, a SoftStop command forces the bridges to exit 
                                                from high impedance state; no motion is performed. */
    void hardStop();                        /*  The HardStop command causes an immediate motor stop with infinite deceleration. When the motor is in high impedance state, a HardStop command forces the bridges to exit 
                                                from high impedance state; no motion is performed. */
    void softHiZ();                         /*  The SoftHiZ command disables the power bridges (high impedance state) after a deceleration to zero */
    void hardHiZ();                         /*  The HardHiZ command immediately disables the power bridges (high impedance state) and raises the HiZ flag  */
    
    int getStatus();                        /*  The GetStatus command returns the STATUS register value.The GetStatus command resets the STATUS register warning flags. The command forces 
                                                the system to exit from any error state. The GetStatus command DOES NOT reset the HiZ flag. */
                                                
    void SetLowSpeedOpt(bool enable);       /*  When the motor is driven at a very low speed using a small driving voltage, the resulting 
                                                phase current can be distorted. When enabled, speed profile minimum speed is forced to zero.  */                                           
                                                
    void setMicroSteps(int microSteps);     /*  Enter microsepping mode. Valid entries: 1,2,4,8,16,32,64 and 128 */ 
    void setMaxSpeed(float speed);          /*  Set MaxSpeed on velocity curve */
    void setMinSpeed(float speed);          /*  Set MinSpeed on velocity curve */
    void setAcc(float acceleration);
    void setDec(float deceleration);
    void setOverCurrent(unsigned int mA_current);   /* Set overcurrent threshold value.Enables when the current in any of the Power MOSFETs exceeds a programmed overcurrent */
    void setThresholdSpeed(float thresholdSpeed);   /* Set speed threshold value. When the actual speed exceeds this value, the step mode is automatically 
                                                       switched to full-step two-phase on. Write 0.0 to disable switching and ALWAYS remain in microstepping.   */
    void setStallCurrent(float mA_current);         /* Set register contains the stall detection threshold values. 
                                                       The available range is from 31.25 mA to 4 A with a resolution of 31.25 mA.   */
    
    float getSpeed();                       /*  Obtain the current motor speed, expressed in step/tick   */
    
    void setMark();                         /*  Set MARK register according to current ABS_Position  to use in goMark() fucntion    */
    void setMark(long value);               /*  Set MARK register according to the user   */
    long getPos();                          /*  Get current position   */
    
        
    private:
    
    DigitalOut _pin;
    
    char Transfer(char data);                                   /* Function handles SPI transfer with SS  */
    
    unsigned long Param(unsigned long value, char bit_length);  /* Function handles register data manipulation */
    
    void SetParam(char param, unsigned long value);             /* Write data to a param register  */
    
    unsigned long GetParam(char param);                         /* Read data from a param register  */
    
    long convert(unsigned long val);                            /* Get signed long from 22-bit 2's complement  */
    
    unsigned long SpeedCalc(float Steps_Sec);                   /* Calculates parameter for SPEED register based on desired steps/sec value */
    unsigned long MaxSpeedCalc(float Steps_Sec);                /* Calculates parameter for MAX_SPEED register based on desired steps/sec value */
    unsigned long MinSpeedCalc(float Steps_Sec);                /* Calculates parameter for MIN_SPEED register based on desired steps/sec value  */
    unsigned long FSCalc(float Steps_Sec);                      /* Calculates parameter for FS_SPD register based on desired steps/sec value     */ 
    unsigned long AccCalc(float stepsPerSecSquared);            /* Calculates parameter for ACC register based on desired steps/sec^2 value    */
    unsigned long DecCalc(float stepsPerSecSquared);            /* Calculates parameter for DEC register based on desired steps/sec value      */
    
    
};

#endif