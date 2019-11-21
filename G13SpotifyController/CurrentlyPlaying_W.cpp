#include "CurrentlyPlaying_W.h"


CurrentlyPlaying_W::CurrentlyPlaying_W() {

	CreateContainers();
	Run();
}


CurrentlyPlaying_W::~CurrentlyPlaying_W()
{
}

void CurrentlyPlaying_W::CreateContainers() {

	// main song title text
	TextComponent* text = new Lucida_TC(LOGI_LCD_MONO_WIDTH -1, 1, 0);
	text->RenderText("A really long song name that keeps going and going");
	components["title"] = text;

	// artist title text
	text = new Compact_TC(LOGI_LCD_MONO_WIDTH -1, 1, 11);
	text->RenderText("A guy on spotify with a long name lol its a really long one");
	components["artist"] = text;

	// sepearting bar
	Component* bar = new Bar_C(
		1, 
		18, 
		Direction::HORIZONTAL, 
		LOGI_LCD_MONO_WIDTH - 1,
		BarType::DOTTED
	);
	components["topBar"] = bar;
}

