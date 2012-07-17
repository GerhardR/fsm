# FSM - C++ Template-based Finite State Machine

Copyright Gerhard Reitmayr 2007-2012

This small header file fsm.h provides a simple state machine implementation based on C++ template programming. It supports handling incoming events and entry/exit calls for each state. It does not support hierarchical state machines, tracing, explicit start/stop states etc.

It's aim is to be usable with minimal overhead in the client code. Thus, there is no need to declare and implement hierarchies of state classes or construct tables of state transitions and functions to call. The example in lightswitch.cpp shows a simple usage example of a light switch that only receives a single kind of event.


