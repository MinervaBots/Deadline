//COISAS PENDENTES: millis(), read_sensors(), read_border(), read_pot()

#include <stdbool.h>
#include "functions.h"
#include "millis.h"

void main(){
     port_setup();
     PWM_setup();
     while( !buttonIsPressed() );
     Delay_ms(500);
     LED=1;
     //Calibrando
     while( !buttonIsPressed() ){
      //qtra.calibrate();
      //qtrd.calibrate();
     }
     //Calibrado
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