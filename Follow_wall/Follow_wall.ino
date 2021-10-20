#include "marshinator.h"

// Follow around the wall clockwise]

// Uncomment lines with ultrasound_2_list when connected up

// List 1: Left US sensor
// List 2: Front US sensor
track_t ultrasound_1_list, ultrasound_2_list;

void setup() {
  //pinMode(NINA_RESETN, OUTPUT);         
  //digitalWrite(NINA_RESETN, LOW);  

  Serial.begin(9600);  // set up Serial library at 9600 bps
  Serial.println("Initialise program");  
  //SerialNina.begin(115200);

  setup_sensors();
  setup_motors();

  //Initial drive state
  driving_state_t initial_dr = FORWARDS;
  set_drive(initial_dr , drive_speed);
}


void loop(){
 Serial.println("Loop");

/*if (SerialNina.available()){
    char serial_in = SerialNina.read();
    Serial.println(serial_in);
    if(serial_in == '0'){
      Serial.println("Terminate program");
      exit(0);
    }
  }*/

  // Reads left ultrasonic output and adds to the list if the robot not anaomolous (travelling < 1 m/s) - otherwise repeats previous reading
  double ultra_Op = read_ultrasound(1,20);
  double derivative = (ultra_Op - ultrasound_1_list.fetch(0))/delta_t;
  if (ultrasound_1_list.n == 0 || abs(derivative) < 100) ultrasound_1_list.add(ultra_Op);
  else ultrasound_1_list.add(ultrasound_1_list.fetch(0));
  Serial.print("Py ");
  Serial.println(ultrasound_1_list.fetch(0));

  // Reads front ultrasonic output and adds it to list
  ultrasound_2_list.add(read_ultrasound(2,20));

  // Execute right turn if close to the end wall
  if(ultrasound_2_list.fetch(0) < 6) corner_turn();
  
  // Check if within bounds and moving in the right direction -- Correct if otherwise
  if (ultrasound_1_list.n >= 4){
    derivative = calc_finite_difference(ultrasound_1_list, delta_t);

    // Calls turn and pulse function if out of bounds and moving away
    if (derivative >= 0.0 && ultrasound_1_list.fetch(0) > upper_wall_bound) turn_and_pulse(false);
    else if (derivative <= 0.0 && ultrasound_1_list.fetch(0) < lower_wall_bound) turn_and_pulse(true);
  }

  //Print of distance/derivative values
  Serial.print("Derivative - ");
  Serial.print(derivative);
  Serial.print(" - Distance - ");
  Serial.println(ultrasound_1_list.fetch(0)); 
  //Serial.print(" - Front - ");
  //Serial.println(ultrasound_2_list.fetch(0));
  
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
  Serial.println(ultrasound_1_list.fetch(0)); 

  //Turn robot
  if (turn_right) d_state = RIGHT;
  else d_state = LEFT;
  set_drive(d_state, drive_speed);
  delay(300); // turning time for experimentation
  //Reset
  reset_after_turn(4);
}

void corner_turn(void){
  driving_state_t d_state = RIGHT;
  set_drive(d_state, drive_speed);
  //Edit for 90 degree turn
  delay(1000);
  reset_after_turn(4);
}

void reset_after_turn(int N){
   driving_state_t d_state = FORWARDS;
   set_drive(d_state, drive_speed);
   for (int i = 0; i < N; i++){
   delay(delta_t);
   ultrasound_1_list.add(read_ultrasound(1,20));
   ultrasound_2_list.add(read_ultrasound(2,20));
  }  
}
