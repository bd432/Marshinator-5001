#include "marshinator.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

//Turns the robot right by a given angle
void turn_right(double angle){
  double turn_time = angle * turn_scale_factor;
  driving_state_t d_state = RIGHT;
  delay(turn_time);
  d_state = FORWARDS;
}


//Turns the robot left by a given angle
void turn_left(double angle){
  double turn_time = angle * turn_scale_factor;
  driving_state_t d_state = LEFT;
  delay(turn_time);
  d_state = FORWARDS;
}
   
