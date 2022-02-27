//real-time LED Clock
#include "RTClib.h"
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

static uint8_t hue_h;
static uint8_t hue_m;
static uint8_t hue_s;

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
}

void loop () {

  for (int i = 0; i < NUM_LEDS;) {
    for (int j = 0; j < NUM_LEDS;) {
      for (int k = 0; k < NUM_LEDS;) {
        if (millis() - previousMillis >= 500) {  // update s hand per 500ms
          previousMillis = millis();
          leds_s.fadeToBlackBy(100);
          leds_s[(k + s * 2) % NUM_LEDS] = CHSV(20, 255, 200);
          k++;   // advance one s hand led per update
          if (k == 60) {  
            leds_m.fadeToBlackBy(50);
            leds_m[(j + m * 2) % NUM_LEDS] = CHSV(80, 255, 200);
            j++;   // advance one m hand led when s hand finish half a loop
          }
          FastLED.show();   // display LED per update
        }
      }
      leds_m.fadeToBlackBy(50);
      leds_m[(j + m * 2) % NUM_LEDS] = CHSV(80, 255, 200);
      j++;   // advance one m hand led when s hand finish a loop
      if (j == 60) {  
        leds_h.fadeToBlackBy(50);
        leds_h[(i + m * 2) % NUM_LEDS] = CHSV(80, 255, 200);
        i++;   // advance one h hand led when m hand finish half a loop
      }
    }
    leds_h.fadeToBlackBy(50);
    leds_h[(i + h * 2) % NUM_LEDS] = CHSV(140, 255, 200);
    i++;   // advance one h hand led when m hand finish a loop
  }


}
