#include <iostream>
#include <string>

#include "fsm.h"

using namespace std;

/// example

enum States { OFF, ON };

struct LightSwitch {
    /// List of states
    typedef LIST2(OFF, ON) StateList;
    /// default template version for the event function
    template <int> int event() { cout << "undefined event handler" << endl; return 0; }
    template <int> void enter() { cout << "undefined enter" << endl; }
    template <int> void exit() { cout << "undefined leave" << endl; };
};

/// implement the specializations for states that require behaviour
template <> int LightSwitch::event<ON>( ) { cout << "reveived push" << endl; return OFF; };
template <> int LightSwitch::event<OFF>(  ) { cout << "received push" << endl; return ON; };

template <> void LightSwitch::enter<ON>() { cout << "enter ON - turn on the light" << endl; }
template <> void LightSwitch::enter<OFF>() { cout << "enter OFF - turn off the light" << endl; }

template <> void LightSwitch::exit<ON>() { cout << "exit ON - do nothing" << endl; }
template <> void LightSwitch::exit<OFF>() { cout << "exit OFF - do nothing" << endl; }

int main() {

    LightSwitch c;
    StateMachine<LightSwitch> sm(c);

    sm.work();
    sm.work();
    sm.work();
    sm.work();
    sm.work();
    sm.work();
}
