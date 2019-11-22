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

	void AddValue(std::string name, std::wstring filename);
	void AddValue(std::string name, UIContainer* container);

	void SetValue(std::string name);

private:
	std::map<std::string, UIContainer*> value;

};

