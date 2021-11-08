#include "Marshinator_5001.h"

// List 1: Left US sensor
// List 2: Front US sensor

sensor_list_t ultrasound_1_list, ultrasound_2_list;
robot_state_t robot_state = IDLE;

void setup() {

  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program"); 

  // Setup inputs/output
  setup_pins();
  setup_motors();
  setup_LED();

  //Initial drive state
  driving_state_t initial_dr = STATIONARY;
  set_drive(initial_dr , drive_speed);
}


void loop(){
  // Static variables
  static double polar_coor[2];
  static unsigned long start_time;
  static bool red_block;

  switch (robot_state){
    case IDLE:
    // Initial Idle cases exits when button is pressed 
      set_drive(STATIONARY, drive_speed);
      Serial.println("Idle");
      if (switchOn()) {
        robot_state = MOVE_TO_BLOCKS;
        start_time = millis();
        
        pickup_block(0,50); // Raise arm
        turn_and_check_left(45, 10, false); //turns to align with wall
      }
      break;
    case MOVE_TO_BLOCKS:
      Serial.println("Move along wall");

      set_drive(FORWARDS, drive_speed);
      // Follows wall round to the other side of the arena
      follow_wall(1, 18000, false, lower_wall_bound_1, upper_wall_bound_1, true);
      follow_wall(1 ,18000, true, 10, 15, false);
      set_drive(STATIONARY, drive_speed);
      ledState=LOW; //Turns off LED now stationary
      delay(1000);

      // Exits Follow wall after detecteing white line
      if(blockSensor(50)) {
        // Skips straight to identifying block if block already in the arms
        robot_state = IDENTIFY_BLOCK; 
        set_drive(STATIONARY, drive_speed);
        start_time = millis();
        break;
      }
      
      // Reverse initially before entering "SWEEP_AREA"
      drive_with_LED(2500, 10, BACKWARDS);
      set_drive(STATIONARY, drive_speed);
      ledState=LOW; //Turn off LED now robot has stopped moving
      
      robot_state = SWEEP_AREA;
      break;
      
    case SWEEP_AREA: 
      Serial.println("Sweep strip ");
      // Runs sweep strip and adjusts to the right 4 times 
      for (int i = 0; i < 4; i++){
        if (sweep_strip()){
          // Skip to "IDENTIFY_BLOCK" if block detecteed in sweep_strip()
          robot_state  = IDENTIFY_BLOCK;
          ledState = 0;
          break;
        }
      }
      // Runs sweep strip and adjusts to the left 4 times 
      for (int i = 0; i < 4; i++){
        if (sweep_strip_left()){
          // Skip to "IDENTIFY_BLOCK" if block detecteed in sweep_strip_left()
          robot_state  = IDENTIFY_BLOCK;
          ledState = 0;
          break;
        }
      }
      break;
    case SCAN_BLOCKS:
      // REDUNDANT CODE; replaced by SWEEP_AREA 
      // Scans radar and rotates or moves forwards until block is detected
      // radar_scan() return polar coor of block if detected
      // Robot moves rotates and collects block if detected


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
        else if (polar_coor[1] > 0) {turn_and_check_left(polar_coor[1], 10, false); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
        else {turn_and_check_right(-1 * polar_coor[1], 10, false); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
      }

      turn_and_check_left(30,10, false);
      set_drive(STATIONARY, drive_speed);
      ledState = LOW; //turns off move LED now robot is stationary

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
        else if (polar_coor[1] > 0) {turn_and_check_left(polar_coor[1], 10, false); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
        else {turn_and_check_right(-1 * polar_coor[1], 10, false); robot_state = COLLECT_BLOCK; set_drive(FORWARDS, 255); start_time = millis(); break;}
      }

      turn_and_check_right(30,10, false);
      drive_with_LED(800, 10, FORWARDS);
      set_drive(STATIONARY, drive_speed);
      ledState=LOW; 


      // Rotate and repeat if no block detected
      //else {turn_and_check_left(45,10, false);Serial.println("No block detected - Rotate");}
      break;
    case COLLECT_BLOCK:
      // REDUNDANT CODE; replaced by SWEEP_AREA 
      // Robot moves to collect block after radar_scan
      // If timeout exceeded due to not detecting the block, program returns to radar_scan

      if (blockSensor(60)) {
        robot_state = IDENTIFY_BLOCK;
        set_drive(STATIONARY, drive_speed);
        start_time = millis();
        break;
      }
      // Return to Scan blocks if time out
      if (millis() - start_time > collect_block_timout * 1000){
        set_drive(BACKWARDS, drive_speed);
        delay(100);
        turn_and_check_right(180, 10, false);
        set_drive(STATIONARY, drive_speed);
        robot_state = SCAN_BLOCKS;
      } 
     break;


    case IDENTIFY_BLOCK:
    ledState = 0;

      if (!blockSensor(50)) {
        // Program returns to sweep area if no block detected
        drive_with_LED(800, 5, BACKWARDS);
        robot_state = SWEEP_AREA; break;
      }
      // Set variable red_block from result of block_type_detectio()
      // Function uses the arm to lift the block and then assesses whether the block is still in front of the sensor
      Serial.print ("Block type - ");
      if (block_type_detection()) {red_block = true; Serial.println("Red");}
      else {red_block = false; Serial.println("Blue");}

      // Uses sensors to identify where teh wall is, rotates towards it and moves forawrds until adjacent and executes a 90 degree right turn
      find_wall();
      //Moves program to MOVE_TO_DROP
      robot_state = MOVE_TO_DROP;

      break;
    case MOVE_TO_DROP:
      // Follows the wall and turns right once
      set_drive(FORWARDS, drive_speed);
      follow_wall(1, 18000, false, lower_wall_bound_1, upper_wall_bound_1, true);

      if (red_block){
        // If the block is Red:
        // Move follow the wall again utnil the corner and turn
        follow_wall(1, 18000, false, lower_wall_bound_1, upper_wall_bound_1, true);
        // Turn 45 degrees and move until in the red square
        turn_and_check_right(45, 10, false);
        drive_with_LED(2000, 10, FORWARDS);
        // Lifts the arm max amount so the block is dropped off
        // Reverse and turn so the block is not in the arms
        deposit_block_and_reverse();
        // Move towards the wall and turn 90 
        turn_and_check_left(50, 10, false);
        move_until_corner_turn(10);
      }
      else{
        // If the block is Blue:
        // Follows the wall but with wider bounds (20-35 cm)
        // Exits follow wall when the robot hits the blue line
        follow_wall(1 ,18000, true, 20, 35, true);
        // Lifts the arm max amount so the block is dropped off
        // Reverse and turn so the block is not in the arms
        deposit_block_and_reverse();
        //Move towards the wall and follow it until the first corner
        move_until_corner_turn(10);
        follow_wall(1, 18000, false, lower_wall_bound_1, upper_wall_bound_1, true);

      }
      // Reset program by moving to MOVE_TO_BLOCKS
      robot_state = MOVE_TO_BLOCKS;
    
      break;

    case TEST:
      // Robot state just used for testing the integration of different functions without running the whole script
      turn_and_check_left(1800, 10, false);
      set_drive(STATIONARY, drive_speed);
      delay(10000);
      turn_and_check_right(1800, 10, false);

      break;
  }
}
