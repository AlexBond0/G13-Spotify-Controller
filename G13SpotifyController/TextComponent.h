#pragma once

#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
using _json = nlohmann::json;

#include <vector>
#include <map>

#include "Component.h"


class TextComponent:
	public Component
{
public:
	TextComponent(int width, int height, int posx, int posy);
	~TextComponent();

protected:
	_json LoadTextLayout(std::wstring filename);
	void ConstructFont(UIContainer rawText, _json layout);

	std::map<std::string, UIContainer*> dictionary;

private:
};

