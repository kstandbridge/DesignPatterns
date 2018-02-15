#define _SCL_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <boost/signals2.hpp>
using namespace boost::signals2;

namespace Mediator_EventBus
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
    
    // In this example we'll have a simple ball game. Whereby the coach is aware when a player scores
    // We can achieve this using an EventBroker, also known as an EventBus

    // Our event interface
    struct Event
    {
        virtual ~Event() = default;
        virtual void print() const = 0;
    };

    struct Player; // forward declaration

    // The scored event, this is what the coach listens to
    struct PlayerScored : Event
    {
        string player_name;
        int goals_scored_so_far;

        PlayerScored(const string& playerName, int goalsScoredSoFar)
            : player_name{playerName},
              goals_scored_so_far{goalsScoredSoFar}
        {
        }

        void print() const override
        {
            cout << player_name 
                 << " has scored! (their " 
                 << goals_scored_so_far 
                 << " goal)" << endl;
        }
    };

    // This is our EventBroker
    struct Game
    {
        signal<void(Event*)> events; // We use boost::signals, this is our observer
    };

    struct Player
    {
        string name;
        int goals_scored = 0;
        Game& game;


        Player(const string& name, Game& game)
            : name{name},
              game{game}
        {
        }

        void score()
        {   // When a player scores, we can create the event and pass it to our broker
            goals_scored++;
            PlayerScored ps{name, goals_scored};
            ps.print(); // Lets also print out the scored details
            game.events(&ps);
        }
    };

    // Our coach who will listen for events
    struct Coach
    {
        Game& game;

        explicit Coach(Game& game)
            : game{game}
        {
            game.events.connect([](Event* e)
            {
                // Our event is polymorphic, because we're using pointers we can safely perform
                // a dynamic_cast without a try/catch block, as the result will be a nullptr
                PlayerScored* ps = dynamic_cast<PlayerScored*>(e);
                if(ps && ps->goals_scored_so_far < 3) // Lets only do something for the first couple of goals
                {
                    cout << "coach says: well done, " << ps->player_name << endl;
                }
            });
        }

        ~Coach()
        {   // A good idea to clean up some
            game.events.disconnect_all_slots();
        }
    };

}

using namespace Mediator_EventBus;

int Mediator_EventBus_main(int argc, char* argv[])
{
    Game game;
    Player player{"Kenneth", game};
    Coach coach{ game };

    player.score();
    player.score();
    player.score();

	getchar();
	return EXIT_SUCCESS;
}