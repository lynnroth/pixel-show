/*
Pins

0 - Pad0
1 .
2 .
3 .
4 - Pad4


11 - MODE 0
12 - MODE 1
13 - Select0 
14 - Select1 (NC)


16 - NeoPixel Pin
17 - NeoPixel Status Strand


A0 - Pot
A1 - Audio
*/

#include <gamma.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_GFX.h>
#include "fix_fft.h"
#include "Section.h"
#include <EEPROM.h>
#include "EEPROMAnything.h"

#define SECTION_PIXEL_COUNT 27
#define SECTION_COUNT 10
#define PIXEL_PIN 17
#define STATUS_PIXEL_PIN 16

#define PAD_PIN_START 19
#define PAD_COUNT 5

#define MODE1_DISTANCE 3

#define MODE0_PIN 11
#define MODE1_PIN 12

#define SELECT0_PIN 13
#define SELECT1_PIN 14


// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(SECTION_PIXEL_COUNT * SECTION_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel status_strip = Adafruit_NeoPixel(4, STATUS_PIXEL_PIN, NEO_GRB + NEO_KHZ800);


enum State { OFF, CHASE, FREEZE, SOLID };

bool oldState[PAD_COUNT];
bool newState[PAD_COUNT];
int pad_pin[PAD_COUNT];
int showType = 0;
int currentMode = 0;

int pixelCount = SECTION_PIXEL_COUNT * SECTION_COUNT;
bool button_triggered = false;

bool rainbowState = false;
SectionClass Sections[SECTION_COUNT];

unsigned long mode1_delay = 50UL;
unsigned long mode1_timerA = 0;
unsigned long mode1_timerB = 0;
uint32_t rainbow[7];

long interval = 1000;
long mode3_interval = 1000;
int gain = 0;

void setup() {
	Serial.begin(115200);
	delay(2000);
	EEPROM_readAnything(0, interval);
	EEPROM_readAnything(4, gain);
	EEPROM_readAnything(8, mode3_interval);


	Serial.print("Interval = " );
	Serial.println(interval);
	
	Serial.print("Gain = ");
	Serial.println(gain);


	Serial.print("Mode3_Interval = ");
	Serial.println(mode3_interval);

	initControls();

	initPads();

	initSections();
	
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'

	status_strip.begin();
	status_strip.show();

	rainbow[0] = strip.Color(255, 0, 0);
	rainbow[1] = strip.Color(255, 64, 0);
	rainbow[2] = strip.Color(255, 255, 0);
	rainbow[3] = strip.Color(0, 255, 0);
	rainbow[4] = strip.Color(0, 0, 192);
	rainbow[5] = strip.Color(75, 0, 130);
	rainbow[6] = strip.Color(255, 0, 255);

}


uint32_t Color_Off = strip.Color(0, 0, 0);
uint32_t Color_Red = status_strip.Color(20, 0, 0);
uint32_t Color_Green = status_strip.Color(0, 20, 0);
uint32_t Color_Blue = status_strip.Color(0, 0, 20);

uint32_t Color_Gold = strip.Color(255, 115, 5);


uint32_t mode0_color = Color_Red;
uint32_t mode1_color = Color_Green;
uint32_t mode2_color = Color_Blue;
uint32_t mode3_color = Color_Gold; //Gold




void loop() {

	currentMode = getMode();
	
	if (currentMode == 0)  //OFF - RED
	{
		status_strip.setPixelColor(0, mode0_color);
		status_strip.show();
		pixels_Off();
		button_triggered = false;
		rainbowState = false;
		ClearSections();
		resetButtons();
	}
	else if (currentMode == 1) //Rainbow waterfall - GREEN  //Multi Color Chase / Freeze - GREEN
	{
		readControls_mode1();
		

		status_strip.setPixelColor(0, mode1_color);
		status_strip.show(); 
		button_triggered = false;

		//mode1();
		modeRainbow();
	}
	else if (currentMode == 2) //Audio - BLUE
	{
		readControls_mode2();

		status_strip.setPixelColor(0, mode2_color);
		status_strip.show(); 

		mode2();
	}
	else if (currentMode == 3) //Gold Chase / Solid - GOLD
	{
		readControls_mode3();

		status_strip.setPixelColor(0, mode3_color);
		status_strip.show();

		mode3();
	}
}

void ClearSections()
{
	Sections[0].state = OFF;
	Sections[1].state = OFF;
	Sections[3].state = OFF;
	Sections[2].state = OFF;
	Sections[4].state = OFF;
	Sections[5].state = OFF;
	Sections[6].state = OFF;
	Sections[7].state = OFF;
	Sections[8].state = OFF;
	Sections[9].state = OFF;
}

int oldval = 1;


void readControls_mode1()
{
	int val1 = digitalRead(SELECT0_PIN);
	if (val1) delay(10);

	int val = digitalRead(SELECT0_PIN);
	if (val1 == 0 && val == 0 && oldval == 1)
	{
		oldval = 0;
		
		int newinterval = analogRead(0);

		if (interval != newinterval)
		{
			status_strip.setPixelColor(1, status_strip.Color(255, 0, 0));
			status_strip.show();

			interval = newinterval;
			
			EEPROM_writeAnything(0, interval);

			Serial.print("Save Interval = ");
			Serial.println(interval);

			delay(200);

			status_strip.setPixelColor(1, status_strip.Color(0, 0, 0));
			status_strip.show();
		}
	}
	else if (val == 1)
	{
		oldval = 1;
	}

}


void readControls_mode3()
{
	int val1 = digitalRead(SELECT0_PIN);
	if (val1) delay(10);

	int val = digitalRead(SELECT0_PIN);
	if (val1 == 0 && val == 0 && oldval == 1)
	{
		oldval = 0;

		int newinterval = analogRead(0);

		if (mode3_interval != newinterval)
		{
			status_strip.setPixelColor(1, status_strip.Color(255, 0, 0));
			status_strip.show();

			mode3_interval = newinterval;

			EEPROM_writeAnything(8, mode3_interval);

			Serial.print("Save Mode3_Interval = ");
			Serial.println(mode3_interval);

			delay(200);

			status_strip.setPixelColor(1, status_strip.Color(0, 0, 0));
			status_strip.show();
		}
	}
	else if (val == 1)
	{
		oldval = 1;
	}

}


void readControls_mode2()
{
	int val1 = digitalRead(SELECT0_PIN);
	if (val1) delay(10);

	int val = digitalRead(SELECT0_PIN);
	if (val1 == 0 && val == 0 && oldval == 1)
	{
		status_strip.setPixelColor(1, status_strip.Color(255, 0, 0));
		status_strip.show();

		oldval = 0;

		int newgain = analogRead(0);
		newgain = map(newgain, 0, 1023, 0, 1024);

		if (gain != newgain)
		{
			gain = newgain;

			EEPROM_writeAnything(4, gain);
			Serial.print("Save gain = ");
			Serial.println(gain);

			delay(200);

			status_strip.setPixelColor(1, status_strip.Color(0, 0, 0));
			
			uint32_t c = 0;
			if (gain > 800)
			{
				c = status_strip.Color(10, 0, 0);
			}
			else if (gain > 500)
			{
				c = status_strip.Color(10, 5, 0);
			}
			else if (gain > 200)
			{
				c = status_strip.Color(10, 10, 0);
			}
			else
			{
				c = status_strip.Color(0, 10, 0);
			}

			status_strip.setPixelColor(2, c);
			status_strip.show();
		}
	}
	else if (val == 1)
	{
		oldval = 1;
	}

}


int rainbowCount = 0;
long rainbowlastmillis = 0;

void modeRainbow()
{
	loadButtons();

	for (int s = 0; s < SECTION_COUNT/2; s++)
	{
		if (newState[s] == LOW)
		{
			rainbowState = true;
		}
	}

	if (rainbowState)
	{
		unsigned long currentMillis = millis();

		if (currentMillis - rainbowlastmillis <= interval * 2) {
			return;
		}
		// save the last time
		rainbowlastmillis = currentMillis;

		rainbowCount++;
		rainbowCount = rainbowCount % 7;

		for (int s = 0; s < SECTION_COUNT / 2; s++)
		{
		/*	Serial.print(s);
			Serial.print("\t");

			Serial.print(rainbowCount);
			Serial.print("\t");*/

			int c = (s + rainbowCount) % 7;

		/*	Serial.print(c);
			Serial.print("\t");

			Serial.print(s * 2);
			Serial.print("\t");

			Serial.print(s * 2 + 1);
			Serial.print("\t");
			Serial.println();
			*/
			mode_solid(Sections[s * 2], rainbow[c]);
			mode_solid(Sections[s * 2 + 1], rainbow[c]);
		}
		strip.show();
	}
}


				
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





void mode3()
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
				Sections[s].state = SOLID;
			}
		}
	}

	for (int s = 0; s < SECTION_COUNT; s++)
	{

		switch (Sections[s].state)
		{
		case OFF:
			mode_solid3(Sections[s], Color_Off);
			break;
		case SOLID:
			mode_solid3(Sections[s], Color_Gold);
			break;
		case CHASE:
			mode_chase_singleColor3(Sections[s], Color_Gold);
			break;
		case FREEZE:
			mode_freeze3(Sections[s]);
			break;
		default:
			break;
		}
	}
	strip.show();
}


void mode_chase3(SectionClass & section)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= mode3_interval) {
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


void mode_chase_singleColor3(SectionClass & section, uint32_t single_color)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= mode3_interval) {
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
			color = single_color;
		}
		strip.setPixelColor(p, color);
	}
	section.IncrementStepCount();
}

void mode_freeze3(SectionClass & section)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= mode3_interval) {
		return;
	}
	// save the last time
	section.lastmillis = currentMillis;
}

void mode_solid3(SectionClass & section, uint32_t color)
{
	unsigned long currentMillis = millis();

	if (currentMillis - section.lastmillis <= mode3_interval) {
		return;
	}
	// save the last time
	section.lastmillis = currentMillis;

	for (uint16_t i = 0; i < section.Length(); i++) {
		int p = section.GetPixel(i);
		strip.setPixelColor(p, color);
	}
}
