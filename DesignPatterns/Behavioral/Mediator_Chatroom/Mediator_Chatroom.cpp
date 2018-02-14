#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include "ChatRoom.h"
#include "ChatPerson.h"

namespace Mediator_Chatroom
{
    // Motivation
    // Components may go in and out of a system at any time (created/destroyed)
    // - Chat room participants
    // - Players in a MMORPG
    // It makes no sense for them to have direct references to one another
    // Solution: have them all refer to some central component that favilitates communication

    // Mediator pattern
    // A component that favilitates communication between 
    // other components without them being aware of each
    // other or having direct (referential) access to each other
}

using namespace Mediator_Chatroom;

// In this example we will use the Mediator pattern to implement a chat room
// The ChatRoom acts as our Mediator here.

int Mediator_main(int argc, char* argv[])
{
    ChatRoom room;

    auto john = room.join(Person{"john"});
    auto jane = room.join(Person{"jane"});

    john->say("hi room");
    jane->say("oh, hey john");

    auto simon = room.join(Person{ "simon" });
    simon->say("hi everyone!");

    // Notice how we don't use a reference to Simon
    jane->pm("simon", "glad you could join us, simon");


    getchar();
    return EXIT_SUCCESS;
}
