#include "Marshinator_5001.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>



//Function for turning right and simultaneously checking to flash the move LED
void turn_and_check_right(double angle, double dt){

  double total_turn_time = angle * turn_scale_factor_right;
  unsigned long running_turn_time = 0;
  unsigned long start_turn_time = millis();

  //sets robot to start turning right
  set_drive(RIGHT, drive_speed);

  while(running_turn_time < total_turn_time){

    // Loops through at given interval dt to keep flashing the LED when required
    running_turn_time = millis() - start_turn_time;
    LED_check();
    delay(dt);
    }
}

//Function for turning left and simultaneously checking to flash the move LED
void turn_and_check_left(double angle, double dt){
  Serial.println("turn and check left");
  double total_turn_time = angle * turn_scale_factor_left;
  unsigned long running_turn_time = 0;
  unsigned long start_turn_time = millis();

  //sets robot to start turning left
  set_drive(LEFT, drive_speed);

  while(running_turn_time < total_turn_time){

    // Loops through at given interval dt to keep flashing the LED when required
    running_turn_time = millis() - start_turn_time;
    LED_check();
    delay(dt);

    }
}

//Drives forward while flashing the LED
 void drive_with_LED(unsigned long max_duration, double dt, driving_state_t d_state){
  unsigned long duration = 0;
  unsigned long start_time = millis();

  //sets robot to start turning right
  set_drive(d_state, drive_speed);

  while(duration < max_duration){

    // Loops through at given interval dt to keep flashing the LED when required
    duration = millis() - start_time;
    LED_check();
    delay(dt);
  }
}
