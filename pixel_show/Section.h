
#ifndef Section_h
#define Section_h

class SectionClass
{
private:
	int length;
	int stepCount;
	int colorCount;
public:

	int id;
	int first;
	int last;
	int dir;
	int state;
	unsigned long lastmillis;
	

	SectionClass() :
		first(0),
		last(0),
		dir(1),
		length(-1),
		state(0)
	{}
	void Init(int p_id, int p_first, int p_last, int p_dir, int p_state);
	int GetPixel(int val);
	int Length();
	int GetColorCount();
	void IncrementColorCount(int val);
	int GetStepCount();
	void IncrementStepCount();
	void PrintStatus();
	void SetLastMillis(unsigned long val);

};

#endif