#define MARKS_NUM 16           //Número de marcações que o seguidor vai ter que contar antes de parar
#define RMOTOR1 PDC1
#define RMOTOR2 PDC4
#define LMOTOR1 PDC3
#define LMOTOR2 PDC2
#define SMOTOR1 PDC5
#define SMOTOR2 PDC6
#define PERIOD 1000
#define BUTTON RB2_bit
#define LED RB3_bit
#define CENTER_POSITION 3500.0  // Sensor de reflectancia Center_position {4 sensores = 1500, 6 sensores => 2500, 8 sensores => 3500}

//----------Constantes--------------//
bool following=0;
int max_speed=0;
int right_counter= 0;
//-----Constantes de controle:-----//
//Controle das Rodas:
float KP = 10;
float KI = 0;
float KD = 0;
//Controle da Haste:
float sKP = 10;
float sKI = 0;
float sKD = 0;
#define DT  0.005
//-------------------------------//
float output=0;
float s_output=0;
float last_error=0;
float s_last_error=0;
float integral = 0;
float s_integral = 0;
float erro_maximo   = 10.0; //       Raposo: atan(LARGURA_SENSOR/(2.0 * ALTURA_SENSOR));


//---------Declaração de funções---------//

//Setup de registradores
void setup();
void PWM_setup();
//Botão
bool buttonIsPressed();
//Controle PID
float pid_control(float error);       
float stem_pid_control(float error);
//Leitura dos sensores:
float read_sensors();
float read_pot();
void read_border();
//Millis
float millis();
//Movimento de motores
void move_robot_old_style(float input);
void move_stem(float input);
//Encoders
void InitQEI1();