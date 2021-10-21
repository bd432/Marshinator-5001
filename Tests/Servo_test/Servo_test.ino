#include <Servo.h>              //Servo library

Servo servo_test;        //initialize a servo object for the connected servo  

int angle = 0;    

void setup() 
{ 
  Serial.begin(9600);
  Serial.println("Initiate program");
  servo_test.attach(9);      // attach the signal pin of servo to pin9 of arduino
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
  for (int i = 0; i < 90; i++){
  Serial.println(i);
  //servo_test.write(0);
  //delay(250);
  servo_test.write(180-i);
  delay(30);
  }


}