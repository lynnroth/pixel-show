/*
Pins

0 - Pad0
1 .
2 .
3 .
4 - Pad4



8 - NeoPixel Pin


*/


#include <Adafruit_NeoPixel.h>

// Digital IO pin connected to the button.  This will be
// driven with a pull-up resistor so the switch should
// pull the pin to ground momentarily.  On a high -> low
// transition the button press logic will execute.
#define PAD_COUNT 5
#define SECTION_PIXEL_COUNT 60
#define SECTION_COUNT 5
#define PIXEL_PIN 8
#define PAD_PIN_START 0


#define MODE1_PIN 
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

int pixelCount = SECTION_PIXEL_COUNT * SECTION_COUNT;

struct Section
{
	int first;
	int last;
};
				  
Section section[SECTION_COUNT];
							  
void setup() {

	initPads();

	initSections();
	
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

void loop() {

	int mode = getMode();

	if (mode == 0)
	{
		pixels_Off();
	}
	else if (mode == 1)
	{
		mode1();
	}
	else if (mode == 2)
	{
		mode2();
	}
	
}


//Gets the mode based on the two pins.  0 = off, 1 = mode1, 2 = mode2
int getMode()
{
	return 0;
}

void mode1()
{
	loadPads();

}


void mode2()
{

}

void loadPads()
{
	
}
