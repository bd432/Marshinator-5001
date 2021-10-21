#include "marshinator.h"


// List 1: Left US sensor
// List 2: Front US sensor

sensor_list_t ultrasound_1_list, ultrasound_2_list;

void setup() {
  //pinMode(NINA_RESETN, OUTPUT);         
  //digitalWrite(NINA_RESETN, LOW);  

  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program");  
  //SerialNina.begin(115200);

  setup_sensors();
  setup_motors();

  //Initial drive state
  driving_state_t initial_dr = FORWARDS;
  set_drive(initial_dr , drive_speed);
}


void loop(){
 turn_left(45);
 follow_wall(2);
 //following this there should soon be a delay and then the robot stops and scans for the blocks
 delay(1000);
 driving_state_t d_state = STATIONARY;
 set_drive(d_state, drive_speed);
 delay(100000);
}
