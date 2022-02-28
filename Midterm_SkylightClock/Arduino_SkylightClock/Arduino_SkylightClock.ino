/*Skylight Clock 
 * Meijie Hu & Zeshu Zhu
 * 
 * A skylight clock that imitate the color of the sky
 * 
 * question: how to achieve desaturated blue?
 * 
 */
#include "RTClib.h"
#include "gammaCorrection.h"
#include <FastLED.h>
#define NUM_LEDS 120

RTC_DS3231 rtc;
DateTime now;

CRGBArray<NUM_LEDS> leds_h;
CRGBArray<NUM_LEDS> leds_m;
CRGBArray<NUM_LEDS> leds_s;

int h = 0;
int m = 0;
int s = 0;

CRGB color_h = CHSV(140, 255, 200);
CRGB color_m = CRGB(gamma8[27],gamma8[42],gamma8[60]);
CRGB color_s = CRGB(gamma8[240],gamma8[148],gamma8[56]);

unsigned long previousMillis;

void setup () {
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, 5>(leds_s, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 6>(leds_m, NUM_LEDS);
  FastLED.addLeds<NEOPIXEL, 7>(leds_h, NUM_LEDS);

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }

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
  for (int i = s*2; i > s*2 - 5; i--) {
    brightness -= 50;
    leds_s[i % NUM_LEDS] = CHSV(50, 255, brightness);
  }
  brightness = 150;
  for (int j = m*2; j > m*2 - 5; j--) {
    brightness -= 30;
    leds_m[j % NUM_LEDS] = CHSV(170, 255, brightness);
  }
  FastLED.show(); 
  
}

void loop () {

  for (int i = 0; i < NUM_LEDS;) {
    for (int j = 0; j < NUM_LEDS;) {
      for (int k = 0; k < NUM_LEDS;) {
        if (millis() - previousMillis >= 50) {  // update s hand per 500ms
          previousMillis = millis();
          leds_s.fadeToBlackBy(50);
          leds_s[(k + s * 2) % NUM_LEDS] = color_s;
          k++;   // advance one s hand led per update
          if (k == 60) {  
            leds_m.fadeToBlackBy(20);
            leds_m[(j + m * 2) % NUM_LEDS] = color_m;
            j++;   // advance one m hand led when s hand finish half a loop
          }
          FastLED.show();   // display LED per update
        }
      }
      leds_m.fadeToBlackBy(20);
      leds_m[(j + m * 2) % NUM_LEDS] = color_m;
      j++;   // advance one m hand led when s hand finish a loop
      if (j == 60) {  
        leds_h.fadeToBlackBy(50);
        leds_h[(i + h * 2) % NUM_LEDS] = color_h;
        i++;   // advance one h hand led when m hand finish half a loop
      }
    }
    leds_h.fadeToBlackBy(50);
    leds_h[(i + h * 2) % NUM_LEDS] = color_h;
    i++;   // advance one h hand led when m hand finish a loop
  }


}
