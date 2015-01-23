
#ifndef Section_h
#define Section_h

class SectionClass
{
private:
	int length;
	int stepCount;
	int colorCount;
public:

	int first;
	int last;
	int dir;
	int state;
	uint64_t lastmillis;



	SectionClass() :
		first(0),
		last(0),
		dir(1),
		length(-1),
		state(0)
	{}
	int GetPixel(int val);
	int Length();
	int GetColorCount();
	void IncrementColorCount(int val);
	int GetStepCount();
	void IncrementStepCount();

};

#endif