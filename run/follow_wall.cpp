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
    delay(delta_t*1000);
    ultrasound_1_list.add(read_ultrasound(1,10));
    ultrasound_2_list.add(read_ultrasound(2,10));
  }
}  

void corner_turn(bool reset){
  Serial.println("Corner turn");
  turn_and_check_right(80, 20, false);
  if (reset) reset_after_turn(4);
}

bool turn_and_pulse(bool turn_right){
  driving_state_t d_state;
  bool line_break = false;

  //if(turn_right) Serial.println("Turn right");
  //else Serial.println("Turn left");
  Serial.println(ultrasound_1_list.fetch(0)); 

  //Turn robot by an angle of 20 degrees to correct path
  if (turn_right){
    Serial.println("Adjust right");
    line_break = turn_and_check_right(15, 20, true);
  }
  else{
    line_break = turn_and_check_left(15, 20, true);
    Serial.println("Adjust left");
  }
  if (line_break) return true;
  //Reset
  reset_after_turn(2);
  return false;

}

void follow_wall(int wall_no, unsigned long max_duration, bool white_line,double lower_wall_bound, double upper_wall_bound, bool corner_turn_bool){

  unsigned long duration = 0;
  unsigned long start_time = millis();
  int corners_turned = 0;

  Serial.println("Follow wall");

  while (corners_turned < wall_no){
    Serial.println("Loop in follow wall");

    // Break from follow wall if reached a white line if condition

    if(white_line && detect_line()) {Serial.println("Exit Follow wall - White line"); return;}
    if (!corner_turn_bool && blockSensor(50)) {return; Serial.println("Exit Follow wall - Block Detected");}
    

    // Reads left ultrasonic output and adds to the list if the robot not anaomolous (travelling < 1 m/s) - otherwise repeats previous reading
    double ultra_Op = read_ultrasound(1,1);
    if(white_line && detect_line()) {Serial.println("Exit Follow wall - White line"); return;}
    double derivative = (ultra_Op - ultrasound_1_list.fetch(0))/delta_t;
    if (ultrasound_1_list.n == 0 || abs(derivative) < 100) ultrasound_1_list.add(ultra_Op);
    else ultrasound_1_list.add(ultrasound_1_list.fetch(0));
    
    //checks
    LED_check();
    if (!corner_turn_bool && blockSensor(50)) {return; Serial.println("Exit Follow wall - Block Detected");}

    Serial.print("Read Ultrasound 1 - ");
    Serial.println(ultrasound_1_list.fetch(0));

    // Reads front ultrasonic output and adds it to list
    ultrasound_2_list.add(read_ultrasound(2,1));

    //checks
    LED_check();
    if(white_line && detect_line()) {Serial.println("Exit Follow wall - White line"); return;}

    Serial.print("Read Ultrasound 2 - ");
    Serial.println(ultrasound_2_list.fetch(0));
    // Execute right turn if close to the end wall
    if(ultrasound_2_list.fetch(0) < 6) {
      if(!corner_turn_bool){ return;}
      corner_turn(true);
      corners_turned += 1 ;
    }
    
  
    // Check if within bounds and moving in the right direction -- Correct if otherwise
    if (ultrasound_1_list.n >= 4){
      derivative = calc_finite_difference(ultrasound_1_list, delta_t);
      // Calls turn and pulse function if out of bounds and moving away
      bool line = false;
      if (derivative >= 0.0 && ultrasound_1_list.fetch(0) > upper_wall_bound) line =turn_and_pulse(false);
      else if (derivative <= 0.0 && ultrasound_1_list.fetch(0) < lower_wall_bound)line = turn_and_pulse(true);
      if (line) return;
    }

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
    delay(500 * delta_t);
    LED_check(); //checks if LED needs to be flashed before moving to next iteration
    if (!corner_turn_bool && blockSensor(50)) {return; Serial.println("Exit Follow wall - Block Detected");} //checks if block in funnel
    if(white_line && detect_line()) {Serial.println("Exit Follow wall - White line"); return;}
    
    delay(500 * delta_t);
    LED_check();
    if(white_line && detect_line()) {Serial.println("Exit Follow wall - White line"); return;}
    
    //Checks for how long follow_wall function has been running
    duration = millis() - start_time;

    //Reverses and turns inwards slightly if the function has been running for too long
    //This is designed to free the robot if it gets stuck on a corner
    if(duration > max_duration){
      drive_with_LED(500, 10, BACKWARDS);
      turn_and_check_left(20, 10, false);
      start_time = millis(); //resets the start time so the duration is set back to zero
    }
  }
  
  Serial.println("Exit Follow wall");
}

void move_until_corner_turn(double timeout){
  unsigned long start_time = millis();
  double ultra_in;
  set_drive(FORWARDS, drive_speed);

  while (true){
    LED_check();
    ultra_in  = read_ultrasound(2,1);
    if (ultra_in < 6 && ultra_in != 0){
      corner_turn(false);
      set_drive(STATIONARY, drive_speed);
      return;
    }
    if (millis() - start_time > 1000* timeout) return;
    LED_check();
    delay(500* delta_t);
    LED_check();
    delay(500*delta_t);
  }
}

void find_wall(void){
   Serial.println("Find wall");
   double  US_1_input, US_2_input;
   while(true){
      US_1_input = read_ultrasound(1,1);
      US_2_input = read_ultrasound(1,1);
      if (US_1_input != 0 && US_2_input != 0 &&  US_1_input < wall_check_threshold && US_2_input < wall_check_threshold){
        move_until_corner_turn(10);
        set_drive(STATIONARY, drive_speed);
        return;
      }
      else turn_and_check_left(10, 10, false);
   }
}

bool sweep_strip(void){

  double US_input;
  set_drive(FORWARDS, drive_speed);

  while (true){
    US_input = read_ultrasound(2,1);
    Serial.println(US_input);
    if (US_input < 6 &&  US_input!= 0.0){
      Serial.print("US 2 input - ");
      Serial.println(US_input);
      drive_with_LED(5000, 10, BACKWARDS);
      turn_and_check_right(30,10, false);
      drive_with_LED(600, 10, FORWARDS);
      turn_and_check_left(30,10, false);
      set_drive(STATIONARY, drive_speed);

      return false;
    }

    if (blockSensor(50)){
      Serial.println("Block detected");
      set_drive(STATIONARY, drive_speed);
      ledState=LOW;
      return true;
    }

    LED_check();
    delay(5);
  }

}
