#include "ProgressBar_C.h"



ProgressBar_C::ProgressBar_C(int width, int posx, int posy)
	: Component(width, 4, posx, posy)
{

	barProgress = 0;
	barWidth = width;

	ConstructBar();
}


ProgressBar_C::~ProgressBar_C()
{
}

void ProgressBar_C::SetProgress(float progress) {

	if (progress >= 0 && progress <= 1) {

		barProgress = progress;
		ConstructBar();
	}
}

void ProgressBar_C::ConstructBar() {

	// render start cap
	for (int sc = 0; sc < 4; sc++)
		_ui.SetPixel(0, sc, _ui.PIXEL_ON);

	// render filled progression
	int pixelsToFill;
	if (barProgress != 0.0f)
		pixelsToFill = (float)barWidth * barProgress;
	else
		pixelsToFill = 0;

	for (int fp = 0; fp < pixelsToFill; fp++) {

		_ui.SetPixel(fp, 1, _ui.PIXEL_ON);
		_ui.SetPixel(fp, 2, _ui.PIXEL_ON);
	}

	// render filled cap
	for (int fc = 0; fc < 4; fc++)
		_ui.SetPixel(pixelsToFill, fc, _ui.PIXEL_ON);

	// render unfilled bar
	bool flipper = true;
	for (int fp = pixelsToFill + 1; fp < barWidth; fp++) {

		_ui.SetPixel(fp, 1, (flipper ? _ui.PIXEL_ON : _ui.PIXEL_OFF));
		_ui.SetPixel(fp, 2, (flipper ? _ui.PIXEL_OFF : _ui.PIXEL_ON));
		flipper = !flipper;
	}

	// render end cap
	for (int ec = 0; ec < 4; ec++)
		_ui.SetPixel(barWidth - 1, ec, _ui.PIXEL_ON);
}