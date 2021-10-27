#include <Arduino.h>
#include "Marshinator_5001.h"
#include <Adafruit_MotorShield.h>



//Adafruit_DCMotor *ArmMotor = AFMS.getMotor(arm_motor);

void setup_arm(void){
  
}



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
  pickup_block(descent_angle_pickup, ascent_angle_pickup);
  bool block_present = sense_block(10);
  if (block_present) {
    digitalWrite(blockLED_Pin, HIGH);
    delay(500);
    digitalWrite(moveLED_Pin, HIGH);
    return true;
  }
  else return false;
}

bool block_detection(void){
  // return if block detected by front line sensor
}