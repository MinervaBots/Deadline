#include "constants.h"

//Millis functions definitions
#define PR1_VALUE  39062  //9765*4
#define PRESCALER  256
#define MACHINE_CYCLE   0.0000001
#define OVERFLOW_TIME  MACHINE_CYCLE * PRESCALER * PR1_VALUE * 1000  //250*4 = 1000
#define MILLIS_SCALER  PRESCALER * MACHINE_CYCLE * 1000


void port_setup()
{
     //int POSCNTcopy = 0;         \\coisa do encoder [em teste]
     //VEL1CNT = 0;
     TRISB14_bit=0;      //RMOTOR1
     TRISB12_bit=0;      //RMOTOR2
     TRISB10_bit=0;      //LMOTOR1
     TRISA10_bit=0;      //LMOTOR2
     TRISC8_bit=0;       //SMOTOR1
     TRISC6_bit=0;       //SMOTOR2
     TRISB2_bit=1;       //Switch
     TRISB3_bit=0;       //Led
     TRISA8_bit=1;       //QEI B direito
     TRISB4_bit=1;       //QEI A direito
     TRISC7_bit=0;       //TX
     
     UART1_Init(4800);   //4800 DE BAUD RATE
     ADC1_Init();        //inicializa conversor anal�gico-digital
     
     Unlock_IOLOCK();

     //PPS_Mapping(54, _INPUT,_U1RX);   //pino 2 RX
     //PPS_Mapping(54, _INPUT,_INDX1);
     PPS_Mapping(55, _OUTPUT,_U1TX);  //pino 3 TX
     PPS_Mapping(36, _INPUT,_QEA1);   //pino 33 QEA1
     PPS_Mapping(24, _INPUT,_QEB1);   //pino 32 QEB1

     Lock_IOLOCK();
}

void PWM_setup(){
     
     PHASE1 = PERIOD;
     PHASE2 = PERIOD;
     PHASE1 = PERIOD;
     PHASE2 = PERIOD;
     PHASE1 = PERIOD;
     PHASE2 = PERIOD;
     
     RMOTOR1=0;
     RMOTOR2=0;
     LMOTOR1=0;
     LMOTOR2=0;
     SMOTOR1=0;
     SMOTOR2=0;
     
     //Setagem dos registradores, � uma boa rever como isso funciona:
     DTR1 = DTR2 =  0;
     // DTR1 = 0;
     ALTDTR1 = ALTDTR2 = 0;
     //  ALTDTR1 = 0;
     IOCON1 = IOCON2 = 0xCC00;
     //IOCON1= 0xCC00;
     PWMCON1 = PWMCON2  = 0x0200;
     //   PWMCON1 = 0x0200;
     FCLCON1 = FCLCON2 = 0x0003;
     //FCLCON1 = 0x0003;
     PTCON2 = 0x0000;

     PTCON = 0x8000;
}

bool buttonIsPressed(){
     if(BUTTON=1)
         return 1;
    else
         return 0;
}

void calibrate(unsigned int numSensors, unsigned int numSamples, unsigned int *pins,unsigned int * calibratedMinimum,unsigned int * calibratedMaximum) //Calibragem de sensores
{
    int i;
    int j;
    unsigned int sensor_values[16];
    unsigned int max_sensor_values[16];
    unsigned int min_sensor_values[16];
    unsigned int _maxValue = 100000;

    if(*calibratedMaximum == 0)
    {
        for(i=0;i<numSensors;i++)
         (calibratedMaximum)[i] = 0;
    }
    
    if(*calibratedMinimum == 0)
    {
        for(i=0;i<numSensors;i++)
            (calibratedMinimum)[i] = _maxValue;
    }
    for(j=0;j<10;j++)
    {
        read(sensor_values,numSensors,numSamples,pins);
        for(i=0;i<numSensors;i++)
        {
            if(j == 0 || max_sensor_values[i] < sensor_values[i])
                max_sensor_values[i] = sensor_values[i];
            if(j == 0 || min_sensor_values[i] > sensor_values[i])
                min_sensor_values[i] = sensor_values[i];
        }
    }
    for(i=0;i<numSensors;i++)
    {
        if(min_sensor_values[i] > (calibratedMaximum)[i])

float read_sensors(){
	


      return 0;         //colocar aqui c�digo de leitura do array de sensores
}

float millis(){
	double millis();
	void T1Interrupt();
	void initTimer();
    
    double timerOverflow = 0;

/****** Timer 1 interruption Function *******/

	void T1Interrupt() iv IVT_ADDR_T1INTERRUPT ics ICS_AUTO {
		timerOverflow += OVERFLOW_TIME;
		IFS0bits.T1IF = 0;
	}


	double millis(){
		return timerOverflow + TMR1*MILLIS_SCALER;
}

	void initTimer()
{
 		//Initialize Timer 1 for millis function
		// machine cycle = 1/(Fosc/2) = 1E-07
		// Overflow = PR1 x prescaler x machine cycle
		//          = 39062 x 256 x 1E-07 = 1000 ms


		T1CON = 0;              // Reset TIMER 1 Configuration
		T1CONbits.TCKPS = 0b11; // Prescaler = 256
		T1CONbits.TON = 1;      // Enables TIMER1

		T1IF_bit = 0;           // timer 1 Overflow flag
		TMR1 = 0;               // Reset TIMER 1 Register
		PR1 = PR1_VALUE;        // TIMER 1 period to interrupt
		IEC0bits.T1IE = 1;      // Enables TIMER 1 interruption
		IPC0bits.T1IP = 1;      // T1IP<2:0> = 1 ( priority 1 interruption)


}

float read_pot(){       //c�digo pra leitura do pot
      return 0;
}

void move_robot_old_style(float control_output){
      if (control_output > 0){
        RMOTOR1 = max_speed;
        RMOTOR2 = 0;
        if (max_speed - control_output < 0){
          LMOTOR1 = 0;
          LMOTOR2 = abs(floor(max_speed - control_output));
        }
        else{
          LMOTOR1 = floor(max_speed - control_output);
          LMOTOR2 = 0;
        }
      }
      else{
          LMOTOR1 = max_speed;
          LMOTOR2 = 0;
          if (max_speed + control_output < 0){
            RMOTOR1 = 0;
            RMOTOR2 = floor(max_speed + control_output);              }
          else{
            RMOTOR1 = floor(max_speed + control_output);
            RMOTOR2 = 0;
          }
      }
}

void move_stem(float input){
      if(input>0){
          if(max_speed - input<0){
             SMOTOR1 = max_speed;
             SMOTOR2 = 0;
          }
          else{
             SMOTOR1 = input;
             SMOTOR2 = 0;
          }
      }
      else{
          if(max_speed + input<0){
             SMOTOR1 = 0;
             SMOTOR2 = max_speed;
          }
          else{
             SMOTOR1 = 0;
             SMOTOR2 = -input;
          }
      }
}

float pid_control(float error)
{
  //pid da posi��o do corpo
  if(millis() - pid_last_run < DT * 1000)
    return output;
  integral += Ki*DT*error;
  output = Kp * error + integral + Kd *(error-last_error)/DT;
  last_error = error;
  pid_last_run = millis();
  return output;
}

float s_pid_control(float error)
{
  //pid de controle da haste
  if(millis() - s_pid_last_run < DT * 1000)
    return s_output;
  s_integral += sKi*DT*error;
  s_output = sKp* error + s_integral + sKd *(error-s_last_error)/DT;
  s_last_error = error;
  s_pid_last_run = millis();
  return s_output;
}



void InitQEI1()  //fun��o para inicializar m�dulo QEI
{
QEI1CONbits.QEIEN = 0b1;
QEI1CONbits.QEISIDL = 0b0;
QEI1CONbits.PIMOD = 0b110;  // antes 000
QEI1CONbits.INTDIV = 0b000;
QEI1CONbits.CNTPOL = 0b0;
QEI1CONbits.GATEN = 0b0;
QEI1CONbits.CCM = 0b00;

QEI1IOCbits.QCAPEN=0b1;        //  mudei
QEI1IOCbits.FLTREN=0b1;
QEI1IOCbits.QFDIV=0b000;
QEI1IOCbits.OUTFNC=0b00;
QEI1IOCbits.SWPAB=0b0;
QEI1IOCbits.HOMPOL=0b0;
QEI1IOCbits.IDXPOL=0b0;
QEI1IOCbits.QEBPOL=0b0;
QEI1IOCbits.QEAPOL=0b0;

QEI1STATbits.PCHEQIEN=0b0;
QEI1STATbits.PCLEQIEN=0b0;
QEI1STATbits.POSOVIEN=0b0;
QEI1STATbits.PCIIEN=0b0;
QEI1STATbits.VELOVIEN=0b0;
QEI1STATbits.HOMIEN=0b0;
QEI1STATbits.IDXIEN=0b0;
}