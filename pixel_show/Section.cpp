#include <Arduino.h>
#include "Section.h"


void SectionClass::Init(int p_id, int p_first, int p_last, int p_dir, int p_state)
{
	id = p_id;
	first = p_first;
	last = p_last;
	dir = p_dir;
	state = p_state;
	
}

void SectionClass::SetLastMillis(unsigned long val)
{
	lastmillis = val;
}

int SectionClass::GetPixel(int val)
{
	if (dir == -1)
	{
		return first - val;
	}
	else
	{
		return first + val;
	}
}

int SectionClass::Length()
{
	if (length <= 0)
	{
		length = abs(last - first) + 1;
	}
	return length;
}

int SectionClass::GetColorCount()
{
	return colorCount;
}

void SectionClass::IncrementColorCount(int val)
{
	colorCount += val;
	if (colorCount > 255)
	{
		colorCount = 0;
	}
}

int SectionClass::GetStepCount()
{
	return stepCount;
}
void SectionClass::IncrementStepCount()
{
	stepCount++;
	if (stepCount >= 3)
	{
		stepCount = 0;
	}
}

void SectionClass::PrintStatus()
{
	Serial.print("Section:"); 
	//Serial.print(first); 
	//Serial.print(",");
	//Serial.print(last);
	//Serial.print("->");
	//Serial.print(Length());
	Serial.print(id);
	Serial.print(":\t");
	Serial.print(stepCount);
	Serial.print("\t");
	Serial.print(colorCount);
	Serial.print("\t");
	Serial.print(state);
	Serial.print("\t");

	Serial.println();
}

