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
	//initialize Sections
	for (int i = 0; i < SECTION_COUNT; i++)
	{
		section[i].first = (i * SECTION_PIXEL_COUNT);
		section[i].last = ((i + 1) * SECTION_PIXEL_COUNT) - 1;
	}
}