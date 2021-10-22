#include "header.h"
#include <Arduino.h>


void setup_sensors(void){
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(shortIRPin, INPUT);
}
  
//Read Ultrasound sensor
double read_ultrasound(int sensor_no,int N) {
	double distance, sum = 0.0;
  int trigPin, echoPin;
  
  // Sensor input -- 1: Left; 2: Front;
  if(sensor_no == 1){
    //Serial.println("Read US 1");
    trigPin = trigPin1;
    echoPin = echoPin1;
  }
  else {
    //Serial.println("Read US 2");
    trigPin = trigPin2;
    echoPin = echoPin2;
  }
 
  for(int i = 0; i < N; i++){
	  digitalWrite(trigPin, LOW);
	  delayMicroseconds(2);
	  digitalWrite(trigPin, HIGH);
	  delayMicroseconds(10);
	  digitalWrite(trigPin, LOW);
	  distance =  pulseIn(echoPin, HIGH) * 0.0340 / 2.0;
    sum += distance;
    delay(10);
  }
 
	return sum/N;
}

double read_shortIR (int average_count) {
  double sum = 0;
  for (int i=0; i< average_count; i++) {
    double sensor_value = analogRead(shortIRPin); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    sum += distance_cm;
  }
  return(sum/average_count);
}
