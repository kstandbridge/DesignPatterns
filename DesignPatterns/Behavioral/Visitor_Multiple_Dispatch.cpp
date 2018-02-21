#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>
#include <typeindex>
#include <map>
using namespace std;


namespace Visitor_Multiple_Dispatch
{
    // This is more of a common practice that works well with the visitor pattern
    // but can be applied elsewhere
    
    // In this example we have a space shooter, working on how to handle collision between
    // different game objects. We'll just output to console for simplicity

    // Some forward declarations
    struct GameObject;
    void collide(GameObject& first, GameObject& second);

    // This is our base game object
    struct GameObject
    {
        virtual ~GameObject() = default;
        virtual type_index type() const = 0; // So we can identify the actual type
        virtual void collide(GameObject& other)
        {   // So we can call collide on the game object, which will just proxy over
            Visitor_Multiple_Dispatch::collide(*this, other);
        }
    };

    // This allows us to not have to implement the type() in all our inheritors
    template<typename T>
    struct GameObjectImpl : GameObject
    {
        type_index type() const override
        {
            return typeid(T);
        }
    };

    // We inherit from the template instead, they are still game objects
    struct Planet : GameObjectImpl<Planet> {};
    struct Asteroid : GameObjectImpl<Asteroid> {};
    struct Spaceship : GameObjectImpl<Spaceship> {};
    struct ArmedSpaceship : Spaceship
    {   // The flaw is, this type is still Spaceship so we'd need to override
        type_index type() const override
        {
            return typeid(ArmedSpaceship);
        }
    };

    // Some outcomes that will happen on collisions
    void spaceship_planet() { cout << "spaceship lands on planet\n"; }
    void asteroid_planet() { cout << "asteroid burns up in atmosphere\n"; }
    void asteroid_spaceship() { cout << "asteroid hits and destroys spaceship\n"; }
    void asteroid_armed_spaceship() { cout << "spaceship shoots asteroid\n"; }

    // Now we can map which function is called when two objects collide
    // ObjectA + ObjectB = FunctionC
    map<pair<type_index, type_index>, void(*)(void)> outcomes{
        {{typeid(Spaceship), typeid(Planet)}, spaceship_planet},
        {{typeid(Asteroid), typeid(Planet)}, asteroid_planet},
        {{typeid(Asteroid), typeid(Spaceship)}, asteroid_spaceship},
        {{typeid(Asteroid), typeid(ArmedSpaceship)}, asteroid_armed_spaceship},
    };

    // We can search the map to find a corresponding function to call
    void collide(GameObject& first, GameObject& second)
    {
        auto it = outcomes.find({first.type(), second.type()});
        if(it == outcomes.end())
        {   // Flip the objects as it would be the same result
            // ObjectA + ObjectB = ObjectB + ObjectA
            it = outcomes.find({second.type(), first.type()});
            if(it == outcomes.end())
            {   // No collision function in map
                cout << "objects pass each other harmlessly\n";
                return;
            }
        }
        it->second(); // execute the function
    }
}

using namespace Visitor_Multiple_Dispatch;

int Visitor_Multiple_Dispatch_main(int argc, char* argv[])
{
    ArmedSpaceship spaceship;
    Asteroid asteroid;
    Planet planet;

    collide(planet, spaceship);
    collide(planet, asteroid);
    collide(spaceship, asteroid);
    collide(planet, planet);
    planet.collide(planet); // We can call collide on the object too


    getchar();
    return EXIT_SUCCESS;
}
