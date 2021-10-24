#include "Marshinator_5001.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);

void setup_motors(void){
    if (!AFMS.begin()) {
        Serial.println("Could not find Motor Shield. Check wiring.");
        while (1);
    }
    Serial.println("Motor Shield found.");
    // Set motors to stop
    Motor1->setSpeed(drive_speed);
    Motor2->setSpeed(drive_speed);
    Motor1->run(RELEASE);
    Motor2->run(RELEASE);
    servo.attach(9);   
}

void set_drive(driving_state_t state, int d_speed){
  Motor1->setSpeed(d_speed);
  Motor2->setSpeed(d_speed);

  switch (state){
    case STATIONARY:
      Motor1->run(RELEASE);
      Motor2->run(RELEASE);
      break;
    case FORWARDS:
      Motor1->run(FORWARD);
      Motor2->run(FORWARD);
      break;    
    case BACKWARDS:
      Motor1->run(BACKWARD);
      Motor2->run(BACKWARD);
      break;    
    case LEFT:
      Motor1->run(FORWARD);
      Motor2->run(BACKWARD);
      break;    
    case RIGHT:
      Motor1->run(BACKWARD);
      Motor2->run(FORWARD);
      break;    
  }
}
