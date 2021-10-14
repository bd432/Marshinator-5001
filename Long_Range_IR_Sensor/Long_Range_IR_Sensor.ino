#define sensor A0 // Sharp IR GP2Y0A02YK0F (20-150cm, analog)

void setup() {
  Serial.begin(9600); // start the serial port
}

void loop() {
  
  // 5v
  float volts = analogRead(sensor)*0.0048828125;  // value from sensor * (5/1024)
  // int distance = 13*pow(volts, -1); // worked out from datasheet graph
  int distance = 50*pow(volts, -1.173);
  delay(1000); // slow down serial port 
  
  Serial.println(distance);   // print the distance
}
