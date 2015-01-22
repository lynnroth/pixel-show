

void pixels_Off()
{
	uint32_t c = strip.Color(0, 0, 0);
	for (uint16_t i = 0; i < strip.numPixels(); i++) {
		strip.setPixelColor(i, c);
	}
	strip.show();
}
			
//
//void startShow(int i) {
//	switch (i)
//	{
//		case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
//			break;
//		case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
//			break;
//		case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
//			break;
//		case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
//			break;
//		case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
//			break;
//		case 5: theaterChase(strip.Color(127, 0, 0), 50); // Red
//			break;
//		case 6: theaterChase(strip.Color(0, 0, 127), 50); // Blue
//			break;
//		case 7: rainbow(20);
//			break;
//		case 8: rainbowCycle(20);
//			break;
//		case 9: theaterChaseRainbow(50);
//			break;
//	}
//}
//
//// Fill the dots one after the other with a color
//void colorWipe(uint32_t c, uint8_t wait) {
//	for (uint16_t i = 0; i < strip.numPixels(); i++) {
//		strip.setPixelColor(i, c);
//		strip.show();
//		if (didModeChange) { return; }
//		delay(wait);
//	}
//}
//
//void rainbow(uint8_t wait) {
//	uint16_t i, j;
//
//	for (j = 0; j < 256; j++) {
//		for (i = 0; i < strip.numPixels(); i++) {
//			strip.setPixelColor(i, Wheel((i + j) & 255));
//		}
//		strip.show();
//		if (didModeChange) { return; }
//		delay(wait);
//	}
//}
//
//// Slightly different, this makes the rainbow equally distributed throughout
//void rainbowCycle(uint8_t wait) {
//	uint16_t i, j;
//
//	for (j = 0; j < 256 * 5; j++) { // 5 cycles of all colors on wheel
//		for (i = 0; i < strip.numPixels(); i++) {
//			strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
//		}
//		strip.show();
//		if (didModeChange) { return; }
//		delay(wait);
//	}
//}
//
////Theatre-style crawling lights.
//void theaterChase(uint32_t c, uint8_t wait) {
//	int breakNow = 0;
//
//	for (int j = 0; j < 10; j++) {  //do 10 cycles of chasing
//	//	if (didModeChange) { break; }
//		for (int q = 0; q < 3; q++) {
//			for (int i = 0; i < strip.numPixels(); i = i + 3) {
//				strip.setPixelColor(i + q, c);    //turn every third pixel on
//			}
//			strip.show();
//			
//			delay(wait);
//
//			for (int i = 0; i < strip.numPixels(); i = i + 3) {
//				strip.setPixelColor(i + q, 0);        //turn every third pixel off
//			}
//		}
//	}
//}
//
////Theatre-style crawling lights with rainbow effect
//void theaterChaseRainbow(uint8_t wait) {
//	int breakNow = 0;
//
//	for (int j = 0; j < 256; j++) {     // cycle all 256 colors in the wheel
//	//	if (didModeChange) { break; }
//		for (int q = 0; q < 3; q++) {
//			for (int i = 0; i < strip.numPixels(); i = i + 3) {
//				strip.setPixelColor(i + q, Wheel((i + j) % 255));    //turn every third pixel on
//			}
//			strip.show();
//			
//			delay(wait);
//
//			for (int i = 0; i < strip.numPixels(); i = i + 3) {
//				strip.setPixelColor(i + q, 0);        //turn every third pixel off
//			}
//		}
//	}
//}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
	WheelPos = 255 - WheelPos;
	if (WheelPos < 85) {
		return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
	}
	else if (WheelPos < 170) {
		WheelPos -= 85;
		return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
	}
	else {
		WheelPos -= 170;
		return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
	}
}

