int ir_sensor = A2;

void setup() {
//initialize serial communications at 9600 bps
Serial.begin(9600);
}
void loop() {
  //double distance = round(0.1*average_value(100))*10; //loop 100 times and get its average
  //read_ShortIR(1);
  Serial.print("Py ");
  Serial.println(read_ShortIR(100)); //print the sensor value
  //Serial.println(analogRead(ir_sensor));
  delay(100); 
}

double read_ShortIR(int average_count) {
  double sum = 0;
  for (int i=0; i<average_count; i++) {
    double sensor_value = analogRead(ir_sensor); //read the sensor value
    double distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
    sum += distance_cm;
    //Serial.print("Py ");
    //Serial.println(sensor_value);
  }
  return(sum/average_count);
}
