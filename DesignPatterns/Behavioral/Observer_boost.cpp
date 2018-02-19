#define _SCL_SECURE_NO_WARNINGS // boost compile errors
#include <string>
#include <vector>
#include <iostream>
#include <mutex>
using namespace std;
#include <boost/any.hpp>
#include <boost/signals2.hpp>
using namespace boost;

namespace Observer_boost
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

    // In this example we have a Person and we'd like to be made aware
    // when their age has changed, perhaps to output to the console

    // We'll make use of boost/signals2 to simplify the approach

    // Lets have our template class to define our signal
    template <typename T>
    struct INotifyPropertyChanged
    {
        virtual ~INotifyPropertyChanged() = default;
        signals2::signal<void(T&, const string&)> PropertyChanged;
    };

    // Now our person class just needs to inherit from the template so we can call PropertyChanged;
    struct Person : INotifyPropertyChanged<Person>
    {
        explicit Person(int age)
            : age{age}
        {
        }

        int getAge() const
        {
            return age;
        }
        void setAge(const int age)
        {
            if(this->age == age) return;
            this->age = age;
            PropertyChanged(*this, "age"); // Let our signal know of the change
        }
    private:
        int age;
    };
    
}

using namespace Observer_boost;

int Observer_boost_main(int argc, char* argv[])
{
    Person p { 123 };
    // We just need to wire up a 'slot', a lamdba that will be invoked
    // We could also declare and use Person to gather values
    p.PropertyChanged.connect([](Person&, const string& property_name) 
    {
        cout << property_name << " has been changed " << endl;
    });
    p.setAge(20); // Now when the age is changed, we should be notified

	getchar();
	return EXIT_SUCCESS;
}


