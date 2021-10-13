#include "marshinator.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>

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
    Motor1->setSpeed(255);
    Motor2->setSpeed(255);
    Motor1->run(RELEASE);
    Motor2->run(RELEASE);
}

void set_drive(int state){
  Motor1->setSpeed(drive_speed);
  Motor2->setSpeed(drive_speed);

  switch (state){
    case 0:
      Motor1->run(RELEASE);
      Motor2->run(RELEASE);
      break;
    case 1:
      Motor1->run(FORWARD);
      Motor2->run(FORWARD);
      break;    
    case 2:
      Motor1->run(BACKWARD);
      Motor2->run(BACKWARD);
      break;    
    case 3:
      Motor1->run(FORWARD);
      Motor2->run(BACKWARD);
      break;    
    case 4:
      Motor1->run(BACKWARD);
      Motor2->run(FORWARD);
      break;    
  }

  
}
