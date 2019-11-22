#include "Requester.h"


Requester::Requester() {

	ReadTokens();
}


Requester::~Requester()
{
}

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

void Requester::OpenSpotifyLogin() {

	// run node server to preform OArth
	system("node ../NodeAuth/app.js");

	// once completed, the node server should have saved access and refresh tokens to the file
	ReadTokens();
}

void Requester::TestRequest() {

		// setup client and request
		http_client client(U("https://api.spotify.com/v1/me"));
		http_request request(methods::GET);

		// combine ACCESS_TOKEN with Bearer keyword
		const std::wstring access = (L"Bearer " + Tools::to_Wstring(ACCESS_TOKEN));

		// add the Authorization header
		request.headers().add(L"Authorization", access);

		pplx::task<void> task = client.request(request)

			.then([](http_response response)-> pplx::task<json::value> {

			if (response.status_code() == status_codes::OK) {
				OutputDebugString("\n- VALID AUTHORIZATION");
				return response.extract_json();
			}
			else {
				OutputDebugString("\n- INVALID AUTHORIZATION");
				return pplx::task_from_result(json::value());
			}; })

			.then([](pplx::task<json::value> previousTask) {
				try {
					const json::value & v = previousTask.get();
					std::wstring rawjson = v.serialize();
					// OutputDebugString((v.to_string()));
				}
				catch (const http_exception &e) {
					std::cout << e.what() << std::endl;
				}
			});

			task.wait();
}

_json Requester::GetCurrentPlayback() {

	// setup client and request
	http_client client(U("https://api.spotify.com/v1/me/player"));
	http_request request(methods::GET);

	// combine ACCESS_TOKEN with Bearer keyword
	const std::wstring access = (L"Bearer " + Tools::to_Wstring(ACCESS_TOKEN));

	// add the Authorization header
	request.headers().add(L"Authorization", access);

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

void Requester::ReadTokens() {

	std::string tokens;
	std::ifstream myfile("tokens.txt");
	if (myfile.is_open()) {

		std::string tokenData(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>()));

		// get the token information from the file
		std::vector <std::string> tokens;
		std::stringstream ss(tokenData);
		std::string token;
		while (std::getline(ss, token, ' ')) {
			tokens.push_back(token);
		}

		ACCESS_TOKEN = tokens[0];
		REFRESH_TOKEN = tokens[1];
	}
}