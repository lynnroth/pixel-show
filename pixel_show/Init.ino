void initPads()
{
	//initialize pad inputs
	for (int i = PAD_PIN_START; i < PAD_PIN_START + PAD_COUNT; i++)
	{
		pinMode(i, INPUT_PULLUP);
		oldState[i] = HIGH;
		newState[i] = HIGH;
		pad_pin[i] = i;
	}
}

void initSections()
{
	Sections[0].Init(0, 26, 0, -1, OFF);
	Sections[1].Init(1, 27, 53, 1, OFF);
	Sections[3].Init(2, 54, 80, 1, OFF);
	Sections[2].Init(3, 107, 81, -1, OFF);
	Sections[4].Init(4, 134, 108, -1, OFF);
	Sections[5].Init(5, 135, 161, 1, OFF);
	Sections[7].Init(6, 162, 188, 1, OFF);
	Sections[6].Init(7, 215, 189, -1, OFF);
	Sections[8].Init(8, 242, 216, -1, OFF);
	Sections[9].Init(9, 243, 269, 1, OFF);
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
