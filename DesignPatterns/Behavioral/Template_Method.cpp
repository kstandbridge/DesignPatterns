#include <iostream>
#include <string>
using namespace std;

namespace Template_Method
{
    // To save confusion, the Template Method design pattern has nothing to do with templates in C++

    // Motivation
    // Algorithms can be decomposed into
    // common parts + specifics
    // Strategy pattern does this through composition
    // - High-level algorithm uses an interface
    // - Concrete implementations implement the interface
    // Template Method does the same thing through inheritance

    // Template Method
    // Allows us to define the 'skeleton' of the algorithm, with
    // concrete implementations defined in subclasses.

    // Lets take a board game as an example
    // Our model is around games that have seveal players,
    // where they take turns in a loop until we have a winner

    // We need a define an algorithm at a high level, so what is the flow of the game
    // Define constituent parts as pure virual functions
    // Inherit the algorithm class, providing necessary function implementations
    
    // Here we describe the algorithm in an abstract class
    class Game
    {
    public:
        explicit Game(int numberOfPlayers)
            : number_of_players{numberOfPlayers}
        {
        }
        
        void run()
        {   // The main game loop
            start();
            while(!have_winner())
                take_turn();

            cout << "player " << get_winner() << " wins." << endl;
        }
    protected:
        // Our inheritors will define how each part of this algorithm works
        virtual void start() = 0;
        virtual bool have_winner() = 0;
        virtual void take_turn() = 0;
        virtual int get_winner() = 0;

        int current_player{ 0 };
        int number_of_players;
    };

    // We'll just simulate playing a game for simplicity
    class Chess : public Game
    {
    public:
        explicit Chess() : Game{2} {}
    protected:

        void start() override
        {
            cout << "Starting a game of chess " << number_of_players << " players" << endl;
        }

        bool have_winner() override
        {
            return turns == max_turns; // Lets just have a max number of turns for simpilicty
        }

        void take_turn() override
        {
            cout << "Turn " << turns << " taken by player " << current_player << endl;
            turns++;
            current_player = (current_player + 1) % number_of_players;
        }

        int get_winner() override
        {
            return current_player;
        }
    private:
        int turns{0}, max_turns{10};
    };

}

using namespace Template_Method;

int Template_Method_main(int argc, char* argv[])
{
    // The only thing thats exposed in our Chess class is run
    Chess chess;
    chess.run();

	getchar();
	return EXIT_SUCCESS;
}


