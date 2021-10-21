#define sensor A0 // Sharp IR GP2Y0A02YK0F (20-150cm, analog)

#include <Arduino.h>

int N = 20;
double sum, distance;

void setup() {
  Serial.begin(9600); // start the serial port
}

void loop() {

  sum = 0;
  for (int i =0; i < N ; i++){
  // 5v
    float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  // int distance = 13*pow(volts, -1); // worked out from datasheet graph
    sum += 50*pow(volts, -1.173);
     // slow down serial port 
  }
  distance = sum/N;

  Serial.print("Py ");
  Serial.println(distance);   // print the distance
  delay(100);
}
