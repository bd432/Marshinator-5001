#include <Wire.h>

const int trigPin = 0;
const int echoPin = A0;
long duration;
int distanceCm, distanceInch;
void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.0340 / 2;
  distanceInch = duration * 0.01330 / 2;
  Serial.print(distanceCm);
  Serial.println(" cm");

  delay(1000);
}
