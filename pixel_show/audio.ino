#define DEBUG 0
#define AUDIOPIN 2

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

uint16_t color = 0;
unsigned long lastmillis = 0;

void mode2()
{
	unsigned long currentMillis = millis();

	if (currentMillis - lastmillis <= interval) {
		return;
	}
	// save the last time
	lastmillis = currentMillis;

	for (int i = 1; i < 54; i++)
	{
		color += 50;
		matrix.drawCircle(26, 3, i, color);
	}
	//Serial.println(color);
	matrix.show();
}


void mode2a()
{
	if (!button_triggered)
	{
		loadButtons();

		if (newState[0] == 0 || newState[1] == 0 || newState[2] == 0 || newState[3] == 0 || newState[4] == 0)
		{
			button_triggered = true;
		}

		if (!button_triggered)
		{
			return;
		}
	}

	//	Serial.println(gain);

	for (int i = 0; i < BANDS; i++) 
	{    // 64 bins = 32 bins of usable spectrum data

		int val = analogRead(AUDIOPIN);
		//Serial.print(gain);
		//Serial.print("\t");

		//Serial.print(val);
		//Serial.print("\t");
		val = val - 230 + gain;

		//Serial.print(val);
		//Serial.print("\t");

		//val = map(val, -1014, 1024 - gain, -127, 127);
		
		//Serial.print(val);
		//Serial.print("\t");
		//Serial.println();

		data[i] = (val); 
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

#define L0 2
#define L1 4
#define L2 6
#define L3 8
#define L4 10

//30
//50
//70
//90
//100
unsigned int colormap = 0;
int avg_max = 10;

void Display(){

	matrix.clear();

	for (int x = 1; x < MATRIX_X; x++) 
	{ 
		if (data_avgs[x] > max)
			max = data_avgs[x];
		if (data_avgs[x] < min)
			min = data_avgs[x];


		if (data_avgs[x] > avg_max)
			avg_max = data_avgs[x];


		if (DEBUG)
		{
			char v = ' ';

			if (data_avgs[x] > L4)
			{
				v = '^';
			}
			else if (data_avgs[x] > L3)
			{
				v = '|';
			}
			else if (data_avgs[x] > L2)
			{
				v = ':';
			}
			else if (data_avgs[x] > L1)
			{
				v = ',';
			}
			else if (data_avgs[x] > L0)
			{
				v = '.';
			}
						
			//Serial.print(v);
			//Serial.println();
		}
		
		uint16_t color = 0;
		colormap = data_avgs[x] + x; //  map(data_avgs[x], L0, avg_max, 0, 255);
		color = Wheel(colormap);
		
		//current
		int y = 0;
		if (data_avgs[x] > L4)
		{
			y = 4;
		}
		else if (data_avgs[x] > L3)
		{
			y = 3;
		}
		else if (data_avgs[x] > L2)
		{
			y = 2;
		}
		else if (data_avgs[x] > L1)
		{
			y = 1;
		}
		else if (data_avgs[x] > L0)
		{
			y = 0;
		}
		else
		{
			color = 0;
		}
		
		matrix.drawLine(x, 0, x, y, color);

		//Peak
		bool peak = false;
		if (peaks[x] > data_avgs[x] && peaks[x] > L4)
		{
			peak = true;
			y = 4;
		}
		else if (peaks[x] > data_avgs[x] && peaks[x] > L3)
		{
			peak = true;
			y = 3;
		}
		else if (peaks[x] > data_avgs[x] && peaks[x] > L2)
		{
			peak = true;
			y = 2;
		}
		else if (peaks[x] > data_avgs[x] && peaks[x] > L1)
		{
			peak = true;
			y = 1;
		}
	
		if (peak)
		{
			colormap = map(peaks[x], 0, 15, 0, 255);
			color = Wheel(colormap);
			matrix.drawPixel(x, y, color);
		}
	}


	//if (DEBUG)
	//{
	//	Serial.print("  ");
	//	Serial.print(max);
	//	Serial.print("\t");
	//	Serial.print(min);

	//	Serial.println();

	//}

	matrix.show();



	//debugLoop++;
	//if (DEBUG && (debugLoop > 99))
	//{
	//	Serial.print("Free RAM = ");
	//	Serial.println(freeRam(), DEC);
	//	Serial.println(millis(), DEC);

	//	debugLoop = 0;
	//}
}


int freeRam() {
	extern int __heap_start, *__brkval;
	int v;
	return (int)&v - (__brkval == 0 ? (int)&__heap_start : (int)__brkval);
}

int DecayTest = 0;

void decay(int decayrate){
	
	DecayTest++;
	// reduce the values of the last peaks by 1 
	if (DecayTest == decayrate){
		for (int x = 0; x < MATRIX_X; x++) {
			peaks[x] = peaks[x] - 1;  // subtract 1 from each column peaks
			
		}
		DecayTest = 0;
	}

	
	

}

