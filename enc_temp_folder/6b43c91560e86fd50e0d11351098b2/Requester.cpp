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

//void Requester::TestRequest() {
//
//		// setup client and request
//		http_client client(U("https://api.spotify.com/v1/me"));
//		http_request request(methods::GET);
//
//		// combine ACCESS_TOKEN with Bearer keyword
//		const std::wstring access = (L"Bearer " + Tools::to_Wstring(ACCESS_TOKEN));
//
//		// add the Authorization header
//		request.headers().add(L"Authorization", access);
//
//		pplx::task<void> task = client.request(request)
//
//			.then([](http_response response)-> pplx::task<json::value> {
//
//			if (response.status_code() == status_codes::OK) {
//				OutputDebugString("\n- VALID AUTHORIZATION");
//				return response.extract_json();
//			}
//			else {
//				OutputDebugString("\n- INVALID AUTHORIZATION");
//				return pplx::task_from_result(json::value());
//			}; })
//
//			.then([](pplx::task<json::value> previousTask) {
//				try {
//					const json::value & v = previousTask.get();
//					std::wstring rawjson = v.serialize();
//					// OutputDebugString((v.to_string()));
//				}
//				catch (const http_exception &e) {
//					std::cout << e.what() << std::endl;
//				}
//			});
//
//			task.wait();
//}

_json Requester::GetCurrentPlayback() {

	CheckTokenExpiry();

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
	std::ifstream myfile("tokens.json");
	if (myfile.is_open()) {

		std::string tokenData(
			(std::istreambuf_iterator<char>(myfile)),
			(std::istreambuf_iterator<char>()));

		// get the token information from the file
		// std::vector <std::string> tokens;
		
		std::stringstream ss(tokenData);
		_json tokens = _json::parse(ss);

		//std::string token;
		//while (std::getline(ss, token, ' ')) {
		//	tokens.push_back(token);
		//}

		if (tokens.size() != 5) {

			OutputDebugString("\nTOKEN SETUP INVALID!");
			OutputDebugString("\nOpening authentication window...");

			OpenSpotifyLogin();
		}
		else {

			ACCESS_TOKEN = tokens["access_token"].get<std::string>();
			REFRESH_TOKEN = tokens["refresh_token"].get<std::string>();
			ENCODED_CLIENT_DATA = tokens["encoded_client_data"].get<std::string>();

			// std::string::size_type sz;
			TOKEN_VALID_FROM = (tokens["currentTime"].get<unsigned long long>() / 1000);
			TOKEN_VALID_TILL =
				TOKEN_VALID_FROM + (tokens["expires_in"].get<unsigned long long>());
		}
	}
}

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

		_json newTokenData = task.get();

		ACCESS_TOKEN = newTokenData["access_token"].get<std::string>();
	
		std::time_t result = std::time(nullptr);
		TOKEN_VALID_FROM = result;
		TOKEN_VALID_TILL =
			TOKEN_VALID_FROM + (newTokenData["expires_in"].get<unsigned long long>());
	}
}