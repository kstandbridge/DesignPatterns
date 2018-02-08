#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

namespace CommandPattern
{
	// Motiviation
	// Ordinary C++ statements are perishable, you cannot undo a field assignment
	// A sequence of actions cannot be serialized
	// We want an object that represents an operation

	// Command design pattern
	// An object which represents an instruction to perform a particular action.
	// Contains all information necessary for the action to be taken.


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

	struct Command
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
		void call() const
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

		void undo() const
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

}

using namespace CommandPattern;

int CommandPattern_main(int argc, char* argv[])
{
	BankAccount ba;
	vector<Command> commands
	{
		Command{ba, Command::deposit, 100},
		Command{ba, Command::withdraw, 200}
	};

	cout << "Start, balance: " << ba.balance << endl;

	for(auto& cmd : commands)
		cmd.call();
	
	cout << "After commands, balance: " << ba.balance << endl;

	// We need to go through the vector in reverse
	for_each(commands.rbegin(), commands.rend(), [](const Command& cmd)
	{
		cmd.undo();
	});

	cout << "After undo, balance: " << ba.balance << endl;

	getchar();
	return EXIT_SUCCESS;
}