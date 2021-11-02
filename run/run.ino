#include "Marshinator_5001.h"

// List 1: Left US sensor
// List 2: Front US sensor

sensor_list_t ultrasound_1_list, ultrasound_2_list;
robot_state_t robot_state = IDLE;

void setup() {

  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program"); 

  setup_pins();
  setup_motors();
  setup_LED();

  //Initial drive state
  driving_state_t initial_dr = STATIONARY;
  set_drive(initial_dr , drive_speed);
}


void loop(){
  static double polar_coor[2];
  static unsigned long start_time;
  bool red_block;

  switch (robot_state){
    case IDLE:
      set_drive(STATIONARY, drive_speed);
      Serial.println("Idle");
      if (switchOn()) {
        robot_state = SWEEP_AREA;
        //set_drive(FORWARDS, 255);
        start_time = millis();
        // Raise arm
        pickup_block(0,50);
      }
      break;
    case MOVE_TO_BLOCKS:
      Serial.println("Move along wall");
      //Turns to align with wall and starts driving
      turn_and_check_left(45, 10);
      set_drive(FORWARDS, drive_speed);
      //Follows wall round to the other side of the arena
      follow_wall(1, 100000, false, lower_wall_bound_1, upper_wall_bound_1, true);
      follow_wall(1 ,150000, true, lower_wall_bound_2, upper_wall_bound_2, false);
      set_drive(STATIONARY, drive_speed);
      ledState=LOW; //Turns off LED now stationary
      delay(1000);

      if(blockSensor()) {
        robot_state = IDENTIFY_BLOCK; 
        set_drive(STATIONARY, drive_speed);
        start_time = millis();
        break;
      }

      drive_with_LED(2500, 10, BACKWARDS);
      set_drive(STATIONARY, drive_speed);
      
      
      ledState=LOW; //Turn off LED now robot has stopped moving
      robot_state = SCAN_BLOCKS;
      break;
    case SWEEP_AREA: 
      Serial.println("Sweep strip ");

      if (sweep_strip()){
        robot_state  = IDENTIFY_BLOCK;
        break;
      }
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
          break;
        }
        // Turn towards block and move ten move to collect
        else if (polar_coor[1] > 0) {turn_and_check_left(polar_coor[1], 10); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
        else {turn_and_check_right(-1 * polar_coor[1], 10); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
      }

      turn_and_check_left(30,10);
      set_drive(STATIONARY, drive_speed);

      if (radar_scan(polar_coor)) {
        // Turn to block if not aligned - move to
        if (polar_coor[1] < anglar_block_tolerance && polar_coor[1] > -anglar_block_tolerance) {
          robot_state = COLLECT_BLOCK;    // Calibration needed so that an angle 0 corresponds to straight ahead
          set_drive(FORWARDS, 255); // Drive at slower speed for collecting block
          Serial.println("Radar identified block");
          Serial.print("Angle - ");
          Serial.println(polar_coor[1]);
          start_time = millis(); // Reset timer for COLLECT_BLOCK
          break;
        }
        // Turn towards block and move ten move to collect
        else if (polar_coor[1] > 0) {turn_and_check_left(polar_coor[1], 10); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
        else {turn_and_check_right(-1 * polar_coor[1], 10); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
      }

      turn_and_check_right(30,10);
      drive_with_LED(800, 10, FORWARDS);
      set_drive(STATIONARY, drive_speed);


      // Rotate and repeat if no block detected
      //else {turn_and_check_left(45,10);Serial.println("No block detected - Rotate");}
      break;
    case COLLECT_BLOCK:
      if (blockSensor()) {
        robot_state = IDENTIFY_BLOCK; // Account for radar offset
        set_drive(STATIONARY, drive_speed);
        start_time = millis();
        break;
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
      //if (!blockSensor()) {robot_state = SCAN_BLOCKS; break;};

      Serial.print ("Block type - ");
      if (block_type_detection()) {red_block = true; Serial.println("Red");}
      else {red_block = false; Serial.println("Blue");}

      find_wall();
      robot_state = MOVE_TO_DROP;

      break;
    case MOVE_TO_DROP:
      //Returns home
      set_drive(FORWARDS, drive_speed);
      follow_wall(1, 100000, false, lower_wall_bound_1, upper_wall_bound_1, true);

      if (red_block){
        follow_wall(1, 100000, false, lower_wall_bound_1, upper_wall_bound_1, true);
        follow_wall(1 ,150000, true, lower_wall_bound_2, upper_wall_bound_2, true);
        deposit_block_and_reverse();
      }
      else{
        follow_wall(1 ,150000, true, lower_wall_bound_2, upper_wall_bound_2, true);
        deposit_block_and_reverse();
      }

      find_wall();
      robot_state = MOVE_TO_BLOCKS;
    
      break;

    case TEST:
      Serial.println(analogRead(line2Pin));
      //if (detect_line())Serial.println("Line");
      //else Serial.println("False");
      delay(100);
      break;
  }
}
