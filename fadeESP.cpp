/* ----------fadeESP---------------------------

    Derived class from ESPled

    This class can be used to control
    fading effects using
    an LED attached to PWM pins.

    on arduino uno:
    3~
    5~
    6~
    9~
    10~
    11~



*/


#include "ESPled.h"


//Methods
// No begin method as it already exists in ESPled, and the new function
// doesnt change anything.(HAHAHA)

/*
  void fadeESP::setup(int pin, byte startBrightness) {

  pwmPin = pin;                                 // Set up PWM pin (default 9 - for now)
  ledPin = pin;                                // required to interface with ESPled library.

  pinMode(pwmPin, OUTPUT);                          // Set PWM pin to output
  fadeESP::updatePWM(startBrightness);                       // update output with current brightness

  }
*/




void fadeESP::setup(byte startBrightness) {

  ESPled::ledPin = pwmPin;                                // required to interface with ESPled library.

  // Arduino Board Code
  // pinMode(pwmPin, OUTPUT);                          // Set PWM pin to output

  // ESP32 Code
  ledcSetup(ledCH, frequency, resolution);              // Set up PWM Driver
  ledcAttachPin(pwmPin, ledCH);                          // Attach PWM drive channel to output pin


  fadeESP::updatePWM(startBrightness);                       // update output with current brightness

}




// Method to write ledBrightness to PWM pin


void fadeESP::updatePWM(byte brightness) {

// Arduino Board Code
//  analogWrite(pwmPin, brightness);

// ESP32 Code
  ledcWrite(ledCH, ledBrightness);
  //Serial.println(brightness);

}





// Void fade moment and fade event should be able to stop startFading without explicilty calling stopFading

// Fades led in or out once by jumping to a start value then fading from there
void fadeESP::fadeMoment(byte minimum, byte maximum, int fadeDirection, uint32_t timeMs) {

  if (fadeDirection == FADE_UP || fadeDirection == OFF) {                // makes sure that fadeState has a direction to move
    fadeState = FADE_UP;
    ledBrightness = minimum;
  } else if (fadeDirection == FADE_DOWN) {
    fadeState = FADE_DOWN;
    ledBrightness = maximum;
  }

  interval = fadeESP::calculateFade(minimum, maximum, timeMs);


  pulsesRemaining = 1;

}





// Fades led in and out a defined number of times
void fadeESP::fadeEvent(byte minimum, byte maximum, int repeats, uint32_t timeMs) {

  interval = fadeESP::calculateFade(minimum, maximum, timeMs);

  pulsesRemaining = repeats * 2;  // Double the number of pulses as the algorithm subtracks 2 for every phase.

  if (fadeState == OFF) {             // starts fading if fading is turned off.
    fadeState = FADE_UP;
  }

} // Can be used to fade





// Starts Fade event that continues untill it is stopped
void fadeESP::startFading(byte minimum, byte maximum, uint32_t timeMs) {


  interval = fadeESP::calculateFade(minimum, maximum, timeMs);

  if (fadeState == OFF) {
    fadeState = FADE_UP;
  }


  /*
    Serial.print("timeMicros: ");
    Serial.println(timeMicros);
    Serial.print("Range: ");
    Serial.println(range);
    Serial.print("fadeState: ");
    Serial.println(fadeState);
    Serial.println(interval);
    uint32_t intervalMillis = interval/1000;
    Serial.print("interval Millis: ");
    Serial.println(intervalMillis);
  */
}




void fadeESP::stopFading() {

  fadeState = OFF;
  pulsesRemaining = 0;

}  // Instantly stops fading



void fadeESP::fadeOut(byte endBrightness, uint32_t timeMs) {

  interval = fadeESP::calculateFade(ledBrightness, endBrightness, timeMs);



  if (ledBrightness > endBrightness) {
    fadeState = FADE_DOWN;
    minBrightness = endBrightness;
  } else if (ledBrightness < endBrightness) {
    fadeState = FADE_UP;
    maxBrightness = endBrightness;
  } else if (ledBrightness == endBrightness) {
    fadeState = OFF;
  }

  pulsesRemaining = 1;


} // fades in or out using the current brightness as a starting positon








// Final loop for performing all fadeESP events and moments
void fadeESP::performFades() {


  if (pulsesRemaining > 0 || fadeState != OFF) {

    currentInterval = micros();

    if (currentInterval - lastInterval >= interval) {
      switch (fadeState) {
        case OFF:
          // Do nothing
          break;

        case FADE_UP:
          ledBrightness++;
          if (ledBrightness >= maxBrightness) {
            fadeState = FADE_DOWN;
            pulsesRemaining = fadeESP::updatePulses(pulsesRemaining);
          }
          break;

        case FADE_DOWN:
          ledBrightness--;
          if (ledBrightness <= minBrightness) {
            fadeState = FADE_UP;
            pulsesRemaining = fadeESP::updatePulses(pulsesRemaining);
          }
          break;

        default:
          // Also Do Nothing
          break;
      }
      lastInterval = currentInterval;

      fadeESP::updatePWM(ledBrightness);

    }

  } else {

    ESPled::performBlink();
  }
}


void fadeESP::advancedFade(byte startBrightness, byte endBrightness, uint32_t timeOne, uint32_t timeTwo, int repeats) {};   // Future implementation will cover this method




uint32_t fadeESP::calculateFade(byte minimum, byte maximum, uint32_t timeMs) {


  int range = maximum - minimum;

  if (range < 0) {
    range = range * (-1);
  }

  minBrightness = minimum;

  maxBrightness = maximum;

  uint32_t timeMicros = timeMs * 1000;



  timeMicros = (timeMicros / range);

  return timeMicros;


}

int fadeESP::updatePulses(int pulseNumber) {
  if (pulseNumber > 0) {
    pulseNumber--;
    if (pulseNumber == 0) {
      fadeState = OFF;
    }
  }
  return pulseNumber;
}
