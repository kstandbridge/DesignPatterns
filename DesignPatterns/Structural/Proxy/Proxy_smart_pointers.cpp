#include <iostream>
#include <ostream>
#include <memory>

namespace Proxy_smart_pointers
{
	// Proxy
	// A class that is functioning as an interface to a particular resource.
	// That resource may be remote, expensive to construct, or may require 
	// logging or some other added functionality


	// In this example we'll look at smart pointers

	// We have a pure virtual bank account
	struct BankAccount
	{
		virtual ~BankAccount() = default;
		virtual void deposit(int amount) = 0;
		virtual void withdraw(int amount) = 0;
	};

	// A concrete implementation
	struct CurrentAccount : BankAccount
	{
		explicit CurrentAccount(const int balance)
			: balance{balance}
		{}

		void withdraw(int amount) override
		{
			if(amount <= balance) balance -= amount;
		}
		void deposit(int amount) override
		{
			balance += amount;
		}

		friend std::ostream& operator<<(std::ostream& os, const CurrentAccount& obj)
		{
			return os << " balance: " << obj.balance;
		}

	private:
		int balance;
	};
}

using namespace Proxy_smart_pointers;
int Proxy_smart_pointers_main(int argc, char* argv[])
{
	// Normally to create on the heap we would allocate as such
	BankAccount* a = new CurrentAccount{ 123 };
	a->deposit(321);
	delete a; // Then later we need to remember to free up that space

	// Smart points add a proxy to handle this
	auto b = std::make_shared<CurrentAccount>(123);
	// Internal they have a reference count, and are automatically deleted
	// once no references are left

	// However we are left with Polymorphic equivalent of a BankAccount*
	b->deposit(321); // This is becase the -> operator is overloaded internally
	std::cout << *b << std::endl; // We can even dereference to get the value, of course this is overloaded internally
	CurrentAccount* underlying = b.get(); // We can also get the actual pointer

	// This proxy, has allowed b to appear and function much the same as a, with additional functionallity internally

	getchar();
	return EXIT_SUCCESS;
}


