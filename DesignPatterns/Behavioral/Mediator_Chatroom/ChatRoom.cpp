#include "ChatRoom.h"
#include <algorithm>

namespace Mediator_Chatroom
{
    Person* ChatRoom::PersonReference::operator->() const
    {
        // overloaded so it acts like a Person
        return &people[index];
    }

    void ChatRoom::broadcast(const string& origin, const string& message)
    {
        for(auto& p : people)
            if(p.name != origin)
                p.receive(origin, message);
    }

    ChatRoom::PersonReference ChatRoom::join(Person&& p)
    {
        string join_msg = p.name + " joins the chat";
        broadcast("room", join_msg);

        p.room = this;
        people.emplace_back(p);
        return {people, people.size() - 1};
    }

    void ChatRoom::message(const string& origin, const string& who, const string& message)
    {
        auto target = find_if(begin(people),
                              end(people),
                              [&](const Person& p) { return p.name == who; });
        if(target != end(people))
        {
            target->receive(origin, message);
        }
    }
}
