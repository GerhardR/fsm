#include <iostream>
#include <string>

#include "fsm.h"

using namespace std;

/// example

struct Event { int state; };

enum States { OFF, ON };

struct LightSwitch {
    /// List of states
    typedef LIST2(OFF, ON) StateList;
    /// default template version for the event function
    template <int> int event(Event & ev) { cout << "undefined event handler " << ev.state << endl; return ev.state; }
    template <int> void enter() { cout << "undefined enter" << endl; }
    template <int> void exit() { cout << "undefined leave" << endl; };
};

/// implement the specializations for states that require behaviour
template <> void LightSwitch::enter<ON>() { cout << "enter ON - turn on the light" << endl; }
template <> void LightSwitch::enter<OFF>() { cout << "enter OFF - turn off the light" << endl; }

template <> void LightSwitch::exit<ON>() { cout << "exit ON - do nothing" << endl; }
template <> void LightSwitch::exit<OFF>() { cout << "exit OFF - do nothing" << endl; }

int main() {

    LightSwitch c;
    StateMachine<LightSwitch> sm(c);
    Event eOn = {ON};
    Event eOff = {OFF};

    sm.work(eOn);
    sm.work(eOff);
    sm.work(eOn);
    sm.work(eOn);
}
