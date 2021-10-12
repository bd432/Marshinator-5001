
#include <Adafruit_MotorShield.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);

int driving_state = 0;
char serial_in;

// Send commands to teh arduino by typing 0-4 in the Serial link
// 0 = Stationary
// 1 = Forwards
// 2 = Backwards
// 3 = Steer right
// 4 = Steer left


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Reset Motor test");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
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

void loop() {
  if (Serial.available()){
    serial_in = Serial.read();
    if(serial_in >= '0' && serial_in <= '4'){
      driving_state = (serial_in - '0');
      Serial.println(driving_state);
      set_drive(driving_state);
    }
  }
}

void set_drive(int state){
  Motor1->setSpeed(255);
  Motor2->setSpeed(255);

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
