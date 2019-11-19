#pragma once

#include "Tools.h"

std::wstring Tools::to_Wstring(std::string str) {

	std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
	return converter.from_bytes(str);
};
