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


#include "Section.cpp"
#include <FFT.h>
#include <EEPROM.h>
#include "EEPROMAnything.h"
#include <Adafruit_NeoPixel.h>

#define PAD_COUNT 5
#define SECTION_PIXEL_COUNT 4//60
#define SECTION_COUNT 1
#define PIXEL_PIN 16
#define PAD_PIN_START 0

#define MODE1_PIN 12
#define MODE2_PIN 13
#define SAVE_PIN 14
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

enum State { OFF, CHASE, FREEZE, SOLID };


bool oldState[PAD_COUNT];
bool newState[PAD_COUNT];
int pad_pin[PAD_COUNT];
int showType = 0;
int currentMode = 0;

int pixelCount = SECTION_PIXEL_COUNT * SECTION_COUNT;

				  
SectionClass Sections[SECTION_COUNT];

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
unsigned int colorcount = 0;


void setup() {
	Serial.begin(115200);

	EEPROM_readAnything(0, interval);

	/*Serial.print("Interval = " );
	Serial.print(interval);*/
	

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
	if (!digitalRead(10))
	{
		int newinterval = analogRead(0);

		if (interval != newinterval)
		{
			interval = newinterval;
			//EEPROM_writeAnything(0, interval);
		}
	}
}
uint32_t Color_Off = strip.Color(0, 0, 0);


void mode1()
{
	loadButtons();

	for (int s = 0; s < SECTION_COUNT; s+=2)
	{
		if (newState[0] == 0)
		{
			if (Sections[0].state == OFF || Sections[0].state == FREEZE || Sections[0].state == SOLID)
			{
				Sections[0].state = CHASE;
			}
		}
		else
		{
			if (Sections[0].state == CHASE)
			{
				Sections[0].state = FREEZE;
			}
		}

		switch (Sections[0].state)
		{
		case OFF:
			mode_solid(Sections[s], Color_Off);
			mode_solid(Sections[s+1], Color_Off);
			break;
		case SOLID:
			mode_solid(Sections[s], strip.Color(128, 128, 0));
			mode_solid(Sections[s + 1], strip.Color(128, 128, 0));
			break;
		case CHASE:
			mode_chase(Sections[s]);
			mode_chase(Sections[s+1]);
			break;
		case FREEZE:
			mode_freeze(Sections[s]);
			mode_freeze(Sections[s+1]);
			break;
		default:
			break;
		}
	}
}

void mode_freeze(SectionClass section)
{
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis <= interval) {
		return;
	}
	// save the last time
	previousMillis = currentMillis;
}

void mode_solid(SectionClass section, uint32_t color)
{
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis <= interval) {
		return;
	}
	// save the last time
	previousMillis = currentMillis;


	for (uint16_t i = 0; i < section.Length(); i++) {
		int p = section.GetPixel(i);
		strip.setPixelColor(p, color);
	}

	strip.show();
}


	//unsigned long currentMillis = millis();

	//if (currentMillis - previousMillis <= interval) {
	//	return;
	//}
	//// save the last time you blinked the LED 
	//previousMillis = currentMillis;
	//
	//for (int s = 0; s < SECTION_COUNT; s++)
	//{
	//	for (int i = 0; i < section.length; i++)
	//	{
	//		int p = section[s].GetPixel(i);
	//		Serial.println(p);
	//		
	//		uint32_t color = 0; 
	//		if (stepcount == i)
	//		{
	//			color = Wheel(colorcount);
	//			colorcount += 10;
	//			if (colorcount > 255) colorcount = 0;
	//		}
	//		strip.setPixelColor(p, color);
	//	}
	//	strip.show();
	//	stepcount++;
	//	if (stepcount >= section[s].length)
	//	{
	//		stepcount = 0;
	//	}
	//}



void mode_chase(SectionClass section)
{
	
	unsigned long currentMillis = millis();

	if (currentMillis - previousMillis <= interval) {
		return;
	}
	// save the last time
	previousMillis = currentMillis;

	for (int s = 0; s < SECTION_COUNT; s++)
	{
		for (int i = 0; i < section.Length(); i++)
		{
			int p = section.GetPixel(i);
			
			uint32_t color = 0;
			if (stepcount == i)
			{
				color = Wheel(colorcount);
				colorcount += 10;
				if (colorcount > 255) colorcount = 0;
			}
			strip.setPixelColor(p, color);
		}
		strip.show();
		stepcount++;
		if (stepcount >= section.Length())
		{
			stepcount = 0;
		}
	}
}


void mode2()
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
			color = Wheel(colorcount);
			colorcount+=10;
			if (colorcount > 255) colorcount = 0;
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


void mode3()
{
	uint32_t c = strip.Color(255, 0, 0);
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
	}
	strip.show();
}
