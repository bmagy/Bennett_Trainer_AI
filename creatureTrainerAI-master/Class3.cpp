//  Class3.cpp
//  Helper Class3
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#include "Class3.h"

ActualCreature::ActualCreature(){
	name = -1;
	currentHP = -1;
	damageTaken = -1;
	isAlive = true;
}

MyParty::MyParty(){
	currentCreatureIndex = 0;
	partySize = 4;
}

int MyParty::countAlive(){
	int count = 0;
	for (int i = 0; i < partySize; i++){
		if (party[i].isAlive){
			count++;
		}
	}
	return count;
}

int MyParty::findAlive(){
	for (int i = 0; i < partySize; i++){
		if (party[i].isAlive && i != currentCreatureIndex){
			return i;
		}
	}
	return -1;
}

int MyParty::findLargestHP(){
	int maxIndex = -1;
	int largestHP = 0;
	for (int i = 0; i < partySize; i++){
		if (party[i].currentHP > largestHP && i != currentCreatureIndex){
			largestHP = party[i].currentHP;
			maxIndex = i;
		}
	}
	return maxIndex;
}