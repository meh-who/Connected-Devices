#include <ArduinoBLE.h>
#include "RTClib.h"
#include "gammaCorrection.h"
#include <FastLED.h>
#define NUM_LEDS 120

RTC_Millis rtc;
DateTime now;

CRGBArray<NUM_LEDS> leds_h;
CRGBArray<NUM_LEDS> leds_m;
CRGBArray<NUM_LEDS> leds_s;

int h = 0;
int m = 0;
int s = 0;
int r[100];
int g[100];
int b[100];
int counter = 0;
int Reset = 4;
int pres = 0;
int prem = 0;
int preh = 0;
int precounter = 0;



CRGB color_h = CRGB(gamma8[40], gamma8[148], gamma8[150]);
CRGB color_m = CRGB(gamma8[255], gamma8[204], gamma8[126]);
CRGB color_s = CRGB(gamma8[155], gamma8[169], gamma8[193]);


BLEService ledService("26b68f48-bddc-42fa-8a03-38be7b2bfc6e");
BLEUnsignedIntCharacteristic switchCharacteristic("26b68f48-bddc-42fa-8a03-38be7b2bfc6e", BLERead | BLEWrite);


void setup() {
  digitalWrite(Reset, HIGH);
  delay(200);
  pinMode(Reset, OUTPUT);
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, 2>(leds_s, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 5>(leds_m, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 9>(leds_h, NUM_LEDS);

  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  //check time
  now = rtc.now() + TimeSpan(0, 0, 0, 6);
  h = now.hour() % 12;
  m = now.minute();
  s = now.second();

  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  
  BLE.setLocalName("CLOCK");
  BLE.setAdvertisedService(ledService);

  ledService.addCharacteristic(switchCharacteristic);

  BLE.addService(ledService);

  // assign event handlers for connected, disconnected to peripheral
  // declare in setup and listen to event every update
  BLE.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLE.setEventHandler(BLEDisconnected, blePeripheralDisconnectHandler);

  // assign event handlers for characteristic
  switchCharacteristic.setEventHandler(BLEWritten, switchCharacteristicWritten);

  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println(("Bluetooth device active, waiting for connections..."));
}


void loop() {

  //constantly detect BLE radio events and handle them, used with event handler
  BLE.poll();

  //check time
  now = rtc.now() + TimeSpan(0, 0, 0, 6);
  h = now.hour() % 12;
  m = now.minute();
  s = now.second();

  //if minute changes,re-display LED
  if (m != prem) {
    for (int j = 0; j < 121; j++) {
    leds_m[j % NUM_LEDS] = CRGB::Black;
    }
    for (int j = m * 2 ; j > m * 2 - 5; j--) {
      // 60 is to move 0 to the top of the clock and prevent negative num
      // % NUM_LEDS is to make the leds loop
      leds_m[(j + 60) % NUM_LEDS] = color_m; 
    }
    FastLED.show();
    prem = m;
  }

  //if second changes, re-display LED
  if (s != pres) {
    Serial.println(s);
    for (int i = 0; i < 121; i++) {
      leds_s[i % NUM_LEDS] = CRGB::Black;
    }
    for (int i = s * 2 ; i > s * 2 - 5; i--) {
      leds_s[(i + 60) % NUM_LEDS] = color_s;
    }
    FastLED.show();
    pres = s;
  }


}

// connect handler, only at the moment of successful connecting
// blink led when connected
void blePeripheralConnectHandler(BLEDevice central) {

  for (int i = 0; i < 121; i++) {
    leds_s[i % NUM_LEDS] = CRGB::Black;
  }
  for (int i = 0; i < 121; i++) {
    leds_m[i % NUM_LEDS] = CRGB::Black;
  }
  FastLED.show();
  delay(200);

  for (int i = s * 2 ; i > s * 2 - 5; i--) {
    leds_s[(i + 60) % NUM_LEDS] = color_s;
  }
  for (int j = m * 2 ; j > m * 2 - 5; j--) {
    leds_m[(j + 60) % NUM_LEDS] = color_m;
  }
  FastLED.show();
  delay(200);

  for (int i = 0; i < 121; i++) {
    leds_s[i % NUM_LEDS] = CRGB::Black;
  }
  for (int i = 0; i < 121; i++) {
    leds_m[i % NUM_LEDS] = CRGB::Black;
  }
  FastLED.show();
  delay(200);

  for (int i = s * 2 ; i > s * 2 - 5; i--) {
    leds_s[(i + 60) % NUM_LEDS] = color_s;
  }
  for (int j = m * 2 ; j > m * 2 - 5; j--) {
    leds_m[(j + 60) % NUM_LEDS] = color_m;
  }
  FastLED.show();
  delay(200);
  
}


// disconnect handler, only at the moment of successful disconnecting
// still buggy
void blePeripheralDisconnectHandler(BLEDevice central) {
  for (int i = 0; i < 121; i++) {
    leds_s[i % NUM_LEDS] = CRGB::Red;
  }
  FastLED.show();
  delay(200);//milles
  for (int i = 0; i < 121; i++) {
    leds_s[i % NUM_LEDS] = CRGB::Black;
  }
  FastLED.show();
  delay(200);

  for (int i = 0; i < 121; i++) {
    leds_s[i % NUM_LEDS] = CRGB::Red;
  }
  FastLED.show();
  delay(200);
  for (int i = 0; i < 121; i++) {
    leds_s[i % NUM_LEDS] = CRGB::Black;
  }
  FastLED.show();
  delay(200);
}


// color change handler
void switchCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  
  // record color values polled from BLE
  if (counter < 3) {
    int rgbx = switchCharacteristic.value();
    int red = rgbx % 256;
    r[counter] = red;
    rgbx >>= 8;
    int green = rgbx % 256;
    g[counter] = green;
    rgbx >>= 8;
    int blue = rgbx % 256;
    b[counter] = blue;

    Serial.println("RGBX");
    Serial.println(switchCharacteristic.value());

    Serial.print("1RGB");
    Serial.print(r[0]);
    Serial.print(",");
    Serial.print(g[0]);
    Serial.print(",");
    Serial.println(b[0]);

    Serial.print("2RGB");
    Serial.print(r[1]);
    Serial.print(",");
    Serial.print(g[1]);
    Serial.print(",");
    Serial.println(b[1]);

    Serial.print("3RGB");
    Serial.print(r[2]);
    Serial.print(",");
    Serial.print(g[2]);
    Serial.print(",");
    Serial.println(b[2]);
    counter = counter + 1;

    Serial.print("counter: ");
    Serial.println(counter);

    color_s = CRGB(gamma8[r[0]], gamma8[g[0]], gamma8[b[0]]);
    color_m = CRGB(gamma8[r[1]], gamma8[g[1]], gamma8[b[1]]);
    color_h = CRGB(gamma8[r[2]], gamma8[g[2]], gamma8[b[2]]);
  }
  if (counter > 2) {
    counter = 0;
  }


  // if second hand color changes, re-display LED
  if (counter == 1) {
    for (int i = 0; i < 121; i++) {
      leds_s[i % NUM_LEDS] = CRGB::Black;
    }
    for (int i = s * 2 ; i > s * 2 - 5; i--) {
      leds_s[(i + 60) % NUM_LEDS] = color_s;
    }
  }
  //if minute hand color changes, re-display LED
  else if (counter == 2) {
    for (int j = 0; j < 121; j++) {
    leds_m[j % NUM_LEDS] = CRGB::Black;
    }
    for (int j = m * 2 ; j > m * 2 - 5; j--) {
      leds_m[(j + 60) % NUM_LEDS] = color_m;
    }
  }
  FastLED.show();
}
