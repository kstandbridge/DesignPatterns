#pragma once
#include "ChatPerson.h"

namespace Mediator_Chatroom
{
    struct ChatRoom
    {
        // The mediator has direct references to all the people
        vector<Person> people; // Notice private

        // We use a person reference when joining so we can have unique indentifiers
        class PersonReference
        {
            vector<Person>& people;
            unsigned int index;
        public:

            PersonReference(vector<Person>& persons, const size_t index)
                : people(persons),
                  index(index)
            {
            }

            Person* operator->() const; // overloaded so it acts like a Person
        };

        // Rather than exposing the vector<Person> we can wrap this functionality
        // in a join method, so it returns our PersonReference
        PersonReference join(Person&& p);

        void broadcast(const string& origin, const string& message);
        void message(const string& origin, const string& who, const string& message);
    };
}
