#include "TextComponent.h"



TextComponent::TextComponent(int width, int height, int posx, int posy)
	: Component(width, height, posx, posy)
{

}


TextComponent::~TextComponent() {

	for (std::pair<char, Character> letter : dictionary) {

		delete letter.second.ui;
	}
}
 
_json TextComponent::LoadTextLayout(std::wstring filename) {

	_json textLayout;

	std::ifstream myfile(filename);
	if (myfile.is_open()) {

		std::string rawJSON(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>())
		);

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

		newChar = (character["char"].get<std::string>()).at(0);

		// create the new UIContainer
		currentUIContainer = new UIContainer(
			textWidth,
			textHeight
		);

		// create the Character container
		Character newC;
		newC.ui = currentUIContainer;
		newC.width = textWidth;

		dictionary[newChar] = newC;

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

	// if text needs updating
	if (text != currentText) {

		if (text.length() == 0) {

			// clear current component
			_ui.Clear();
		}
		else {

			// save new text data
			currentText = text;
			RenderSetup();

			TextToUI();
		}
	}
}

void TextComponent::SetScroll(int scroll) {

	currentTextScroll = scroll;

	TextToUI();
}

void TextComponent::SetAutoScroll(bool scrolling) {

	autoScroll = scrolling;
}

void TextComponent::Update() {

	if (autoScroll && isTooLong) {

		if (scrollWait <= 0) {

			// change direction once max is reached
			if (currentTextScroll == maxScroll)
				scrollForewards = false;

			if (scrollForewards)
				currentTextScroll--;
			else
				currentTextScroll++;

			// render text
			TextToUI();

			// wait again when scroll returned
			if (currentTextScroll == 0) {

				scrollForewards = true;
				scrollWait = SCROLL_PAUSE;
			}
		}

		// wait to scroll
		else {

			scrollWait--;
		}
	}
}

void TextComponent::TextToUI() {

	// clear current component
	_ui.Clear();

	// get how far along rendering should begin
	int renderColumn = 0;
	int textStart = currentTextScroll;
	int letterOffset = 0;

	// get which letter would fall in this column
	int currentwidth = 0;
	int currentLetter = 0;
	bool foundStartChar = false;

	Character letter = GetSafeCharacter(currentText.at(0));
	char currentChar;

	// is textStart negative?
	if (textStart < 0) {

		int letterColumn = textStart;

		while (!foundStartChar) {

			currentChar = currentText.at(currentLetter);

			// get distance letter spans
			letterColumn += GetSafeWidth(currentChar);

			if (letterColumn >= 0) {

				letterOffset = GetSafeWidth(currentChar) - letterColumn;

				// save the letter UIContainer
				letter = GetSafeCharacter(currentChar);

				// skip ahead for spaces
				if (currentChar == ' ') {

					renderColumn += (GetSafeWidth(currentChar) - letterOffset);
					letterOffset = 0;
					currentLetter++;

					currentChar = currentText.at(currentLetter);
					letter = GetSafeCharacter(currentChar);
				}

				foundStartChar = true;
			}

			else {

				currentLetter++;
			}
		}
	}

	// start renderColumn at textstart
	else {

		renderColumn = textStart;
	}

	// for each column of the renderable space
	BYTE pixel;
	int renderingwidth = (
		_ui.GetWidth() < renderableTextWidth + currentTextScroll
		? _ui.GetWidth()
		: renderableTextWidth + currentTextScroll
	);

	for (renderColumn; renderColumn < renderingwidth; renderColumn++) {

		// get next letter if needed
		if (letterOffset == letter.width) {

			letterOffset = 0;
			currentLetter++;

			if (currentLetter >= currentText.length())
				return;

			currentChar = currentText.at(currentLetter);

			// skip over spaces
			while (currentChar == ' ') {

				renderColumn += SPACE_WIDTH;
				currentLetter++;

				currentChar = currentText.at(currentLetter);
			}

			letter = GetSafeCharacter(currentText.at(currentLetter));
		}

		// render column
		for (int y = 0; y < letter.ui->GetHeight(); y++) {

			pixel = letter.ui->GetPixel(letterOffset, y);

			if (pixel >= 128)
				_ui.SetPixel(renderColumn, y, pixel);
		}

		letterOffset++;
	}
}

// calculate fill renderable width of given text
void TextComponent::RenderSetup() {

	// calculate width of renderable text
	renderableTextWidth = 0;
	for (char letter : currentText)
		renderableTextWidth += GetSafeWidth(letter);

	currentTextScroll = 0;
	
	isTooLong = (renderableTextWidth > _ui.GetWidth());
	if (isTooLong) {

		maxScroll = _ui.GetWidth() - renderableTextWidth;
		scrollWait = SCROLL_PAUSE;
	}
}

// calculate width of character with safety
int TextComponent::GetSafeWidth(char letter) {

	if (letter == ' ') {

		return SPACE_WIDTH;
	}

	else 
		return GetSafeCharacter(letter).width;
}

// get a safe Character object from the Dictionary given any character
Character TextComponent::GetSafeCharacter(char letter) {

	if (!hasUppercase)
		letter = tolower(letter);

	if (dictionary.count(letter) == 0)
		letter = '?';

	return dictionary[letter];
}