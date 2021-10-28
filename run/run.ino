#include "Marshinator_5001.h"

// List 1: Left US sensor
// List 2: Front US sensor

sensor_list_t ultrasound_1_list, ultrasound_2_list;
robot_state_t robot_state = IDENTIFY_BLOCK;

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
  Serial.println("Drive");
}


void loop(){
  static double polar_coor[2];
  static unsigned long start_time;
  //Serial.println("Loop");

  switch (robot_state){
    case IDLE:
      set_drive(STATIONARY, drive_speed);
      if (digitalRead(switchPin)) {
        robot_state = MOVE_TO_BLOCKS;
      }
      break;
    case MOVE_TO_BLOCKS:
      Serial.println("Move along wall");
      //Turns to align with wall and starts driving
      turn_and_check_left(45, 10);
      set_drive(FORWARDS, drive_speed);
      //Follows wall round to the other side of the arena
      follow_wall(1, 1000000000000);
      follow_wall(1 ,1500000000000);
      set_drive(STATIONARY, drive_speed);
      delay(10000);
      //Goes forward for a bit longer before stopping to scan for blocks
      drive_with_LED(1000, 10, FORWARDS);
      set_drive(STATIONARY, drive_speed);
      ledState=LOW; //Turn off LED now robot has stopped moving
      robot_state = SCAN_BLOCKS;
      break;
    case SCAN_BLOCKS:
    Serial.println("Scan");
      // Stop robot
      set_drive(STATIONARY, drive_speed);
      // Scan radar
      if (radar_scan(polar_coor)) {
        // Turn to block if not aligned - move to
        if (polar_coor[1] < anglar_block_tolerance && polar_coor[1] > -anglar_block_tolerance) {
          robot_state = COLLECT_BLOCK;    // Calibration needed so that an angle 0 corresponds to straight ahead
          set_drive(FORWARDS, 255); // Drive at slower speed for collecting block
          Serial.println("Radar identified block");
          Serial.print("Angle - ");
          Serial.println(polar_coor[1]);
          start_time = millis(); // Reset timer for COLLECT_BLOCK
        }
        else if (polar_coor[1] > 0) turn_and_check_left(polar_coor[1], 10);
        else turn_and_check_right(-1 * polar_coor[1], 10);
      }
      // Rotate and repeat if no block detected
      else {turn_and_check_right(90,10);Serial.println("No block detected - Rotate");}
      break;
    case COLLECT_BLOCK:
      if (sense_block(10)) {
        robot_state = IDENTIFY_BLOCK; // Account for radar offset
        set_drive(STATIONARY, drive_speed);
        start_time = millis();
      }
      // Return to Scan blocks if time out
      if (millis() - start_time > collect_block_timout * 1000){
        set_drive(BACKWARDS, drive_speed);
        delay(100);
        turn_and_check_right(180, 10);
        set_drive(STATIONARY, drive_speed);
        robot_state = SCAN_BLOCKS;
      } 
     break;
    case IDENTIFY_BLOCK:
      pickup_block(0,30);
      block_type_detection();
      
      

      break;
    case MOVE_TO_DROP:
      //Returns home
      set_drive(FORWARDS, drive_speed);
      follow_wall(2, 1000);
      break;
    case TEST:
      Serial.println("Pickup");
      pickup_block(5,5);
      delay(1500);
      break;
  }
}
