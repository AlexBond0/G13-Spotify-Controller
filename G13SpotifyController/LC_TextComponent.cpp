#include "LC_TextComponent.h"



LC_TextComponent::LC_TextComponent(int width, int posx, int posy)
	: TextComponent(width, 11, posx, posy)
{
	// load text
	UIContainer* rawText = new UIContainer(L"../Text/Lucida Sans/text.bmp");

	// load JSON data
	_json textLayout = LoadTextLayout(L"../Text/Lucida Sans/layout.json");

	// create individual containers for each char
	ConstructFont(*rawText, textLayout);
}


LC_TextComponent::~LC_TextComponent()
{
}