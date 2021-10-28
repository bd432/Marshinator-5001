#include "header.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

Servo servo;

bool block_scan(double polar_coor[]){
  // Return relative polar coor of block

  Serial.println("Start radar");

  static double radar_data[radar_N], radar_data_der[radar_N-8], peaks[peaks_N][3], blocks[blocks_N][2], cart_coor[radar_N][2];
  bool detected;

  // Reset lists
  for(int i=0; i < peaks_N; i++ ){for(int j=0; j<3;j++) peaks[i][j] = 0;}
  for(int i=0; i < blocks_N; i++ ){for(int j=0; j<2;j++) blocks[i][j] = 0;}

  // Scan and collect data for the radar
  //servo.write(90-start_angle);
  servo.write(start_angle);
  delay(500);
  double angle, input;
  for (int i =0; i < radar_N ; i++){
    //angle = 90 - start_angle + i * angular_res;
    angle = start_angle + i * angular_res;
    servo.write(angle);
    //delay(1);
    //delayMicroseconds(500);
    input =  read_shortIR(20);
    if (input > range_cutoff) radar_data[i] = range_cutoff;
    else radar_data[i] = input;
  }
  Serial.println("Finish Scan");
    
  // Convert radar data to cartesian coordinates
  double theta = 0.0;
  for(int i = 0; i < radar_N; i++){
    cart_coor[i][0] = -cos(theta * M_PI/180) * radar_data[i];
    cart_coor[i][1] = sin(theta * M_PI/180) * radar_data[i];
    theta += angular_res;
  }
  Serial.println("Converted to coor");

  // Identify rising/falling peaks
  double dot[radar_N-2], dot_avr[radar_N-5];

  // Calculate average of the dot product for each pair of relative adjacent vectors
  for(int i = 0; i < radar_N-2; i++) dot[i] = (cart_coor[i+1][0]-cart_coor[i][0])*(cart_coor[i+2][0]-cart_coor[i+1][0]) + (cart_coor[i+1][1]-cart_coor[i][1])*(cart_coor[i+2][1]-cart_coor[i+1][1]);
  // Calculate local average fo 4 adjacent dot products
  for(int i = 0; i < radar_N-5; i++) dot_avr[i] = (dot[i] + dot[i+1] + dot[i+2] + dot[i+3])/4.0;
  
  int peak_state = 0, peak_length, peak_no = 0; // Length given as relative position (peak of 1 is 0)
  // Iterate through dot_avr
  for(int i = 0; i < radar_N-5; i++){
    if (peak_state != 0){
      // If in peak assess whether above threshold - record data if peak ended
      if (dot_avr[i] > threshold) peak_length += 1;
      else{
        peaks[peak_no][1] = peak_length;
        peaks[peak_no][2] = peak_state;
        peak_state = 0;
        peak_no++;
      }
    }
    else{
      // If not in peak and above threshold assess whether the function is assending or descending and strat new peak
      if (dot_avr[i] > threshold){
        peaks[peak_no][0] = i;
        peak_length = 0;
        if (  (cart_coor[i+4][0] - cart_coor[i][0]) * cart_coor[i+2][0] + (cart_coor[i+4][1] - cart_coor[i][1]) * cart_coor[i+2][1] < 0) peak_state = -1;
        else peak_state = 1;
      }
    }
  }

  // Ignore peaks at the edge of the function
  if (peak_state != 0) peaks[peak_no][0] = 0;

  Serial.println("Identified peaks");
  Serial.print("Peak no - ");
  Serial.println(peak_no);

  // Detect blocks from peaks

  double r, r_centre, theta_centre;
  unsigned int block_start, block_end, block_no = 0;

  for (int i = 0; i < peaks_N; i++){
    Serial.println("Start identification");
    if (peaks[i+1][2] == 0) break;
    Serial.print("Peak 1 - ");
    Serial.print(peaks[i][2]);
    Serial.print(" - Peak 2 -  ");
    Serial.println(peaks[i+1][2]);
    // Check for descending / ascending peaks
    if (peaks[i][2] == -1 && peaks[i+1][2] == 1){
      Serial.println("Block triggered");
      block_start = peaks[i][0] + peaks[i][1] + 2;
      block_end = peaks[i+1][0] + 2;

      theta = (angular_res * (block_start + block_end) / 2.0 - theta_offset) * M_PI/ 180 ; // In rad including offset
      r = 0;

      Serial.print("Start - ");
      Serial.println(block_start);
      Serial.print("End - ");
      Serial.println(block_end);

      for (int j = block_start; j < block_end + 1; j++)  r += radar_data[j];
      r = r/(1 + block_end-block_start);

      Serial.print("R rad - ");
      Serial.println(r);
      Serial.print("Theta rad - ");
      Serial.println(theta * 180/M_PI);
      Serial.print("Width1 - ");
      Serial.println((block_end-block_start)* angular_res * r * M_PI/180.0);

      theta_centre = atan( (r * sin(theta) - x_offset)/(r * cos(theta) + y_offset) ) * 180 / M_PI;
      r_centre = sqrt((r * sin(theta) - x_offset)*(r * sin(theta) - x_offset) + (r * cos(theta) + y_offset)*(r * cos(theta) + y_offset));
      Serial.print("Theta cen - ");
      Serial.println(theta_centre);

      blocks[block_no][0] = r_centre;
      blocks[block_no][1] = theta_centre;
      block_no++;
    }
  }
  Serial.print("Block no - ");
  Serial.println(block_no);

  Serial.println("Identified blocks");

  // Print data
  if(block_no > 0) {//select_block(polar_coor);
    Serial.print("R- ");
    Serial.println(blocks[0][0]);
    Serial.print("Theta- ");
    Serial.println(blocks[0][1]);
  }
  else Serial.println("Failed");
  
  for (int i = 0; i < radar_N; i++){
    Serial.print("R - ");
    Serial.println(radar_data[i]);
    delay(10);
    Serial.print("Theta - ");
    Serial.println(i * angular_res);
    delay(10);
    Serial.print("X - ");
    Serial.println(cart_coor[i][0]);
    delay(10);
    Serial.print("Y - ");
    Serial.println(cart_coor[i][1]);
    delay(10);
  }

  Serial.println("Finish print");

  
  /*
  for(int i = 0; i < 5; i++){
  //Serial.println("End");
  }/*
  for (int i = 0; i < radar_N-8; i++){
    Serial.print("Der - ");
    Serial.println(radar_data_der[i]);
  }*/

  

  // Scan for peaks
  
  //detected = detect_blocks(peaks, blocks, radar_data);

  if(block_no > 0) return true;
  else return false;
}

void differentiate(double list[], double output[], double dx, int N){

  output[0] = (list[1]-list[0])/dx;
  for (int i = 1; i < N-2; i++) output[i] = (list[i+1]-list[i-1])/(2.0 * dx);
  output[N-2] = (list[N-1] - list[N-2])/dx;
}

void calc_finite_difference(double list[], double output[], double dx, int N){ // dt in s

  // Calculates a  derivative using previous 4 points
  double derivative, coeff[5] = {1, -8, 0, 8, -1};  //coeff[9] = {3.5717e-3, -3.8095e-2, 0.2, -0.8, 0, 0.8, -0.2, 3.8095e-2, -3.5717e-3};
  
  for (int i = 0; i < N-8; i++){
    derivative = 0;
    for(int j = 0; j < 5; j++){
      derivative += coeff[j] * list[i+j];
    }
    derivative = derivative/(12.0 * dx);
    output[i] = derivative;
  }
}


void convert_cartesian(double r[], double cartesian[][2]){
  double theta = 0.0;
  for(int i = 0; i < radar_N; i++){
    cartesian[i][0] = -cos(theta * M_PI/180) * r[i];
    cartesian[i][1] = sin(theta * M_PI/180) * r[i];
    theta += angular_res;
  }
}






void select_block(double polar_coor[]){
  delay(10);
}