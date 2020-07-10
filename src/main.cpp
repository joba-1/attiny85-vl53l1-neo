#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include "VL53L1X.h"

#ifdef VL53L1X_TINY
  #include <TinyWireM.h>
  #define Wire   TinyWireM
#else
  #include <Wire.h>
#endif

#define LED_PIN  CONFIG_LED_PIN

#define DATA_PIN CONFIG_DATA_PIN
#define NUM_LEDS 2
#define NEO_IO   (NEO_GRB+NEO_KHZ800)

#define DELAY_MS 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_IO);
VL53L1X tof;

uint32_t Wheel( byte WheelPos ) {
  if (WheelPos < 85) {
      return strip.Color(255 - WheelPos * 3, WheelPos * 3, 0);
  } else if (WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(0, 255 - WheelPos * 3, WheelPos * 3);
  } else {
      WheelPos -= 170;
      return strip.Color(WheelPos * 3, 0, 255 - WheelPos * 3);
  }
}

void rainbow() {
    static uint8_t step = 0;
    for (uint16_t i = 0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i + step) & 255));
    }
    strip.show();
    step++;
}

void color(uint32_t c, unsigned long delay_ms ) {
  for (uint16_t i = 0; i < strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
  }
  strip.show();
  delay(delay_ms);
}

void alarm_color(uint32_t c, unsigned long delay_ms ) {
  digitalWrite(LED_PIN, HIGH);
  strip.setPixelColor(0, c);
  strip.show();
  delay(delay_ms);
  digitalWrite(LED_PIN, LOW);
}

void setup() {
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Wire.begin();

  #ifndef VL53L1X_TINY
    Serial.begin(115200);
    Serial.println("\nVL53L1X Neo Test");
  #endif

  strip.begin();

  tof.setTimeout(500);
  if( tof.init() ) {
    // (default after init()) tof.setDistanceMode(VL53L1X::Long);
    // (default after init()) tof.setMeasurementTimingBudget(50000);
    tof.startContinuous(50);
    alarm_color(strip.Color(64,255,64), 100); // light green: start ok
  }
  else {
    color(strip.Color(255,64,64), 100); // light red start error
    delay(1000);
  }

  digitalWrite(LED_PIN, LOW);
}

void loop() {
  static uint16_t prev_mm = 0;
  static uint16_t min_mm = 0xffff;
  static uint16_t max_mm = 0;

  uint16_t mm = tof.read();

  if( tof.timeoutOccurred() ) {
    alarm_color(strip.Color(255,64,255), 100); // light pink: timeout error
  }
  else if( mm > 4500 ) {
    alarm_color(strip.Color(128,64,255), 100); // light violet: range error
  }
  else {
    if( mm >= max_mm ) {
      max_mm = mm + 1;
    }
    if( mm < min_mm ) {
      min_mm = mm;
    }

    if( mm != prev_mm ) {
      uint32_t fraction = mm - min_mm; // >= 0
      fraction *= 256;                 // prepare scale for 0 .. 255
      fraction /= (max_mm - min_mm);   // scales range (min .. max) to (0 .. 255)
      color(Wheel(fraction), 0);
      prev_mm = mm;
      #ifndef VL53L1X_TINY
        Serial.printf("%5u %5u %5u\n", min_mm, mm, max_mm);
      #endif
    }
  }

  delay(DELAY_MS);
}