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
  
  bool block_present = blockSensor(60); // Checks if block is still in front of sensor after attempted pickup
  if (block_present) {
    digitalWrite(GreenLED_Pin, HIGH); // Flashes the green LED to show block is non metal
    delay(5000);
    digitalWrite(GreenLED_Pin, LOW);
    pickup_block(0,30); // Raises arm
    pickup_block(150,0);  // Lowers arm to trap block
    return false; // Returns false if block is still present
  }
  else {
    digitalWrite(RedLED_Pin, HIGH); // Flashes the red LED to show block contains metal
    delay(5000);
    digitalWrite(RedLED_Pin, LOW);
    pickup_block(0,30); // Raises arm
    pickup_block(150,0);  // Lowers arm to trap block
    return true; // Returns true if block has been successfully picked up by magnet
  }
}


// Function to deposit block after reaching the required drop zone
// And then move away from drop zone without hitting block 
void deposit_block_and_reverse(void){
  set_drive(STATIONARY, drive_speed); // Stops and turns off move LED
  ledState = LOW; 
  pickup_block(0,300); // Moves arm upwards to release block(non-metal)/dislodge block from magnet(metal)
  block_dropoff_and_reset(150,0); // Raises arm further to make sure metal block is dislodged and then resets arm
  drive_with_LED(800, 10, BACKWARDS); // Reverses out and turns 90 degrees to face wall
  turn_and_check_left(90,10, false);
  set_drive(STATIONARY, drive_speed);
  ledState = LOW; 
}
