#include "Timer_C.h"



Timer_C::Timer_C(int posx, int posy, int startSeconds)
	: Compact_TC(19, posx, posy)
{

	SetTime(startSeconds);
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