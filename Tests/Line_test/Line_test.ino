int linePin =  A3;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program"); 
  pinMode(linePin, INPUT);
}

void loop() {
  // Put your main code here, to run repeatedly:
  Serial.println(analogRead(linePin));
  delay(100);
}

bool lineSensor(void) {
  if (analogRead(linePin) > 100) return true; 
  else return false;
}