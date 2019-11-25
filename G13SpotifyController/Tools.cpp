#pragma once

#include "Tools.h"

_json Tools::setup;

std::wstring Tools::to_Wstring(std::string str) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
};

std::string Tools::EncodeBodyURI(_json body) {

	std::string bodyURI = "";
	int members = body.size();

	for (auto& element : body.items()) {

		bodyURI += element.key();
		bodyURI += "=";
		bodyURI += element.value().get<std::string>();

		members--;

		if (members != 0)
			bodyURI += "&";
	}

	return bodyURI;
}

void Tools::ReadSetupFile() {

	std::string tokens;
	std::ifstream myfile("Setup.json");
	if (myfile.is_open()) {

		std::string tokenData(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>()));

		std::stringstream ss(tokenData);
		setup = _json::parse(ss);
	}
}

std::wstring Tools::GetFolder(std::string folderID) {

	return to_Wstring(setup["folders"][folderID].get<std::string>());
}
