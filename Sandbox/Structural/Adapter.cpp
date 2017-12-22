#define _SCL_SECURE_NO_WARNINGS
#include <vector>
#include <boost/algorithm/string.hpp>
#include <iostream>

namespace Adapter
{
	// Adapter - Think of a plug (electrical devices)
	// We cannot modify our electrionic gadgets to support every possible interface
	// So we use a device (an adapter) which gives us the interface we require

	// In this example we are looking at the static functions in boost/string
	// The problem here is we lack discoverability, we would like intellisense
	// when working with strings to we know what functions are availible

	// Lets create our own String class
	class String
	{
		std::string s; // wrapper around std::string

	public:
		String(const std::string& cs) // don't make this explicit
			: s{cs}
		{
		}

		// We can call into boost
		String to_lower() const
		{
			std::string ss(s);
			boost::to_lower(ss);
			return { ss }; // Will call the String(const string& cs) ctor due to implicit conversion.
		}

		std::vector<std::string> split(const std::string& delimeter = " ") const
		{
			std::vector<std::string> result;
			boost::split(result, 
			             s, 
			             boost::is_any_of(delimeter), 
			             boost::token_compress_on);
			return result;
		}

		// Add any other boot functions we'd like to use
	};

	
}

using namespace Adapter;

int Adapter_main(int argc, char* argv[])
{
//	BEFORE
//	std::string s{"Hello World"};
//	
//	boost::to_lower(s);
//	std::vector<std::string> parts;
//	boost::split(parts, s, boost::is_any_of(" "));

//	AFTER

	Adapter::String s{"Hello   World"};
	auto parts = s.to_lower() // We have discoverability!
				  .split();	  // Since it returns a new string, we can chain call
	
	for (const auto& p : parts)
		std::cout << "<" << p << ">" << std::endl;

	system("PAUSE");
	return 0;
}
