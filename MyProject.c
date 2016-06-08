//COISAS PENDENTES: read_border(), read_pot()

#include <stdbool.h>
#include "functions.h"
#include "millis.h"

void main(){
     port_setup();
     PWM_setup();
     
     #ifdef DEBUG
     UART1_Write_text("Esperando Botão");
     UART1_Write_text("\n");
     #endif
     
     while( !buttonIsPressed() );
     Delay_ms(500);
     LED=1;
     
     #ifdef DEBUG
     UART1_Write_text("Calibrando");
     UART1_Write_text("\n");
     #endif
     
     while( !buttonIsPressed() ){
      //qtra.calibrate();
      //qtrd.calibrate();
     }
     
     #ifdef DEBUG
     UART1_Write_text("Calibrado");
     UART1_Write_text("\n");
     #endif
     
     LED=1;
     Delay_ms(500);
     LED=0;
     while( !buttonIsPressed() );
     LED=1;
     Delay_ms(500);
     LED=0;
     following = 1;
     Delay_ms(1000);
     LED=1;
     max_speed = PERIOD/2;
     
     #ifdef DEBUG
     UART1_Write_text("Seguindo");
     UART1_Write_text("\n");
     #endif
     
     while(following)
     {
                     move_stem(s_output);
                     move_robot_old_style(output);
                     //read_border();
                     if ( (right_counter > (MARKS_NUM-1) )){
                       following = 0;
                       LED=0;
                     }
     }
}