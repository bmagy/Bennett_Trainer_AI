//
//  Trainer.cpp
//  CreatureTrainer
//
//  You may NOT #include any .h files we provide you (Battle, Creature, CreatureType,
//  EECSRandom, Party, PrintHelper)
//  other than the ones name "ClassX.h" and "Trainer.h"
//  You may #include other <> libraries if you wish to
//
//  Created by Jeremy Gibson on 11/4/14.
//  Copyright (c) 2014 Jeremy Richard Gibson. All rights reserved.
//

#include <iostream>
#include <sstream>
#include <fstream>
#include "Trainer.h"
#include "Class1.h"
#include "Class2.h"
#include "Class3.h"
#include "Class4.h"
#include "Class5.h"

//#include "CreatureType.h"

using namespace std;

const string CREATURE_NAMES[] = { "Axolotyl", "Bittern", "C-lacanth",
		"Dugong", "Echidna", "Fossa", "Guanaco", "Honeybdgr", "Ibex", "Jackal",
		"Kiwi", "Loris", "Megapode", "Narwhal", "Ovenbird", "Pika", "Quagga",
		"RockHyrax", "Solenodon", "Tuatara", "Uromastyx", "Vaquita", "Wolverine",
		"Xenops", "Yaminon", "Zebu" };

const string ELEMENT_NAMES[] = { "Air", "Bombast", "Cool", "Dark",
		"Earth", "Funk", "Gamma", "Hammer" };

const int NUM_CREATURES = 26;
const int NUM_ELEMENTS = 8;

Trainer::Trainer(){
	winCount = 0;
	moveCount = 0;
	consecutiveSwaps = 0;
}

string Trainer::makeMove(stringstream& situation) {
    // pull the entire string out of stringStream& situation
    string situationString = situation.str();
    // situationString now includes ALL of the lines that will be sent to cout
    
    // Here's how to use the stringSplit function
    vector<string> lines = splitString(situationString, "\n");
    // This splits situationString into a vector of individual lines.
    // (it splits on the new line character "\n")
    // To access the 3rd line (as in 0, 1, 2, 3), you would use bracket access
    //   as in: lines[3]
    

    // This loop will iterate through all the lines
    int pipeLine = -1;
	int enemyIntroLine = -1;
	int myCreatureAttackLine = -1;
	int enemyCreatureAttackLine = -1;

	bool myCreatureFainted = false;
	bool enemyCreatureFainted = false;
    // lines.size() tells us how many elements are in the vector<string> lines
    // In this case, it tells us how many lines we pulled in
    for (int i=0; i<lines.size(); i++) {
        // Store each line in the string line
        string line = lines[i];
        // Store the first char of line in the char c
        char c = line[0];

		stringstream ss;
		string firstWord;
		string secondWord;
		string thirdWord;

		ss << line;
		ss >> firstWord >> secondWord >>thirdWord;
		if (firstWord == "Your"){
			if (myCreatureDex.findCreatureName(secondWord) != -1){
				if (thirdWord != "faints!"){
					myCreatureAttackLine = i;
				}
				else{
					myCreatureFainted = true;
				}
			}
		}
		else if (firstWord == "Enemy"){
			if (myCreatureDex.findCreatureName(secondWord) != -1){
				if (thirdWord != "faints!"){
					enemyCreatureAttackLine = i;
				}
				else{
					enemyCreatureFainted = true;
					winCount++;
				}
			}
		}
		else if (firstWord == "It's"){
			if (secondWord == "an"){
				if (thirdWord == "enemy"){
					enemyIntroLine = i;
				}
			}
		}
        // If c is a pipe ('|'), then this is the line that tells us about our
        //    party and the health of each Creature we own.
        if (c == '|') {
            // Store which line number this is
            pipeLine = i;
        }
    }
    
    // Now that we know which line has the health, do something with it
    //cout << lines[pipeLine] << "\n"; // Output for testing only
    
    // Split the party health line by pipes "|"
    vector<string> creatureHealthBits = splitString(lines[pipeLine], "|");
	
    // This gives us something like the vector description below
    /* { "",
         " *Jackal     10/10",
         " Yaminon    17/17",
         " Megapode   13/13",
         " Jackal     10/10",
         "" } */
    // You can see that the first and last pipe caused empty strings
    
    // Now, go through this information and pull out info for each entry
    // We do 1 to <creatureHealthBits.size()-1 because we only need the middle
    //    four elements (and are skipping the empty ones.
    for (int i=1; i<creatureHealthBits.size()-1; i++) {
        // cout just for testing
        //cout << creatureHealthBits[i] << "\n";
        
        // We are putting the string into a stringStream so we can use >>
        stringstream ss;
        ss << creatureHealthBits[i];
        
        // For example, we have a stringstream ss containing " *Jackal   10/10 "
        // pull the name as a string
        string name;
        ss >> name; // name = "*Jackal"
		if (name[0] == '*'){
			myParty.currentCreatureIndex = i - 1;
			string nameTemp = name;
			name = "";
			for (int j = 1; j < nameTemp.length(); j++){
				name += nameTemp[j];
			}
		}
		ActualCreature& consideredCreature = myParty.party[i-1];
		if (consideredCreature.name == -1){
			consideredCreature.name = myCreatureDex.findCreatureName(name);
		}
        // pull the health
        int health;
        ss >> health; // health = 10
		int previousHealth = consideredCreature.currentHP;
		consideredCreature.currentHP = health;
		if (consideredCreature.currentHP == 0){
			consideredCreature.isAlive = false;
		}
		//adjusting the listed rest
		int& listedRestHPGain = myCreatureDex.creatureDex[consideredCreature.name].restHPGain;

		if (previousHealth != -1){
			int healthChange = health - previousHealth;
			if (healthChange > listedRestHPGain && healthChange < 6 && healthChange > 0){
				listedRestHPGain = healthChange;
			}
		}
		

        // Get rid of the slash char
        char slash;
        ss >> slash; // slash = '/'
        
        // get max health
        int maxHealth;
        ss >> maxHealth; // maxHealth = 10
		if (myCreatureDex.creatureDex[consideredCreature.name].baseHP == -1){
			myCreatureDex.creatureDex[consideredCreature.name].baseHP = maxHealth;
		}
	
	}

	ActualCreature& currentCreature = myParty.party[myParty.currentCreatureIndex];

     if (myCreatureAttackLine != -1){
		stringstream ss;
		ss << lines[myCreatureAttackLine];
		string junk;
		string myCreatureName;
		string enemyCreatureName;
		string myAttackName;
		int myDamage;
		ss >> junk >> myCreatureName >> junk >> junk >> enemyCreatureName 
		   >> junk >> myAttackName >> junk >> myDamage;
		
		if (enemyCreature.name == -1){
			enemyCreature.name =  myCreatureDex.findCreatureName(enemyCreatureName);
		}

		DexEntry& enemyCreatureDexEntry = myCreatureDex.creatureDex[enemyCreature.name];
		DexEntry& myCreatureDexEntry =  myCreatureDex.creatureDex[currentCreature.name];
		int& myListedAttackType = myCreatureDexEntry.attackType;
		int& myListedBaseAttack = myCreatureDexEntry.baseAttack;

		if (myListedAttackType == -1){
			myListedAttackType = myCreatureDex.findElementName(myAttackName);
		}
		if (!enemyCreatureFainted){
			if (myDamage < 4){
				if (myListedBaseAttack == -1){
					myListedBaseAttack = 2*myDamage;
				}
				if (enemyCreatureDexEntry.strengthType == -1){
					enemyCreatureDexEntry.strengthType = myListedAttackType;
				}
			}
			else if(myDamage > 6){
				if (myListedBaseAttack == -1){
					myListedBaseAttack = myDamage / 2;
				}
				if (enemyCreatureDexEntry.weaknessType == -1){
					enemyCreatureDexEntry.weaknessType = myListedAttackType;
				}
			}
			else{
				if (myListedBaseAttack == -1){
					myListedBaseAttack = myDamage;
				}
				if (!inArray(myListedAttackType, enemyCreatureDexEntry.neutralTypes, enemyCreatureDexEntry.neutralTypesSize)){
					enemyCreatureDexEntry.neutralTypes[enemyCreatureDexEntry.neutralTypesSize] = myListedAttackType;
					enemyCreatureDexEntry.neutralTypesSize++;
				}
			}
		}
		enemyCreature.damageTaken += myDamage;
		if (enemyCreatureFainted){
			enemyCreatureDexEntry.baseHP = enemyCreature.damageTaken;
			enemyCreature.name = -1;
			enemyCreature.damageTaken = 0;
		}
		if (enemyCreature.currentHP != -1){
			enemyCreature.currentHP -= myDamage;
		}
	}

	if (enemyCreatureAttackLine != -1){
		stringstream ss;
		ss << lines[enemyCreatureAttackLine];
		string junk;
		string enemyCreatureName;
		string myCreatureName;
		string enemyAttackName;
		int enemyDamage;
		ss >> junk >> enemyCreatureName >> junk >> junk >> myCreatureName 
		   >> junk >> enemyAttackName >> junk >> enemyDamage;

		DexEntry& enemyCreatureDexEntry = myCreatureDex.creatureDex[enemyCreature.name];
		DexEntry& myCreatureDexEntry =  myCreatureDex.creatureDex[currentCreature.name];
		int& enemyListedAttackType = enemyCreatureDexEntry.attackType;
		int& enemyListedBaseAttack = enemyCreatureDexEntry.baseAttack;

		if (enemyListedAttackType == -1){
			enemyListedAttackType = myCreatureDex.findElementName(enemyAttackName);
		}
		if (!myCreatureFainted){
			if (enemyDamage < 4){
				if (enemyListedBaseAttack == -1){
					enemyListedBaseAttack = 2*enemyDamage;
				}
				if (myCreatureDexEntry.strengthType == -1){
					myCreatureDexEntry.strengthType = enemyListedAttackType;
				}
			}
			else if(enemyDamage > 6){
				if (enemyListedBaseAttack == -1){
					enemyListedBaseAttack = enemyDamage / 2;
				}
				if (myCreatureDexEntry.weaknessType == -1){
					myCreatureDexEntry.weaknessType = enemyListedAttackType;
				}
			}
			else{
				if (enemyListedBaseAttack == -1){
					enemyListedBaseAttack = enemyDamage;
				}
				if (!inArray(enemyListedAttackType, myCreatureDexEntry.neutralTypes, myCreatureDexEntry.neutralTypesSize)){
					myCreatureDexEntry.neutralTypes[myCreatureDexEntry.neutralTypesSize] = enemyListedAttackType;
					myCreatureDexEntry.neutralTypesSize++;
				}
			}
		}
    }

	if (enemyIntroLine != -1){
		stringstream ss;
		ss << lines[enemyIntroLine];
		string junk;
		string enemyCreatureNameTemp;
		string enemyCreatureName;
		ss >> junk >> junk >> junk >> enemyCreatureNameTemp;
		for (int j = 0; j < enemyCreatureNameTemp.length()-1; j++){
			enemyCreatureName += enemyCreatureNameTemp[j];
		}
		if (enemyCreature.name == -1){
			enemyCreature.name = myCreatureDex.findCreatureName(enemyCreatureName);
		}
		enemyCreature.currentHP = myCreatureDex.creatureDex[enemyCreature.name].baseHP;
	}
    
    
/*
    // This makes ct a copy of the CreatureType for Creature 0 (the Axolotyl).
    // All CreatureType and Element names start with a different letter of the
    //    alphabet (CreatureType is A-Z, Element is A-H).
    CreatureType ct = CreatureType::TYPES[0];
    // You can then get the type of ct, which is 0 because it's the Axolotyl
    int type = ct.getType();
    // You can also directly get info from CreatureTypes::TYPES elements.
    int elementalWeakness3 = CreatureType::TYPES[3].getElementalWeakness();
*/
    
    // Output for human player
    // Instead of doing this, you will need to replace the code of Trainer 
    // to parse the input situation and create a proper response,
    // (like "a" for attack or "s3" to swap to the creature in slot 3).
    cout << situationString;
    
	string swaps[4] = {"s1", "s2", "s3", "s4"};
	string response = "a";

	myCreatureDex.printDex();
	printBattleInfo();


	int clincherIndex = findClincher(myCreatureFainted);
	if (willWin(currentCreature, enemyCreature, false) && winCount > 30){
		response = "a";
	}
	else if (clincherIndex != -1 && clincherIndex != myParty.currentCreatureIndex && winCount > 30){
		int swapIndex = clincherIndex;
		if (0 <= swapIndex && swapIndex < myParty.partySize && consecutiveSwaps < 5){
			response = swaps[swapIndex];
		}
		else{
			response == "a";
		}
	}
	else{
		if (currentCreature.currentHP > calcDamage(enemyCreature, currentCreature)){
			response = "a";
		}
		else{
			int swapIndex = myParty.findLargestHP();
			if (swapIndex != myParty.currentCreatureIndex){
				if (0 <= swapIndex && swapIndex < myParty.partySize && consecutiveSwaps < 5){
					response = swaps[swapIndex];
				}
				else{
					response = "a";
				}
			}
			else{
				response = "a";
			}
		}
	}
	/*
	else if (currentCreature.currentHP > calcDamage(enemyCreature, currentCreature)){
		response = "a";
	}
	else{
		int swapIndex = decideSwap(myCreatureFainted);
		if (0 <= swapIndex && swapIndex < myParty.partySize && consecutiveSwaps < 5){
			response = swaps[swapIndex];
		}
		else{
			response = "a";
		}
	}
	*/	
	moveCount++;

	if (response == "a"){
		consecutiveSwaps = 0;
	}
	else if (response[0] == 's'){
		consecutiveSwaps++;
	}

	if (winCount == 50){
		cout << "stop!" << endl;
	}

    return response;
}


void Trainer::finalSituation(stringstream& situation) {
    cout << situation.str();
    
}



vector<string> Trainer::splitString(string inString, string splitOn) {
    vector<string> elements;
    string substring;
    
    size_t splitIndex = inString.find(splitOn),
    substrStart = 0;
    
    // keep searching until no spaces remain
    while (splitIndex != string::npos) {
        substring = inString.substr(substrStart, splitIndex - substrStart);
        elements.push_back(substring);
        substrStart = splitIndex + 1;
        // start at next character after finding substring
        splitIndex = inString.find(splitOn, substrStart);
    }
    // add last element to vector
    substring = inString.substr(substrStart, inString.length() - substrStart);
    elements.push_back(substring);
    
    return elements;
}

bool Trainer::inArray(int x, int arr[], int size){
	for (int i = 0; i < size; i++){
		if (arr[i] == x){
			return true;
		}
	}
	return false;
}

void Trainer::printBattleInfo(){
	ofstream fout;
	fout.open("BattleInfo.txt");
	fout << "Win Count: " << winCount << endl;
	fout << "Move Count: " << moveCount << endl;
	for (int i = 0; i < myParty.partySize; i++){
		ActualCreature& creatureTemp = myParty.party[i];
		DexEntry creatureTempDexEntry = myCreatureDex.creatureDex[creatureTemp.name];
		string nameTemp;
		if (creatureTemp.name == -1){
			nameTemp = "Unknown";
		}
		else{
			nameTemp = CREATURE_NAMES[creatureTemp.name];
		}
		fout << i + 1 << ". ";
		if (i == myParty.currentCreatureIndex){
			fout << "*";
		}
		fout << nameTemp << endl;
		fout << "Current HP: " << creatureTemp.currentHP << "/" 
			 << creatureTempDexEntry.baseHP << endl;
		
		string attackTypeName;
		string strengthTypeName;
		string weaknessTypeName;

		if (creatureTempDexEntry.attackType == -1){
			attackTypeName = "Unknown";
		}
		else{
			attackTypeName = ELEMENT_NAMES[creatureTempDexEntry.attackType];
		}
		if (creatureTempDexEntry.strengthType == -1){
			strengthTypeName = "Unknown";
		}
		else{
			strengthTypeName = ELEMENT_NAMES[creatureTempDexEntry.strengthType];
		}
		if (creatureTempDexEntry.weaknessType == -1){
			weaknessTypeName = "Unknown";
		}
		else{
			weaknessTypeName = ELEMENT_NAMES[creatureTempDexEntry.weaknessType];
		}

		fout << CREATURE_NAMES[creatureTempDexEntry.name]   << endl
			 << "Attack Type: "   << attackTypeName   << endl
			 << "Strength Type: " << strengthTypeName << endl
			 << "Weakness Type: " << weaknessTypeName << endl
			 << "Neutral Types: ";

		for (int j = 0; j < creatureTempDexEntry.neutralTypesSize; j++){
			fout << ELEMENT_NAMES[creatureTempDexEntry.neutralTypes[j]] << " ";
		}
		fout << endl;
		fout << "Calculated Damage (sent): " << calcDamage(creatureTemp, enemyCreature) << endl;
		fout << "Calculated Damage (received): " << calcDamage(enemyCreature, creatureTemp) << endl;
		fout << "Base Attack: " << creatureTempDexEntry.baseAttack << endl
			 << "Base HP: " << creatureTempDexEntry.baseHP << endl
			 << "Rest HP Gain: " << creatureTempDexEntry.restHPGain << endl;
		fout << endl;
	}
	
	string nameTemp;
	if (enemyCreature.name == -1){
		nameTemp = "Unknown";
	}
	else{
		nameTemp = CREATURE_NAMES[enemyCreature.name];
	}
	DexEntry enemyCreatureDexEntry = myCreatureDex.creatureDex[enemyCreature.name];
	fout << "Enemy: "         << nameTemp << endl
		 << "Damage Taken: " << enemyCreature.damageTaken << endl;
	fout << "Current HP: " << enemyCreature.currentHP << "/" 
			 << enemyCreatureDexEntry.baseHP << endl;
	
	string attackTypeName;
	string strengthTypeName;
	string weaknessTypeName;

	if (enemyCreatureDexEntry.attackType == -1){
		attackTypeName = "Unknown";
	}
	else{
		attackTypeName = ELEMENT_NAMES[enemyCreatureDexEntry.attackType];
	}
	if (enemyCreatureDexEntry.strengthType == -1){
		strengthTypeName = "Unknown";
	}
	else{
		strengthTypeName = ELEMENT_NAMES[enemyCreatureDexEntry.strengthType];
	}
	if (enemyCreatureDexEntry.weaknessType == -1){
		weaknessTypeName = "Unknown";
	}
	else{
		weaknessTypeName = ELEMENT_NAMES[enemyCreatureDexEntry.weaknessType];
	}
	fout << CREATURE_NAMES[enemyCreatureDexEntry.name]   << endl
		 << "Attack Type: "   << attackTypeName   << endl
		 << "Strength Type: " << strengthTypeName << endl
		 << "Weakness Type: " << weaknessTypeName << endl
		 << "Neutral Types: ";
	
	for (int j = 0; j < enemyCreatureDexEntry.neutralTypesSize; j++){
		fout << ELEMENT_NAMES[enemyCreatureDexEntry.neutralTypes[j]] << " ";
	}
	fout << endl;
	fout << "Base Attack: " << enemyCreatureDexEntry.baseAttack << endl
			 << "Base HP: " << enemyCreatureDexEntry.baseHP << endl
			 << "Rest HP Gain: " << enemyCreatureDexEntry.restHPGain << endl;
	fout.close();
}

int Trainer::calcDamage(ActualCreature attacker, ActualCreature victim){
	int damageDefault = 5;
	if (attacker.name == -1 || victim.name == -1){
		return damageDefault;
	}
	DexEntry attackerDexEntry = myCreatureDex.creatureDex[attacker.name];
	DexEntry victimDexEntry   = myCreatureDex.creatureDex[victim.name];

	int attackerAttackType = attackerDexEntry.attackType;
	int attackerBaseAttack = attackerDexEntry.baseAttack;
	int victimStrength   = victimDexEntry.strengthType;
	int victimWeakness   = victimDexEntry.weaknessType;

	if (attackerAttackType == victimWeakness && attackerAttackType != -1){
		if (attackerBaseAttack != -1){
			return 2*attackerBaseAttack;
		}
		else{
			return 2*damageDefault;
		}
	}
	else if(attackerAttackType == victimStrength && attackerAttackType != -1){
		if (attackerBaseAttack != -1){
			return attackerBaseAttack/2;
		}
		else{
			return damageDefault/2;
		}
	}
	else{
		if (inArray(attackerBaseAttack, victimDexEntry.neutralTypes, 
			victimDexEntry.neutralTypesSize)){
				return attackerBaseAttack;
		}
		else if (attackerBaseAttack != -1){
			return attackerBaseAttack;
		}
		else{
			return damageDefault;
		}
	}
}

bool Trainer::willWin(ActualCreature myCreature, ActualCreature theirCreature, bool swappedIn){
	int defaultHP = 15;
	int myDamage    = calcDamage(myCreature, theirCreature);
	int theirDamage = calcDamage(theirCreature, myCreature);
	int myHP    = myCreature.currentHP;

	if (swappedIn){
		myHP -= theirDamage;
	}
	int theirHP = theirCreature.currentHP;
	if (theirHP == -1){
		theirHP = defaultHP - theirCreature.damageTaken;
	}

	int myTurnsToKill = theirHP/myDamage;
	if (theirHP % myDamage != 0){
		myTurnsToKill++;
	}

	int theirTurnsToKill = myHP/theirDamage;
	if (myHP % theirDamage != 0){
		theirTurnsToKill++;
	}

	if (myTurnsToKill <= theirTurnsToKill){
		return true;
	}
	else{
		return false;
	}
}

int Trainer::decideSwap(bool hasFainted){
	int clincherIndex = findClincher(hasFainted);
	if (clincherIndex != -1){
		return clincherIndex;
	}
	return myParty.findLargestHP();
}

int Trainer::findClincher(bool hasFainted){
	for (int i = 0; i < myParty.partySize; i++){
		if (willWin(myParty.party[i], enemyCreature, !hasFainted)){
			return i;
		}
	}
	return -1;
}

	/*
	DexEntry enemyCreatureDexEntry = myCreatureDex.creatureDex[enemyCreature.name];
	DexEntry partyCreatureDexEntries[4];

	int scoreArray[4] = {0, 0, 0, 0};
	int maxHPIndex = myParty.findLargestHP();
	scoreArray[maxHPIndex] += 1;

	for (int i = 0; i < myParty.partySize; i++){
		partyCreatureDexEntries[i] = myCreatureDex.creatureDex[myParty.party[i].name];
	}
	if (enemyCreatureDexEntry.attackType != -1){
		for (int i = 0; i < myParty.partySize; i++){
			if (partyCreatureDexEntries[i].strengthType == enemyCreatureDexEntry.attackType){
				if (i != myParty.currentCreatureIndex){
					scoreArray[i] += 2;
				}
			}
			if (partyCreatureDexEntries[i].weaknessType == enemyCreatureDexEntry.attackType){
				if (i != myParty.currentCreatureIndex){
					scoreArray[i] -= 2;
				}
			}
		}
	}
	if (enemyCreatureDexEntry.weaknessType != -1){
		for (int i = 0; i < myParty.partySize; i++){
			if (partyCreatureDexEntries[i].attackType == enemyCreatureDexEntry.weaknessType){
				if (i != myParty.currentCreatureIndex){
					scoreArray[i] += 2;
				}
			}
		}
	}
	if (enemyCreatureDexEntry.strengthType != -1){
		for (int i = 0; i < myParty.partySize; i++){
			if (partyCreatureDexEntries[i].attackType == enemyCreatureDexEntry.strengthType){
				if (i != myParty.currentCreatureIndex){
					scoreArray[i] -= 2;
				}
			}
		}
	}
	*/
	