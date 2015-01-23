void initPads()
{
	//initialize pad inputs
	for (int i = PAD_PIN_START; i < PAD_PIN_START + PAD_COUNT; i++)
	{
		pinMode(i, INPUT_PULLUP);
		oldState[i] = HIGH;
		pad_pin[i] = i;
	}
}

void initSections()
{
	Sections[0].first = 26;
	Sections[0].last = 0;
	Sections[0].dir = -1;
	Sections[0].state = OFF;

	Sections[1].first = 27;
	Sections[1].last = 53;
	Sections[1].dir = 1;
	Sections[1].state = OFF;

	Sections[2].first = 80;
	Sections[2].last = 54;
	Sections[2].dir = -1;
	Sections[2].state = OFF;

	Sections[3].first = 81;
	Sections[3].last = 107;
	Sections[3].dir = 1;
	Sections[3].state = OFF;

	Sections[4].first = 134;
	Sections[4].last = 108;
	Sections[4].dir = -1;
	Sections[4].state = OFF;

	Sections[5].first = 135;
	Sections[5].last = 161;
	Sections[5].dir = 1;
	Sections[5].state = OFF;

	/*Sections[0].first = 26;
	Sections[0].last = 0;
	Sections[0].dir = -1;

	Sections[1].first = 27;
	Sections[1].last = 53;
	Sections[1].dir = 1;*/

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
