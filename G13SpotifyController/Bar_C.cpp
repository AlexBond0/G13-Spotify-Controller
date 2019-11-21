#include "Bar_C.h"



Bar_C::Bar_C(int x, int y, Direction dir, int length, BarType type)
	: Component(
		calculatedW(dir, length),
		calculatedH(dir, length),
		x, 
		y
	) {

	direction = dir;
	barType = type;
	barLength = length;

	ConstructBar();
}


Bar_C::~Bar_C()
{
}

void Bar_C::ConstructBar() {

	int stride = (barType == BarType::SOLID ? 1 : 2);

	for (int p = 0; p < barLength; p += stride) {

		_ui.SetPixel(p, 0, _ui.PIXEL_ON);
	}
}

int Bar_C::calculatedW(Direction dir, int length) {

	return (dir == Direction::HORIZONTAL ? length : 1);
}

int Bar_C::calculatedH(Direction dir, int length) {

	return (dir == Direction::HORIZONTAL ? 1 : length);
}