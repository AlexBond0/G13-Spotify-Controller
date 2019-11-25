#include <windows.h>

#include <LogitechLCDLib.h>
#include "Requester.h"
#include "CurrentlyPlaying_W.h"

#include <string>

int WINAPI WinMain(HINSTANCE inst, HINSTANCE prev, LPSTR cmd, int show)
{
	OutputDebugString("\nLoading Setup File...");
	Tools::ReadSetupFile();

	OutputDebugString("\nLoading G13...\n");

	// connect to G13
	std::wstring message = L"Spotify Controller";
	bool ret = LogiLcdInit(&message[0], LOGI_LCD_TYPE_MONO);

	if (ret == TRUE) {

		OutputDebugString("\nG13 CONNECTED");

		Requester* spotify = new Requester();

		// make sure user credentials are valid
		if (!spotify->Initiate()) {

			spotify->OpenSpotifyLogin();
		}

		Window w = CurrentlyPlaying_W(spotify);

		delete spotify;
	}
	else {

		OutputDebugString("\nG13 NOT CONNECTED");
	}

	LogiLcdShutdown();
	return 0;
}
