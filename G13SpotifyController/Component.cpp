#include "Component.h"



Component::Component(int width, int height, int posx, int posy)
	: _ui(width, height)
{
	positionX = posx;
	positionY = posy;
}

Component::Component(std::wstring filename, int posx, int posy) 
	: _ui(filename)
{
	positionX = posx;
	positionY = posy;
}

Component::Component(std::wstring filename)
	: _ui(filename)
{

}

Component::~Component()
{
}


void Component::Render(UIContainer* screen) {

	if (IsVisible)
		screen->Imprint(_ui, positionX, positionY);
}