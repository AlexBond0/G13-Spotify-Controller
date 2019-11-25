#include "Compact_TC.h"



Compact_TC::Compact_TC(int width, int posx, int posy)
	: TextComponent(width, 7, posx, posy)
{
	// get correct folder path
	std::wstring folder = Tools::GetFolder("Text");

	// load text
	UIContainer* rawText = new UIContainer(folder + L"/Compact/text.bmp");

	// load JSON data
	_json textLayout = LoadTextLayout(folder + L"/Compact/layout.json");

	// create individual containers for each char
	ConstructFont(*rawText, textLayout);

	// delete rawText;
}

Compact_TC::~Compact_TC()
{
}
