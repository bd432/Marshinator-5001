#include "marshinator.h"

// List 1: Left US sensor
// List 2: Front US sensor

sensor_list_t ultrasound_1_list, ultrasound_2_list;
//previousMillis = 0;

void setup() {
  //pinMode(NINA_RESETN, OUTPUT);         
  //digitalWrite(NINA_RESETN, LOW);  

  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program");  
  //SerialNina.begin(115200);

  setup_pins();
  setup_motors();
  setup_LED();

  //Initial drive state
  driving_state_t initial_dr = STATIONARY;
  set_drive(initial_dr , drive_speed);
}


void loop(){

 //Turns to align with wall and starts driving
 turn_and_check_left(45, 0.1);
 set_drive(FORWARDS, drive_speed);

 //Follows wall round to the other side of the arena
 follow_wall(2);

 //Goes forward for a bit longer before stopping to scan for blocks
 drive_with_LED(1000, 10, FORWARDS);
 set_drive(STATIONARY, drive_speed);
 ledState=LOW; //Turn off LED now robot has stopped moving


//scans for blocks
delay(10000);

//block identification and pick up protocol

//Returns home
set_drive(FORWARDS, drive_speed);
follow_wall(2);





 
}
