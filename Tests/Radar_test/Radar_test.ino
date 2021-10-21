#include "header.h"

double block_location[2];
char serial_in;


void setup() {

  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program");  

  setup_sensors();
  servo.attach(9);  

}

void loop(){
  if (Serial.available()){
    serial_in = Serial.read();
    if(serial_in == '0'){
      block_scan(block_location);
      Serial.print("R = ");
      Serial.println(block_location[0]);
      Serial.print("Theta = ");
      Serial.println(block_location[1]);
    }
  }



}