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
int Reset = 2;


CRGB color_h = CRGB(140, 255, 200);
CRGB color_m = CRGB(gamma8[27], gamma8[42], gamma8[60]);
CRGB color_s = CRGB(gamma8[240], gamma8[148], gamma8[56]);

unsigned long previousMillis;

BLEService ledService("19B10000-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedIntCharacteristic switchCharacteristic("19B10001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = LED_BUILTIN; // pin to use for the LED


void setup() {
  digitalWrite(Reset, HIGH);
  delay(200);
  pinMode(Reset, OUTPUT);
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 5>(leds_s, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(leds_m, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(leds_h, NUM_LEDS);

  // When time needs to be set on a new device, or after a power loss, the
  // following line sets the RTC to the date & time this sketch was compiled
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));

  while (!Serial);

  //calibrate for the compiled time
  now = rtc.now() + TimeSpan(0, 0, 0, 11);
  h = now.hour() % 12;
  m = now.minute();
  s = now.second();
  Serial.print(h, DEC);
  Serial.print(':');
  Serial.print(m, DEC);
  Serial.print(':');
  Serial.print(s, DEC);
  Serial.println();


  // pre-display the time on LED
  static uint8_t brightness = 255;
  for (int i = s * 2; i > s * 2 - 5; i--) {
    brightness -= 50;
    leds_s[i % NUM_LEDS] = color_s;
  }
  brightness = 150;
  for (int j = m * 2; j > m * 2 - 5; j--) {
    brightness -= 30;
    leds_m[j % NUM_LEDS] = color_m;
  }
  FastLED.show();
  //////////////////////////////////////////////////////////////////////////////////////////meijie

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.writeValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

      // if the remote device wrote to the characteristic,
      // use the value to control the LED:

      // START:
      //        Serial.println(switchCharacteristic.written());
      //        Serial.println(switchCharacteristic.value());
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()) {   // any value other than 0
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
          Serial.print("1RED");
          Serial.println(r[0]);
          Serial.print("2RED");
          Serial.println(r[1]);
          Serial.print("3RED");
          Serial.println(r[2]);
          Serial.print("1green");
          Serial.println(g[0]);
          Serial.print("2green");
          Serial.println(g[1]);
          Serial.print("3green");
          Serial.println(g[2]);
          Serial.print("1blue");
          Serial.println(b[0]);
          Serial.print("2blue");
          Serial.println(b[1]);
          Serial.print("3blue");
          Serial.println(b[2]);
          counter = counter + 1;
          Serial.print("counter");
          Serial.println(counter);
          if (counter > 2) {
            counter = 0;
          }
          //////////////////////////////////////////////////////////////////////////////////////////////灯
          color_s = CRGB(gamma8[r[0]], gamma8[g[0]], gamma8[b[0]]);
          color_m = CRGB(gamma8[r[1]], gamma8[g[1]], gamma8[b[1]]);
          color_h = CRGB(gamma8[r[2]], gamma8[g[2]], gamma8[b[2]]);


          if ( counter == 2) {
            now = rtc.now() + TimeSpan(0, 0, 0, 11);
            h = now.hour() % 12;
            m = now.minute();
            s = now.second();
            bool checked = true;

          }

          now = rtc.now() + TimeSpan(0, 0, 0, 11);
          h = now.hour() % 12;
          m = now.minute();
          s = now.second();
          for (int i = s * 2 ; i > s * 2 - 5; i--) {
            leds_s[(i + 120) % NUM_LEDS] = color_s; //
          }
          for (int i = s * 2 - 5 ; i > s * 2 - 8; i--) {
            leds_s[(i + 120) % NUM_LEDS] = CRGB::Black;
          }
          FastLED.show();
        }

      }

    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(Reset, LOW);
  }

}
