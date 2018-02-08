#include <string>
#include <iostream>

using namespace std;

namespace ChainOfResponsibility_PointerChain
{
	// A chain of components who all get a chance to process a command or query,
	// optionally having a default processing implementation and an ability to 
	// terminate the processing chain.

	// Command Query Separation
	// Command is asking for an action or change. CREATE/UPDATE/DELETE
	// Query is asking for information. READ
	// CQS is having separate means of sending commands and queries

	// In this example we have a creature class, and we want to add bonuses to its stats
	// 1. Double the creature's attack
	// 2. Increase defense by 1 unless power > 2
	// 3. No bonuses can be applied to this create

	// Our create class
	struct Creature
	{
		string name;
		int attack, defense;
		
		Creature(const string& name, int attack, int defense)
			: name{name},
			  attack{attack},
			  defense{defense}
		{
		}

		friend std::ostream& operator<<(std::ostream& os, const Creature& obj)
		{
			return os
					<< "name: " << obj.name
					<< " attack: " << obj.attack
					<< " defense: " << obj.defense;
		}
	};

	// Our base modifier that others will inherit from
	class CreatureModifier
	{
		CreatureModifier* next = nullptr; // The next modifier in the chain
	protected:
		Creature& creature; // Our creature to modify
	public:

		explicit CreatureModifier(Creature& creature)
			: creature{creature}
		{
		}
		virtual ~CreatureModifier() = default;

		void add(CreatureModifier* cm)
		{	// Adding the next modifier in the chain
			if(next) 
				next->add(cm);
			else 
				next = cm;
		}

		virtual void handle()
		{	// By default we call the underlying modifier
			if(next) next->handle();
		}
	};


	// Double the attack
	class DoubleAttackModifier : public CreatureModifier
	{
	public:
		explicit DoubleAttackModifier(Creature& creature)
			: CreatureModifier{creature}
		{
		}


		void handle() override
		{
			creature.attack *= 2;
			CreatureModifier::handle(); // Call the base, which will call the next modifer in the chain
		}
	};

	class IncreaseDefenseModifier : public CreatureModifier
	{
	public:
		explicit IncreaseDefenseModifier(Creature& creature)
			: CreatureModifier{creature}
		{
		}

		void handle() override
		{	// Another more complicated bonus
			if(creature.attack <= 2)
				creature.defense++;
			CreatureModifier::handle();
		}
	};

	class NoBonusModifier : public CreatureModifier
	{
	public:
		explicit NoBonusModifier(Creature& creature)
			: CreatureModifier{creature}
		{
		}


		void handle() override
		{
			// We intentionally don't call the base
			// Thus we can short circut once it hits this modifier
		}
	};
}

using namespace ChainOfResponsibility_PointerChain;

int ChainOfResponsibility_PointerChain_main(int argc, char* argv[])
{
	Creature goblin{"Goblin",  1, 1};
	cout << goblin << endl;

	CreatureModifier root{goblin};
	DoubleAttackModifier r1{goblin};
	DoubleAttackModifier r1_2{goblin};
	IncreaseDefenseModifier r2{goblin};
	NoBonusModifier no{goblin};

	// The order of these is important, as they're done sequentially
	// Notice NoBonus is first, causes subsequent modifiers to be ignored
	root.add(&no); 
	root.add(&r1);
	root.add(&r1_2);
	root.add(&r2);

	root.handle();

	cout << goblin << endl;
	
	getchar();
	return EXIT_SUCCESS;
}


