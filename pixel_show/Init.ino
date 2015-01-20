void initPads()
{
	//initialize pad inputs
	for (int i = PAD_PIN_START; i < PAD_COUNT + PAD_PIN_START; i++)
	{
		pinMode(i, INPUT_PULLUP);
		oldState[i] = HIGH;
		pad_pin[i] = i + 0; //Start Pad Pins on pin 0
	}
}

void initSections()
{
	section[0].first = 3;
	section[0].last = 0;
	section[0].dir = -1;

	//section[0].first = 29;
	//section[0].last = 0;
	//section[0].dir = -1;

	//section[1].first = 30;
	//section[1].last = 59;
	//section[1].dir = 1;

}

void initControls()
{
	pinMode(SAVE_PIN, INPUT_PULLUP);
	pinMode(MODE1_PIN, INPUT_PULLUP);
	pinMode(MODE2_PIN, INPUT_PULLUP);

	pinMode(MODE1_LED, OUTPUT);
	pinMode(MODE2_LED, OUTPUT);
	pinMode(ON_LED, OUTPUT);

}
