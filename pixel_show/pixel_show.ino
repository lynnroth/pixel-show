/*
Pins

0 - Pad0
1 .
2 .
3 .
4 - Pad4


8 - MODE 1
9 - MODE 2




16 - NeoPixel Pin

21 - LED ON 
22 - LED MODE 1
23 - LED MODE 2

A0 - Pot
*/


#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Adafruit_NeoPixel.h>

#define PAD_COUNT 5
#define SECTION_PIXEL_COUNT 4//60
#define SECTION_COUNT 1
#define PIXEL_PIN 16
#define PAD_PIN_START 0

#define MODE1_PIN 8
#define MODE2_PIN 9
#define SAVE_PIN 10
#define MODE1_LED 22
#define MODE2_LED 23
#define ON_LED 21

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(SECTION_PIXEL_COUNT * SECTION_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState[PAD_COUNT];
bool newState[PAD_COUNT];
int pad_pin[PAD_COUNT];
int showType = 0;
int currentMode = 0;

int pixelCount = SECTION_PIXEL_COUNT * SECTION_COUNT;

struct Section
{
	int first;
	int last;
};
				  
Section section[SECTION_COUNT];

#define MODE1_DISTANCE 3

bool mode1_step = 0;
int mode1_color = 0;
int mode1_q = 0;
unsigned long mode1_delay = 50UL;
unsigned long mode1_timerA = 0;
unsigned long mode1_timerB = 0;


int stepcount = 0;
unsigned long previousMillis = 0;
long interval = 100;
unsigned int color = 0;


void setup() {

	EEPROM_readAnything(0, interval);

	initControls();

	initPads();

	initSections();
	
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() {

	currentMode = getMode();
	
	readControls();

	if (currentMode == 0)
	{
		digitalWrite(ON_LED, HIGH);
		digitalWrite(MODE1_LED, LOW);
		digitalWrite(MODE2_LED, LOW);

		pixels_Off();
	}
	else if (currentMode == 1)
	{
		digitalWrite(ON_LED, LOW);
		digitalWrite(MODE1_LED, HIGH);
		digitalWrite(MODE2_LED, LOW);

		mode1();
	}
	else if (currentMode == 2)
	{
		digitalWrite(ON_LED, LOW);
		digitalWrite(MODE1_LED, LOW);
		digitalWrite(MODE2_LED, HIGH);

		mode2();
	}
}



void readControls()
{
	if (digitalRead(0))
	{
		int newinterval = analogRead(0);

		if (interval != newinterval)
		{
			interval = newinterval;
			//EEPROM_writeAnything(0, interval);
		}
	}
}



void mode1()
{
	
	unsigned long currentMillis = millis();
	
	if (currentMillis - previousMillis <= interval) {
		return;
	}

	// save the last time you blinked the LED 
	previousMillis = currentMillis;

	for (uint16_t i = 0; i < pixelCount; i++) 
	{
		uint32_t color = 0;
		if (stepcount == i)
		{
			color = Wheel(color % 255);
			color++;
		}
		strip.setPixelColor(i, color);
	}
	strip.show();
	stepcount++;
	if (stepcount >= pixelCount)
	{
		stepcount = 0;
	}
	
}


void mode2()
{
	uint32_t c = strip.Color(255, 0, 0);
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
	}
	strip.show();
}
