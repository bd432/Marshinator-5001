#include <Arduino.h>
//#include <Adafruit_MotorShield.h>

// Pin i/o ports
#define trigPin1 0
#define echoPin1 A0
#define trigPin2 1
#define echoPin2 A2
#define shortIRPin A1

// Program constants
#define delta_t 0.05 // s
#define upper_wall_bound 15
#define lower_wall_bound 10
#define drive_speed 200
#define turn_scale_factor 0 //this needs to be tested and defined




class track_t {
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
extern track_t ultrasound_1_list, ultrasound_2_list;

// Function Prototypes
//Main
double calc_finite_difference(track_t list, double dt);
double calc_average(track_t list, int N);
void turn_and_pulse(bool turn_right);
void reset_after_turn(int N);

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
