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
	UIContainer(int width, int height, int posx, int posy);
	UIContainer(std::wstring filename, int posx, int posy);
	~UIContainer();

	bool SetPixel(int x, int y, BYTE state);
	BYTE GetPixel(int x, int y);

	BYTE* GetContainer();

	int GetWidth();
	int GetHeight();
	int GetPosX();
	int GetPosY();

private:

	ULONG_PTR g_gdiplusToken;

	BYTE* UIdata;

	int dataWidth;
	int dataHeight;
	int positionX;
	int positionY;
};

UIContainer operator<< (UIContainer& lhs, UIContainer& rhs);