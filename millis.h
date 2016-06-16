#ifndef MILLIS_H
#define MILLIS_H

#include "functions.h"

//#define PR1_VALUE  39062  //9765*4
#define PR1_VALUE  0xC350 //0xFFFF + 1 = 65530 + 1
//#define PR1_VALUE C350  // PR1*1/Fcy = DT ~> PR1 = DT * Fcy  (DT=0.005 & Fcy =20^6/2 ) ~> PR1 = 50000dec = C350hex
#define PRESCALER  1
#define MACHINE_CYCLE  0.0000001
#define OVERFLOW_TIME  MACHINE_CYCLE * PRESCALER * PR1_VALUE * 1000  //250*4 = 1000
#define MILLIS_SCALER  PRESCALER * MACHINE_CYCLE * 1000

double millis();
void T1Interrupt();
void initTimer();


double timerOverflow = 0;

/****** Timer 1 interruption Function *******/

void T1Interrupt() iv IVT_ADDR_T1INTERRUPT ics ICS_AUTO
{
   s_pid_control(read_sensors(), &s_output);
   pid_control(read_pot(), &output);
   timerOverflow += OVERFLOW_TIME;
   IFS0bits.T1IF = 0;
}


double millis()
{
  return timerOverflow + TMR1*MILLIS_SCALER;
}

void initTimer()
{
 //Initialize Timer 1 for millis function
 // machine cycle = 1/(Fosc/2) = 1E-07
 // Overflow = PR1 x prescaler x machine cycle
 //          = 39062 x 256 x 1E-07 = 1000 ms

//CLKDIVbits.DOZEN = 0;    // ~~testar (canto)
 CLKDIVbits.DOZE = 0b000;       // ~~testar (canto)

 T1CON = 0x0000;              // Reset TIMER 1 Configuration
 //T1CONbits.TCKPS = 0b11; // Prescaler = 256
 T1CONbits.TCKPS = 0b00; // Prescaler = 1
 T1CONbits.TON = 1;      // Enables TIMER1

 T1IF_bit = 0;           // timer 1 Overflow flag
 TMR1 = 0;               // Reset TIMER 1 Register
 PR1 = PR1_VALUE;        // TIMER 1 period to interrupt
 //PR1 = 0xFFFF;
 IEC0bits.T1IE = 1;      // Enables TIMER 1 interruption
 IPC0bits.T1IP = 0b001;      // T1IP<2:0> = 1 ( priority 1 interruption)


}

#endif