#include <Arduino.h>
#include <Servo.h>

// Pin i/o ports
#define trigPin1 0
#define echoPin1 A0
#define trigPin2 1
#define echoPin2 A1
#define shortIRPin A2
#define LED_Pin 10

// Program constants
#define delta_t 0.05// s
#define upper_wall_bound 15
#define lower_wall_bound 10
#define drive_speed 200
#define turn_scale_factor 0 //this needs to be tested and defined
#define LED_interval 500 //ms

// Radar variables
#define radar_N 180
#define angular_res 0.5 // degrees
#define start_angle 30
#define peaks_N 20
#define blocks_N 10
#define threshold 1.5


class sensor_list_t {
  // Class that acts as 100 long list
  // add()ios the same as append
  // fetch retrieves from the end - fetch(0) retrueves last entry ...
  // writes over first entries when above 100
  public:
    unsigned short n;
    void add( double x){pos[p] = x; p++; n ++; if(n > N_MAX) n = 0; if(p == N_MAX) p = 0;}
    double fetch(int i){int j = p - i - 1; if(j < 0) j+= N_MAX; return pos[j];}

  private:
    const int N_MAX = 100;
    unsigned short p;
    double pos[100];  
};

//Driving state variable to store the state the motors are in
enum driving_state_t { STATIONARY = 0, FORWARDS = 1, BACKWARDS = 2, RIGHT = 3, LEFT = 4};

// Declare global variables in all files
extern sensor_list_t ultrasound_1_list, ultrasound_2_list;
extern Servo servo;

//LED variables
extern unsigned long currentMillis;  //stores current time when doing LED check
extern unsigned long previousMillis; //stores time that the LED previously blinked
extern int ledState; //stores the state of the LED

// Function Prototypes
//Follow wall
double calc_finite_difference(sensor_list_t list, double dt);
double calc_average(sensor_list_t list, int N);
void turn_and_pulse(bool turn_right);
void reset_after_turn(int N);
void follow_wall(int wall_no);

//LED
void setup_LED(void);
void LED_check(void);

//Motor Test
void setup_motors(void);
void set_drive(driving_state_t state, int d_speed);

//Drive
void turn_right(double angle);
void turn_left(double angle);
void turn_and_check_right(double angle, double angular_resolution);
void turn_and_check_left(double angle, double angular_resolution);
void drive_with_LED(unsigned long duration, double resolution, driving_state_t d_state);

//Sensors
void setup_pins(void);
double read_ultrasound(int sensor_no,int N);
double read_shortIR(int average_count);

//Radar
bool radar_scan(double polar_coor[]);
