#include "Timer_C.h"



Timer_C::Timer_C(int posx, int posy, int startSeconds)
	: Compact_TC(18, posx, posy)
{

	SetTime(startSeconds);
	// _ui.inverted = true;
}

Timer_C::Timer_C(int posx, int posy)
	: Compact_TC(18, posx, posy)
{

	SetTime(0);
}


Timer_C::~Timer_C()
{
}

void Timer_C::SetTime(int seconds) {

	if (seconds > 0) {

		// calculate times
		int mins = (seconds / 60);
		int secs = (seconds % 60);

		std::string timeString = ("> 99");
		if (mins <= 99) {

			// pad numbers appropriately
			std::string minsS = (
				mins < 10 
				? "0" + std::to_string(mins) 
				: std::to_string(mins)
			);
			std::string secsS = (
				secs < 10 
				? "0" + std::to_string(secs) 
				: std::to_string(secs)
			);

			timeString = minsS + ":" + secsS;
		}

		RenderText(timeString);
	}
	else
		RenderText("00:00");
}