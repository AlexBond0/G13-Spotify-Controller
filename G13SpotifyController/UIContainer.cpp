#include "UIContainer.h"



UIContainer::UIContainer(int width, int height, int posx, int posy) {

	UIdata = new BYTE[width * height];
	std::fill_n(UIdata, width * height, 0);

	dataWidth = width;
	dataHeight = height;

	positionX = posx;
	positionY = posy;


}

UIContainer::UIContainer(std::wstring filename, int posx, int posy) {

	// Start Gdiplus 
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, NULL);

	HDC hdc = ::GetDC(NULL);
	const WCHAR* filepointer = filename.c_str();

	Gdiplus::Bitmap* image = Gdiplus::Bitmap::FromFile(filepointer);

	// Get the bitmap handle
	HBITMAP hBitmap = NULL;
	Status status = image->GetHBITMAP(RGB(0, 0, 0), &hBitmap);

	BYTE* byteBitmapMono;
	
	if (status == Ok) {

		BITMAPINFO bitmapInfoMono = { 0 };
		bitmapInfoMono.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);

		// Check what we got
		int ret = GetDIBits(hdc, hBitmap, 0,
			0,
			NULL,
			&bitmapInfoMono, DIB_RGB_COLORS);

		int bitmapSize = bitmapInfoMono.bmiHeader.biWidth * bitmapInfoMono.bmiHeader.biHeight;

		byteBitmapMono = new BYTE[bitmapSize * 4];
		UIdata = new BYTE[bitmapSize];

		bitmapInfoMono.bmiHeader.biCompression = BI_RGB;

		// this value needs to be inverted, or else image will show up upside/down
		bitmapInfoMono.bmiHeader.biHeight = -bitmapInfoMono.bmiHeader.biHeight;

		// Gets the "bits" from the bitmap and copies them into a buffer 
		// which is pointed to by byteBitmapMono.
		ret = GetDIBits(hdc, hBitmap, 0,
			-bitmapInfoMono.bmiHeader.biHeight, // height here needs to be positive. Since we made it negative previously, let's reverse it again to make it positive.
			byteBitmapMono,
			(BITMAPINFO *)&bitmapInfoMono, 
			DIB_RGB_COLORS);

		// Transform 32bpp data into 8bpp. Let's just take the value of first of 4 bytes (Blue)
		for (int ii = 0; ii < (bitmapSize * 4); ii = ii + 4)  {
			UIdata[ii / 4] = byteBitmapMono[ii];
		}

		delete byteBitmapMono;
	}

	// delete things when done 
	if (image) {

		delete image;
		image = NULL;
	}

	// Shutdown Gdiplus 
	Gdiplus::GdiplusShutdown(g_gdiplusToken);
}


UIContainer::~UIContainer() {

	delete UIdata;
}

// set single pixel in the container, returns if success
bool UIContainer::SetPixel(int x, int y, BYTE state) {

	// new pixel is within data range
	if (x >= 0 && x <= dataWidth && y >= 0 && y <= dataHeight) {

		UIdata[(dataWidth * y) + x] = state;
		return true;
	}
	else
		return false;
}

// get single pixel
BYTE UIContainer::GetPixel(int x, int y) {

	// new pixel is within data range
	if (x >= 0 && x <= dataWidth && y >= 0 && y <= dataHeight) {

		return UIdata[(dataWidth * y) + x];
	}
	else
		return 0;
}

// return full container
BYTE* UIContainer::GetContainer() {

	return UIdata;
}

// imprint the right-hand container onto the left-hand container
UIContainer operator<< (UIContainer& lhs, UIContainer& rhs) {

	// get widths and heights once
	int rhWidth = rhs.GetWidth();
	int rhHeight = rhs.GetHeight();
	int lhWidth = lhs.GetWidth();
	int lhHeight = lhs.GetHeight();

	// calculate offset between containers
	int offsetX = rhs.GetPosX() - lhs.GetPosX();
	int offsetY = rhs.GetPosY() - lhs.GetPosY();


	int relativeX;
	int relativeY;

	// loop over right hand container
	for (int rhx = 0; rhx < rhWidth; rhx++) {
		for (int rhy = 0; rhy < rhHeight; rhy++) {

			relativeX = rhx + offsetX;
			relativeY = rhy + offsetY;

			// set pixel in the left hand container according to
			lhs.SetPixel(relativeX, relativeY, rhs.GetPixel(rhx, rhy));
		}
	}

	return lhs;
}

int UIContainer::GetWidth() {

	return dataWidth;
}

int UIContainer::GetHeight() {

	return dataHeight;
}

int UIContainer::GetPosX() {

	return positionX;
}

int UIContainer::GetPosY() {

	return positionY;
}