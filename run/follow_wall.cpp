#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include "Marshinator_5001.h"


double calc_finite_difference(sensor_list_t list, double dt){ // dt in s

  // Calculates a  derivative using previous 4 points
  if (list.n < 4) return 0;
  double derivative = 0.0, coeff[4] = {11.0, -18.0, 9.0,-2.0};
  for (int i = 0; i < 4; i++) derivative += coeff[i] * list.fetch(i);
  derivative  = derivative /(6.0 * dt);
  return derivative;
}


double calc_average(sensor_list_t list, int N){
  
  // Calculates average from last N points
  if (list.n < N) return 0;
  double average = 0.0;
  for(int i = 0; i < N;i++) average += list.fetch(i);
  average = average/N;
  return average;
}

void reset_after_turn(int N){
   driving_state_t d_state = FORWARDS;
   set_drive(d_state, drive_speed);
   for (int i = 0; i < N; i++){
   LED_check();
   delay(delta_t);
   ultrasound_1_list.add(read_ultrasound(1,20));
   ultrasound_2_list.add(read_ultrasound(2,20));
  }
}  

void corner_turn(void){
  turn_and_check_right(90, 0.1);
  reset_after_turn(4);
}

void turn_and_pulse(bool turn_right){
  driving_state_t d_state;

  if(turn_right) Serial.println("Turn right");
  else Serial.println("Turn left");
  Serial.println(ultrasound_1_list.fetch(0)); 

  //Turn robot by an angle of 20 degrees to correct path
  if (turn_right){
    turn_and_check_right(20, 0.1);
  }
  else
  turn_and_check_left(20, 0.1);
  //Reset
  reset_after_turn(4);
}

void follow_wall(int wall_no, unsigned long max_duration){

  unsigned long duration = 0;
  unsigned long start_time = millis();
  int corners_turned = 0;
  Serial.println("Follow wall");
  while (corners_turned < wall_no and duration < max_duration){
    Serial.println("Loop in follow wall");
    

    // Reads left ultrasonic output and adds to the list if the robot not anaomolous (travelling < 1 m/s) - otherwise repeats previous reading
    double ultra_Op = read_ultrasound(1,20);
    double derivative = (ultra_Op - ultrasound_1_list.fetch(0))/delta_t;
    if (ultrasound_1_list.n == 0 || abs(derivative) < 100) ultrasound_1_list.add(ultra_Op);
    else ultrasound_1_list.add(ultrasound_1_list.fetch(0));
    LED_check();

    Serial.println("Read Ultrasound 1");

    //delay(100 * delta_t); didnt realise this was in here will try running without before deleting


    // Reads front ultrasonic output and adds it to list
    /*
    ultrasound_2_list.add(read_ultrasound(2,20));
    Serial.println("Read Ultrtasound 2");

    // Execute right turn if close to the end wall
    if(ultrasound_2_list.fetch(0) < 6) {
      corner_turn();
      corners_turned += 1 ;
    }
    */
  
    // Check if within bounds and moving in the right direction -- Correct if otherwise
    if (ultrasound_1_list.n >= 4){
      derivative = calc_finite_difference(ultrasound_1_list, delta_t);
      // Calls turn and pulse function if out of bounds and moving away
      if (derivative >= 0.0 && ultrasound_1_list.fetch(0) > upper_wall_bound) turn_and_pulse(false);
      else if (derivative <= 0.0 && ultrasound_1_list.fetch(0) < lower_wall_bound) turn_and_pulse(true);
    }
    Serial.println("Check turn");

    //  Print of distance/derivative values
    /*
    Serial.print("Derivative - ");
    Serial.print(derivative);
    Serial.print(" - Distance - ");
    Serial.println(ultrasound_1_list.fetch(0)); 
    Serial.print(" - Front - ");
    Serial.println(ultrasound_2_list.fetch(0));
    */
    
    // Delay for time dt
    delay(1000 * delta_t);
    LED_check(); //checks if LED needs to be flashed before moving to next iteration

    //Checks for how long follow_wall function has been running
    duration = millis() - start_time;

  }
}
