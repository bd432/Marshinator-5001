#include "marshinator.h"
#include <Arduino.h>


void setup_sensors(void){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(shortIRPin, OUTPUT);
}
  
//Read Ultrasound sensor
double read_ultrasound(int N) {
	double distance, sum = 0.0;
 
  for(int i = 0; i < N; i++){
	  digitalWrite(trigPin, LOW);
	  delayMicroseconds(2);
	  digitalWrite(trigPin, HIGH);
	  delayMicroseconds(10);
	  digitalWrite(trigPin, LOW);
	  distance =  pulseIn(echoPin, HIGH) * 0.0340 / 2.0;
    sum += distance;
    delay(1);
  }
	return sum/N;
}

//Functions for reading the Short IR Sensor
/*double read_shortIR(int average_count) {
  double sum = 0;
  for (int i=0; i<average_count; i++) {
    double sensor_value = analogRead(shortIRPin); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    sum += distance_cm;
  }
  return(sum/average_count);
}*/



double read_shortIR(int average_count) {
//int ir_sensor = A0;
  double distance = round(0.1*average_value(100))*10; //loop 100 times and get its average
  return distance;
}

double average_value(int average_count) {
  double sum = 0;
  for (int i=0; i<average_count; i++) {
    double sensor_value = analogRead(shortIRPin); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    sum = sum + distance_cm;
  }
  return(sum/average_count);
}
