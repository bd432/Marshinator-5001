#include "marshinator.h"

track_t ultrasound_list;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initialise program");

  setup_motors();
}


void loop(){
  static int driving_state = 0;
  ultrasound_list.add(read_ultrasound());

  // Calculate orientation?
  
  read_ultrasound();
}


  
