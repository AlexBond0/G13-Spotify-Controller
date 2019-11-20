#include "CurrentlyPlaying_W.h"


CurrentlyPlaying_W::CurrentlyPlaying_W() {

	CreateContainers();
}


CurrentlyPlaying_W::~CurrentlyPlaying_W()
{
}

void CurrentlyPlaying_W::CreateContainers() {


	components.push_back(new Component(L"SmallCircle.bmp", 100, 20));
	components.push_back(new Component(L"SmallCircle.bmp", 70, 30));
	components.push_back(new Component(L"SmallCircle.bmp", 120, 15));

	TextComponent* text = new Lucida_TC(100, 0, 0);
	text->RenderText("A song on spotify :)");
	components.push_back(text);

	text = new Compact_TC(100, 0, 20);
	text->RenderText("A song on spotify :)");
	components.push_back(text);

	Run();
}

