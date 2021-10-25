#include "Marshinator_5001.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>

Servo servo;

bool radar_scan(double polar_coor[]){
  // Return relative polar coor of block

  static double radar_data[radar_N], radar_data_der[radar_N-8], peaks[peaks_N][3], blocks[blocks_N][2], cart_coor[radar_N][2];
  bool detected;

  // Reset lists
  for(int i=0; i < peaks_N; i++ ){for(int j=0; j<3;j++) peaks[i][j] = 0;}
  for(int i=0; i < blocks_N; i++ ){for(int j=0; j<2;j++) blocks[i][j] = 0;}
  
  // Scan and collect data for the radar
  servo.write(90-start_angle);
  delay(2000);
  double angle, input;
  for (int i =0; i < radar_N ; i++){
    angle = 90 - start_angle + i * angular_res;
    servo.write(angle);
    delay(40);
    input =  read_shortIR(100);
    if (input > range_cutoff) radar_data[i] = range_cutoff;
    else radar_data[i] = input;
  }
    
  // Convert radar data to cartesian coordinates
  double theta = 0.0;
  for(int i = 0; i < radar_N; i++){
    cart_coor[i][0] = -cos(theta * M_PI/180) * radar_data[i];
    cart_coor[i][1] = sin(theta * M_PI/180) * radar_data[i];
    theta += angular_res;
  }

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

  // Detect blocks from peaks

  double r;
  unsigned int block_start, block_end, block_no = 0;

  for (int i = 0; i < peaks_N-1; i++){
    // Break if no more peaks
    if (peaks[i+1][2] == 0) break;
    // Check for descending / ascending peaks
    if (peaks[i][2] == -1 && peaks[i+1][2] == 1){
      // Locate position n array of start and end of the block
      block_start = peaks[i][0] + peaks[i][1] + 2;
      block_end = peaks[i+1][0] + 2;
      // Calculate the average angle
      theta = angular_res * (block_start + block_end) / 2.0;
      // calculate the average distance to the block
      r = 0;
      for (int j = block_start; j < block_end + 1; j++)  r += radar_data[j];
      r = r/(1 + block_end-block_start);
      // Store in blocks
      blocks[block_no][0] = r;
      blocks[block_no][1] = theta;
      block_no++;
    }
  }

  // Return first block if detected - Could implement selection process for blocks
  if(block_no > 0){
    polar_coor[0] = blocks[0][0];
    polar_coor[1] = blocks[0][1];
    return true;
  } 
  else return false;
}