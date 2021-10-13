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
  double der;

  double ultra_OP = read_ultrasound();
  der = (ultra_OP - ultrasound_list.fetch(0))/delta_t;
  if (ultrasound_list.n == 0 || abs(der) < 100) ultrasound_list.add(ultra_OP);
  else ultrasound_list.add(ultrasound_list.fetch(0));

  


  //ultrasound_list.add(read_ultrasound());

  if (ultrasound_list.n > 4){
    der = calc_finite_difference(ultrasound_list, delta_t);
    double average = calc_average(ultrasound_list, 4);
    
    if (der > 0.0 && average > upper_wall_bound){ 
      turn_reset(false);
    }
    else if (der < 0.0 && average < lower_wall_bound) turn_reset(true);
  }

  Serial.print("Derivative - ");
  Serial.print(der);
  Serial.print(" ; Distance - ");
  Serial.println(ultrasound_list.fetch(0));

  delay(1000 * delta_t);

}



double calc_finite_difference(track_t list, double dt){ // dt in s
  if (list.n < 4) return 0;
  double derivative = 0.0;
  derivative += 11.0 * list.fetch(0);
  derivative += -18.0 * list.fetch(1);
  derivative += 9.0 * list.fetch(2);
  derivative += -2.0 * list.fetch(3);
  
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

  if(turn_right) Serial.println("Turn right");
  else Serial.println("Turn left");
  
}



  
 
  
