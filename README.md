# Color Coded Distance with Neopixel and VL53L0X on Digispark/ATTiny85

Not tested since I currently only have VL53L1X boards...
Should display a rainbow color dependent on current distance. 
Colors selection adjusted automatically to range between min and max measured distance

## Installation

This is for VS Code, but should be easily adaptable to another PlatformIO environment or even Arduino IDE

* Clone git repository into a folder
* In VS Code switch to Explorer view
* Add folder to workspace (popup menu in Explorer pane)
* VS Code should install Platform IDE extension now, if not done already
* PlatformIO then installs support for digispark board and arduino framework as given in platformio.ini
* PlatformIO also installs dependent library Adafruit_NeoPixel as given in platformio.ini
* Select PlatformIO Upload (e.g. via arrow at bottom status line of VS Code)
* Wait for this message: Uploading .pio/build/digispark/firmware.hex
* Only then plug in digispark (or compatible) board into an USB port

Now flashing begins automatically.
After flashing is complete, sketch will start: 
Short blink of onboard LED and Neopixels go white, then color changes depending on distance

## Wiring of Digispark thumb and Neopixel

    Digispark Neopixel
    5V ------ +
    Gnd ----- -
    PB4 ----- Din

    Digispark VL53L0X
    5V ------ Vin
    Gnd ----- Gnd
    PB0 ----- SDA
    PB2 ----- SCL

## Power supply

The digispark can be plugged into an USB port, any USB power supply or similar if they provide enough power for your Neopixel strip.

You can also use the digispark pads 5V and Gnd. Maybe Vin (~7-30V) and Gnd works too. (not tested, consult your datasheet. Maybe you need to power your Neopixels 5V "+" seperately, but then don't forget to connect the "Gnd"s)

Have fun,
Joachim