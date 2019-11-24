#include "UIContainer.h"



UIContainer::UIContainer(int width, int height) {

	UIdata = new BYTE[width * height];
	std::fill_n(UIdata, width * height, 0);

	dataWidth = width;
	dataHeight = height;

	inverted = false;
}

UIContainer::UIContainer(std::wstring filename) {

	LoadFile(filename);
}


void UIContainer::LoadFile(std::wstring filename) {

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

		// get the size of the incoming bitmap
		int bitmapSize = bitmapInfoMono.bmiHeader.biWidth * bitmapInfoMono.bmiHeader.biHeight;
		dataWidth = bitmapInfoMono.bmiHeader.biWidth;
		dataHeight = bitmapInfoMono.bmiHeader.biHeight;

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

	if (UIdata)
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

void UIContainer::Imprint(UIContainer& stamp, int x, int y) {

	// get widths and heights once
	int rhWidth = stamp.GetWidth();
	int rhHeight = stamp.GetHeight();
	int lhWidth = GetWidth();
	int lhHeight = GetHeight();

	int relativeX;
	int relativeY;

	BYTE pixel;

	// loop over right hand container
	for (int rhx = 0; rhx < rhWidth; rhx++) {
		for (int rhy = 0; rhy < rhHeight; rhy++) {

			relativeX = rhx + x;
			relativeY = rhy + y;
			pixel = stamp.GetPixel(rhx, rhy);

			// apply inversion if needed
			if (stamp.inverted) {
				pixel = 255 - pixel;
			}

			// set pixel in the left hand container according to offsets
			if (pixel >= 128)
				SetPixel(relativeX, relativeY, pixel);
		}
	}
}

void UIContainer::Clear() {
	
	std::fill_n(UIdata, dataWidth * dataHeight, 0);
}

int UIContainer::GetWidth() {

	return dataWidth;
}

int UIContainer::GetHeight() {

	return dataHeight;
}
