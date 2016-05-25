#define MARKS_NUM 16
#define RMOTOR1 PDC1
#define RMOTOR2 PDC4
#define LMOTOR1 PDC3
#define LMOTOR2 PDC2
#define SMOTOR1 PDC5
#define SMOTOR2 PDC6
#define PERIOD 1000
#define BUTTON RB2_bit
#define LED RB3_bit

//Constantes:
bool following=0;
int max_speed=0;
int right_counter= 0;
//---Constantes de controle:----
//Controle das Rodas:
float KP = 10;
float KI = 0;
float KD = 0;
//Controle da Haste:
float sKP = 10;
float sKI = 0;
float sKD = 0;
#define DT  0.005
//-------------------------------
float output=0;
float s_output=0;
float s_pid_last_run=0;
float pid_last_run=0;
float last_error=0;
float s_last_error=0;
float integral = 0;
float s_integral = 0;
float angular_speed = 0;
float stem_position = 0;


//Declaração de funções:
void setup();
bool buttonIsPressed();
float pid_control(float error);
float stem_pid_control(float error);
float read_sensors();
float read_pot();
float millis();
void move_robot_old_style(float input);
void move_stem(float input);
void read_border();
void InitQEI1();
