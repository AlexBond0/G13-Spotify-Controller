#include "TextComponent.h"



TextComponent::TextComponent(int width, int height, int posx, int posy)
	: Component(width, height, posx, posy)
{

}


TextComponent::~TextComponent() {

	for (std::pair<char, UIContainer*> letter : dictionary) {

		delete letter.second;
	}
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
	int kerning = 0;

	int startColumn = 0;
	int endColumn = 0;

	std::string stringChar;
	char newChar;

	UIContainer* currentUIContainer;

	hasUppercase = layout["has_case"];

	for (_json character : layout["layout"]) {

		// calculate the size of UIContainer needed for the character
		bool nectCharFound = false;
		while (!nectCharFound) {

			endColumn++;
			if (rawText.GetPixel(endColumn, 0) < 200)
				nectCharFound = true;
		}

		// calculate width and accound for gap between characters
		kerning = character["kerning"];
		textWidth = endColumn - startColumn - 1 + kerning;

		// create the new UIContainer
		newChar = (character["char"].get<std::string>()).at(0);
		dictionary[newChar] = new UIContainer(
			textWidth,
			textHeight,
			0,
			0
		);
		currentUIContainer = dictionary[newChar];

		// copy over the pixels
		for (int x = 0; x < textWidth; x++) {
			for (int y = 1; y <= textHeight; y++) {

				currentUIContainer->SetPixel(
					x,
					y - 1,
					rawText.GetPixel(x + startColumn, y)
				);
			}
		}

		startColumn = endColumn;
	}
}

void TextComponent::RenderText(std::string text) {

	// clear current component
	_ui.Clear();

	int currentColumn = 0;
	UIContainer* container;

	// loop over each letter (also exit if ran out of space)
	for (char letter : text) {

		// space
		if (letter == ' ') {

			currentColumn += 4;
		}

		else {

			// account for no upper case
			if (!hasUppercase)
				letter = tolower(letter);

			// unknown character
			if (!dictionary[letter])
				letter = '?';

			container = dictionary[letter];

			// loop over character container
			BYTE pixel;
			for (int x = 0; x < container->GetWidth(); x++) {
				for (int y = 0; y < container->GetHeight(); y++) {

					pixel = container->GetPixel(x, y);

					if (pixel >= 128)
						_ui.SetPixel(x + currentColumn, y, pixel);
				}
			}

			// move the column along
			currentColumn += container->GetWidth();
		}
	}
}