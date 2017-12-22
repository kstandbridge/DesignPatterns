#pragma once
#include <memory>
#include <string>

namespace Pimpl
{
	struct User
	{
		User();

		~User();
		User(User&&);
		User& operator=(User&&);

		std::string name;
		void sayHello();

	private:
		// Pointer to the implementation
		struct impl;
		std::unique_ptr<impl> pimpl;
	};
	
}
