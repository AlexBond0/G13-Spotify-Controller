#pragma once
#include "Component.h"

#include <string>
#include <vector>
#include <map>

class Icon_C :
	public Component
{
public:
	Icon_C(int width, int height, int posx, int posy);
	~Icon_C();

	void SetValue(std::string name);

	static void LoadIcons();

private:

	static std::map<std::string, UIContainer*> icons;
	static void ConstructIcons(UIContainer rawIcons, _json layout);

};

