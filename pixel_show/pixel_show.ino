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
A7 - Audio
*/

#include <gamma.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include "fix_fft.h"
#include "Section.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"

#define PAD_COUNT 5
#define SECTION_PIXEL_COUNT 27
#define SECTION_COUNT 10
#define PIXEL_PIN 16
#define PAD_PIN_START 0

#define MODE1_DISTANCE 3

#define MODE1_PIN 12
#define MODE2_PIN 13
#define SAVE_PIN 14
#define MODE1_LED 22
#define MODE2_LED 23
#define ON_LED 21


// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
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

unsigned long mode1_delay = 50UL;
unsigned long mode1_timerA = 0;
unsigned long mode1_timerB = 0;

long interval = 1000;

void setup() {
	Serial.begin(115200);

	EEPROM_readAnything(0, interval);

	Serial.print("Interval = " );
	Serial.print(interval);
	
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
	
	for (int s = 0; s < SECTION_COUNT; s++)
	{
		int button = s / 2;

		if (newState[button] == LOW)
		{
			if (Sections[s].state == OFF || Sections[s].state == FREEZE || Sections[s].state == SOLID)
			{
				Sections[s].state = CHASE;
			}
		}
		else
		{
			if (Sections[s].state == CHASE)
			{
				if (button == 0 || button == 2)
				{
					Sections[s].state = FREEZE;
				}
				else
				{
					Sections[s].state = SOLID;
				}
			}
		}
	}

	for (int s = 0; s < SECTION_COUNT; s++)
	{
		
		switch (Sections[s].state)
		{
		case OFF:
			mode_solid(Sections[s], Color_Off);
			break;
		case SOLID:
			mode_solid(Sections[s], strip.Color(80, 64, 0));
			break;
		case CHASE:
			mode_chase(Sections[s]);
			break;
		case FREEZE:
			mode_freeze(Sections[s]);
			break;
		default:
			break;
		}
	}
	strip.show();
}


void mode_chase(SectionClass & section)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= interval) {
		return;
	}
	// save the last time
	section.SetLastMillis(currentMillis);
	
	for (int i = 0; i < section.Length(); i++)
	{
		uint32_t color = 0;
		int p = section.GetPixel(i);
		if (i % MODE1_DISTANCE == section.GetStepCount())
		{
			color = Wheel(section.GetColorCount());
			section.IncrementColorCount(10);
		}
		strip.setPixelColor(p, color);
	}
	section.IncrementStepCount();
}

void mode_freeze(SectionClass & section)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= interval) {
		return;
	}
	// save the last time
	section.lastmillis = currentMillis;
}

void mode_solid(SectionClass & section, uint32_t color)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= interval) {
		return;
	}
	// save the last time
	section.lastmillis = currentMillis;

	for (uint16_t i = 0; i < section.Length(); i++) {
		int p = section.GetPixel(i);
		strip.setPixelColor(p, color);
	}
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



//
//
//void mode_chase()
//{
//	
//	unsigned long currentMillis = millis();
//	
//	if (currentMillis - previousMillis <= interval) {
//		return;
//	}
//
//	// save the last time you blinked the LED 
//	previousMillis = currentMillis;
//
//	for (uint16_t i = 0; i < pixelCount; i++) 
//	{
//		uint32_t color = 0;
//		if (stepcount == i)
//		{
//			color = Wheel(colorcount);
//			colorcount+=10;
//			if (colorcount > 255) colorcount = 0;
//		}
//		strip.setPixelColor(i, color);
//	}
//	strip.show();
//	stepcount++;
//	if (stepcount >= pixelCount)
//	{
//		stepcount = 0;
//	}
//	
//}
