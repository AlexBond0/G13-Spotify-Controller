#pragma once

#include <codecvt>
#include <string>
#include <sstream>
#include <iostream>

#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>
using _json = nlohmann::json;


class Tools
{
public:
	static std::wstring to_Wstring(std::string);
	static std::string EncodeBodyURI(_json body);
	static void ReadSetupFile();
	static std::wstring GetFolder(std::string folderID);

	static _json setup;
};