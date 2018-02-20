#include <iostream>
#include <string>
#include <vector>
using namespace std;

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
// We'll use boost MSM to simplify this approach

//back-end
#include <boost/msm/back/state_machine.hpp>

// front-end
#include <boost/msm/front/state_machine_def.hpp>
#include <boost/msm/front/functor_row.hpp>

// MSM = Meta state machine
namespace msm = boost::msm;
// MPL = Meta programming library
namespace mpl = boost::mpl;

using namespace msm::front;

namespace State_boost
{
    // So we can output the state later
    vector<string> state_names {
        "off hook"s,
        "connecting"s,
        "connected"s,
        "on hold"s,
        "destroyed"s
    };

    // Each of our triggers
    struct CallDialed {};
    struct HungUp {};
    struct CallConnected {};
    struct PlacedOnHold {};
    struct TakenOffHold {};
    struct LeftMessage {};
    struct PhoneThrownIntoWall {};

    // Our state machine needs to inherit from boost::msm
    struct PhoneStateMachine : state_machine_def<PhoneStateMachine>
    {
        // Each of our states are created as structs within the state machine
        // Notice that they all need to inherit from boost::msm
        struct OffHook : state<> {};
        // We can invoke additional events, such as when the state is set
        struct Connecting : state<>
        {
            template <class Event, class FSM>
            void on_entry(Event const& evt, FSM&)
            {
                cout << "We are connecting..." << endl;
            } // we can also do on_exit for example
        };

        // This is not a state, but an action that can be invoked during a state transition
        struct PhoneBeingDestoryed
        {
            template <class EVT, class FSM, class SourceState, class TargetState>
            void operator()(EVT const&, FSM&, SourceState&, TargetState&)
            {
                cout << "Phone breaks into a million pieces" << endl;
            }
        };

        // We can also setup a guard to decide if the above action will be invoked
        bool angry { false };
        struct CanDestoryPhone
        {
            template <class EVT, class FSM, class SourceState, class TargetState>
            bool operator()(EVT const&, FSM& fsm, SourceState&, TargetState&)
            {
                return fsm.angry;
            }
        };

        struct Connected : state<> {};
        struct OnHold : state<> {};
        struct PhoneDestroyed : state<> {};

        // required by boost msn to have a transition table
        // Here we choose which trigger causes the state to transition to another state
        struct transition_table : mpl::vector<
            Row<OffHook, CallDialed, Connecting>, // <currentState, Trigger, newState>
            Row<Connecting, CallConnected, Connected>,
            Row<Connected, PlacedOnHold, OnHold>,
            Row<OnHold, PhoneThrownIntoWall, PhoneDestroyed, PhoneBeingDestoryed, CanDestoryPhone> // <currentState, Trigger, newState, actionToInvoke, guard>
        >{};

        typedef OffHook initial_state;

        // If we don't have a map for a state to transition on a certain trigger
        // we can create an addition event to be called as such
        template <class FSM, class Event>
        void no_transition(Event const& e, FSM&, int state)
        {
            cout << "No transition from state " << state_names[state]
                 << " on event " << typeid(e).name() << endl;
        }
    };
}

using namespace State_boost;

int State_boost_main(int argc, char* argv[])
{
    msm::back::state_machine<PhoneStateMachine> phone;

    const auto info = [&]() // Just to simplify the output
    {
        auto i = phone.current_state()[0];
        cout << "The phone is currently " << state_names[i] << endl;
    };

    info();
    phone.process_event(CallDialed{}); // We can just pass our trigger
    info(); // The state will change based on the transition_table
    phone.process_event(CallConnected{});
    info();
    phone.process_event(PlacedOnHold{});
    info();
    phone.process_event(PhoneThrownIntoWall{}); // Disallowed transition, we are not angry
    info(); // Still shows as on hold
    phone.angry = true;
    phone.process_event(PhoneThrownIntoWall{});
    info(); // The phone is destoryed
    phone.process_event(CallDialed{}); // transition error, we have no state mapping for a destroyed phone

	getchar();
	return EXIT_SUCCESS;
}


