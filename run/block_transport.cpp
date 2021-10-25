#include <Arduino.h>
#include "Marshinator_5001.h"
#include <Adafruit_MotorShield.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *ArmMotor = AFMS.getMotor(arm_motor);

void setup_arm(void){
  ArmMotor->setSpeed(drive_speed);
}

// Function that gets the arm to descend through an angle to attach to the block 
// and then raise through an angle to pick the block up
void pickup_block(double descent_angle ,double ascent_angle){
 ArmMotor -> run(FORWARD);
 delay(descent_angle * arm_scale_factor);
 ArmMotor -> run(RELEASE);
 delay(500);
 ArmMotor -> run(BACKWARD);
 delay(ascent_angle * arm_scale_factor);
}

// Function that gets the arm to raise further to drop the block off using the shell mechanism 
// and then return the arm to its original position
void block_dropoff_and_reset (double ascent_angle, double descent_angle){
  ArmMotor -> run(BACKWARD);
  delay(ascent_angle * arm_scale_factor);
  ArmMotor -> run(RELEASE);
  delay(500);
  ArmMotor -> run(FORWARD);
  delay(descent_angle * arm_scale_factor);
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
  bool block_present = sense_block();
  if (block_present) return true;
  else return false;
}

bool block_detection(void){
  // return if block detected by front line sensor
}
