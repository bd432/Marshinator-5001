#include "marshinator.h"
#include <Arduino.h>
#include <Adafruit_MotorShield.h>
#include <Servo.h>




bool block_scan(double polar_coor[]){
  // Return relative polar coor of block
  static double radar_data[radar_N], radar_data_der[radar_N-1], peaks[radar_N][3], blocks[radar_N][2];
  double threshold = 100; // Edit threshold depending on data
  bool detected;

  // Reset lists
  for(int i=0; i <radar_N; i++ ){for(int j=0; j<3;j++) peaks[i][j] = 0}
  for(int i=0; i <radar_N; i++ ){for(int j=0; j<2;j++) blocks[i][j] = 0}

  scan_radar(radar_data);
  
  // Differentiate radar data
  differentiate(radar_data, radar_data_der, radar_N);

  

  // Scan for peaks
  scan_peaks(peaks, radar_data_der, radar_N, threshold);

  detected = detect_blocks(peaks, blocks), radar_data;

  select_block(polar_coor);

  return detected;
}

void scan_radar(double list[]){
  // Scan and fill array

}

void differentiate(double list[], double output[], double dx, int N){

  output[0] = (list[1]-list[0])/dx;
  for (int i = 1; i < N-2; i++) output[i] = (list[i+1]-list[i-1])/(2.0 * dx);
  output[N-2] = (list[N-1] - list[N-2])/dx;

}

void scan_peaks(double peaks[][], double der[], int N, double threshold){
  int i = 0, peak_state = 0, peak_start = 0, peak_length = 0, peak_no = 0;

  // Store peak as [start, length, type]
  // Type; +1 :rising; -1 :falling

  while (i < N-1){
    if (peak_state == 1){
      if (der[i] > threshold) peak_length++;
      else{
        peaks[peak_no][0] = peak_start;
        peaks[peak_no][1] = peak_length;
        peaks[peak_no][2] = peak_state;
        peak_no++;
        if (der[i] < -threshold){
          peak_state = -1;
          peak_start = i;
          peak_length = 1;
        }
        else{
          peak_state = 0;
        }
      }
    }
    else if (peak_state == -1){
      if (der[i] < -threshold) peak_length++;
      else{
        peaks[peak_no][0] = peak_start;
        peaks[peak_no][1] = peak_length;
        peaks[peak_no][2] = peak_state;
        peak_no++;
        if (der[i] > threshold){
          peak_state = 1;
          peak_start = i;
          peak_length = 1;
        }
        else{
          peak_state = 0;
        }
      }
    }
    else{
      if (der[i] < -threshold){
        peak_state = -1;
        peak_start = i;
        peak_length = 1;
      }
      else if (der[i] < threshold){
        peak_state = 1;
        peak_start = i;
        peak_length = 1;
      }
    }
    i++;
  }
  if (peak_state !=0){
    peaks[peak_no][0] = peak_start;
    peaks[peak_no][1] = peak_length;
    peaks[peak_no][2] = peak_state;
    peak_no++;
  }

}


bool detect_blocks(double peaks[][], double blocks[][], double data[]{
  //Return false if no blocks present
  if (peaks[0][2] == 0) return false;

  // Store peak as [start, length, type]
  // Type; +1 :rising; -1 :falling

  double r, angle;
  int block_start, block_end, block_no = 0;

  for (int i = 0; i < radar_N-1; i++){
    if (peaks[i+1][2] == 0){
      if (block_no == 0) return false;
      else return true;
    }

    if (peaks[i][2] == -1 && peaks[i+1][2] == 1){
      block_start = peaks[i][0] + peaks[i][1];
      block_end = peaks[i+1][0];

      theta = -start_angle + angular_res * (block_start + block_end) / 2.0;
      r = 0;
      for (int j = block_start; j < block_end + 1; j++){ r += data[j];
      r = r/(1 + block_end-block_start);

      blocks[block_no][0] = r;
      blocks[block_no][1] = theta;
      block_no++;
    }
  }
}

void select_block(){


}

}