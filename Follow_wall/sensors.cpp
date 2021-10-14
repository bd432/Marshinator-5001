#include "marshinator.h"
#include <Arduino.h>


void setup_sensors(void){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(shortIRPin, OUTPUT);
}
  
//Read Ultrasound sensor
long read_ultrasound() {
	long duration, distanceCm;

	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distanceCm = duration * 0.0340 / 2.0;
	return distanceCm;
}

//Functions for reading the Short IR Sensor
double IR_average_value(int average_count) {
  double sum = 0;
  for (int i=0; i<average_count; i++) {
    double sensor_value = analogRead(shortIRPin); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    sum = sum + distance_cm;
  }
return(sum/average_count);
}

double read_shortIR(void) {  
  double distance = IR_average_value(100); //loop 100 times and get its average
  Serial.println(distance); //print the sensor value
  delay(500); //delay 500ms (0.5 second) 
  return distance;
}
