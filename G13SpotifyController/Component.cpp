#include "Component.h"



Component::Component(int width, int height, int posx, int posy)
	: _ui(width, height, posx, posy)
{

}

Component::Component(std::wstring filename, int posx, int posy) 
	: _ui(filename, posx, posy)
{

}

Component::Component(std::wstring filename)
	: _ui(filename)
{

}

Component::~Component()
{
}


void Component::Render(UIContainer* screen) {

	screen->Imprint(_ui);
}