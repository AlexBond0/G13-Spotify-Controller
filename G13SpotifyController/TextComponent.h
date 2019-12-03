#pragma once

#include <vector>
#include <map>

#include "Component.h"


struct Character {
	UIContainer* ui;
	int width;
};


class TextComponent:
	public Component
{
public:
	TextComponent(int width, int height, int posx, int posy);
	~TextComponent();

	void RenderText(std::string text);

protected:
	_json LoadTextLayout(std::wstring filename);
	void ConstructFont(UIContainer rawText, _json layout);

	std::map<char, Character> dictionary;

	bool hasUppercase;

private:
	
	std::string currentText = "";
	bool hasLoaded = false;
};

