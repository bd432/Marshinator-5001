#include <Arduino.h>
//#include <Adafruit_MotorShield.h>

// Pin i/o ports
#define trigPin 0
#define echoPin A0

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

enum driving_state

// Function Prototypes
void setup_motors(void);
long read_ultrasound(void);
