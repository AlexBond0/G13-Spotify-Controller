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
#include <ctime>

#include "Tools.h"


using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

using _json = nlohmann::json;

class Requester
{
public:
	Requester();
	~Requester();

	bool Initiate();

	void OpenSpotifyLogin();

	_json GetCurrentPlayback();

private:

	std::string ACCESS_TOKEN;
	std::string REFRESH_TOKEN;
	std::string ENCODED_CLIENT_DATA;

	unsigned long long TOKEN_VALID_FROM;
	unsigned long long TOKEN_VALID_TILL;

	void ReadTokens();

	void CheckTokenExpiry();

};

