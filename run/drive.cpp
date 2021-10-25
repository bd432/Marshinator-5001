#include "Marshinator_5001.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

//Turns the robot right by a given angle
void turn_right(double angle, bool forward_after_turn){ 
  //the forward_after_turn variable allows this function to be 
  //reused in the turn and check functions without causing the robot to 
  //change its drive state at the end of each microturn loop 
  double turn_time = angle * turn_scale_factor;
  driving_state_t d_state = RIGHT;
  delay(turn_time);
  if(forward_after_turn){
   d_state = FORWARDS;
   set_drive(d_state, drive_speed);
  }
}


//Turns the robot left by a given angle
void turn_left(double angle, bool forward_after_turn){
  double turn_time = angle * turn_scale_factor;
  driving_state_t d_state = LEFT;
  delay(turn_time);
  if(forward_after_turn){
    d_state = FORWARDS;
    set_drive(d_state, drive_speed);
  }
}

//Function for turning right and simultaneously checking to flash the move LED
void turn_and_check_right(double angle, double angular_resolution){
  double angle_turned = 0 ;
  for(angle_turned = 0; angle_turned < angle; angle_turned+=angular_resolution){
    
    // Turns by a small amount and checks to see if the LED needs flashing
    turn_right(angular_resolution, false);
    LED_check();
    }
}

//Function for turning left and simultaneously checking to flash the move LED
void turn_and_check_left(double angle, double angular_resolution){
  double angle_turned = 0;
  for(angle_turned= 0; angle_turned < angle; angle_turned += angular_resolution){
    //turns by a small amount and checks to see if the LED needs flashing 
    //loops over this until the full angle has been turned
    turn_right(angular_resolution, false);
    LED_check();
    }
}

//Drives forward while flashing the LED
 void drive_with_LED(unsigned long duration, double resolution, driving_state_t d_state){
  double moving_time = 0 ;
  
  //Loops round checking the LED while driving forwards
  //The length of each loop depends on the resolution given in ms
  for(moving_time=0; moving_time < duration; moving_time+= resolution){
    set_drive(d_state, drive_speed);
    LED_check();
    delay(resolution);
  }
}
