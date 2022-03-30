/*
 * By Meijie Hu
 * 
 * Arduino send JSON information to webpage
 * scroll though the links or user input elements using a rotary encoder, 
 * and to activate any given element by pressing the encoder’s pushbutton.
 * Display the reading from a sound detector in a div of the page.
 */
#include <Encoder.h>
#include <Arduino_JSON.h>
 
// encoder on pins 2 and 3
Encoder myEncoder(2, 3);
// previous position of the encoder:
int lastPosition = 0;
// steps of the encoder's shaft:
int steps = 0;
 
 
const int buttonPin = 4;    // pushbutton pin
int lastButtonState = LOW;  // last button state
int debounceDelay = 5;       // debounce time for the button in ms

const int sensorPin = A0;
int lastSensorVal = 0;

JSONVar outgoing;
 
void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP);

  outgoing["shaft"] = 0;
  outgoing["button"] = 0;
  outgoing["sound"] = false;
}
 
void loop() {
  // read the pushbutton:
  int buttonState = digitalRead(buttonPin);
  //  // if the button has changed:
  if (buttonState != lastButtonState) {
    // debounce the button:
    delay(debounceDelay);
    // if button is pressed:
    if (buttonState == LOW) {
      outgoing["button"] = steps; //save step to JSON button
    }
  }
  // save current button state for next time through the loop:
  lastButtonState = buttonState;

 
  // read the encoder:
  int newPosition = myEncoder.read();
  // compare current and last encoder state:
  int change = newPosition - lastPosition;
  // if it's changed by 4 or more (one detent step):
  if (abs(change) >= 4) {
    // get the direction (-1 or 1):
    int encoderDirection = (change / abs(change));
    steps += encoderDirection;
    // if you want to make the steps rollover, use this:
    if (steps < 0) steps = 23;
    steps = steps % 24;
    outgoing["shaft"] = steps; //save step to JSON encoder
 
    // save encoder position for next time through loop:
    lastPosition = newPosition;
  }

  // read the sound detector:
  int sensorVal = analogRead(sensorPin);
  //if you are speaking
  if (abs(sensorVal - lastSensorVal) >= 100){
    outgoing["sound"] = true; //save bool to JSON sensor
    lastSensorVal = sensorVal;
  }
  else{
    outgoing["sound"] = false;
    lastSensorVal = sensorVal;
  }
  
  Serial.println(outgoing);
  delay(10);
}
