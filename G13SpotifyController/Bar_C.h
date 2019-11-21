#pragma once
#include "Component.h"

enum class Direction {
	VERTICAL,
	HORIZONTAL
};

enum class BarType {
	SOLID,
	DOTTED
};


class Bar_C :
	public Component
{
public:

	Direction direction;
	BarType barType;
	int barLength;

	Bar_C(int x, int y, Direction dir, int length, BarType type);
	~Bar_C();

private:

	int calculatedW(Direction dir, int length);
	int calculatedH(Direction dir, int length);

	void ConstructBar();
};

