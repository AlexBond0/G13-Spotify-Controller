#pragma once

#include <Windows.h>
#include <algorithm> 
#include <string>
#include <vector>

// needed for image handling
 #include <gdiplus.h>
 using namespace Gdiplus;
 #pragma comment(lib,"gdiplus.lib")

class UIContainer
{
public:
	UIContainer(int width, int height);
	UIContainer(std::wstring filename);
	~UIContainer();

	void Imprint(UIContainer& stamp, int x, int y);
	void Clear();

	bool SetPixel(int x, int y, BYTE state);
	BYTE GetPixel(int x, int y);

	BYTE* GetContainer();

	int GetWidth();
	int GetHeight();
	// int GetPosX();
	// int GetPosY();

	const BYTE PIXEL_ON = 255;
	const BYTE PIXEL_OFF = 0;

	bool inverted;

private:

	void LoadFile(std::wstring filename);

	ULONG_PTR g_gdiplusToken;

	BYTE* UIdata;

	int dataWidth;
	int dataHeight;
	// int positionX;
	// int positionY;
};

UIContainer operator<< (UIContainer& lhs, UIContainer& rhs);