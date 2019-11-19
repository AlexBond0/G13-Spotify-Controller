#include "Requester.h"


std::string Requester::ACCESS_TOKEN = "";
std::string Requester::REFRESH_TOKEN = "";


Requester::Requester()
{
}


Requester::~Requester()
{
}

void Requester::OpenSpotifyLogin() {

	// run node server to preform OArth
	system("node ../NodeAuth/app.js");

	// once completed, the node server should have saved access and refresh tokens to the file
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

void Requester::TestRequest() {

	//// run node server to preform OArth
	//system("node ../NodeAuth/app.js");

	//// once completed, the node server should have saved access and refresh tokens to the file
	//std::string tokens;
	//std::ifstream myfile("tokens.txt");
	//if (myfile.is_open()) {

	//	std::string tokenData(
	//		(std::istreambuf_iterator<char>(myfile)),
	//		(std::istreambuf_iterator<char>()));

	//	// get the token information from the file
	//	std::vector <std::string> tokens;
	//	std::stringstream ss(tokenData);
	//	std::string token;
	//	while (std::getline(ss, token, ' ')) {
	//		tokens.push_back(token);
	//	}
	//	
	//	std::string ACCESS_TOKEN = tokens[0];
	//	std::string REFRESH_TOKEN = tokens[1];

		//==================================
		// get user info for example

		// setup client and request
		http_client client(U("https://api.spotify.com/v1/me"));
		http_request request(methods::GET);

		// convert ACCESS_TOKEN to a wstring, then combine with Bearer keyword
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
		std::wstring wToken = converter.from_bytes(ACCESS_TOKEN);
		const std::wstring access = (L"Bearer " + wToken);

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

			//TimeSpan ts = TimeSpan.FromMilliseconds(150);
			//if (!t.Wait(ts))
			//	Console.WriteLine("The timeout interval elapsed.");


		//uri_builder builder;
		//// std::wstring A = toWstring(authorizationData);

		//builder.append_query(U("Authorization"), U("Bearer BQDNy48QiBgsHxDEj0GeWUXwktyb-TfhiEgNrXvvMwIgTHibrogXcYW9gXhMAmOiFd89nqiq7-yNioqLTVr70Tu-gRUprm1tLwQ9jusT8vZrwVNvZULS6cz79-y1Eg-8mmqz8wCFMTFLIKNfiJRODxnYoU1CO8EQlKSX1COs"));
		//auto path_query_fragment = builder.to_string();

		//// OutputDebugString(path_query_fragment);

		//// Make an HTTP GET request and asynchronously process the response
		//pplx::task<void> authorisation = 
		//	client.request(methods::GET, path_query_fragment)
		//	.then([](http_response response)
		//{

		//	std::string p = "peepee";
		//	// Display the status code that the server returned
		//	//std::wostringstream stream;
		//	//stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
		//	//std::wcout << stream.str();

		//	//stream.str(std::wstring());
		//	//stream << L"Content type: " << response.headers().content_type() << std::endl;
		//	//stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
		//	//std::wcout << stream.str();

		//	//auto bodyStream = response.body();
		//	//streams::stringstreambuf sbuffer;
		//	//auto& target = sbuffer.collection();

		//	//bodyStream.read_to_end(sbuffer).get();

		//	//stream.str(std::wstring());
		//	//stream << L"Response body: " << target.c_str();
		//	//std::wcout << stream.str();
		//});
	//}
	//else OutputDebugString("Unable to open tokens.txt");


	//try {



		//auto fileStream = std::make_shared<ostream>();


		//jsonpp request;

	/*	request["client_id"] = CLIENT_ID;
		request["response_type"] = "code";
		request["redirect_uri"] = "localhost:8080/";
		request["scope"] = "user-modify-playback-state user-read-playback-state user-read-currently-playing";*/


		//http_client client(U("https://accounts.spotify.com/authorize"));

		//uri_builder builder;
		//// Append the query parameters: ?method=flickr.test.echo&name=value
		//builder.append_query(U("client_id"), U("3637514e0efd45039de424919821c07c"));
		//builder.append_query(U("response_type"), U("code"));
		//builder.append_query(U("redirect_uri"), U("localhost:8080/"));
		//builder.append_query(U("scope"), U("user-modify-playback-state user-read-playback-state user-read-currently-playing"));


		//auto path_query_fragment = builder.to_string();

		//// system("open http://google.com");
		//ShellExecute(NULL, "open", url.c_str)


		//// Make an HTTP GET request and asynchronously process the response
		//pplx::task<void> authorisation = 
		//	client.request(methods::GET, path_query_fragment)
		//	.then([](http_response response)
	//	{
	//		// Display the status code that the server returned
	//		std::wostringstream stream;
	//		stream << L"Server returned returned status code " << response.status_code() << L'.' << std::endl;
	//		std::wcout << stream.str();

	//		stream.str(std::wstring());
	//		stream << L"Content type: " << response.headers().content_type() << std::endl;
	//		stream << L"Content length: " << response.headers().content_length() << L"bytes" << std::endl;
	//		std::wcout << stream.str();

	//		//auto bodyStream = response.body();
	//		//streams::stringstreambuf sbuffer;
	//		//auto& target = sbuffer.collection();

	//		//bodyStream.read_to_end(sbuffer).get();

	//		//stream.str(std::wstring());
	//		//stream << L"Response body: " << target.c_str();
	//		//std::wcout << stream.str();
	//	});
	//}

		//// Open stream to output file.
		//pplx::task<void> requestTask = 
		//	fstream::open_ostream(U("results.html"))
		//	.then([=](ostream outFile) {

		//	*fileStream = outFile;

		//	//// Create http_client to send the request.
		//	//http_client client(U("http://www.bing.com/"));

		//	//// Build request URI and start the request.
		//	//uri_builder builder(U("/search"));
		//	//builder.append_query(U("q"), U("cpprestsdk github"));
		//	//return client.request(methods::GET, builder.to_string());
		//});


		//// Handle response headers arriving.
		//.then([=](http_response response) {

		//	printf("Received response status code:%u\n", response.status_code());

		//	// Write response body into the file.
		//	return response.body().read_to_end(fileStream->streambuf());
		//})

		//// Close the file stream.
		//.then([=](size_t) {

		//	return fileStream->close();
		//});

		//// Wait for all the outstanding I/O to complete and handle any exceptions
		//try {

		//	requestTask.wait();
		//}
		//catch (const std::exception &e) {

		//	printf("Error exception:%s\n", e.what());
		//}
	//catch (int e){

	//}
}
