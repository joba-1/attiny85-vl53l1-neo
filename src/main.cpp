#include <Arduino.h>

#include <Adafruit_NeoPixel.h>

#include "VL53L1X.h"
#include "TinyWireM.h"

#define LED_PIN  1

#define DATA_PIN 4
#define NUM_LEDS 2
#define NEO_IO   (NEO_GRB+NEO_KHZ800)

#define DELAY_MS 100

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, DATA_PIN, NEO_IO);
VL53L1X tof;

uint32_t Wheel(byte WheelPos) {
  if (WheelPos < 85) {
      return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if (WheelPos < 170) {
      WheelPos -= 85;
      return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
      WheelPos -= 170;
      return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
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

void setup() {
  // pinMode(LED_PIN, OUTPUT);
  // digitalWrite(LED_PIN, LOW);

  strip.begin();
  // color(strip.Color(64,64,255), 100);

  TinyWireM.begin();

  // color(strip.Color(64,255,255), 100);

  tof.setTimeout(500);
  if( tof.init(true) ) {
    // (default after init()) tof.setDistanceMode(VL53L1X::Long);
    // (default after init()) tof.setMeasurementTimingBudget(50000);
    tof.startContinuous(50);
    // color(strip.Color(64,255,64), 100);
  }
  else {
    // color(strip.Color(255,64,64), 100);
    // delay(1000);
  }
}

void loop() {
  static uint16_t prev_mm = 0;
  static uint16_t min_mm = 0xffff;
  static uint16_t max_mm = 0;

  // static int led = LOW;
  // led = (led == LOW) ? HIGH : LOW;
  // digitalWrite(LED_PIN, led);

  // rainbow();

  uint16_t mm = tof.read();
  if( tof.timeoutOccurred() ) {
    // color(strip.Color(255,64,255), 100);
  }
  else if( mm <= 4000 ) {
    if( mm >= max_mm ) {
      max_mm = mm + 1;
    }
    if( mm < min_mm ) {
      min_mm = mm;
    }

    if( mm != prev_mm ) {
      color(Wheel(map(mm, min_mm, max_mm, 0, 256)), 0);
      prev_mm = mm;
    }
  }

  delay(DELAY_MS);
}