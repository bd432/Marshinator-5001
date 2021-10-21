#include "marshinator.h"
#include <Arduino.h>

//Function that checks how long since the LED has last blinked and flashes it if necessary
void LED_check(void){
  const long interval = LED_interval;
  currentMillis = millis();
  
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }

    // set the LED with the ledState of the variable:
    digitalWrite(LED_Pin, ledState);
  }
}

void setup_LED(void){
  ledState = 0;
  previousMillis = 0;
}
