#pragma once

#include <vector>
#include <map>

#include "Component.h"


#define SPACE_WIDTH 4;
#define SCROLL_PAUSE 20;

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

	void SetScroll(int scroll);
	void SetAutoScroll(bool scrolling);
	void Update();

protected:
	_json LoadTextLayout(std::wstring filename);
	void ConstructFont(UIContainer rawText, _json layout);

	std::map<char, Character> dictionary;

	bool hasUppercase;

private:

	std::string currentText = "";
	bool hasLoaded = false;
	
	int renderableTextWidth;

	bool autoScroll = false;
	bool isScrolling = false;
	bool isTooLong;
	bool scrollForewards = true;

	int currentTextScroll;
	int maxScroll;
	int scrollWait;

	void RenderSetup();
	void TextToUI();

	int GetSafeWidth(char letter);
	Character GetSafeCharacter(char letter);
};

