#include "header.h"

double block_location[2];
char serial_in;


void setup() {

  Serial.begin(4800);  // set up Serial library at 9600 bps
  Serial.println("Initialise program");  

  setup_sensors();
  servo.attach(9);  

}

void loop(){
  if (Serial.available()){
    serial_in = Serial.read();

    Serial.println(serial_in);
    if(serial_in == '0'){
      block_scan(block_location);
      //Serial.print("R = ");
      //Serial.println(block_location[0]);
      //Serial.print("Theta = ");
      //Serial.println(block_location[1]);
    }

    if(serial_in == '1'){
      for (int i=0; i < 100; i++){
        //read_shortIR(1);
        Serial.print("IR out = ");
        Serial.println(read_shortIR(100));
        delay(30);
      }
    }
  }

  //Serial.println(" ");
}