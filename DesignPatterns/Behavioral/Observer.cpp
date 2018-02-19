#include <string>
#include <vector>
#include <iostream>
#include <mutex>
using namespace std;
#include <boost/any.hpp>
using namespace boost;

namespace Observer
{
    // Motivation
    // We need to be informed when certain things happen
    // - Objects property changed
    // - Object does something
    // - Some external event occurs
    // We want to listen to events and be notified
    // when they occur
    
    // Observer
    // An observer is an object that wishes to be informed
    // about events happening in the system, typically by
    // providing a callback function to call when events occur.
    // The entity generating the events is sometimes called
    // observable

    // In this example we'll have a person, 
    // when their age changes we want to be notified so we can log to the console
    // we would also like to be made aware when they can vote

    struct Person; // forward declaration

    // Interface, so we can have multiple different listeners
    struct PersonListener
    {
        virtual ~PersonListener() = default;
        virtual void PersonChanged(Person& p, 
                                   const string& property_name,
                                   const any new_value) = 0;
    };

    // Mutex - mutual exclusion, for thread safety
    static mutex mtx;

    // our person
    struct Person
    {
        explicit Person(int age)
            : age{age}
        {
        }

        // Age is private so we have Getter and setters
        int getAge() const
        {
            return age;
        }

        // Within set we need to notify of the change
        void setAge(const int age)
        {
            if(this->age == age) return; // short circut when nothing changed

            const auto old_can_vote = GetCanVote(); // This doesn't have a setter so we need to get it prior

            this->age = age;
            notify("age", this->age); // let listeners know age changed

            const auto new_can_vote = GetCanVote();

            if(old_can_vote != new_can_vote)
            {
                notify("can_vote", new_can_vote); // let listeners know can_vote changed
            }
        }

        // Read only property
        bool GetCanVote() const
        {
            return age >= 16;
        }

        // A listener wants to be made aware of changes
        void subscribe(PersonListener* pl)
        {
            lock_guard<mutex> guard{ mtx }; // Any other threads that try to lock this mtx will wait for this to unlock

            if(find(begin(listeners), end(listeners), pl) == end(listeners)) // Prevent double subscription
                listeners.push_back(pl);
        }

        // A listener wants to stop being made aware of changes
        void unsubscribe(PersonListener* pl)
        {
            lock_guard<mutex> guard{ mtx }; // thread safety

            for(auto it = listeners.begin(); it != listeners.end(); ++it)
            {
                if(*it == pl) // This is our PersonListener
                {
                    *it = nullptr; // will be removed in notify, more thread safe
                }
            }
        }

        // Let all listeners know a property has changed
        void notify(const string& property_name, const any new_value)
        {
            lock_guard<mutex> guard{ mtx }; // thread safety

            for(const auto listener : listeners)
            {
                if(listener) // check for nullptr
                    listener->PersonChanged(*this, property_name, new_value);
            }
            // When listens unsubscribe they are set to nullptr, as its more thread safe. They can be removed here
            listeners.erase(remove(listeners.begin(), listeners.end(), nullptr), listeners.end());
        }

    private:
        int age;
        vector<PersonListener*> listeners;
    };

    // Our console logger
    struct ConsoleListener : PersonListener
    {
        void PersonChanged(Person& p, const string& property_name, const any new_value) override
        {
            std::cout << "person's " << property_name << " has been changed to ";
            if(property_name == "age")
            {
                cout << any_cast<int>(new_value);
            }
            else if(property_name == "can_vote")
            {
                cout << any_cast<bool>(new_value);
            }
            cout << endl;
        }
    };

    // When using locks, its always important to consider deadlocks
    // Thats where two threads are waiting on each other to release the lock
    struct BadListener : PersonListener
    {
        void PersonChanged(Person& p, const string& property_name, const any new_value) override
        {
            p.unsubscribe(this); // will cause a deadlock

            // Whats actually going on here is.
            // 1. Notify was called, which takes a lock
            // 2. Which then calls PersonChanged (where we are now)
            // 3. We call to unsubscribe, which takes a lock
            // Stop 3. is now waiting on the lock taken in step 1. Which will remain locked until we leave this method
            // Deadlock

            
        }
    };

}

using namespace Observer;

int Observer_main(int argc, char* argv[])
{
    Person p{ 14 };
    
    ConsoleListener cl;
    p.subscribe(&cl);
    p.subscribe(&cl); // We have checks in place to prevent double subscription
    
    p.setAge(15);
    p.setAge(16); // Should also notify of can_vote change

    p.unsubscribe(&cl);
    p.setAge(17); // Should not be notified

	getchar();
	return EXIT_SUCCESS;
}


