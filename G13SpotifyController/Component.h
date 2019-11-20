#pragma once

#include "UIContainer.h"


class Component
{
public:
	Component(int width, int height, int posx, int posy);
	Component(std::wstring filename, int posx, int posy);
	~Component();

	void Render(UIContainer* screen);

private:

	UIContainer _ui;
	
};

