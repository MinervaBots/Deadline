#include <stdlib.h>

/* Pessoal as únicas funções que devemos chamar são, a Calibrate no início  e a ReadLine que usamos
dento do return da função read_sensors, ou seja, ela não é chamada no código principal. No arduíno nós chamavamos
várias funções referentes aos sensores dentro do funçoes.h e constantes.h. Agora não vamos precisar mais fazer isso, eu juntei
todas elas na Calibrate de  uma vez. */

/* %%%%%%%%%%%%%%%%%%% Read sensors %%%%%%%%%%%%%%%%%%%*/
void read(unsigned int *sensor_values, unsigned int numSensors, unsigned int numSamples, unsigned int *pins)
{
    unsigned char i, j;
    char txt[10];

    if (pins == 0)
        return;

    for(i = 0; i < numSensors; i++)
        sensor_values[i] = 0;

    for (j = 0; j < numSamples; j++)
    {
        for (i = 0; i < numSensors; i++)
        {
            sensor_values[i] += ADC1_Get_Sample(pins[i]);
        }
    }

    for (i = 0; i <numSensors; i++)
     {
        sensor_values[i] = (sensor_values[i] + numSamples) /numSamples;
    //                      IntToStr(sensor_values[i],txt);
      //  UART1_Write_Text(txt);
        //UART1_Write_Text("\n");
     }


}
/* %%%%%%%%%%%%%%%%%%% Read sensors and scale to a value between calibratedMaximum and calibratedMinimum%%%%%%%%%%%%%%%%%%%*/
void readCalibrated(unsigned int *sensor_values, unsigned int numSensors,unsigned int numSamples,
unsigned int *pins,unsigned int * calibratedMinimum,unsigned int * calibratedMaximum)
{
    int i;
    unsigned int calmin,calmax;
    unsigned int denominator;
    signed int x = 0;
    
    // if not calibrated, do nothing
    if(!calibratedMinimum || !calibratedMaximum)
     return;
    
    // read the needed values
    read(sensor_values,numSensors,numSamples,pins);

    for(i=0;i<numSensors;i++)
    {
      calmax = calibratedMaximum[i];
      calmin = calibratedMinimum[i];
  
      denominator = calmax - calmin;
  
      if(denominator != 0)
        x = (((signed long)sensor_values[i]) - calmin) * 1000 / denominator;
      
      if(x < 0)
       x = 0;
      
      else if(x > 1000)
       x = 1000;
        
      sensor_values[i] = x;
    }

}
/* %%%%%%%%%%%%%%%%%%% This function is called inside the function read_sensors %%%%%%%%%%%%%%%%%%%*/
int readLine(unsigned char white_line, unsigned int numSensors, unsigned int numSamples,unsigned int *pins,
unsigned int * calibratedMinimum,unsigned int * calibratedMaximum)
{
    unsigned int sensor_values[16];
    unsigned char i, on_line = 0;
    unsigned long avg = 0; // this is for the weighted total, which is long
                       // before division
    unsigned int sum = 0; // this is for the denominator which is <= 64000
    int last_value=0; // assume initially that the line is left.
    int value;
    
    readCalibrated(sensor_values,numSensors,numSamples,pins,calibratedMinimum,calibratedMaximum);

    for(i=0;i<numSensors;i++)
    {
        value = sensor_values[i];
        if(white_line)
            value = 1000-value;

        // keep track of whether we see the line at all
        if(value > 200) 
        {
            on_line = 1;
        }

        // only average in values that are above a noise threshold
        if(value > 50) 
        {
            avg += (long)(value) * (i * 1000);
            sum += value;
        }
    }

    if(!on_line)
    {
        // If it last read to the left of center, return 0.
        if(last_value < (numSensors-1)*1000/2)
            return 0;

        // If it last read to the right of center, return the max.
        else
            return (numSensors-1)*1000;

    }

    last_value = avg/sum;

    return last_value;
}
/* %%%%%%%%%%%%%%%%%%% FUNCTION FOR CALIBRATE SENSORS %%%%%%%%%%%%%%%%%%%*/
void Calibrate(unsigned int numSensors, unsigned int numSamples,
unsigned int *pins,unsigned int * calibratedMinimum,unsigned int * calibratedMaximum)
{
    int i;
    int j;
    unsigned int sensor_values[16];
    unsigned int max_sensor_values[16];
    unsigned int min_sensor_values[16];
    unsigned int _maxValue = 100000;

    //if sensors have not been calibrated
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
            (calibratedMaximum)[i] = min_sensor_values[i];
        if(max_sensor_values[i] < (calibratedMinimum)[i])
            (calibratedMinimum)[i] = max_sensor_values[i];
    }
}