#include "Compact_TC.h"



Compact_TC::Compact_TC(int width, int posx, int posy)
	: TextComponent(width, 7, posx, posy)
{
	// load text
	UIContainer* rawText = new UIContainer(L"../Text/Compact/text.bmp");

	// load JSON data
	_json textLayout = LoadTextLayout(L"../Text/Compact/layout.json");

	// create individual containers for each char
	ConstructFont(*rawText, textLayout);
}

Compact_TC::~Compact_TC()
{
}
