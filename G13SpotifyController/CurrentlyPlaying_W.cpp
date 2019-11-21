#include "CurrentlyPlaying_W.h"


CurrentlyPlaying_W::CurrentlyPlaying_W(Requester* requester) {

	spotify = requester;

	_json currentPlayback = spotify->GetCurrentPlayback();
	CreateContainers(currentPlayback);

	Run();
}


CurrentlyPlaying_W::~CurrentlyPlaying_W()
{
}

void CurrentlyPlaying_W::CreateContainers(_json currentPlayback) {

	if (currentPlayback["is_playing"]) {

		// main song title text
		TextComponent* text = new Lucida_TC(LOGI_LCD_MONO_WIDTH - 1, 1, 0);
		text->RenderText(currentPlayback["item"]["name"]);
		components["title"] = text;

		// artist title text
		text = new Compact_TC(LOGI_LCD_MONO_WIDTH - 1, 1, 11);
		text->RenderText(currentPlayback["item"]["artists"][0]["name"]);
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

		// calculate time diffs
		int songLength = currentPlayback["item"]["duration_ms"];
		int progress = currentPlayback["progress_ms"];

		// stops a divide by zero situation
		// and song playback is in ms so no accuracy lost
		progress++;
		float precentageThrough = (float)progress / (float)songLength;

		// loading bar(float)
		ProgressBar_C* loadBar = new ProgressBar_C(
			LOGI_LCD_MONO_WIDTH - 8,
			4,
			37
		);
		loadBar->SetProgress(precentageThrough);
		components["loadBar"] = loadBar;

		// timers
		Timer_C* timer = new Timer_C(3, 29, progress / 1000);
		components["timerA"] = timer;

		timer = new Timer_C(140, 29, ((songLength - progress) / 1000));
		components["timerB"] = timer;
	}
}

