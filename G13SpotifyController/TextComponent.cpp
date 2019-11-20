#include "TextComponent.h"



TextComponent::TextComponent(int width, int height, int posx, int posy)
	: Component(width, height, posx, posy)
{

}


TextComponent::~TextComponent()
{
}
 
_json TextComponent::LoadTextLayout(std::wstring filename) {

	_json textLayout;

	std::ifstream myfile(filename);
	if (myfile.is_open()) {

		std::string rawJSON(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>()));

		textLayout = _json::parse(rawJSON);
	}
	return textLayout;
}

void TextComponent::ConstructFont(UIContainer rawText, _json layout) {

	int textHeight = layout["height"];
	int textWidth = 0;

	int startColumn = 0;
	int endColumn = 0;
	char newChar;

	UIContainer* currentUIContainer;


	for (_json character : layout["layout"]) {

		// calculate the size of UIContainer needed for the character
		bool nectCharFound = false;
		while (!nectCharFound) {

			endColumn++;
			if (rawText.GetPixel(endColumn, 0) < 200)
				nectCharFound = true;
		}

		// calculate width and accound for gap between characters
		textWidth = endColumn - startColumn - 1;

		// create the new UIContainer
		dictionary[character["char"]] = new UIContainer(
			textWidth,
			textHeight,
			0,
			0
		);
		currentUIContainer = dictionary[character["char"]];

		// copy over the pixels
		for (int x = 0; x < textWidth; x++) {
			for (int y = 1; y < textHeight; y++) {

				currentUIContainer->SetPixel(
					x,
					y - 1,
					rawText.GetPixel(x + startColumn, y)
				);
			}
		}

		startColumn = endColumn;
	}

	int a = 0;
}