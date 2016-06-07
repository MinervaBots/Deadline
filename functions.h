#include "constants.h"


void port_setup()
{
    //------------Motores:----------------//
     TRISB14_bit=0;      //RMOTOR1, pino 14           //Locomoção Direito
     TRISA10_bit=0;      //RMOTOR2, pino 12
     TRISB10_bit=0;      //LMOTOR1, pino 8            //Locomoção Esquerdo
     TRISB12_bit=0;      //LMOTOR2, pino 10
     TRISC6_bit=0;       //SMOTOR1, pino 2            //Servo
     TRISC8_bit=0;       //SMOTOR2, pino 4
     //------------Diversos:--------------//
     TRISA0_bit=1;       //Leitura do Potenciometro, pino 19 [Analógica]
     TRISB2_bit=1;       //Switch, pino 23 [Analógica] ~~> desabilitar no ANSELB
     TRISB3_bit=0;       //Led, pino 24 [Analógica] ~~> desabilitar no ANSELB
     TRISA1_bit=1;       //QEI A direito, pino 20 [Analógica] ~~> desabilitar no ANSELA
     TRISB0_bit=1;       //QEI B direito, pino 21 [Analógica] ~~> desabilitar no ANSELB
     TRISA4_bit=1;       //QEI A esquerdo, pino 34 [Analógica] ~~> desabilitar no ANSELA
     TRISA9_bit=1;       //QEI B esquerdo, pino 35 [Analógica] ~~> desabilitar no ANSELA
     TRISC7_bit=0;       //TX, pino 3   ~~>  talvez não funcione por ser PWML [ Nos últimos testes de PWM, não conseguimos utilizar PWML indepedente de PWMH]
     //-------Array de sensores:-----------//
     TRISC0_bit=1;       //AN6, 1S, pino 25 [Analógica]
     TRISC1_bit=1;       //AN7, 2S, pino 26 [Analógica]
     TRISC2_bit=1;       //AN8, 3S, pino 27 [Analógica]
     TRISC3_bit=1;       //AN29, 4S, pino 36 [Analógica]
     TRISC4_bit=1;       //AN30, 5S, pino 37 [Analógica]
     TRISC5_bit=1;       //AN31, 6S, pino 38 [Analógica]
     TRISB7_bit=1;       //AN25, 7S, pino 43 [Analógica]
     TRISB8_bit=1;       //AN26, 8S, pino 24 [Analógica]
     //--------Sensores de borda:-----------//
     TRISB9_bit=1;       //AN27, SBorda_Esquerdo, pino 1 [Analógica] ~~> Caso não seja possível utilizar o pino 3 para TX, utilizaremos este e usaremos apenas o sensor direito.
     TRISB1_bit=1;       //AN3, SBorda_Direito, pino 22 [Analógica] 
     //---------Portas Analógicas:-----------//
     //Habilitando portas analógicas e desabilitando portas analógicas que serão utilizadas como digitais:
     ANSELA = 0b1111110111101101;
     ANSELB = 0b1111111111110010;
     
     //int POSCNTcopy = 0;         \\coisa do encoder [em teste]
     //VEL1CNT = 0;

     UART1_Init(4800);   //4800 DE BAUD RATE
     ADC1_Init();        //inicializa conversor analógico-digital
     
     // Mapeamento de portas periféricas:  ( Entradas dos Encoders[QEAx] e TX do UART[Para fazer o debug no Arduino] )
     Unlock_IOLOCK();

     PPS_Mapping(55, _OUTPUT,_U1TX);  //pino 3 TX  ~~> será mudado para para o pino 1 caso não seja possível utilizar aqui [como já explicado ali em cima]
     PPS_Mapping(17, _INPUT,_QEA1);   //pino 20, A direito
     PPS_Mapping(32, _INPUT,_QEB1);   //pino 21, B direito
     PPS_Mapping(20, _INPUT,_QEA1);   //pino 34, A esquerdo
     PPS_Mapping(25, _INPUT,_QEB1);   //pino 35, B esquerdo

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
     
     // Setagem dos registradores, é uma boa rever como isso funciona:
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
	


      return 0;         //colocar aqui código de leitura do array de sensores
}

float read_pot(){       //código pra leitura do pot
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

void pid_control(float error, float *output)
{
  //pid da posição do corpo
  integral += Ki*DT*error;
  *output = Kp * error + integral + Kd *(error-last_error)/DT;
  last_error = error;
}

void s_pid_control(float error, float *s_output)
{
  //pid de controle da haste
  s_integral += sKi*DT*error;
  *s_output = sKp* error + s_integral + sKd *(error-s_last_error)/DT;
  s_last_error = error;
}

void InitQEI1()  //função para inicializar módulo QEI
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