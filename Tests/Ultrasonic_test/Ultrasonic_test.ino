#include <Wire.h>

const int trigPin = 0;
const int echoPin = A0;
double duration;
double distanceCm, distanceInch;
double IROutput;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Initiate program");

}
void loop() {
  // Ultrasonic test
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.0340 / 2;

  Serial.print("Py ");
  Serial.println(distanceCm);
  //Serial.print("Duration - ");
  //Serial.println(duration);
  //Serial.print(distanceCm);
  //Serial.println(" cm");


  //IR sensor
  //IROutput = analogRead(A2);
  //Serial.print("IR - ");
  //println(29.988 * pow( IROutput * 5.0/ 1023.0 ,-1.173));
  //Serial.println(IROutput);
  delay(100);
}
