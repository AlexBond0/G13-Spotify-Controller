#pragma once

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
#include <nlohmann/json.hpp>

#include <Windows.h>
#include <algorithm>
#include <iterator>

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <codecvt>
#include <locale>

#include "Tools.h"


using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using jsonpp = nlohmann::json;

class Requester
{
public:
	Requester();
	~Requester();

	static void OpenSpotifyLogin();
	static void TestRequest();

private:

	static std::string ACCESS_TOKEN;
	static std::string REFRESH_TOKEN;

};

