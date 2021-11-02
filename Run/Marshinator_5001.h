#include <Arduino.h>
#include <Servo.h>
#include <Adafruit_MotorShield.h>

// Pin i/o ports
#define trigPin1 7
#define echoPin1 6
#define trigPin2 5
#define echoPin2 4
#define shortIRPin A2
#define moveLED_Pin 1 
#define RedLED_Pin 2
#define GreenLED_Pin 3 
#define blockPin A3
#define switchPin 12
#define buttonPin 12
#define statePin 5
#define line1Pin A0
#define line2Pin A1
#define servoPin 10

// Program constants
#define delta_t 0.1// s
#define upper_wall_bound_1 12 // Bounds for first wall
#define lower_wall_bound_1 6
#define upper_wall_bound_2 12 // Bounds for second wall
#define lower_wall_bound_2 6
#define drive_speed 255
#define turn_scale_factor_left 10//10.98
#define turn_scale_factor_right 10//20
#define arm_scale_factor 12 //this needs to be tested and defined
#define LED_interval 250 //ms
#define collect_block_timout 3 // s 
#define wall_check_threshold 80 //cm

//  Arm Pickup Mechanism        -------- these angles must be tested and then defined later
#define arm_motor 3
#define descent_angle_pickup 50
#define ascent_angle_pickup 50
#define ascent_angle_dropoff 30
#define descent_angle_dropoff 0

// Radar variables
#define radar_N 180
#define angular_res 0.5 // degrees
#define start_angle 30
#define peaks_N 20
#define blocks_N 10
#define threshold 1.5
#define range_cutoff 50
#define theta_offset 10 // Measured clockwise of robot
#define x_offset 12
#define y_offset 22
#define anglar_block_tolerance 5 // degrees



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

// Robot state variables
enum robot_state_t { IDLE = 0, MOVE_TO_BLOCKS = 1, SCAN_BLOCKS = 2, COLLECT_BLOCK = 3, IDENTIFY_BLOCK = 4, MOVE_TO_DROP = 5, TEST = 6, SWEEP_AREA = 7};

// Declare global variables in all files
extern sensor_list_t ultrasound_1_list, ultrasound_2_list;
extern Servo servo;
extern robot_state_t robot_state;
extern Adafruit_MotorShield AFMS;
extern Adafruit_DCMotor *Motor1, *Motor2, *ArmMotor;

//LED variables
extern unsigned long currentMillis;  //stores current time when doing LED check
extern unsigned long previousMillis; //stores time that the LED previously blinked
extern int ledState; //stores the state of the LED

// Function Prototypes
//Follow wall
double calc_finite_difference(sensor_list_t list, double dt);
double calc_average(sensor_list_t list, int N);
bool turn_and_pulse(bool turn_right);
void reset_after_turn(int N);
void corner_turn(bool reset);
void move_until_corner_turn(double timeout);
void find_wall(void);
bool sweep_strip(void);
void follow_wall(int wall_no, unsigned long max_duration, bool white_line,double lower_wall_bound, double upper_wall_bound, bool corner_turn);

//LED
void setup_LED(void);
void LED_check(void);

//Motor Test
void setup_motors(void);
void set_drive(driving_state_t state, int d_speed);

//Drive
bool turn_and_check_left(double angle, double dt, bool line_break);
bool turn_and_check_right(double angle, double dt, bool line_break);
void drive_with_LED(unsigned long duration, double dt, driving_state_t d_state);

//Sensors
void setup_pins(void);
double read_ultrasound(int sensor_no,int N);
double read_shortIR(int average_count);
bool sense_block(int N);
bool switchOn(void);
bool blockSensor(void);
bool detect_line(void);

//Block type identification, pickup, transport
bool block_type_detection(void);
void arm_test (double angle1, double angle2, double angle3, double angle4);
void block_dropoff_and_reset (double ascent_angle, double descent_angle);
void pickup_block(double descent_angle ,double ascent_angle);
bool block_detection(void);
void deposit_block_and_reverse(void);

//Radar
bool radar_scan(double polar_coor[]);
