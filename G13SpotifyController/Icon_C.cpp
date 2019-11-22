#include "Icon_C.h"



Icon_C::Icon_C(int width, int height, int posx, int posy)
	: Component(width, height, posx, posy)
{
}


Icon_C::~Icon_C() {

	for (std::pair<std::string, UIContainer*> icon : value) {

		if (icon.second)
			delete icon.second;
	}
}


void Icon_C::AddValue(std::string name, std::wstring filename) {

	UIContainer* newValue = new UIContainer(filename, _ui.GetPosX(), _ui.GetPosY());
	value[name] = newValue;
}

void Icon_C::AddValue(std::string name, UIContainer* container) {

	value[name] = container;
}

void Icon_C::SetValue(std::string name) {

	_ui.Imprint(*value[name]);
}