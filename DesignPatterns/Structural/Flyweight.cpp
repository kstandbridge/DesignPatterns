#include <boost/bimap.hpp>

#include <string>
#include <iostream>

namespace Flyweight
{
	// A space optimization technique that lets us use less
	// memory by storing externally the data associated with similar objects

	// If multiple objects have identical properties/data
	// Store the data in one place, then any objects can reference it via pointers/tokens

	typedef uint32_t key; // we will use this key to reference said data

	struct User
	{
		User(const std::string& firstName, const std::string& lastName)
			: first_name{add(firstName)}, // We call the static function to set the name
			  last_name{add(lastName)}
		{
		}

		const std::string& get_first_name() const
		{
			return names.left.find(first_name)->second; // We retrieve it from the bimap
		}

		const std::string& get_last_name() const
		{
			return names.left.find(last_name)->second;
		}

		friend std::ostream& operator<<(std::ostream& os, const User& obj)
		{
			return os
					<< "first name: " << obj.first_name << " " << obj.get_first_name()
					<< " last name: " << obj.last_name << " " << obj.get_last_name();
		}

//		std::string first_name, last_name; // We do not store the names in the struct
		key first_name, last_name; // instead we store a key which can be used to reference the value in a bimap

	protected:
		static boost::bimap<key, std::string> names; // Where we actually store the names
		static int seed; // For key generation
		
		static key add(const std::string& s)
		{	// Notice we are returning a key when we add
			auto it = names.right.find(s);
			if(it == names.right.end())
			{	// It doesn't exist in the map so add it
				key id = ++seed;
				names.insert(boost::bimap<key,std::string>::value_type(seed, s));
				return id;
			}
			
			return it->second;
		}
	};

	int User::seed = 0; // Initalize the seed
	boost::bimap<key,std::string> User::names{}; // Initalize the bimap
}

using namespace Flyweight;
int Flyweight_main(int argc, char* argv[])
{
	User john_doe { "John", "Doe" };
	User jane_doe { "Jane", "Doe" }; // Notice the surname has the same value

	std::cout << "John " << john_doe << std::endl;
	std::cout << "Jane " << jane_doe << std::endl; // Notice the surname has the same key value
	
	getchar();
	return EXIT_SUCCESS;
}

#include <boost/flyweight.hpp>
namespace Flyweight_Library
{
	// Boost has provided a library that implements the flyweight pattern for us
	// We just need to specify boost::flyweight<T>

	struct User2
	{
		boost::flyweight<std::string> first_name, last_name;

		User2(const std::string& firstName, const std::string& lastName)
			: first_name{firstName},
			  last_name{lastName}
		{
		}
		
		friend std::ostream& operator<<(std::ostream& os, const User2& obj)
		{
			return os
					<< "first_name: " << obj.first_name
					<< " last_name: " << obj.last_name;
		}
	};
}

using namespace Flyweight_Library;
int Flyweight_Library_main(int argc, char* argv[])
{
	User2 john_doe {"John", "Doe" };
	User2 jane_doe {"Jane", "Doe" };

	std::cout << "John " << john_doe << std::endl;
	std::cout << "Jane " << jane_doe << std::endl;

	// Check to see the last name is pointing to the same memory address
	std::cout << std::boolalpha 
			  << (&jane_doe.last_name.get() == &john_doe.last_name.get())
			  << std::endl;
	
	getchar();
	return EXIT_SUCCESS;
}