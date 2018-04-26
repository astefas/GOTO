/**
 * GOTO
 * Copyright (C) 2018 Alexander Stefas
 */

#ifndef GOTO_h
#define GOTO_h

#if defined(WIRING) && WIRING >= 100
  #include <Wiring.h>
#elif defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif

class GOTO {
	public:
        GOTO();      // Constructor
        byte registerState(byte (*enter)(byte id), byte (*update)(byte id), byte (*exit)(byte id));
        byte registerState(byte (*update)(byte id));
        void update();
        byte gotoNext();
        byte gotoPrevious();
        byte gotoFirst();
        byte gotoLast();
        byte gotoState(byte id);
        boolean updateLoop;
        byte currentState;
        byte nextState;
        byte prevState;
        boolean wrapStates;
        byte numOfStates;
     private:
        struct state {
            byte stateID;
            void (*enterCB)(byte id);
            void (*updateCB)(byte id);
            void (*exitCB)(byte id);
        };
        boolean hasBeenUpdated;
        state *registeredStates;
        void resetNextState();
        void enterState(byte id);
        void exitState(byte id);
        void process();
};

#endif //GOTO_h