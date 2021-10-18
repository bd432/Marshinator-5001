#include "marshinator.h"
#include <Arduino.h>


void setup_sensors(void){
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(shortIRPin, INPUT);
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

<<<<<<< HEAD
//Function for reading the Short IR Sensor
=======
//Functions for reading the Short IR Sensor
<<<<<<< HEAD
>>>>>>> 58997246a6c8d99740cb5cd845d379fba106fda0
=======
>>>>>>> parent of 79af727 (Monday Lab)
double read_shortIR(int average_count) {
  double sum = 0;
  for (int i=0; i<average_count; i++) {
    double sensor_value = analogRead(shortIRPin); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    sum += distance_cm;
  }
<<<<<<< HEAD
double distance =(sum/average_count);
Serial.println(distance);
=======
  return(sum/average_count);
>>>>>>> 58997246a6c8d99740cb5cd845d379fba106fda0
}
