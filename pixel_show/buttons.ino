

void loadButtons()
{
	// Get current button state.
	for (int i = 0; i < PAD_COUNT; i++)
	{
		newState[i] = digitalRead(pad_pin[i]);

		// Check if state changed from high to low (button press).
		if (newState[i] == LOW && oldState[i] == HIGH) {
			// Short delay to debounce button.
			delay(20);
			// Check if button is still low after debounce.
			newState[i] = digitalRead(pad_pin[i]);
			if (newState[i] == LOW) {
				showType++;
				if (showType > 9)
					showType = 0;
				startShow(showType);
			}
		}

		// Set the last button state to the old state.
		oldState[i] = newState[i];
	}
}



//Gets the mode based on the two pins.  0 = off, 1 = mode1, 2 = mode2
int getMode()
{
	int mode1 = digitalRead(MODE1_PIN);
	int mode2 = digitalRead(MODE2_PIN);

	int mode = 0;  //default mode 0

	if (mode1 && !mode2) { mode = 2; }
	if (!mode1 && mode2) { mode = 1; }

	return mode;
}

int didModeChange()
{
	int newMode = getMode();
	if (newMode != currentMode)
	{
		return true;
	}
	return false;
}

void readControls()
{
	if (!digitalRead(SAVE_PIN))
	{
		int newinterval = analogRead(0);

		if (interval != newinterval)
		{
			interval = newinterval;
			EEPROM_writeAnything(0, interval);
		}
	}
}

