#include "Requester.h"


Requester::Requester() {

	ReadTokens();
}


Requester::~Requester()
{
}

// log in a Spotify user and retreive valid session keys
bool Requester::Initiate() {

	// setup client and request
	http_client client(U("https://api.spotify.com/v1/me"));
	http_request request(methods::GET);

	// combine ACCESS_TOKEN with Bearer keyword
	const std::wstring access = (L"Bearer " + Tools::to_Wstring(ACCESS_TOKEN));

	// add the Authorization header
	request.headers().add(L"Authorization", access);

	pplx::task<bool> task = client.request(request)

		.then([](http_response response) {

			if (response.status_code() == status_codes::OK) {
				OutputDebugString("\n- VALID AUTHORIZATION");
				return true;
			}
			else {
				OutputDebugString("\n- INVALID AUTHORIZATION");
				return false;
			}; 
		});

	task.wait();
	return task.get();
}

// open the node application to log a user into Spoitify
void Requester::OpenSpotifyLogin() {

	// run node server to preform OArth
	OutputDebugString("\nOpening Spotify login...");
	system("node ../NodeAuth/app.js");

	// once completed, the node server should have saved access and refresh tokens to the file
	ReadTokens();
}

// get the current Spotify playback details
_json Requester::GetCurrentPlayback() {

	CheckTokenExpiry();

	// setup client and request
	http_client client(U("https://api.spotify.com/v1/me/player"));
	http_request request(methods::GET);

	// combine ACCESS_TOKEN with Bearer keyword
	const std::wstring access = (L"Bearer " + Tools::to_Wstring(ACCESS_TOKEN));

	// add the Authorization header
	request.headers().add(L"Authorization", access);

	// process the request
	pplx::task<_json> task = client.request(request)

		.then([](http_response response)-> pplx::task<json::value> {

		if (response.status_code() == status_codes::OK) {
			return response.extract_json();
		}
		else {
			return pplx::task_from_result(json::value());
		}; })

		.then([](pplx::task<json::value> previousTask) {
			try {
				const json::value & v = previousTask.get();
				std::wstring rawjson = v.serialize();
				return _json::parse(rawjson);
			}
			catch (const http_exception &e) {
				std::cout << e.what() << std::endl;
			}
		});

	task.wait();
	return task.get();
}

// read the tokens obtained by the Node login system
void Requester::ReadTokens() {

	std::string tokens;
	std::ifstream myfile("tokens.json");
	if (myfile.is_open()) {

		std::string tokenData(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>()));
		
		std::stringstream ss(tokenData);
		_json tokens = _json::parse(ss);

		if (tokens.size() != 5) {

			OutputDebugString("\nTOKEN SETUP INVALID!");
			OutputDebugString("\nOpening authentication window...");

			OpenSpotifyLogin();
		}
		else {

			OutputDebugString("\nRetreived valid tokens...");

			// save the obtained tokens
			ACCESS_TOKEN = tokens["access_token"].get<std::string>();
			REFRESH_TOKEN = tokens["refresh_token"].get<std::string>();
			ENCODED_CLIENT_DATA = tokens["encoded_client_data"].get<std::string>();

			TOKEN_VALID_FROM = (tokens["currentTime"].get<unsigned long long>() / 1000);
			TOKEN_VALID_TILL =
				TOKEN_VALID_FROM + (tokens["expires_in"].get<unsigned long long>());
		}
	}
}

// confirms the current Spotify tokens are still valid, and updates if not
void Requester::CheckTokenExpiry() {

	std::time_t currentTime = std::time(nullptr);

	// token has expired
	if (TOKEN_VALID_TILL < currentTime) {

		OutputDebugString("\nACCESS_TOKEN EXPIRED!");
		OutputDebugString("\nREQUESTING NEW ACCESS_TOKEN...");

		// setup client and request
		http_client client(U("https://accounts.spotify.com/api/token"));
		http_request request(methods::POST);

		// construct request header
		const std::wstring access = (L"Basic " + Tools::to_Wstring(ENCODED_CLIENT_DATA));
		request.headers().add(L"Authorization", access);
		request.headers().add(L"Content-Type", "application/x-www-form-urlencoded");

		// construct request body
		_json body;
		body["grant_type"] = "refresh_token";
		body["refresh_token"] = REFRESH_TOKEN;
		request.set_body(Tools::EncodeBodyURI(body), "application/x-www-form-urlencoded");
		
		// process request
		pplx::task<_json> task = client.request(request)

			.then([](http_response response) {

				if (response.status_code() == status_codes::OK) {
					return response.extract_json();
				}
				else {
					return pplx::task_from_result(json::value());
				}; 
			})

			.then([](pplx::task<json::value> previousTask) {

				try {
					const json::value & v = previousTask.get();
					std::wstring rawjson = v.serialize();
					return _json::parse(rawjson);
				}
				catch (const http_exception &e) {
					std::cout << e.what() << std::endl;
				}
			});

		task.wait();

		OutputDebugString("\nRETREIVED NEW ACCESS TOKEN\n");

		// use the new access token
		_json newTokenData = task.get();

		ACCESS_TOKEN = newTokenData["access_token"].get<std::string>();
	
		std::time_t result = std::time(nullptr);
		TOKEN_VALID_FROM = result;
		TOKEN_VALID_TILL =
			TOKEN_VALID_FROM + (newTokenData["expires_in"].get<unsigned long long>());
	}
}