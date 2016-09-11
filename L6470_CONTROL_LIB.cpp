#include "mbed.h"
#include "L6470_CONTROL_LIB.h"
#include "L6470_REGList.h"


SPI  mySPI(p5,p6,p7);                                         /*Initialize SPI on 5 MOSI, 6 MISO, 7 CLK      */
 
L6470_IC::L6470_IC(PinName pin) : _pin(pin)                 /* _pin(pin) means to pass pin to the DigitalOut constructor   */
{
    _pin = 1;                                               /*  default the output to 1   */   
    
}


void L6470_IC::Init()
{
   /*
   This is the generic initialization function to set up the Mbed to
   communicate with the L6470 chip.
   */
    
    _pin = 1;   /*Keep SS High before any Transfer() call  */
    
    resetDevice();  /* Reset L6470 to its defaults       */
    wait_ms(10);
    
    mySPI.format(8,3);
    /*
    Check communications. The CONFIG register should power up to 0x2E88, so we can use that to check the communications.  
    */
    
    /*
    if (GetParam(CONFIG) == 0x2E88)
        {
            pc.printf("%s \n","Communication Established. Device is Ready.");
        }
    else
        {
            pc.printf("%s \n","Communication Error");
        }
    */
    SetParam(KVAL_RUN, 150);   //  KVal = 220/255 * VS  
    SetParam(KVAL_ACC, 150);
    SetParam(KVAL_DEC, 150);
    SetParam(KVAL_HOLD,120);
   

  
    /* 
    CONFIG Register Setup:  
    
    
    PWM frequency divisor = 1
    PWM frequency multiplier = 2 (62.5kHz PWM frequency)
    Slew rate is 260V/us
    Do NOT shut down bridges on overcurrent
    Disable motor voltage compensation
    Hard stop on switch low
    16MHz internal oscillator, nothing on output
    */
    
    SetParam(CONFIG,CONFIG_PWM_INT_1 | CONFIG_PWM_DEC_2 | CONFIG_POW_SR_260Vus | CONFIG_OC_NSD | CONFIG_EN_VSCOMP_OFF | CONFIG_SW_MODE_HARD | CONFIG_INT_16MHZ);
    
    getStatus();   /* Clear STATUS Flags and reset start-up UVLO */
    
    
}

char L6470_IC::Transfer(char data)

{
    char data_received;
    
    _pin = 0;   /* Slave Select (Chip Enable) goes LOW -  ready to write   */
    
    data_received = mySPI.write(data);
    
    _pin = 1;   /* Slave Select (Chip Enable) goes HIGH -  finished writing   */
    
    return data_received;
    
}


unsigned long L6470_IC::Param(unsigned long value, char bit_length)
{
   /*  Write the value without worrying about length; pass a bit length parameter from the calling function   */
    
    unsigned long ret_val = 0;   /*return this to generalize this function for both read and write of registers.*/
    
    /* Determine how many bytes constitute the transfer  */
    
    char byte_length = bit_length / 8;
    
    if(bit_length % 8   > 0)      /*To account for partial bytes, we max at byte_length that would fit all bits   */
    {
        byte_length = byte_length + 1; 
    }

    /* if the value passed was too high, max it out.  */ 
    unsigned long mask = 0xFFFFFFFF >> (32 - bit_length);   /* Max possible register lenght is 22 bits  */
    if(value > mask)
    {
        value = mask;      
    }

/* Data reconstruction sequence follows.  ret_val will be re-assembled in steps starting from MSByte */




    if (byte_length == 3)      /*  Isolates  MSbyte by r-shift and transfers over SPI. Receives MSbyte and l-shiftes back to position   */
    {
      ret_val = ret_val  |  (unsigned long)(Transfer((char)(value >> 16))) << 16;
    }
    
    if (byte_length >= 2)
    {
       ret_val = ret_val  | (unsigned long)(Transfer((char)(value >> 8))) << 8;
    }
    
    if (byte_length >= 1)
    {
       ret_val = ret_val  | (unsigned long)(Transfer((char)(value)));
    }

return (ret_val & mask);    /* Mask-off any unnecessary bits for robustness  */
}


unsigned long L6470_IC::RegHandler(char param, unsigned long value)
{
    
    unsigned long ret_val = 0;   // This is a temp for the value to return.
    /*
    This switch structure handles the appropriate action for each register.
    This is necessary since not all registers are of the same length, either
    bit-wise or byte-wise, so we want to make sure we mask out any spurious
    bits and do the right number of transfers. That is handled by the dSPIN_Param()
    function, in most cases, but for 1-byte or smaller transfers, we call
    Xfer() directly.
    */
    switch (param)
    {
    /*
    ABS_POS is the current absolute offset from home. It is a 22 bit number expressed
    in two's complement. At power up, this value is 0. It cannot be written when
    the motor is running, but at any other time, it can be updated to change the
    interpreted position of the motor.
    */
      case ABS_POS:
        ret_val = Param(value, 22);
        break;
    /*
    EL_POS is the current electrical position in the step generation cycle. It can
    be set when the motor is not in motion. Value is 0 on power up.
    */
      case EL_POS:
        ret_val = Param(value, 9);
        break;
    /*
    MARK is a second position other than 0 that the motor can be told to go to. As
    with ABS_POS, it is 22-bit two's complement. Value is 0 on power up.
    */
      case MARK:
        ret_val = Param(value, 22);
        break;
    /*
    SPEED contains information about the current speed. It is read-only. It does 
    NOT provide direction information.
    */
      case SPEED:
        ret_val = Param(0, 20);
        break; 
    /*
    ACC and DEC set the acceleration and deceleration rates.
    Cannot be written while motor is running. Both default to 0x08A on power up.
    AccCalc() and DecCalc() functions exist to convert steps/s/s values into
    12-bit values for these two registers.
    */
      case ACC: 
        ret_val = Param(value, 12);
        break;
      case DEC: 
        ret_val = Param(value, 12);
        break;
    /*
    MAX_SPEED is just what it says - any command which attempts to set the speed
    of the motor above this value will simply cause the motor to turn at this
    speed. Value is 0x041 on power up.
    MaxSpdCalc() function exists to convert steps/s value into a 10-bit value
    for this register.
    */
      case MAX_SPEED:
        ret_val = Param(value, 10);
        break;
    /*
    MIN_SPEED controls two things - the activation of the low-speed optimization
    feature and the lowest speed the motor will be allowed to operate at. LSPD_OPT
    is the 13th bit, and when it is set, the minimum allowed speed is automatically
    set to zero. This value is 0 on startup.
    MinSpdCalc() function exists to convert steps/s value into a 12-bit value for this
    register. SetLowSpeedOpt() function exists to enable/disable the optimization feature.
    */
      case MIN_SPEED: 
        ret_val = Param(value, 12);
        break;
    /*
    FS_SPD register contains a threshold value above which microstepping is disabled
    and the dSPIN operates in full-step mode. Defaults to 0x027 on power up.
    SCalc() function exists to convert steps/s value into 10-bit integer for this
    register.
    */
      case FS_SPD:
        ret_val = Param(value, 10);
        break;
    /*
    KVAL is the maximum voltage of the PWM outputs. These 8-bit values are ratiometric
    representations: 255 for full output voltage, 128 for half, etc. Default is 0x29.
    The implications of different KVAL settings is too complex to dig into here, but
    it will usually work to max the value for RUN, ACC, and DEC. Maxing the value for
    HOLD may result in excessive power dissipation when the motor is not running.
    */
      case KVAL_HOLD:
        ret_val = Transfer((char)(value));
        break;
      case KVAL_RUN:
        ret_val = Transfer((char)(value));
        break;
      case KVAL_ACC:
        ret_val = Transfer((char)(value));
        break;
      case KVAL_DEC:
        ret_val = Transfer((char)(value));
        break;
    /*
    INT_SPD, ST_SLP, FN_SLP_ACC and FN_SLP_DEC are all related to the back EMF
    compensation functionality. Please see the datasheet for details of this
    function- it is too complex to discuss here. Default values seem to work
    well enough.
    */
      case INT_SPEED:
        ret_val = Param(value, 14);
        break;
      case ST_SLP: 
        ret_val = Transfer((char)(value));
        break;
      case FN_SLP_ACC: 
        ret_val = Transfer((char)(value));
        break;
      case FN_SLP_DEC: 
        ret_val = Transfer((char)(value));
        break;
    /*
    K_THERM is motor winding thermal drift compensation. Please see the datasheet
    for full details on operation- the default value should be okay for most users.
    */
      case K_THERM: 
        ret_val = Transfer((char)value & 0x0F);
        break;
    /*
    ADC_OUT is a read-only register containing the result of the ADC measurements.
    This is less useful than it sounds; see the datasheet for more information.
    */
      case ADC_OUT:
        ret_val = Transfer(0);
        break;
    /*
    Set the overcurrent threshold. Ranges from 375mA to 6A in steps of 375mA.
    A set of defined constants is provided for the user's convenience. Default
    value is 3.375A- 0x08. This is a 4-bit value.
    */
      case OCD_TH: 
        ret_val = Transfer((char)value & 0x0F);
        break;
    /*
    Stall current threshold. Defaults to 0x40, or 2.03A. Value is from 31.25mA to
    4A in 31.25mA steps. This is a 7-bit value.
    */
      case STALL_TH: 
        ret_val = Transfer((char)value & 0x7F);
        break;
    /*
    STEP_MODE controls the microstepping settings, as well as the generation of an
    output signal from the dSPIN. Bits 2:0 control the number of microsteps per
    step the part will generate. Bit 7 controls whether the BUSY/SYNC pin outputs
    a BUSY signal or a step synchronization signal. Bits 6:4 control the frequency
    of the output signal relative to the full-step frequency; see datasheet for
    that relationship as it is too complex to reproduce here.
    */
      case STEP_MODE:
        ret_val = Transfer((char)(value));
        break;
    /*
    ALARM_EN controls which alarms will cause the FLAG pin to fall. A set of constants
    is provided to make this easy to interpret. By default, ALL alarms will trigger the
    FLAG pin.
    */
      case ALARM_EN: 
        ret_val = Transfer((char)value);
        break;
    /*
    CONFIG contains some assorted configuration bits and fields.Value on boot is 0x2E88; this
    can be a useful way to verify proper start up and operation of the dSPIN chip.
    */
      case CONFIG: 
        ret_val = Param(value, 16);
        break;
    /*
    STATUS contains read-only information about the current condition of the chip. 
    */
      case STATUS:  // STATUS is a read-only register
        ret_val = Param(0, 16);
        break;
      default:
        ret_val = Transfer((char)(value));
        break;
    }
    return ret_val;
}

void L6470_IC::SetParam(char param, unsigned long value)
{
    Transfer(SET_PARAM | param);
    RegHandler(param, value);
    
}
    
unsigned long L6470_IC::GetParam(char param)
{
    Transfer(GET_PARAM | param);
    return RegHandler(param, 0);
        
        
        
}

void L6470_IC::resetDevice()
{
    /*
    Reset device to power up conditions. Equivalent to toggling the STBY
    pin or cycling power.
    */
    Transfer(RESET_DEVICE);
}

int L6470_IC::getStatus()
{
    /*
    Fetch and return the 16-bit value in the STATUS register. Resets any warning flags and exits any error states. Using GetParam()
    to read STATUS does not clear these values.
    */
    
    int temp = 0;
    Transfer(GET_STATUS);
    temp = Transfer(0) << 8;   /*MSByte received and stacked  */
    temp = temp | Transfer(0);  /*LSByte received. STATUS Registed reception is completed */
    return temp;
}


bool L6470_IC::isBusy()
{
    int status = getStatus();
    return !((status >> 1) & 0x01);  /*Check BUSY bit in STATUS Register: 0 - BUSY, 1 - NOT BUSY  */
    
}


void L6470_IC::Run(char dir, float speed)
{
    
    unsigned long tempSpeed = SpeedCalc(speed);
    
    Transfer(RUN | dir);             /* RUN Command byte sent. IC prepared for reception of SPEED data     */
    
    Transfer((char)(tempSpeed >> 16));  /* Transfer first 4 bits (20..17) of MSByte and so on */  
    Transfer((char)(tempSpeed >> 8));
    Transfer((char)(tempSpeed));
    
}
unsigned long L6470_IC::SpeedCalc(float Steps_Sec)
{
    /*To convert SPEED  to steps/s use: [steps/s] = (SPEED x 2^-28)/tick where tick is 250 ns (or 250e-9 sec). Range:     0 <= steps/s <= 15625 with resolution 0.015 steps/sec */
    
    float tempSPEED = Steps_Sec * 67.1089;   /*SPEED = steps/s * (tick / 2^-28)    */
    
    if( (unsigned long) long(tempSPEED) > 0x000FFFFF)   /* Make sure that the value does not exceed 20-bit register capacity */
    {
        return tempSPEED = 0x000FFFFF;    
    }
    else
    {
        return (unsigned long)tempSPEED;    
    }
}

void L6470_IC::Step_Clock(char dir)
{
   Transfer(STEP_CLOCK | dir);    
}

void L6470_IC::Move(char dir, long n_steps)
{
    Transfer(MOVE | dir);
    
    if(n_steps > 0x3FFFFF)    /*  Make sure the number of desired steps does not sxceed the register capacity 22 bits   */
    {
        n_steps = 0x3FFFFF;   
    }
    
    Transfer((char)(n_steps >> 16));
    Transfer((char)(n_steps >> 8));
    Transfer((char)(n_steps));

}
 void L6470_IC::goTo(long ABS_pos)
{
    Transfer(GOTO);
    
    if (ABS_pos > 0x001FFFFF)
    {
         ABS_pos = 0x001FFFFF;
    }
        
         if (ABS_pos < -2097152)
    {
         ABS_pos = -2097152;
    }
    
    Transfer((char)(ABS_pos >> 16));
    Transfer((char)(ABS_pos >> 8));
    Transfer((char)(ABS_pos));     
          
}

void L6470_IC::goTo_DIR(char dir, long ABS_pos)
{
    Transfer(GOTO_DIR | dir);
    
   if (ABS_pos > 0x001FFFFF)
    {
         ABS_pos = 0x001FFFFF;
    }
        
         if (ABS_pos < -2097152)
    {
         ABS_pos = -2097152;
    }
    
    Transfer((char)(ABS_pos >> 16));
    Transfer((char)(ABS_pos >> 8));
    Transfer((char)(ABS_pos));
    
}

void L6470_IC::goUntil(char action, char dir, unsigned long speed)
{
    Transfer(GO_UNTIL | action | dir);
    
    if (speed > 0x3FFFFF)
    {
         speed = 0x3FFFFF;
    }
    
    Transfer((char)(speed >> 16));
    Transfer((char)(speed >> 8));
    Transfer((char)(speed));
    
}

void L6470_IC::releaseSW(char action, char dir)
{
    Transfer(RELEASE_SW | action | dir);    
}

void L6470_IC::goHome()
{
    Transfer(GO_HOME);  
}

void L6470_IC::goMark()
{
    Transfer(GO_MARK);  
}

void L6470_IC::resetPosition()
{
    Transfer(RESET_POS); 
}

void L6470_IC::softStop()
{
    Transfer(SOFT_STOP);   
}

 void L6470_IC::hardStop()
{
    Transfer(HARD_STOP); 
}

void L6470_IC::softHiZ()
{
    Transfer(SOFT_HIZ);    
}

void L6470_IC::hardHiZ()
{
    Transfer(HARD_HIZ);    
}

void L6470_IC::setMicroSteps(int microSteps)
{
    char tempSTEP = 0;
    
    switch(microSteps)
    {
        case 1: tempSTEP = STEP_SEL_FULL;
        break;
        
        case 2: tempSTEP = STEP_SEL_HALF;
        break;
        
        case 4: tempSTEP = STEP_SEL_1_4;
        break;
        
        case 8: tempSTEP = STEP_SEL_1_8;
        break;
        
        case 16: tempSTEP = STEP_SEL_1_16;
        break;
        
        case 32: tempSTEP = STEP_SEL_1_32;
        break;
        
        case 64: tempSTEP = STEP_SEL_1_64;
        break;
        
        case 128: tempSTEP = STEP_SEL_1_128;
        break;
        
        default:  tempSTEP = STEP_SEL_FULL;
        break;
      
    }
    
    SetParam(STEP_MODE, !SYNC_EN | tempSTEP | SYNC_SEL_1);   /* SYNCronization disabled - BUSY/SYNC output is forced low during command execution. SYNC_SEL = fFS  */
}

void L6470_IC::setMaxSpeed(float speed)
{   
    SetParam(MAX_SPEED, MaxSpeedCalc(speed));   
}
unsigned long L6470_IC::MaxSpeedCalc(float Steps_Sec)
{
    /* #To convert MAX_SPEED to steps/s use: [steps/s] = (MIN_SPEED x 2^-18)/tick.  Range: 15.25 <= steps/s <= 15610 with resolution 15.25 steps/sec */
    
    float tempSPEED = Steps_Sec * 0.065536;   /*MAX_SPEED = steps/s * (tick / 2^-18)    */
    
    if( (unsigned long) long(tempSPEED) > 0x000003FF)   /* Make sure that the value does not exceed 20-bit register capacity */
    {
        return tempSPEED = 0x000003FF;    
    }
    else
    {
        return (unsigned long)tempSPEED;    
    }   
}

void L6470_IC::setMinSpeed(float speed)
{
    SetParam(MIN_SPEED, MinSpeedCalc(speed));
}
unsigned long L6470_IC::MinSpeedCalc(float Steps_Sec)
{
    /* To convert MIN_SPEED to steps/s use: [steps/s] = (MIN_SPEED x 2^-24)/tick.  Range: 0 <= steps/s <= 976.3 with resolution 0.238 steps/sec.
       Register's MSB Enbales LOW SPEED OPTIMIZATION LSPD_OPT */
       
    float tempSPEED = Steps_Sec * 4.194304;   /*MIN_SPEED = steps/s * (tick / 2^-24)    */
    
    if( (unsigned long) long(tempSPEED) > 0x00000FFF)
    {    
        return tempSPEED = 0x00000FFF;
    }
    else
    {
        return (unsigned long) tempSPEED;    
    }
}

void L6470_IC::setAcc(float acceleration)
{
    /*  The ACC register contains the speed profile acceleration expressed in step/tick^2 (format unsigned fixed point 0.40).
        In order to convert ACC value in steps/s^2, the following formula can be used:
        
        [steps/s^2]= (ACC*2^(-40))/(tick^2)
        
        where ACC is the integer number stored in the register and tick is 250 ns.
        The 0xFFF value of the register is reserved and it should never be used.
        Must write to this register after motor has stopped. WILL ignore command if motor is running and throw NOTPERF_CMD flag to rise.
    */
    unsigned long accelerationBYTES = AccCalc(acceleration);
    SetParam(ACC, accelerationBYTES);
    
}
void L6470_IC::setDec(float deceleration)
{
    // [steps/s^2]= (DEC*2^(-40))/(tick^2)
    // Just like ACC this register must be set before the motor is running.
    unsigned long decelerationBYTES = DecCalc(deceleration);
    SetParam(DEC, decelerationBYTES);
}
    
unsigned long L6470_IC::AccCalc(float stepsPerSecSquared)
{   
    // stepsPerSecSquared can be selected from a range of 14.55 to 59590 per L6470 Datasheet.
    // Using steps/sec^2 we solve for ACC or DEC
    // by multiplying steps/sec^2 by (tick^2)/(2^-40)
    float temp = stepsPerSecSquared * 0.068719;
    if( (unsigned long) long(temp) > 0x00000FFE)
    {
         return 0x00000FFE;
    }
    else if( (unsigned long) long(temp) < 0x00000001)
    {
         return 0x00000001;
    }
    else 
    {
        return (unsigned long) long(temp);
    }
}
unsigned long L6470_IC::DecCalc(float stepsPerSecSquared)
{
    float temp = stepsPerSecSquared * 0.068719;
    if( (unsigned long) long(temp) > 0x00000FFF)
    {
         return 0x00000FFF;
    }
    else if( (unsigned long) long(temp) < 0x00000001)
    {
         return 0x00000001;
    }
    else 
    {
        return (unsigned long) long(temp);
    }
}


void L6470_IC::setOverCurrent(unsigned int mA_current)
{
    char tempCurrent = 0;
    
    switch(mA_current)
    {
        case 375: tempCurrent = OCD_TH_375mA;
        break;
        
        case 750: tempCurrent = OCD_TH_750mA;
        break;
        
        case 1125: tempCurrent = OCD_TH_1125mA;
        break;
        
        case 1500: tempCurrent = OCD_TH_1500mA;
        break;
        
        case 1875: tempCurrent = OCD_TH_1875mA;
        break;
        
        case 2250: tempCurrent = OCD_TH_2250mA;
        break;
        
        case 2625: tempCurrent = OCD_TH_2625mA;
        break;
        
        case 3000: tempCurrent = OCD_TH_3000mA;
        break;
        
        case 3375: tempCurrent = OCD_TH_3375mA;
        break;
        
        case 3750: tempCurrent = OCD_TH_3750mA;
        break;
        
        case 4125: tempCurrent = OCD_TH_4125mA;
        break;
        
        case 4500: tempCurrent = OCD_TH_4500mA;
        break;
        
        case 4875: tempCurrent = OCD_TH_4875mA;
        break;
        
        case 5250: tempCurrent = OCD_TH_5250mA;
        break;
        
        case 5625: tempCurrent = OCD_TH_5625mA;
        break;
        
        case 6000: tempCurrent = OCD_TH_6000mA;
        break;
        
        default: tempCurrent = OCD_TH_1875mA;
        break;
      
    }
    
    SetParam(OCD_TH, tempCurrent);   
}

void L6470_IC::setThresholdSpeed(float thresholdSpeed)
{
    if(thresholdSpeed == 0.0)
    {
        SetParam(FS_SPD, 0x3FF);    /*  The system always works in microstepping mode     */
    }
    
    else
    {
        SetParam(FS_SPD, FSCalc(thresholdSpeed));   
    }
    
}
unsigned long L6470_IC::FSCalc(float Steps_Sec)
{
    /* #To convert FS_SPD to step/s use: [steps/s] = 2^18 x(FS_SPD + 0.5)/tick.  Range:  7.63 <= steps/s <= 15625 with resolution 15.25 steps/sec  */
    
    float tempSPEED = (Steps_Sec * 0.065536)- 0.5;
    
    if( (unsigned long) long(tempSPEED) > 0x000003FF)
    {
        return 0x000003FF;
    }
    else 
    {
        return (unsigned long) long(tempSPEED);
    }
}

void L6470_IC::setStallCurrent(float mA_current)
{
    char tempThreshCurrent = (char)( floor(mA_current / 31.25) - 1);
    
    if(tempThreshCurrent > 0x7F)
    {
        tempThreshCurrent = 0x7F;
    }
    
    SetParam(STALL_TH,tempThreshCurrent);
  
}

void L6470_IC::SetLowSpeedOpt(bool enable)
{
    /* Enable or disable the low-speed optimization option. If enabling, the other 12 bits of the register will be automatically zero.
       When disabling, the value will have to be explicitly written by the user with a SetParam() call.   */
       
       Transfer(SET_PARAM | MIN_SPEED);
       
    if (enable)
    {
        Param(0x1000, 13);
    }
    else 
    {
        Param(0, 13);
    }
    
}

float L6470_IC:: getSpeed()
{
    float tick_sec = GetParam(SPEED) * 0.014901;
    
    return (float) tick_sec;
}

long L6470_IC::getPos()
{
    unsigned long position = GetParam(ABS_POS);
    
    return convert(position); 
}


void L6470_IC::setMark()
{
    long value = getPos();
    
    Transfer(MARK);
    
    if (value > 2097151)
    {
         value = 2097151;
    }
    if (value < -2097152)
    {
         value = -2097152;
    }
    
    
    Transfer((char)(value >> 16));
    Transfer((char)(value >> 8));
    Transfer((char)(value));
    
}
void L6470_IC::setMark(long value){
    
    Transfer(MARK);
    
    if (value > 2097151)
    {
         value = 2097151;
    }
    if (value < -2097152)
    {
         value = -2097152;
    }
    
    
    Transfer((char)(value >> 16));
    Transfer((char)(value >> 8));
    Transfer((char)(value));
}


long L6470_IC::convert(unsigned long val)
{
      int MSB = val >> 21;                                 // Extract the sign of of 2's complement value from register ABS_POS.
      
      val = val & 0x001FFFFF;                              // Mask to retain 21-bit
      
      if(MSB == 1)                                         // If extracted sign is negative pad 21-bit with 1's.
      {
          val = (long)(val | -2097152 );   //0xFFE00000
      }
       
       return val;                                         // If not negative just return value
}

