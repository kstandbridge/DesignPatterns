#include <iostream>
#include <vector>
#include <memory>
using namespace std;

namespace Momento
{
    // Motivation
    // An object or system goes through changes
    // - a bank account gets deposits and withdrawals
    // There are different ways of navigating those changes
    // One way is to record every change (Command Pattern) and teach a command to undo itself
    // Another is to simply save snapshots of the system

    // Moment
    // A token/handle representing the system state. Lets us
    // roll back to the state when the token was generated.
    // May or may not directtly expose state information.
    
    // In this example we'll have a bank account
    // whereby a snapshot is taken when deposits happen
    // So we can rollback the account to a certain point

    // The momento will need to contain all variables in the system needed to roll back
    class Momento
    {
        // We want these variables to be private, as we are not exposing state information in these tokens
        int balance; // Its a simple system, so only one variable to keep track off
    public:
        Momento(int balance)
            : balance{balance}
        {
        }
        friend class BankAccount; // Our bank account can access state information
    };

    // Our bank account
    class BankAccount
    {
        int balance = 0;
        vector<shared_ptr<Momento>> changes; // All changes to the system
        int current; // What change number are we currently on
    public:
        explicit BankAccount(int balance)
            : balance{balance}
        {
            changes.emplace_back(make_shared<Momento>(balance));
            current = 0;
        }

        shared_ptr<Momento> desposit(int amount)
        {
            // A snapshot is taken and returned on every deposit
            balance += amount;
            auto momento = make_shared<Momento>(balance);
            changes.push_back(momento);
            ++current;
            return momento;
        }

        void restore(const shared_ptr<Momento>& m)
        {   // Revert all variables in the system to match the snapshot
            if(m)
            {
                balance = m->balance;
                changes.push_back(m);
                current = changes.size() - 1;
            }
        }

        // Much like restore, we can undo/redo using the snapshots within the changes collection
        shared_ptr<Momento> undo()
        {
            // Only if theres something to go back to
            if(current > 0)
            {
                --current;
                auto m = changes[current];
                balance = m->balance;
                return m; // we can return the snapshot to allow them to restore
            }
            return{};
        }

        shared_ptr<Momento> redo()
        {
            // Again only if theres something to redo
            if(current + 1 < changes.size())
            {
                ++current;
                auto m = changes[current];
                balance = m->balance;
                return m;
            }
            return{};
        }       

        friend std::ostream& operator<<(std::ostream& os, const BankAccount& obj)
        {
            return os << "balance: " << obj.balance;
        }
    };

}

using namespace Momento;

int Momento_main(int argc, char* argv[])
{
    BankAccount ba{100};
    auto m1 = ba.desposit(25); // 125
    ba.desposit(25); // 150
    ba.desposit(25); // 175
    cout << ba << endl;

    ba.undo(); // 150
    cout << "Undo 1: " << ba << endl;
    ba.undo(); // 125
    cout << "Undo 2: " << ba << endl;
    ba.redo(); // 150
    cout << "Redo 1: " << ba << endl;
    ba.redo(); // 175
    cout << "Redo 2: " << ba << endl;

    ba.restore(m1);
    cout << "Restore to 125: " << ba << endl;
    ba.undo();
    cout << "Undo the restore: " << ba << endl;

	getchar();
	return EXIT_SUCCESS;
}