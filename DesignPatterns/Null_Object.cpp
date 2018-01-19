#include <iostream>
#include <string>
#include <memory>

namespace Null_Object
{
	// Null Object

	// When component A uses component B, if typically assumes that B is actually present
	// -	You inject B, not e.g., optional<B>
	// -	You do not inject a pointer and then check for nullptr everywhere
	// There is no option of telling A not to use an instance of B
	// -	Its use is hard coded
	// Thus, we build a no-op, non-fuinction inheritor of B and pass that into A

	// A no-op object that satisfies the dependency requirement of some other object

	// In this example, our BankAccount logs transactions.
	// The idea here, is we don't want to have to put checks for NULLs everywhere

	
	// Our pure virtual logger 
	struct Logger
	{
		virtual ~Logger() = default;
		virtual void info(const std::string& s) = 0;
		virtual void warn(const std::string& s) = 0;
	};

	// Our Bank account relies on the logger
	struct BankAccount
	{
		std::shared_ptr<Logger> log; // References pure virtual
		std::string name;
		int balance = 0;


		BankAccount(const std::shared_ptr<Logger>& logger, const std::string& name, int balance)
			: log{logger},
			  name{name},
			  balance{balance}
		{
		}

		void deposit(int amount)
		{
			balance += amount;
			log->info(
				"Deposited $" + std::to_string(amount) + " to " + name 
				+ ", balance is now $" + std::to_string(balance));
		}
		
		void withdraw(int amount)
		{
			if(balance >= amount)
			{
				balance -= amount;
				log->info(
					"Withdrew $" + std::to_string(amount) + " from " + name 
					+ ", balance is now $" + std::to_string(balance));
			}
			else
			{
				log->warn(
					"Tried to withdraw $" + std::to_string(amount) + " from " + name 
					+ ", but couldn't due to low balance of $" + std::to_string(balance));
			}
		}
	
	};

	// An implementation of logger that just logs to the console
	struct ConsoleLogger : Logger
	{
		void info(const std::string& s) override
		{
			std::cout << "INFO: " << s << std::endl;
		}
		void warn(const std::string& s) override
		{
			std::cout << "WARNING!!! " << s << std::endl;
		}
	};

	// Our Null Object logger, which does nothing
	struct NullLogger: Logger
	{
		void info(const std::string& s) override
		{
		}

		void warn(const std::string& s) override
		{
		}
	};

}

using namespace Null_Object;

int Null_Object_main(int argc, char* argv[])
{
//	auto logger = std::make_shared<ConsoleLogger>();

	auto logger = std::make_shared<NullLogger>(); // As such we can use the Null Object version
	BankAccount account(logger, "primary account", 1000); // and create an instance of Bank account

	account.deposit(2000); // That can execute functions without having check for nulls everywhere
	account.withdraw(2500);
	account.withdraw(1000);

	getchar();
	return EXIT_SUCCESS;
}


