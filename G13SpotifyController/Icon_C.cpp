#include "Icon_C.h"


std::map<std::string, UIContainer*> Icon_C::icons;

Icon_C::Icon_C(int width, int height, int posx, int posy)
	: Component(width, height, posx, posy)
{

}

Icon_C::~Icon_C() {

	for (std::pair<std::string, UIContainer*> icon : icons) {

		if (icon.second)
			delete icon.second;
	}
}

void Icon_C::SetValue(std::string name) {

	_ui.Clear();
	_ui.Imprint(*icons[name], 0, 0);
}

void Icon_C::LoadIcons() {

	// load icons
	UIContainer* rawIcons = new UIContainer(L"../Text/Icons/icons.bmp");

	// load JSON data
	_json iconLayout;
	std::ifstream myfile(L"../Text/Icons/layout.json");
	if (myfile.is_open()) {

		std::string rawJSON(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>()));

		iconLayout = _json::parse(rawJSON);
	}

	ConstructIcons(*rawIcons, iconLayout);

	// delete rawIcons;
}

void Icon_C::ConstructIcons(UIContainer rawIcons, _json layout) {

	int iconHeight = layout["height"];
	int iconWidth = 0;

	int startColumn = 0;
	int endColumn = 0;

	std::string iconName;

	UIContainer* currentUIContainer;

	for (_json icon : layout["layout"]) {

		// calculate the size of UIContainer needed for the icon
		bool nectCharFound = false;
		while (!nectCharFound) {

			endColumn++;
			if (rawIcons.GetPixel(endColumn, 0) < 200)
				nectCharFound = true;
		}

		// calculate width and accound for gap between characters
		iconWidth = endColumn - startColumn - 1;

		// create the new UIContainer
		iconName = icon.get<std::string>();
		icons[iconName] = new UIContainer(
			iconWidth,
			iconHeight
		);
		currentUIContainer = icons[iconName];

		// copy over the pixels
		for (int x = 0; x < iconWidth; x++) {
			for (int y = 1; y <= iconHeight; y++) {

				currentUIContainer->SetPixel(
					x,
					y - 1,
					rawIcons.GetPixel(x + startColumn, y)
				);
			}
		}

		startColumn = endColumn;
	}
}