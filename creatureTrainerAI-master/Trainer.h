//
//  Trainer.h
//  CreatureTrainer
//
//  Created by Jeremy Gibson on 11/4/14.
//  Copyright (c) 2014 Jeremy Richard Gibson. All rights reserved.
//

#ifndef __CreatureTrainer__Trainer__
#define __CreatureTrainer__Trainer__

#include <stdio.h>
#include <string>
#include <sstream>
#include <vector>
#include "class1.h"
#include "class2.h"
#include "Class3.h"
using namespace std;

class Trainer {
public:
	Trainer();
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  returns 'a' or 's#' given the situation.
                 This is where the bulk of your code will go. The implementation
                 of your AI goes here.
     */
    string makeMove(stringstream& situation);

    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Prints out the final situation
     */
    void finalSituation(stringstream& situation);
    
    
    /**
     * Requires: Nothing.
     * Modifies: Nothing.
     * Effects:  Splits a string into a vector of strings
     */
    vector<string> splitString(string inString, string splitOn=" ");

private:
    // vvvvvv Your changes Trainer.h go between this line…
    CreatureDex myCreatureDex;
	MyParty myParty;
	ActualCreature enemyCreature;
	int winCount;
	int moveCount;
	int consecutiveSwaps;

    void printBattleInfo();
	bool inArray(int x, int arr[], int size);
	bool willWin(ActualCreature attacker, ActualCreature victim, bool swappedIn);
	int findClincher(bool hasFainted);
	int calcDamage(ActualCreature attacker, ActualCreature victim);
    int decideSwap(bool hasFainted);
    
    // ^^^^^^ And this line. That's it!
    
};

#endif /* defined(__CreatureTrainer__Trainer__) */
