#include "User.h"
#include "iostream"

namespace Pimpl
{
	struct User::impl
	{
		void sayHello(User* user);
	};

	void User::impl::sayHello(User* user)
	{
		std::cout << "Hello there, my name is " << user->name << std::endl;
	}

	User::User() 
	: pimpl{std::make_unique<impl>()} // We create our implementation
	{	
	}

	User::~User() = default; // Smart Pointers auto clean up
	User::User(User&&) = default; // move constructor
	User& User::operator=(User&&) = default; // move assignment

	// We can proxy over commands to our implementation
	void User::sayHello()
	{
		pimpl->sayHello(this);
	}
}
