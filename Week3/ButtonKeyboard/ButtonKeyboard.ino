#include <Keyboard.h>
#include <Mouse.h>

const int buttonPin = 2;
const int button2Pin = 3;
char leftKey = KEY_LEFT_ARROW;
char rightKey = KEY_RIGHT_ARROW;

int buttonState = 0;
int lastButtonState = 0;
int button2State = 0;
int lastButton2State = 0;

void setup() {
  pinMode(buttonPin, INPUT);
  pinMode(button2Pin, INPUT);
  Keyboard.begin();
}

void loop() {
  buttonState = digitalRead(buttonPin);
  button2State = digitalRead(button2Pin);

  Serial.print(buttonState);
  Serial.print(", ");
  Serial.println(button2State);

  if (buttonState != lastButtonState){
    if (buttonState == 1){
      Keyboard.press(leftKey);
    }else{
      Keyboard.release(leftKey);
    }
    lastButtonState = buttonState;
  }

  if (button2State != lastButton2State){
    if (button2State == 1){
      Keyboard.press(rightKey); 
    }else{
      Keyboard.release(rightKey); 
    }
    lastButton2State = button2State;
  }
}
