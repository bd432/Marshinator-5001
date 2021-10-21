#include <Arduino.h>
#include <Servo.h>

// Pin i/o ports
#define trigPin1 0
#define echoPin1 A0
#define trigPin2 1
#define echoPin2 A1
#define shortIRPin A2

// Program constants
#define delta_t 0.1// s
#define upper_wall_bound 15
#define lower_wall_bound 10
#define drive_speed 200
#define turn_scale_factor 0 //this needs to be tested and defined

// Radar variables
#define radar_N 90
#define angular_res 1 // degrees
#define start_angle 45


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


enum driving_state_t { STATIONARY = 0, FORWARDS = 1, BACKWARDS = 2, RIGHT = 3, LEFT = 4};

// Declare global variables in all files
extern sensor_list_t ultrasound_1_list, ultrasound_2_list;
extern Servo servo;

// Function Prototypes
//Follow wall
double calc_finite_difference(sensor_list_t list, double dt);
double calc_average(sensor_list_t list, int N);
void turn_and_pulse(bool turn_right);
void reset_after_turn(int N);
void follow_wall(int wall_no);

//Motor Test
void setup_motors(void);
void set_drive(driving_state_t state, int d_speed);

//Drive
void turn_right(double angle);
void turn_left(double angle);

//Sensors
void setup_sensors(void);
double read_ultrasound(int sensor_no,int N);
double read_shortIR(int average_count);

//Radar
bool block_scan(double polar_coor[]);
void scan_radar(double list[]);
void differentiate(double list[], double output[], double dx, int N);
void scan_peaks(double peaks[][3], double der[], int N, double threshold);
bool detect_blocks(double peaks[][3], double blocks[][2], double data[]);
void select_block(double polar_coor[]);
