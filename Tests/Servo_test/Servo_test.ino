#include <Servo.h>              //Servo library

Servo servo_test;        //initialize a servo object for the connected servo  

int angle = 0;    

void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Initiate program");
  servo_test.attach(10);      // attach the signal pin of servo to pin9 of arduino
} 

void loop() { 
  /*
  for(int angle = 0; angle < 360; angle += 1){    // command to move from 0 degrees to 180 degrees 
    servo_test.write(angle);                 //command to rotate the servo to the specified angle
    delay(30);                       
  } 
  Serial.println("1");
  delay(250);
  for(int angle = 360; angle>=1; angle-=5)     // command to move from 180 degrees to 0 degrees 
  {                               
    servo_test.write(angle);              //command to rotate the servo to the specified angle
    delay(30);                       
  } 
  Serial.println("2");
  delay(250);
  */

  /*
  for (int i = 20; i < 180; i++){
    servo_test.write(i);
    Serial.println(i);
    delay(30);
  }
  */

  servo_test.write(30);
  Serial.println(30);
  delay(500);

  servo_test.write(130);
  //Serial.println(150);
  delay(500);



}