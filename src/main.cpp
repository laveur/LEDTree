/* This example shows how to display a moving rainbow pattern on
 * an APA102-based LED strip. */

/* By default, the APA102 library uses pinMode and digitalWrite
 * to write to the LEDs, which works on all Arduino-compatible
 * boards but might be slow.  If you have a board supported by
 * the FastGPIO library and want faster LED updates, then install
 * the FastGPIO library and uncomment the next two lines: */
// #include <FastGPIO.h>
// #define APA102_USE_FAST_GPIO

#include <APA102.h>

// Function Definitions
rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v);
void shiftLEDArray(rgb_color ledArray[], uint16_t size);

// Define which pins to use.
const uint8_t trunkDataPin = 5;
const uint8_t branch1LeftDataPin = 6;
const uint8_t branch1RightDataPin = 7;
const uint8_t branch2LeftDataPin = 8;
const uint8_t branch2RightDataPin = 9;
const uint8_t branch3LeftDataPin = 10;
const uint8_t branch3RightDataPin = 11;
const uint8_t clockPin = 12;

// Create an object for writing to the LED strip.
APA102<trunkDataPin, clockPin> trunkStrip;
APA102<branch1LeftDataPin, clockPin> branch1LeftStrip;
APA102<branch1RightDataPin, clockPin> branch1RightStrip;
APA102<branch2LeftDataPin, clockPin> branch2LeftStrip;
APA102<branch2RightDataPin, clockPin> branch2RightStrip;
APA102<branch3LeftDataPin, clockPin> branch3LeftStrip;
APA102<branch3RightDataPin, clockPin> branch3RightStrip;

// Set the number of LEDs to control.
const uint16_t trunkLEDCount = 60;
const uint16_t branch1LeftLEDCount = 60;
const uint16_t branch1RightLEDCount = 60;
const uint16_t branch2LeftLEDCount = 60;
const uint16_t branch2RightLEDCount = 60;
const uint16_t branch3LeftLEDCount = 60;
const uint16_t branch3RightLEDCount = 60;

// Set the brightness to use (the maximum is 31).
const uint8_t brightness = 1;

// Create a buffer for holding the colors (3 bytes per color).
rgb_color trunkColors[trunkLEDCount] = { rgb_color(0, 0, 0) };
rgb_color branch1LeftColors[branch1LeftLEDCount] = { rgb_color(0, 0, 0) };
rgb_color branch1RightColors[branch1RightLEDCount] = { rgb_color(0, 0, 0) };
rgb_color branch2LeftColors[branch2LeftLEDCount] = { rgb_color(0, 0, 0) };
rgb_color branch2RightColors[branch2RightLEDCount] = { rgb_color(0, 0, 0) };
rgb_color branch3LeftColors[branch3LeftLEDCount] = { rgb_color(0, 0, 0) };
rgb_color branch3RightColors[branch3RightLEDCount] = { rgb_color(0, 0, 0) };

uint16_t hue = 0;

void setup() {
}

void loop() {
	// Trunk Update
	shiftLEDArray(trunkColors, trunkLEDCount);

	trunkColors[0] = hsvToRgb(hue++, 255, 255);

	// Branch 1 Update
	shiftLEDArray(branch1LeftColors, branch1LeftLEDCount);
	shiftLEDArray(branch1RightColors, branch1RightLEDCount);

	branch1LeftColors[0] = branch1RightColors[0] = trunkColors[10];

	// Branch 2 Update
	shiftLEDArray(branch2LeftColors, branch2LeftLEDCount);
	shiftLEDArray(branch2RightColors, branch2RightLEDCount);

	branch2LeftColors[0] = branch2RightColors[0] = trunkColors[30];

	// Branch 3 Update
	shiftLEDArray(branch3LeftColors, branch3LeftLEDCount);
	shiftLEDArray(branch3RightColors, branch3RightLEDCount);

	branch3LeftColors[0] = branch3RightColors[0] = trunkColors[50];

	if (hue == 360) {
		hue = 0;
	}

	trunkStrip.write(trunkColors, trunkLEDCount, brightness);
	branch1LeftStrip.write(branch1LeftColors, branch1LeftLEDCount, brightness);
	branch1RightStrip.write(branch1RightColors, branch1RightLEDCount, brightness);
	branch2LeftStrip.write(branch2LeftColors, branch2LeftLEDCount, brightness);
	branch2RightStrip.write(branch2RightColors, branch2RightLEDCount, brightness);
	branch3LeftStrip.write(branch3LeftColors, branch3LeftLEDCount, brightness);
	branch3RightStrip.write(branch3RightColors, branch3RightLEDCount, brightness);

	delay(10);
}

/* Converts a color from HSV to RGB.
 * h is hue, as a number between 0 and 360.
 * s is the saturation, as a number between 0 and 255.
 * v is the value, as a number between 0 and 255. */
 rgb_color hsvToRgb(uint16_t h, uint8_t s, uint8_t v) {
    uint8_t f = (h % 60) * 255 / 60;
    uint8_t p = (255 - s) * (uint16_t)v / 255;
    uint8_t q = (255 - f * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t t = (255 - (255 - f) * (uint16_t)s / 255) * (uint16_t)v / 255;
    uint8_t r = 0, g = 0, b = 0;
    switch((h / 60) % 6){
        case 0: r = v; g = t; b = p; break;
        case 1: r = q; g = v; b = p; break;
        case 2: r = p; g = v; b = t; break;
        case 3: r = p; g = q; b = v; break;
        case 4: r = t; g = p; b = v; break;
        case 5: r = v; g = p; b = q; break;
    }
    return rgb_color(r, g, b);
}

void shiftLEDArray(rgb_color ledArray[], uint16_t size) {
	for (uint16_t index = size - 1; index > 0; index--) {
		ledArray[index] = ledArray[index - 1];
	}
}
