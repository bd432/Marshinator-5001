#include "Marshinator_5001.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>



//Function for turning right by a given angle and simultaneously checking to flash the move LED
//Also checks the line detector repeatedly if line_break parameter is set to true
bool turn_and_check_right(double angle, double dt, bool line_break){

  double total_turn_time = angle * turn_scale_factor_right; //calculates the time it takes to turn the given angle
  unsigned long running_turn_time = 0;
  unsigned long start_turn_time = millis(); //sets the start time to the current time 

  //sets robot to start turning right
  set_drive(RIGHT, drive_speed);

  //Iterates through repeatedly to check if the move LED needs flashing
  //until the time the function has been running reaches the calculated turn time
  while(running_turn_time < total_turn_time){
    if(line_break && detect_line()) return true; //breaks out if the loop if a line is detected (if line_break parameter is true)
    running_turn_time = millis() - start_turn_time; 
    LED_check();
    delay(dt);
  }
  return false;
}

//Function for turning left and simultaneously checking to flash the move LED
//Also checks the line detector repeatedly if line_break parameter is set to true
bool turn_and_check_left(double angle, double dt, bool line_break){

  double total_turn_time = angle * turn_scale_factor_left; //calculates the time it takes to turn a given angle
  unsigned long running_turn_time = 0;
  unsigned long start_turn_time = millis(); //sets the start time to the current time

  //sets robot to start turning left
  set_drive(LEFT, drive_speed);

  
  //Iterates through repeatedly to check if the move LED needs flashing
  //until the time the function has been running reaches the calculated turn time
  while(running_turn_time < total_turn_time){
    if(line_break && detect_line()) return true; //breaks out if the loop if a line is detected (if line_break parameter is true)
    running_turn_time = millis() - start_turn_time;
    LED_check();
    delay(dt);
  }
  return false;
}

//Drives in a given direction while flashing the move LED
 void drive_with_LED(unsigned long max_duration, double dt, driving_state_t d_state){
  unsigned long duration = 0;
  unsigned long start_time = millis(); //sets start time to current_time 

  //sets robot to specified drive state
  set_drive(d_state, drive_speed);

  while(duration < max_duration){

    // Loops through at given interval dt to keep flashing the LED when required
    duration = millis() - start_time;
    LED_check();
    delay(dt);
  }
}
