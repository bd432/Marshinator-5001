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
#define radar_N 180
#define angular_res 0.5 // degrees
#define start_angle 30
#define peaks_N 20
#define blocks_N 10
#define threshold 1
#define range_cutoff 40
#define theta_offset 10 // Measured clockwise of robot
#define x_offset 10
#define y_offset 10

extern Servo servo;

// Function Prototypes

//Sensors
void setup_sensors(void);
double read_ultrasound(int sensor_no,int N);
double read_shortIR(int average_count);

//Radar
bool block_scan(double polar_coor[]);
//void scan_radar(double list[]);
//void calc_finite_difference(double list[], double output[], double dx, int N);
//void scan_peaks(double peaks[][3], double cart_coor[][2], int N, double threshold);
bool detect_blocks(double peaks[][3], double blocks[][2], double data[]);
void select_block(double polar_coor[]);
//void convert_cartesian(double r[], double cartesian[][2]);

