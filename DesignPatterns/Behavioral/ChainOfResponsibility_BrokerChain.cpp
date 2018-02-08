#define _SCL_SECURE_NO_WARNINGS

#include <string>
#include <iostream>
#include <ostream>
using namespace std;
#include <boost/signals2.hpp>
using namespace boost::signals2;



namespace ChainOfResponsibility_BrokerChain
{
	// A chain of components who all get a chance to process a command or query,
	// optionally having a default processing implementation and an ability to 
	// terminate the processing chain.

	// Command Query Separation
	// Command is asking for an action or change. CREATE/UPDATE/DELETE
	// Query is asking for information. READ
	// CQS is having separate means of sending commands and queries

	// In this example we have a creature class, and we want to add bonuses to its stats
	// However we don't want these bonuses to be temporary, they could exist only while in scope
	// For this we'll setup an eventbus
	
	// Our query for looking up stats
	struct Query
	{
		string creature_name;
		enum Argument { attack, defense } argument;
		int result;

		Query(const string& creatureName, Argument argument, int result)
			: creature_name{creatureName},
			  argument{argument},
			  result{result}
		{
		}
	};

	// Our event bus/event broker
	// This provides communication between creates and modifiers
	struct Game
	{	
		signal<void(Query&)> queries;
	};

	// Our creature
	struct Creature
	{
		Game& game; // takes a reference to the game
		int attack, defense;
	public:
		string name;

		Creature(Game& game, int attack, int defense, const string& name)
			: game{game},
			  attack{attack},
			  defense{defense},
			  name{name}
		{
		}

		int GetAttack() const
		{
			Query q{name, Query::Argument::attack, attack}; // Add a query
			game.queries(q); // put it on the bus
			// All listeners will have executed and this the q.result is available
			return q.result;
		}

		friend std::ostream& operator<<(std::ostream& os, const Creature& obj)
		{
			return os
					<< "attack: " << obj.GetAttack() // Important that we use GetAttack not the raw value
					<< " defense: " << obj.defense // If we had a defense modifier we'd create a GetDefense() method
					<< " name: " << obj.name;
		}
	};

	// Our base modifier
	class CreateModifier
	{
		Game& game;
		Creature& creature;
	public:

		CreateModifier(Game& game, Creature& creature)
			: game{game},
			  creature{creature}
		{
		}

		virtual ~CreateModifier() = default;
	};

	class DoubleAttackModifier : public CreateModifier
	{
		connection conn;
	public:
		DoubleAttackModifier(Game& game, Creature& creature)
			: CreateModifier{game, creature}
		{
			// We add an event to the broker
			conn = game.queries.connect([&](Query& q)
			{
				if(q.creature_name == creature.name && // Would be better to use unique Ids
				   q.argument == Query::Argument::attack)
				{
					q.result *= 2; // We are only changing the returned value from this query. Not the raw value
				}
			});
		}

		virtual ~DoubleAttackModifier()
		{
			conn.disconnect(); // Unwire the event
		}
	};
}

using namespace ChainOfResponsibility_BrokerChain;

int ChainOfResponsibility_BrokerChain_main(int argc, char* argv[])
{
	Game game;
	Creature goblin{game, 2, 2, "Strong Goblin"};

	cout << goblin << endl;

	{	// While in scope, the modifier has an effect
		DoubleAttackModifier dam{game, goblin};

		cout << goblin << endl;
	}

	// Out of scope we are back to normal stats
	cout << goblin << endl;

	getchar();
	return EXIT_SUCCESS;
}


