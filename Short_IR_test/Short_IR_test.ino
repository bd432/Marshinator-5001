int ir_sensor = A1;
void setup() {
//initialize serial communications at 9600 bps
Serial.begin(9600);
}
void loop() {
int distance = round(0.1*average_value(100))*10; //loop 100 times and get its average
Serial.println(distance); //print the sensor value
delay(500); //delay 500ms (0.5 second)
}
int average_value(int average_count) {
int sum = 0;
for (int i=0; i<average_count; i++) {
int sensor_value = analogRead(ir_sensor); //read the sensor value
int distance_cm = pow(3027.4/sensor_value, 1.2134); //convert readings to distance(cm)
sum = sum + distance_cm;
}
return(sum/average_count);
}
