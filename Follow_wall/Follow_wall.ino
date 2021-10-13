#include "marshinator.h"

// Follow around the wall clockwise
track_t ultrasound_list;
void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initialise program");
  

  setup_sensors();
  setup_motors();
}


void loop(){
  // Define static variables
  static driving_state_t driving_state = FORWARDS;
  //static int drive_speed = 255;
  //static track_t ultrasound_list;


  ultrasound_list.add(read_ultrasound());

  if (ultrasound_list.n > 4){
    double der = calc_finite_difference(ultrasound_list, delta_t);
    double average = calc_average(ultrasound_list, 4);
    
    if (der > 0.0 && average > upper_wall_bound){ 
      turn_reset(false);
    }
    else if (der < 0.0 && average < lower_wall_bound) turn_reset(true);
  }

}



double calc_finite_difference(track_t list, double dt){ // dt in s
  if (list.n < 4) return 0;
  double derivative = 0.0;
  derivative += 11 * list.fetch(0);
  derivative += -18 * list.fetch(1);
  derivative += 9 * list.fetch(2);
  derivative += -2 * list.fetch(3);
  derivative  = derivative /(6.0 * dt);
  return derivative;
}

double calc_average(track_t list, int N){
  if (list.n < N) return 0;
  double average = 0.0;
  for(int i = 0; i++; i < N) average += list.fetch(i);
  average = average/N;
  return average;
}

void turn_reset(bool turn_right){
  driving_state_t d_state;
  if (turn_right) d_state = RIGHT;
  else d_state = LEFT;
  set_drive(d_state, drive_speed);
  delay(300); // turning time for experimentation
  d_state = FORWARDS;
  set_drive(d_state, drive_speed);
  for (int i = 0; i < 4; i++){
    delay(delta_t);
    ultrasound_list.add(read_ultrasound());
  }
}



  
 
  
