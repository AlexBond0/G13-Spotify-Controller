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

	// main song title text
	TextComponent* text = new Lucida_TC(LOGI_LCD_MONO_WIDTH - 1, 1, 0);
	components["title"] = text;

	// artist title text
	text = new Compact_TC(LOGI_LCD_MONO_WIDTH - 1, 1, 11);
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
	components["loadBar"] = loadBar;

	// timers
	Timer_C* timer = new Timer_C(3, 22);
	components["timerA"] = timer;

	timer = new Timer_C(140, 22);
	components["timerB"] = timer;

	// icons
	Icon_C::LoadIcons();

	Icon_C* shuffle = new Icon_C(10, 10, 60, 34);
	components["shuffle"] = shuffle;

	Icon_C* play = new Icon_C(10, 10, 75, 34);
	components["play"] = play;

	Icon_C* replay = new Icon_C(10, 10, 84, 34);
	components["replay"] = replay;

	Icon_C* liked = new Icon_C(10, 10, 97, 34);
	components["liked"] = liked;

	Icon_C* sleep = new Icon_C(10, 10, 147, 34);
	components["sleep"] = sleep;
}

void CurrentlyPlaying_W::Render() {

	// check if a song has ended
	CalculateCurrentTime();
	if (CalculateSongProgress(currentTimePassed) > 0.99) {

		// stop constant API calls
		sleepModeActive = true;
		APIPoll();
	}

	UpdatePlaybackContainers();

	Window::Render();
}

void CurrentlyPlaying_W::OnAPITimer() {

	if (!sleepModeActive)
		APIPoll();
}

void CurrentlyPlaying_W::APIPoll() {

	// get the current playback status
	currentPlayback = spotify->GetCurrentPlayback();

	// save commonly used values
	jsonProgress = currentPlayback["progress_ms"].get<int>() - 1000;
	jsonSongLength = currentPlayback["item"]["duration_ms"].get<int>();
	 
	previousTime = ::GetTickCount();
	newTime = ::GetTickCount();

	// keep track of when the song changes 
	if (currentTrackID == "") {
		currentTrackID = currentPlayback["item"]["id"].get<std::string>();
	}
	else if (currentTrackID != currentPlayback["item"]["id"].get<std::string>()) {

		// update song containers
		UpdateSongContainers();
		currentTrackID = currentPlayback["item"]["id"].get<std::string>();
	}
}

void CurrentlyPlaying_W::UpdateSongContainers() {

	// main song title text
	static_cast<TextComponent*>(components["title"])
		->RenderText(currentPlayback["item"]["name"]);

	// artist title text
	std::string artistList = 
		currentPlayback["item"]["artists"][0]["name"].get<std::string>();

	for (int x = 1; x < currentPlayback["item"]["artists"].size(); x++)
		artistList += (
			", " + 
			currentPlayback["item"]["artists"][x]["name"].get<std::string>()
		);

	static_cast<TextComponent*>(components["artist"])
		->RenderText(artistList);
}

void CurrentlyPlaying_W::UpdatePlaybackContainers() {

	// get time elapsed
	CalculateCurrentTime();

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

	std::string replayIcon;
	if (currentPlayback["repeat_state"] == "context") replayIcon = "repeat_on";
	else if (currentPlayback["repeat_state"] == "off") replayIcon = "repeat_off";
	else if (currentPlayback["repeat_state"] == "track") replayIcon = "repeat_once";
	static_cast<Icon_C*>(components["replay"])
		->SetValue(replayIcon);

	static_cast<Icon_C*>(components["liked"])
		->SetValue("like_off");

	static_cast<Icon_C*>(components["sleep"])
		->SetValue(
			sleepModeActive
			? "sleep_on"
			: "sleep_off"
		);
}

float CurrentlyPlaying_W::CalculateSongProgress(int timePassed) {

	// stops a divide by zero situation
	if (jsonProgress == 0)
		jsonProgress = 1;

	return (float)(jsonProgress + timePassed) / (float)(jsonSongLength);
}

void CurrentlyPlaying_W::OnBtnChange() {

	// come out of sleep mode
	sleepModeActive = false;
	APIPoll();
}

void CurrentlyPlaying_W::CalculateCurrentTime() {

	// get time elapsed
	if (currentPlayback["is_playing"])
		newTime = ::GetTickCount();

	currentTimePassed = newTime - previousTime;
}

void CurrentlyPlaying_W::OnBtn0Change(bool isPressed) {

}

void CurrentlyPlaying_W::OnBtn1Change(bool isPressed) {

}

void CurrentlyPlaying_W::OnBtn2Change(bool isPressed) {

}

void CurrentlyPlaying_W::OnBtn3Change(bool isPressed) {

}
