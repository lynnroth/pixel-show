#include <Arduino.h>
#include "Section.h"




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
	if (this->length <= 0)
	{
		this->length = abs(this->last - this->first) + 1;
	}
	return this->length;
}

int SectionClass::GetColorCount()
{
	return this->colorCount;
}

void SectionClass::IncrementColorCount(int val)
{
	this->colorCount += 10;
	if (this->colorCount > 255)
	{
		this->colorCount = 0;
	}
}

int SectionClass::GetStepCount()
{
	return this->stepCount;
}
void SectionClass::IncrementStepCount()
{
	Serial.print("\t\tthis->stepCount: ");
	Serial.print(this->stepCount);
	int val = this->stepCount + 1;
	(this->stepCount) = val;
	Serial.print("\t\tVal: ");
	Serial.print(val);
	Serial.print("\t\tIncrement: ");
	Serial.print(this->stepCount);
	if (this->stepCount > 3)
	{
		Serial.print("\t\tReset: ");

		this->stepCount = 0;
	}
	Serial.println();
}


