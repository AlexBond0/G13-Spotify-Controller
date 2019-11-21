#pragma once
#include "Component.h"
class ProgressBar_C :
	public Component
{
public:
	ProgressBar_C(int width, int posx, int posy);
	~ProgressBar_C();

	void SetProgress(float progress);

private:

	float barProgress;
	int barWidth;

	void ConstructBar();
};

