/**
 * GOTO
 * Copyright (C) 2018 Alexander Stefas
 */

#include "GOTO.h"	

GOTO::GOTO():wrapStates(true),numOfStates(0),currentState(NULL), nextState(NULL), prevState(NULL), updateLoop(false), hasBeenUpdated(false) {

}

byte GOTO::registerState(byte (*enter)(byte id), byte  (*update)(byte id), byte (*exit)(byte id)){
	registeredStates = (state *) realloc(registeredStates, (numOfStates + 1) * sizeof(state));
	
	byte currentRegisteringState = numOfStates;
	
	registeredStates[numOfStates].stateID = currentRegisteringState;
	registeredStates[numOfStates].enterCB = enter;
	registeredStates[numOfStates].updateCB = update;
	registeredStates[numOfStates].exitCB = exit;
  	
  	numOfStates++;

  	return currentRegisteringState;
}

byte GOTO::registerState(byte (*update)(byte id)){
	registeredStates = (state *) realloc(registeredStates, (numOfStates + 1) * sizeof(state));

	byte currentRegisteringState = numOfStates;

	registeredStates[numOfStates].stateID = currentRegisteringState;
	registeredStates[numOfStates].enterCB = NULL;
	registeredStates[numOfStates].updateCB = update;
	registeredStates[numOfStates].exitCB = NULL;
  	
  	numOfStates++;

  	return currentRegisteringState;
}

void GOTO::process(){
	exitState(currentState);
	prevState = currentState;
	currentState = nextState;
	resetNextState();
	hasBeenUpdated = false;
	enterState(currentState);
}

void GOTO::update(){
	if(updateLoop) (*registeredStates[currentState].updateCB)(currentState);
	else if(!hasBeenUpdated){
		(*registeredStates[currentState].updateCB)(currentState);
		hasBeenUpdated = true;
	}
}

byte GOTO::gotoNext(){
	nextState = currentState + 1;
	if(nextState > numOfStates - 1) {
		if(wrapStates) nextState = 0;
	}

	process();

	return currentState;
}

byte GOTO::gotoFirst(){
	nextState = 0;

	process();

	return currentState;
}

byte GOTO::gotoLast(){
	nextState = numOfStates - 1;

	process();

	return currentState;
}

byte GOTO::gotoPrevious(){
	if(currentState == 0) {
		if(wrapStates) nextState = numOfStates - 1;
	}
	else nextState = currentState - 1;

	process();

	return currentState;
}

byte GOTO::gotoState(byte id){
	if(id >= 0 && id <= numOfStates - 1){
		if(id != currentState){
			nextState = id;
			
			process();

			return currentState;
		}
	}
}

void GOTO::enterState(byte id){
	if(registeredStates[id].enterCB != NULL) {
		registeredStates[id].enterCB(id);
		hasBeenUpdated = false;
	}
}
        
void GOTO::exitState(byte id){
	if(registeredStates[id].exitCB != NULL) registeredStates[id].exitCB(id);
}

void GOTO::resetNextState(){
	nextState = NULL;
}