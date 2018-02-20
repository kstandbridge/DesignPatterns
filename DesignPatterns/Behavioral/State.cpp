#include <iostream>
#include <string>
#include <vector>
#include <map>
using namespace std;

namespace State_Pattern
{
    // Motivation
    // Consider an ordinary telephone (not smart phone)
    // What you do with it depends on the state
    // of the phone/line
    // - If it's ringing or you want to make a call,
    //   you can pick it up
    // - Phone must be off the hook to
    //   take/make a call
    // - If you are calling someone, and it's busy,
    //   you put the handset down
    // Changes in the state can be explicit or in
    // response to events (e.g., Observer)

    // State
    // A pattern in which the object's behavior is determined by its
    // state. An object transitions from one state to another
    // (something needs to trigger the transition). A formalized
    // construct which manages states and transitions is called a
    // state machine.

    // Lets use the phone example, we need to know the current state of the phone
    enum class State
    {
        OffHook,
        Connecting,
        Connected,
        OnHold
    };

    // So we can output to the console
    inline ostream& operator<<(ostream& os, const State& s)
    {
        switch(s)
        {
            case State::OffHook:
                os << "off the hook";
                break;
            case State::Connecting:
                os << "connecting";
                break;
            case State::Connected:
                os << "connected";
                break;
            case State::OnHold:
                os << "on hold";
                break;
            default: break;
        }
        return os;
    }

    // Now we need something that will cause the state to change
    enum class Trigger
    {
        CallDialed,
        HungUp,
        CallConnected,
        PlacedOnHold,
        TakenOffHold,
        LeftMessage
    };

    // again so we can output to the console
    inline ostream& operator<<(ostream& os, const Trigger& t)
    {
        switch(t)
        {
            case Trigger::CallDialed:
                os << "call dialed";
                break;
            case Trigger::HungUp:
                os << "hung up";
                break;
            case Trigger::CallConnected:
                os << "call connected";
                break;
            case Trigger::PlacedOnHold:
                os << "placed on hold";
                break;
            case Trigger::TakenOffHold:
                os << "taken off hold";
                break;
            case Trigger::LeftMessage:
                os << "left message";
                break;
            default: break;
        }
        return os;
    }
}

using namespace State_Pattern;

int State_Pattern_main(int argc, char* argv[])
{
    // Now we can map out all of the states, along 
    // with what triggers are available and which
    // state that trigger will lead to.
    map<State, vector<pair<Trigger, State>>> rules;

    // The phone is off the hood. So we can dial,
    // which will lead to connecting
    rules[State::OffHook] = {
        { Trigger::CallDialed, State::Connecting }
    };

    // While the phone is connecting, we can either
    // Hang up to return to the OffHook state or,
    // The call can be connected, which leads to
    // the connected state
    rules[State::Connecting] = {
        { Trigger::HungUp, State::OffHook },
        { Trigger::CallConnected, State::Connected }
    };

    rules[State::Connected] = {
        { Trigger::LeftMessage, State::OffHook },
        { Trigger::HungUp, State::OffHook },
        { Trigger::PlacedOnHold, State::OnHold }
    };

    rules[State::OnHold] = {
        { Trigger::TakenOffHold, State::Connected },
        { Trigger::HungUp, State::OffHook },
    };

    // Lets state with the phone off the hook
    State currentState{ State::OffHook };

    while(true) // Infinate loop
    {
        // So we can display the currentState, and using the map
        // ask which trigger to use, which in turn changes the state.
        cout << "The phone is currently " << currentState << endl;
    
        
select_trigger: // Avoid using triggers/goto. 
        cout << "Select a trigger: " << endl;

        int i = 0;
        for(auto& item : rules[currentState])
        {
            cout << i++ << ". " << item.first << endl;
        }

        int input;
        cin >> input;
        if(input < 0 || (input+1) > rules[currentState].size())
        {
            cout << "Incorrect option. Please try again." << endl;
            goto select_trigger; // Just to simlify the approach use the trigger
        }

        currentState = rules[currentState][input].second;
    }

    cout << "We are done using the phone." << endl;
    
    getchar();
    return EXIT_SUCCESS;
}
