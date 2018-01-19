#include <iostream>
#include <string>

#include <cpprest/http_client.h>
#include <cpprest/filestream.h>
using namespace utility;                    // Common utilities like string conversions
using namespace web;                        // Common features like URIs.
using namespace web::http;                  // Common HTTP functionality
using namespace web::http::client;          // HTTP client features
using namespace concurrency::streams;       // Asynchronous streams

namespace communication_proxy
{
	// In this example we are our resource has moved to a service
	// Assume we have a rest API in place that handling the response

	// Our pure virtual
	struct Pingable
	{
		virtual ~Pingable() = default;
		virtual std::wstring ping(const std::wstring& message) = 0;
	};

	// The old implementation
	struct Pong : Pingable
	{
		std::wstring ping(const std::wstring& message) override
		{
			return message + L" pong";
		}
	};

	// The new implementation
	struct RemotePong : Pingable
	{
		std::wstring ping(const std::wstring& message) override
		{
			http_client client(U("http://localhost:9149"));
			uri_builder builder(U("/api/pingpong"));
			builder.append(message);
			pplx::task<std::wstring> task = 
				client.request(methods::GET, builder.to_string())
					  .then([=](http_response r)
							{
								return r.extract_string();
							});
			task.wait();
			return  task.get();
		}
	};

	void tryit(Pingable& p)
	{
		std::wcout << p.ping(L"ping") << "\t";
	}
}

using namespace communication_proxy;
int communication_proxy_main(int argc, char* argv[])
{
//	Pong pp;
	RemotePong pp; // The implementation stays the same, we just use the proxy instead
	for(size_t i = 0; i < 10; i++)
	{
		tryit(pp); // The input is polymorphic, thus out output should be the same
	}

	getchar();
	return EXIT_SUCCESS;
}