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
	text->RenderText("NIT3 TALES MIX");
	components["title"] = text;

	// artist title text
	text = new Compact_TC(LOGI_LCD_MONO_WIDTH -1, 1, 11);
	text->RenderText("SHIFT K3Y, DJ Zinc, MJ Cole, Chris Lorenzo");
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

	// loading bar
	ProgressBar_C* loadBar = new ProgressBar_C(
		LOGI_LCD_MONO_WIDTH - 8,
		4,
		30
	);
	loadBar->SetProgress(0.3);
	components["loadBar"] = loadBar;
}

