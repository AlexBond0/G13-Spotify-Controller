#pragma once
#include "Compact_TC.h"

class Timer_C :
	public Compact_TC
{
public:
	Timer_C(int posx, int posy, int startSeconds);
	~Timer_C();

	void SetTime(int seconds);

private:

};

