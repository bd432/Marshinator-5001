#include "marshinator.h"
#include <Arduino.h>

long read_ultrasound() {
	//Set pins for Ulrasound
	long duration, distanceCm;

	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distanceCm = duration * 0.0340 / 2;
	return distanceCm;
}

void setup_sensors(void){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
