/*

     Library to control a single LED

     Can be used to make simple animiations for basic UIs
     Utelising single LEDs

     Created by Declan Heard. 01/08/2020
     Released into Public Domain.

*/


#include "ESPled.h"

//#include <autoDelay.h>                            // Not used for any LED functions ( though ESPled borrows from topology) // Only used for demo functions

#include <buttonObject.h>

//#define ledPin 0                         // Define LED pin. If undefined - defaults to pin 13.
#define INITIAL_STATE 1                  // Defines if LED pin starts with initial state ON or OFF - defaults to OFF

//ESPled ledONE(9);

#define PWM_PIN 2                                   // If undefined defaults to pin D2 (internal LED)
#define LED_CH  0                                 //    16 Channels Total (0 to 15 Valid)
#define PWM_FREQ 5000
#define PWM_RESO 8                                 // No of Bits, 8 = byte = 255 steps

#define INITIAL_BRIGHTNESS 100

fadeESP ledOne(PWM_PIN, LED_CH, PWM_FREQ, PWM_RESO);

//autoDelay basicDelay;                 // Set up instance of autoDelay


#define MIN_BRIGHT 0
#define MAX_BRIGHT 255
#define TIME_MS   1000


#define BUTTON_PIN 13
//#define BUTTON_PULL_HIGH
buttonObject button(BUTTON_PIN, BUTTON_PULL_HIGH);


void setup() {

  ledOne.setup(INITIAL_BRIGHTNESS);

  ledOne.startFading(MIN_BRIGHT, MAX_BRIGHT, TIME_MS);

  button.begin();

}





bool fadingOn = false;



void loop() {



  ledOne.performFades();


  button.buttonLoop();

  if (button.shortPress) {
    if (fadingOn) {
      fadingOn = false;
      ledOne.stopFading();
    } else {
      fadingOn = true;
      ledOne.startFading(MIN_BRIGHT, MAX_BRIGHT, TIME_MS);
    }
    button.buttonReset();
  }


}
