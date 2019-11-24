#pragma once

#include "Tools.h"

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

//std::string body;
//body = "grant_type=refresh_token";
//body += ("&refresh_token=" + REFRESH_TOKEN);
//request.set_body(body, "application/x-www-form-urlencoded");