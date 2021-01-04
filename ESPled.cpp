/*

     Library to control a single LED

     Can be used to make simple animations for basic UIs
     Using single LEDs

     Created by Declan Heard. 01/08/2020
     Released into Public Domain.

*/







#include "ESPled.h"


ESPled::ESPled(int led_pin):    // Anything needed to set up instance goes here
  ledPin(led_pin)
{
}


/*
  void ESPled::begin(int pin, int initialState) {              // Sets up ledPin as output pin.

  ledPin = pin;
  pinMode(ledPin, OUTPUT);

  if (initialState) {
    ESPled::turnOn();
  } else {
    ESPled::turnOff();
  }

  }
*/

void ESPled::begin(int initialState) {              // Sets up ledPin as output pin.


  pinMode(ledPin, OUTPUT);

  if (initialState) {
    ESPled::turnOn();
  } else {
    ESPled::turnOff();
  }

}




void ESPled::turnOn() {
  digitalWrite(ledPin, HIGH);
  ledState = HIGH;
}




void ESPled::turnOff() {
  digitalWrite(ledPin, LOW);
  ledState = LOW;
}


void ESPled::toggleLED() {
  if (ledState) {
    ESPled::turnOff();
    ledState = LOW;
  } else {
    ESPled::turnOn();
    ledState = HIGH;
  }
}



void ESPled::startBlink(long onDuration, long offDuration) {   // Starts a constant blinking that continues until stopBlink is called
  blinkActive = true;
  blinkOnDuration = onDuration;
  blinkOffDuration = offDuration;
}


void ESPled::stopBlink() {                                                 // Stops the blinking, including if callBlink has been set
  blinkActive = false;
  ESPled::turnOff();                                                            //pulls LED low to stop the blinking
  blinkQuantity = 0;                                                              // Resets to zero, unnecessary but ties up loose ends
}




//--------------- Blink Events ------------------------------
// This is a way of setting up a single time event with a number of blinks and their duration.
//Could be used to respond to user inputs, button pushes or to notify user of an action taken.


// First Set up a blink event ---------------------

void ESPled::callBlink(int numberofBlinks, long onDuration, long offDuration) {                    // When called this function sets up a blink event, which is performed by the performBlink fuction which is called once per loop

  blinkQuantity = numberofBlinks;                                                                          // This may be verbose for what it is, but it allows the user to call ESPled.callBlink(number, duration, duration); and have this executed later.
  blinkOnDuration = onDuration;                                                                             // alternatively this would need to be a structure, otherwise the user would have to set all 3 variables independently to call the blink event
  blinkOffDuration = offDuration;

  blinkActive = true;

}




// the perform blink function is called each time through the loop, but only actuates if blink quantity has been set to > 0 OR blinkOn has been set true.

void ESPled::performBlink() {     // This function is called once per loop and it performs any blinks that have been set up elsewhere in the program  // Could be passed blink on duration and blink off duration (long blinkOnTime, long blinkOffTime) this might allow changing these values between loops for different effects, but it seems to be over

  if (blinkActive) {                //if blinkOn has been set true

    currentAction = millis();              // save the current time

    if (ledState) {                                // if the LED is already high

      if (currentAction - lastAction >= blinkOnDuration) {     // and and the allotted time has passed since it was set high

        ESPled::turnOff();                                    // Turn the LED off
        lastAction = currentAction;                                // save the time at which the LED was turned off


        if (blinkQuantity > 0) {                                                  // checks if blinkQuantity is greater than zero > set by blink Event
          blinkQuantity--;                                                // a blink has been done so decrement the number of blinks left to actuate
          if (blinkQuantity == 0) {                                                // if a blink even has been called but has now been expended
            blinkActive = false;                                                        // stop the blinking (else it will carry on forever)
          }
        }
      }
    }  else {                                                               // End of if(ledState == HIGH) > Now if ledState == LOW
      if (currentAction - lastAction >= blinkOffDuration) {                       // if the LED has been switched off for greater than or equal to the blink off duration
        ESPled::turnOn();                                                      // turn the LED on
        lastAction = currentAction;                                // save the time at which the LED was turned on.
        // Do not decrement as the last action needs to be the LED turning off -- EDIT: Add function to count decrements
        //Based on the initial LEDstate?
      }
    }
  }
}
