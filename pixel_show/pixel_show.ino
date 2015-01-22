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


#include "fix_fft.h"
#include "Section.cpp"
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

#define AUDIOPIN 7
//char im[128];
//char data[128];
//char data_avgs[14];
//
//int min = 1024;
//int max = 0;
//
//void mode2()
//{
//	unsigned long currentMillis = millis();
//
//	if (currentMillis - previousMillis <= interval) {
//		return;
//	}
//	// save the last time
//	previousMillis = currentMillis;
//
//	for (int i = 0; i < 128; i++){
//		
//		data[i] = analogRead(AUDIOPIN);
//		im[i] = 0;
//	};
//
//	fix_fft(data, im, 7, 0);
//
//	for (i = 0; i< 64; i++){
//		data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);  // this gets the absolute value of the values in the
//		//array, so we're only dealing with positive numbers
//	};
//
//
//	// average bars together
//	for (i = 0; i<14; i++) {
//		data_avgs[i] = data[i * 4] + data[i * 4 + 1] + data[i * 4 + 2] + data[i * 4 + 3];   // average together
//
//		data_avgs[i] = map(data_avgs[i], 0, 30, 0, 9);                              // remap values for LoL
//	}
//	int value = data_avgs[3];//0 for bass
//
//	//int value = analogRead(AUDIOPIN);
//	if (value < min)
//	{
//		min = value;
//	}
//	if (value > max)
//	{
//		max = value;
//	}
//
//	int value2 = map(value, 0, 8, 0, 255);
//	Serial.print(min);
//	Serial.print("\t"); 
//	Serial.print(max);
//	Serial.print("\t"); 
//	Serial.print(value);
//	Serial.print("\t");
//	Serial.println(value2);
//
//	if (value2 < 64)
//	{
//		strip.setPixelColor(0, strip.Color(0, 255, 0));
//		strip.setPixelColor(1, strip.Color(0, 0, 0));
//		strip.setPixelColor(2, strip.Color(0, 0, 0));
//		strip.setPixelColor(3, strip.Color(0, 0, 0));
//	}
//	else if (value2 < 128)
//	{
//		strip.setPixelColor(0, strip.Color(0, 255, 0));
//		strip.setPixelColor(1, strip.Color(0, 255, 0));
//		strip.setPixelColor(2, strip.Color(0, 0, 0));
//		strip.setPixelColor(3, strip.Color(0, 0, 0));
//	}
//	else if (value2 < 128)
//	{
//		strip.setPixelColor(0, strip.Color(0, 255, 0));
//		strip.setPixelColor(1, strip.Color(0, 255, 0));
//		strip.setPixelColor(2, strip.Color(255, 255, 0));
//		strip.setPixelColor(3, strip.Color(0, 0, 0));
//	}
//	else
//	{
//		strip.setPixelColor(0, strip.Color(0, 255, 0));
//		strip.setPixelColor(1, strip.Color(0, 255, 0));
//		strip.setPixelColor(2, strip.Color(255, 255, 0));
//		strip.setPixelColor(3, strip.Color(255, 0, 0));
//	}
//	strip.show();
//}



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




void mode_chase()
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
#define DEBUG 0

const int Yres = 255;
const int gain = 0;

#define BANDS 64
float peaks[BANDS];
char im[BANDS], data[BANDS];
char data_avgs[BANDS];
int debugLoop;

void mode2()
{

	for (int i = 0; i < BANDS; i++) {    // 64 bins = 32 bins of usable spectrum data
		data[i] = ((analogRead(AUDIOPIN))); // / 4) - 128);  // chose how to interpret the data from analog in                                      
		im[i] = 0;   // imaginary component
	}

	fix_fft(data, im, 6, 0);   // Send Left channel normalized analog values through fft

	// At this stage, we have two arrays of [0-31] frequency bins deep [32-63] duplicate

	// calculate the absolute values of bins in the array - only want positive values
	for (int i = 0; i < BANDS/2; i++) {
		data[i] = sqrt(data[i] * data[i] + im[i] * im[i]);
		
		data_avgs[i] = data[i];
		
		// Remap values to physical display constraints... that is, 8 display custom character indexes + "_"
		//data_avgs[i] = constrain(data_avgs[i], 0, 9 - gain);     //data samples * range (0-9) = 9
		//data_avgs[i] = map(data_avgs[i], 0, 9 - gain, 0, Yres);  // remap averaged values
		
		
		data_avgs[i] = map(data_avgs[i], 0, 20, 0, Yres);  // remap averaged values
	}

	Two16_LCD();
	decay(1);
}


void Two16_LCD(){
	

	for (int x = 1; x < BANDS/2; x++) {  // init 0 to show lowest band overloaded with hum
		if (data_avgs[x] > peaks[x]) peaks[x] = data_avgs[x];

		//lcd.setCursor(x, 0); // draw first (top) row Left
		//if (peaks[x] == 0) {
		//	lcd.print("_");  // less LCD artifacts than " "
		//}
		//else {
		//	lcd.write(peaks[x]);
		//}

		//lcd.setCursor(x, 1); // draw second (bottom) row Right
		//if (peaks[y] == 0){
		//	lcd.print("_");
		//}
		//else {
		//	lcd.write(peaks[y]);
		//}

		//Serial.print(data[x], DEC);
		//Serial.print("  ");
	}
	 //Serial.println();
	//int value2 = peaks[5];
	int value2 = data_avgs[5];

	Serial.print(data_avgs[5], DEC);
	Serial.print("\t");
	Serial.print(peaks[5], DEC);
	Serial.println();

	if (value2 < 64)
	{
		strip.setPixelColor(0, strip.Color(0, 255, 0));
		strip.setPixelColor(1, strip.Color(0, 0, 0));
		strip.setPixelColor(2, strip.Color(0, 0, 0));
		strip.setPixelColor(3, strip.Color(0, 0, 0));
	}
	else if (value2 < 128)
	{
		strip.setPixelColor(0, strip.Color(0, 255, 0));
		strip.setPixelColor(1, strip.Color(0, 255, 0));
		strip.setPixelColor(2, strip.Color(0, 0, 0));
		strip.setPixelColor(3, strip.Color(0, 0, 0));
	}
	else if (value2 < 128)
	{
		strip.setPixelColor(0, strip.Color(0, 255, 0));
		strip.setPixelColor(1, strip.Color(0, 255, 0));
		strip.setPixelColor(2, strip.Color(255, 255, 0));
		strip.setPixelColor(3, strip.Color(0, 0, 0));
	}
	else
	{
		strip.setPixelColor(0, strip.Color(0, 255, 0));
		strip.setPixelColor(1, strip.Color(0, 255, 0));
		strip.setPixelColor(2, strip.Color(255, 255, 0));
		strip.setPixelColor(3, strip.Color(255, 0, 0));
	}
	strip.show();


	debugLoop++;
	if (DEBUG && (debugLoop > 99)) 
	{
		Serial.print("Free RAM = ");
		Serial.println(freeRam(), DEC);
		Serial.println(millis(), DEC);
		
		debugLoop = 0;
	}
}


int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}


void decay(int decayrate){
	int DecayTest = 1;
	// reduce the values of the last peaks by 1 
	if (DecayTest == decayrate){
		for (int x = 0; x < 32; x++) {
			peaks[x] = peaks[x] - 1;  // subtract 1 from each column peaks
			DecayTest = 0;
		}
	}

	DecayTest++;
}
