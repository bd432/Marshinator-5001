#include "marshinator.h"

// Follow around the wall clockwise


track_t ultrasound_list;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Initialise program");
  

  setup_sensors();
  setup_motors();
  driving_state_t initial_dr = STATIONARY;
  set_drive(initial_dr , drive_speed);
}


void loop(){
  // Define static variables
  static driving_state_t driving_state = STATIONARY;

  //Reads ultrasonic output and adds to the list if the robot not anaomolous (travelling < 1 m/s) - otherwise repeats previous reading
  double ultra_Op = read_ultrasound(20);
  double der = (ultra_Op - ultrasound_list.fetch(0))/delta_t;
  if (ultrasound_list.n == 0 || abs(der) < 100) ultrasound_list.add(ultra_Op);
  else ultrasound_list.add(ultrasound_list.fetch(0));

  if (ultrasound_list.n >= 4){
    der = calc_finite_difference(ultrasound_list, delta_t);
    //double average = calc_average(ultrasound_list, 4);u

    // Calls turn and pulse function if out of bounds and moving away
    //if (der >= 0.0 && ultrasound_list.fetch(0) > upper_wall_bound) turn_and_pulse(false);
    //else if (der <= 0.0 && ultrasound_list.fetch(0) < lower_wall_bound) turn_and_pulse(true);
  }

  //Print of distance/derivative values
  Serial.print("Derivative - ");
  Serial.print(der);
  Serial.print(" - Distance - ");
  Serial.println(ultrasound_list.fetch(0)); 
  

  double IR_Op = read_shortIR(20);

  Serial.print("Short IR - ");
  Serial.println(IR_Op);
  delay(1000 * delta_t);

}



double calc_finite_difference(track_t list, double dt){ // dt in s

  // Calculates a  derivative using previous 4 points
  if (list.n < 4) return 0;
  double derivative = 0.0, coeff[4] = {11.0, -18.0, 9.0,-2.0};
  for (int i = 0; i < 4; i++) derivative += coeff[i] * list.fetch(i);
  derivative  = derivative /(6.0 * dt);
  return derivative;
}

double calc_average(track_t list, int N){
  
  // Calculates average from last N points
  if (list.n < N) return 0;
  double average = 0.0;
  for(int i = 0; i++; i < N) average += list.fetch(i);
  average = average/N;
  return average;
}

void turn_and_pulse(bool turn_right){
  driving_state_t d_state;

  if(turn_right) Serial.println("Turn right");
  else Serial.println("Turn left");
  Serial.print("Distance - ");
  Serial.println(ultrasound_list.fetch(0)); 

  //Turn robot
  if (turn_right) d_state = RIGHT;
  else d_state = LEFT;
  set_drive(d_state, drive_speed);
  delay(200); // turning time for experimentation
  //Drive forwards
  d_state = FORWARDS;
  set_drive(d_state, drive_speed);
  // Record 4 more pulses before returning to script
  for (int i = 0; i < 4; i++){
    delay(delta_t);
    ultrasound_list.add(read_ultrasound(2));
  }

  
}



  
 
  
