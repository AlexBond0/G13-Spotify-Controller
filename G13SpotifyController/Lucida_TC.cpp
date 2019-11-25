#include "Lucida_TC.h"



Lucida_TC::Lucida_TC(int width, int posx, int posy)
	: TextComponent(width, 11, posx, posy)
{
	// get correct folder path
	std::wstring folder = Tools::GetFolder("Text");

	// load text
	UIContainer* rawText = new UIContainer(folder + L"/Lucida Sans/text.bmp");

	// load JSON data
	_json textLayout = LoadTextLayout(folder + L"/Lucida Sans/layout.json");

	// create individual containers for each char
	ConstructFont(*rawText, textLayout);
}


Lucida_TC::~Lucida_TC()
{
}