#include "CurrentlyPlaying_W.h"


CurrentlyPlaying_W::CurrentlyPlaying_W(Requester* requester) {

	// initial spotify API poll
	spotify = requester;
	APIPoll();

	// create UI and run main loop
	CreateContainers(currentPlayback);
	UpdateSongContainers();
	Run();
}


CurrentlyPlaying_W::~CurrentlyPlaying_W() {

}

void CurrentlyPlaying_W::CreateContainers(_json currentPlayback) {

	if (currentPlayback["is_playing"]) {

		// main song title text
		TextComponent* text = new Lucida_TC(LOGI_LCD_MONO_WIDTH - 1, 1, 0);
		// text->RenderText(currentPlayback["item"]["name"]);
		components["title"] = text;

		// artist title text
		text = new Compact_TC(LOGI_LCD_MONO_WIDTH - 1, 1, 11);
		// text->RenderText(currentPlayback["item"]["artists"][0]["name"]);
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
		// int songLength = currentPlayback["item"]["duration_ms"];
		// int progress = currentPlayback["progress_ms"];

		// loading bar
		ProgressBar_C* loadBar = new ProgressBar_C(
			LOGI_LCD_MONO_WIDTH - 8,
			4,
			30
		);
		// loadBar->SetProgress(CalculateSongProgress(0));
		components["loadBar"] = loadBar;

		// timers
		Timer_C* timer = new Timer_C(3, 22); // , progress / 1000);
		components["timerA"] = timer;

		timer = new Timer_C(140, 22); // , ((songLength - progress) / 1000));
		components["timerB"] = timer;

		// icons
		Icon_C::LoadIcons();

		Icon_C* shuffle = new Icon_C(10, 10, 60, 34);
		// shuffle->SetValue("shuffle_on");
		components["shuffle"] = shuffle;

		Icon_C* play = new Icon_C(10, 10, 75, 34);
		// play->SetValue("play");
		components["play"] = play;

		Icon_C* replay = new Icon_C(10, 10, 84, 34);
		// replay->SetValue("repeat_on");
		components["replay"] = replay;
	}
}

void CurrentlyPlaying_W::Render() {

	UpdatePlaybackContainers();

	Window::Render();
}

void CurrentlyPlaying_W::APIPoll() {

	// get the current playback status
	currentPlayback = spotify->GetCurrentPlayback();

	// save commonly used values
	jsonProgress = currentPlayback["progress_ms"].get<int>() - 1000;
	jsonSongLength = currentPlayback["item"]["duration_ms"].get<int>();
	previousTime = ::GetTickCount();

	// keep track of when the song changes 
	if (currentTrackID == "") {
		currentTrackID = currentPlayback["item"]["id"].get<std::string>();
	}

	else if (currentTrackID != currentPlayback["item"]["id"].get<std::string>()) {
		UpdateSongContainers();
		currentTrackID = currentPlayback["item"]["id"].get<std::string>();
	}
}

float CurrentlyPlaying_W::CalculateSongProgress(int timePassed) {

	// stops a divide by zero situation
	if (jsonProgress == 0)
		jsonProgress = 1;

	return (float)(jsonProgress + timePassed) / (float)(jsonSongLength);
}

void CurrentlyPlaying_W::UpdateSongContainers() {

	// main song title text
	static_cast<TextComponent*>(components["title"])
		->RenderText(currentPlayback["item"]["name"]);

	// artist title text
	static_cast<TextComponent*>(components["artist"])
		->RenderText(currentPlayback["item"]["artists"][0]["name"]);
}

void CurrentlyPlaying_W::UpdatePlaybackContainers() {

	// get time elapsed
	newTime = ::GetTickCount();
	currentTimePassed = newTime - previousTime;

	// loading bar
	static_cast<ProgressBar_C*>(components["loadBar"])
		->SetProgress(CalculateSongProgress(currentTimePassed));

	// timers
	int progress = jsonProgress + currentTimePassed;
	int timePassed = (float)progress / 1000.0f;
	int timeLeft = (jsonSongLength - progress) / 1000.0f;

	static_cast<Timer_C*>(components["timerA"])
		->SetTime(timePassed);

	static_cast<Timer_C*>(components["timerB"])
		->SetTime(timeLeft);

	// icons
	static_cast<Icon_C*>(components["shuffle"])
		->SetValue(
			currentPlayback["shuffle_state"]
			? "shuffle_on"
			: "shuffle_off"
		);

	static_cast<Icon_C*>(components["play"])
		->SetValue(
			currentPlayback["is_playing"]
			? "play"
			: "pause"
		);

	static_cast<Icon_C*>(components["replay"])
		->SetValue("repeat_on");
}