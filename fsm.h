/*
Copyright (c) 2007 Gerhard Reitmayr <gerhard.reitmayr@gmail.com>

Permission to use, copy, modify, and distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _FSM_H
#define _FSM_H

/// list of ints as recursive type
template <int HEAD, class TAIL> struct IntList {
    enum { head = HEAD };
    typedef TAIL tail;
};

/// end of list marker type
struct IntListEnd {};

/// switch statement implementation through iterating over the type list and
/// comparing it to a given state
template <class STATELIST>
struct SwitchTemplate {
    template <class CONTEXT>
    static typename CONTEXT::ReturnType work( int state, CONTEXT & context){
        return ((STATELIST::head == state) ?
                 context.template operator()<STATELIST::head>() :
                 SwitchTemplate<typename STATELIST::tail>::work( state, context ));
    }
};

/// end of list marker specialization
template <>
struct SwitchTemplate<IntListEnd> {
    template <class CONTEXT>
    static typename CONTEXT::ReturnType work( int state, CONTEXT & context){
        return typename CONTEXT::ReturnType();
    }
};

/// The actual state machine implementation
/// sub types are functors passed to the Worker class to execute the
/// right template specialization of the underlying member function of the
/// context object.
template <class CONTEXT, class STATELIST = typename CONTEXT::StateList>
struct StateMachine {

    CONTEXT & context;
    int state;

    template <class RET, class DATA> struct CallEvent {
        typedef RET ReturnType;
        CONTEXT & context;
        DATA & data;

        template <int STATE> RET operator()(){
            return context.template event<STATE>(data);
        }
    };

    template <class RET, class DATA> struct CallEventConst {
        typedef RET ReturnType;
        CONTEXT & context;
        const DATA & data;

        template <int STATE> RET operator()(){
            return context.template event<STATE>(data);
        }
    };

    template <class RET> struct CallEventNoData {
        typedef RET ReturnType;
        CONTEXT & context;

        template <int STATE> RET operator()(){
            return context.template event<STATE>();
        }
    };

    struct CallEnter {
        typedef void ReturnType;
        CONTEXT & context;

        template <int STATE> ReturnType operator()(){
            return context.template enter<STATE>();
        }
    };

    struct CallExit {
        typedef void ReturnType;
        CONTEXT & context;

        template <int STATE> ReturnType operator()(){
            return context.template exit<STATE>();
        }
    };

    StateMachine( CONTEXT & c ) : context(c), state(STATELIST::head) {
        CallEnter cee = {context};
        SwitchTemplate<STATELIST>::work(state, cee);
    }

    void changeState(const int newstate){
        CallExit cl = {context};
        SwitchTemplate<STATELIST>::work(state, cl);
        state = newstate;
        CallEnter cee = {context};
        SwitchTemplate<STATELIST>::work(state, cee);
    }

    void work(){
        CallEventNoData<int> ce = {context};
        int newstate = SwitchTemplate<STATELIST>::work(state, ce);
        if(newstate != state)
            changeState(newstate);
    }

    template <class EVENT>
    void work( const EVENT & ev ){
        CallEventConst<int, EVENT> ce = {context, ev};
        int newstate = SwitchTemplate<STATELIST>::work(state, ce);
        if(newstate != state)
            changeState(newstate);
    }

    template <class EVENT>
    void work( EVENT & ev ){
        CallEvent<int, EVENT> ce = {context, ev};
        int newstate = SwitchTemplate<STATELIST>::work(state, ce);
        if(newstate != state)
            changeState(newstate);
    }
};


/// macros for simple state list definition
#define LIST1(a)                IntList<a,IntListEnd>
#define LIST2(a,b)              IntList<a,LIST1(b) >
#define LIST3(a,b,c)            IntList<a,LIST2(b,c) >
#define LIST4(a,b,c,d)          IntList<a,LIST3(b,c,d) >
#define LIST5(a,b,c,d,e)        IntList<a,LIST4(b,c,d,e) >
#define LIST6(a,b,c,d,e,f)      IntList<a,LIST5(b,c,d,e,f) >
#define LIST7(a,b,c,d,e,f,g)    IntList<a,LIST6(b,c,d,e,f,g) >
#define LIST8(a,b,c,d,e,f,g,h)  IntList<a,LIST7(b,c,d,e,f,g,h) >

#endif // _FSM_H
