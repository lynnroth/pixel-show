#define DEBUG 0
#define AUDIOPIN 7

#define BANDS 128
#define FFT_M 7

#define MATRIX_X 54
#define MATRIX_Y 5


char im[BANDS], data[BANDS];
char data_avgs[BANDS/2];
char peaks[BANDS / 2]; 
int debugLoop;

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(MATRIX_X, MATRIX_Y, PIXEL_PIN,
	NEO_MATRIX_BOTTOM + NEO_MATRIX_LEFT +
	NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
	NEO_GRB + NEO_KHZ800);
int max = 0;
int min = 1000;

void mode2()
{
	for (int i = 0; i < BANDS; i++) 
	{    // 64 bins = 32 bins of usable spectrum data
		data[i] = (analogRead(AUDIOPIN) - 230) ; // / 4) - 128);  // chose how to interpret the data from analog in                                      
		im[i] = 0;   // imaginary component

		//if (data[i] > max)
		//	max = data[i];
		//if (data[i] < min)
		//	min = data[i];

	}

	fix_fft(data, im, FFT_M, 0);
	
	for (int i = 0; i < BANDS / 2; i++)
	{
		//normalize values to positive
		char val = sqrt(data[i] * data[i] + im[i] * im[i]);

		//remap
		data_avgs[i] = val;// map(val, 0, 30, 0, 255);
		
		if (data_avgs[i] > peaks[i])
		{
			peaks[i] = data_avgs[i];

		}
	}

	Display();
	decay(1);
}

uint16_t red = matrix.Color(128, 0, 0);
uint16_t blue = matrix.Color(0, 0, 128);
uint16_t green = matrix.Color(0, 128, 0);
uint16_t yellow = matrix.Color(128, 128, 0);
uint16_t orange = matrix.Color(255, 128, 0);
uint16_t white = matrix.Color(255, 255, 255);

#define L0 1
#define L1 3
#define L2 5
#define L3 8
#define L4 10

//30
//50
//70
//90
//100

void Display(){

	matrix.clear();

	for (int x = 1; x < MATRIX_X; x++) 
	{ 
		if (data_avgs[x] > max)
			max = data_avgs[x];
		if (data_avgs[x] < min)
			min = data_avgs[x];

		//char v = ' ';

		//if (data_avgs[x] > L4)
		//{
		//	v = '^';
		//}
		//else if (data_avgs[x] > L3)
		//{
		//	v = '|';
		//}
		//else if (data_avgs[x] > L2)
		//{
		//	v = ':';
		//}
		//else if (data_avgs[x] > L1)
		//{
		//	v = ',';
		//}
		//else if (data_avgs[x] > L0)
		//{
		//	v = '.';
		//}

		//	
		//Serial.print(v);
		uint16_t color = 0;
		int y = 0;
		if (data_avgs[x] > L4)
		{
			y = 4;
			color = red;
		}
		else if (data_avgs[x] > L3)
		{
			y = 3;
			color = orange;
		}
		else if (data_avgs[x] > L2)
		{
			y = 2;
			color = yellow;
		}
		else if (data_avgs[x] > L1)
		{
			y = 1;
			color = green;
		}
		else if (data_avgs[x] > L0)
		{
			y = 0;
			color = blue;
		}

		matrix.drawLine(x, 0, x, y, color);



		//Peak
		bool peak = false;

		if (peaks[x] > data_avgs[x] && peaks[x] > L4)
		{
			peak = true;
			y = 4;
			color = red;
		}
		else if (peaks[x] > data_avgs[x] && peaks[x] > L3)
		{
			peak = true;
			y = 3;
			color = orange;
		}
		else if (peaks[x] > data_avgs[x] && peaks[x] > L2)
		{
			peak = true;
			y = 2;
			color = yellow;
		}
		else if (peaks[x] > data_avgs[x] && peaks[x] > L1)
		{
			peak = true;
			y = 1;
			color = green;
		}
	/*	else if (peaks[x] > data_avgs[x] && peaks[x] > L0)
		{
			peak = true;
			y = 0;
			color = blue;
		}*/

		if (peak)
		{
			matrix.drawPixel(x, y, color);
		}

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
	matrix.show();

	Serial.print("  ");
	Serial.print(max);
	Serial.print("\t");
	Serial.print(min);

	Serial.println();

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
		for (int x = 0; x < MATRIX_X; x++) {
			peaks[x] = peaks[x] - 1;  // subtract 1 from each column peaks
			DecayTest = 0;
		}
	}

	DecayTest++;
}

