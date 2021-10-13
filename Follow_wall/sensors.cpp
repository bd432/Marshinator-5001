#include <Arduino.h>

int trigPin = 0;
int echoPin = A0;

long read_ultrasound(void) {
	//Set pins for Ulrasound

	//pinMode(trigPin, OUTPUT);
	//pinMode(echoPin, INPUT);

	long duration, distanceCm;

	digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = duration * 0.0340 / 2;
	return distanceCm; }
