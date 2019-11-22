#pragma once

#include "UIContainer.h"

#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
using _json = nlohmann::json;

class Component
{
public:
	Component(int width, int height, int posx, int posy);
	Component(std::wstring filename, int posx, int posy);
	Component(std::wstring filename);
	~Component();

	void Render(UIContainer* screen);

protected:

	UIContainer _ui;

	int positionX;
	int positionY;
	
};

