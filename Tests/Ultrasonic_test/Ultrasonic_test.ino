#include <Wire.h>

const int trigPin = 5;
const int echoPin = 4;
double duration;
double distanceCm, distanceInch;
double IROutput;
int N = 1;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
  Serial.println("Initiate program");

}
void loop() {
  // Ultrasonic test
  //unsigned long start = millis();

  double distance, sum = 0.0;
  //delay(20);

  for(int i = 0; i < N; i++){
	  digitalWrite(trigPin, LOW);
	  delayMicroseconds(2);
	  digitalWrite(trigPin, HIGH);
	  delayMicroseconds(10);
	  digitalWrite(trigPin, LOW);
	  distance =  pulseIn(echoPin, HIGH, 5) * 0.0340 / 2.0;
    if (distance == 0) sum += 200;
    else sum += distance;
    //delayMicroseconds(100);
  } 

  Serial.print("Py ");
  Serial.println(sum/N);
  //Serial.println(millis() - start);
  
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
