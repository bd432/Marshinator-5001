
#include <Adafruit_MotorShield.h>


Adafruit_MotorShield AFMS = Adafruit_MotorShield();

Adafruit_DCMotor *Motor1 = AFMS.getMotor(1);
Adafruit_DCMotor *Motor2 = AFMS.getMotor(2);

class track_t {
  // Class that acts as 100 long list
  // add()ios the same as append
  // fetch retrieves from the end - fetch(0) retrueves last entry ...
  // writes over first entries when above 100
  public:
    unsigned short n;
    void add( double x){pos[p] = x; p++; n ++; if(n > N_MAX) n = 0; if(p == N_MAX) p = 0;}
    double fetch(int i){int j = p - j - 1; if(j < 0) j+= N_MAX; return pos[j];}

  private:
    const int N_MAX = 100;
    unsigned short p;
    double pos[100];
    
    
};

track_t ultrasound_list;


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Reset Motor test");

  if (!AFMS.begin()) {         // create with the default frequency 1.6KHz
    Serial.println("Could not find Motor Shield. Check wiring.");
    while (1);
  }
  Serial.println("Motor Shield found.");

  // Set motors to stop
  Motor1->setSpeed(255);
  Motor2->setSpeed(255);
  Motor1->run(RELEASE);
  Motor2->run(RELEASE);
}


void loop(){
  static int driving_state = 0;
  ultrasound_list.add(read_ultrasound());

  // Calculate orientation?
  
  

  
  
  
}

long read_ultrasound(){
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
