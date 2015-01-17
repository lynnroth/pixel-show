

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
