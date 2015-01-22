void initPads()
{
	//initialize pad inputs
	for (int i = PAD_PIN_START; i < PAD_COUNT; i++)
	{
		pinMode(i, INPUT_PULLUP);
		oldState[i] = HIGH;
		pad_pin[i] = i + 0; //Start Pad Pins on pin 0
	}
}

void initSections()
{
	Sections[0].first = 3;
	Sections[0].last = 0;
	Sections[0].dir = -1;



	////initialize Sections
	//for (int i = 0; i < SECTION_COUNT; i++)
	//{
	//	section[i].first = (i * SECTION_PIXEL_COUNT);
	//	section[i].last = ((i + 1) * SECTION_PIXEL_COUNT) - 1;
	//}
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
