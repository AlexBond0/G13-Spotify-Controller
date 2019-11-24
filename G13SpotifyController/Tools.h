#pragma once

#include <string>
#include <codecvt>

#include <nlohmann/json.hpp>
using _json = nlohmann::json;


class Tools
{
public:
	static std::wstring to_Wstring(std::string);
	static std::string EncodeBodyURI(_json body);
};