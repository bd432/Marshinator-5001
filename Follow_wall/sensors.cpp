long read_ultrasound() {
	//Set pins for Ulrasound
	const int trigPin = 0;
	const int echoPin = A0;

	long duration, distanceCm;


	digitalWrite(trigPin, LOW);
	delayMicroseconds(2);
	digitalWrite(trigPin, HIGH);
	delayMicroseconds(10);
	digitalWrite(trigPin, LOW);
	duration = pulseIn(echoPin, HIGH);
	distanceCm = duration * 0.0340 / 2;
	return distanceCm;
}
