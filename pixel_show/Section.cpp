#include <Arduino.h>


class SectionClass
{
private:
	int length;
public:
	
	int first;
	int last;
	int dir;
	int state;
	SectionClass() : first(0), last(0), dir(1), length(-1), state(0) {}
	int GetPixel(int val)
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
	int Length()
	{
		if (length <= 0)
		{
			length = abs(last - first) + 1;
		}
		return length;
	}
};


