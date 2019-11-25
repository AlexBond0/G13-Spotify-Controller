#pragma once
#include "Window.h"
#include "Requester.h"
#include "G13.h"

class CurrentlyPlaying_W :
	public Window
{
public:
	CurrentlyPlaying_W(Requester* requester);
	~CurrentlyPlaying_W();

	void CreateContainers(_json currentPlayback);

private:

	Requester* spotify;
	_json currentPlayback;

	std::string currentTrackID;

	int jsonProgress;
	int jsonSongLength;

	int previousTime;
	int newTime;
	int currentTimePassed;

	bool sleepModeActive = false;

	void Render();
	void APIPoll();

	void OnAPITimer();
	void OnBtnChange();

	float CalculateSongProgress(int timePassed);

	void UpdateSongContainers();
	void UpdatePlaybackContainers();

	void CalculateCurrentTime();
};

