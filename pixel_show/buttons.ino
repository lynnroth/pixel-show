
#define DEBOUNCE_TIME 10

void loadButtons()
{
	
	// Get current button state.
	for (int i = 0; i < PAD_COUNT; i++)
	{
		int n = digitalRead(pad_pin[i]);

		// Check if state changed from high to low (button press).
		if (n == LOW && oldState[i] == HIGH) {
			// Short delay to debounce button.
			delay(DEBOUNCE_TIME);
			// Check if button is still low after debounce.
			n = digitalRead(pad_pin[i]);
			if (n == LOW) {
				oldState[i] = newState[i]; 
				newState[i] = LOW;
			}
		}
		else if (n == HIGH && oldState[i] == LOW)
		{
			delay(DEBOUNCE_TIME);
			// Check if button is still low after debounce.
			n = digitalRead(pad_pin[i]);
			if (n == HIGH) {
				oldState[i] = newState[i]; 
				newState[i] = HIGH;
			}
		}
		
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