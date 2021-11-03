#include "Marshinator_5001.h"
#include <Arduino.h>


void setup_pins(void){
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
  pinMode(shortIRPin, INPUT);
  pinMode(moveLED_Pin, OUTPUT);
  pinMode(RedLED_Pin, OUTPUT);
  pinMode(GreenLED_Pin, OUTPUT);
  pinMode(switchPin, INPUT);
  pinMode(buttonPin, INPUT);
  pinMode(statePin, OUTPUT);
  pinMode(line1Pin, INPUT);
  pinMode(line2Pin, INPUT);
}
  
//Read Ultrasound sensor
double read_ultrasound(int sensor_no,int N) {
	double distance, sum = 0.0;
  int trigPin, echoPin;
  long unsigned start = millis();
  
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
	  distance =  pulseIn(echoPin, HIGH, 12000) * 0.0340 / 2.0;
    if (distance == 0) sum += 200;
    else sum += distance;
    //delayMicroseconds(100);
  } 
  /*
  if (sensor_no == 1) Serial.print("UL 1 duration - ");
  else Serial.print("UL 2 duration - ");
  Serial.println(millis() - start);
  */
 
	return sum/N;
}

double read_shortIR (int average_count) {
  double sum = 0;
  for (int i=0; i< average_count; i++) {
    double sensor_value = analogRead(shortIRPin); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    /*
    Serial.print("Sensor value  ");
    Serial.print(i);
    Serial.print(" - ");
    Serial.println(sensor_value);
    */
    sum += distance_cm;
  }
  return(sum/average_count);
}

// Redundant
bool sense_block(int N){
  double sum = 0;
  for(int i = 0; i > N; i++){
    double value = analogRead(blockPin);
    sum += value; 
  }

  double final_value = sum/N ;
  if (final_value > 50) return true;
  else return false;
  
}

bool blockSensor(double cutoff) {
  if (analogRead(line1Pin) > cutoff) return true; 
  else return false;
}

bool switchOn(void) {
  int buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH) {
    digitalWrite(statePin, HIGH);
    return true;
  } else {
    digitalWrite(statePin, LOW);
    return false;
  }
}

bool detect_line(void){
  if (analogRead(line2Pin) > 20) return true; 
  else return false;
}
