#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

namespace CompositeCommandPattern
{
	// Command pattern + composite pattern
	// Instead of a single object you have multiple objects in a paticual configuration with a uniform api
	
	struct BankAccount
	{
		int balance = 0;
		int overdraft_limit = -500;

		void deposit(int amount)
		{
			balance += amount;
			cout << "deposited " << amount << ", balance now " << balance << endl;
		}

		void withdraw(int amount)
		{
			if(balance - amount >= overdraft_limit)
			{
				balance -= amount;
				cout << "withdrew " << amount << ", balance now " << balance << endl;
			}
		}
	};

	struct ICommand
	{
		virtual ~ICommand() = default;
		virtual void call() const = 0;
		virtual void undo() const = 0;
	};

	struct Command : ICommand
	{
		BankAccount& account; // If we wanted to serialize we could instead use a unique identifier
		enum Action {deposit, withdraw} action;
		int amount;

		Command(BankAccount& account, Action action, int amount)
			: account{account},
			  action{action},
			  amount{amount}
		{
		}

		// We could seperate this out, its just here for simplicity
		void call() const override
		{
			switch(action) { 
				case deposit: 
					account.deposit(amount);
					break;
				case withdraw: 
					account.withdraw(amount);
					break;
				default: 
					break;
			}
		}

		void undo() const override
		{	// Swapped withdraw and deposit
			// Obviously not the best way of doing this,
			// though proves the concept
			switch(action) { 
				case deposit: 
					account.withdraw(amount);
					break;
				case withdraw: 
					account.deposit(amount);
					break;
				default: 
					break;
			}
		}
	};

	struct CommandList : vector<Command>, ICommand
	{	// This is now a vector, it has a storage of commands
		// This is also an ICommand

		CommandList(const initializer_list<Command>& _Ilist)
			: vector<Command>{_Ilist}
		{
		}

		void call() const override
		{
			for(auto& cmd : *this)
				cmd.call();
		}

		void undo() const override
		{
			// We must loop through the collection in reverse
			for_each(rbegin(), rend(), [](const Command& cmd)
			{
				cmd.undo();
			});
		}
	};

}

using namespace CompositeCommandPattern;

int CompositeCommandPattern_main(int argc, char* argv[])
{
	BankAccount ba;
	CommandList commands // Initalize much like a vector
	{
		Command{ba, Command::deposit, 100},
		Command{ba, Command::withdraw, 200}
	};

	cout << "Start, balance: " << ba.balance << endl;

	commands.call();
	
	cout << "After commands, balance: " << ba.balance << endl;

	commands.undo();

	cout << "After undo, balance: " << ba.balance << endl;

	getchar();
	return EXIT_SUCCESS;
}