#include <Arduino.h>
#include "Marshinator_5001.h"
#include <Adafruit_MotorShield.h>

// Tests the block pickup and dropoff mechanism as a whole 
// to make sure the arm correctly returns to its original position
void arm_test (double angle1, double angle2, double angle3, double angle4){
  pickup_block(angle1, angle2);
  block_dropoff_and_reset(angle3, angle4);
}

// Tests whether the block contains metal or not by attempting to lift block and seeing
// if it remains in place after attempted lift
// If function returns true, block contains metal
bool block_type_detection(void){
  // Raise arm
  pickup_block(0,70);
  // Pickup block
  pickup_block(descent_angle_pickup, ascent_angle_pickup);
  delay(100);
  bool block_present = blockSensor(60);
  if (block_present) {
    digitalWrite(GreenLED_Pin, HIGH);
    delay(5000);
    digitalWrite(GreenLED_Pin, LOW);
    // Raise arm
    pickup_block(0,30);
    // Lower to trap block
    pickup_block(150,0);
    return false;
  }
  else {
    digitalWrite(RedLED_Pin, HIGH);
    delay(5000);
    digitalWrite(RedLED_Pin, LOW);
    // Raise arm
    pickup_block(0,30);
    //Lower arm to trap block
    pickup_block(150,0);
    return true;
  }
}


void deposit_block_and_reverse(void){
  set_drive(STATIONARY, drive_speed);
  ledState = LOW;
  pickup_block(0,300);
  block_dropoff_and_reset(150,0);
  drive_with_LED(800, 10, BACKWARDS);
  turn_and_check_left(90,10, false);
  set_drive(STATIONARY, drive_speed);
  ledState = LOW; 
}
