

void loadButtons()
{
	
	// Get current button state.
	for (int i = 0; i < PAD_COUNT; i++)
	{
		int n = digitalRead(pad_pin[i]);

		// Check if state changed from high to low (button press).
		if (n == LOW && oldState[i] == HIGH) {
			// Short delay to debounce button.
			delay(20);
			// Check if button is still low after debounce.
			n = digitalRead(pad_pin[i]);
			if (n == LOW) {
				newState[i] = LOW;
			}
		}
		else
		{
			newState[i] = HIGH;
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