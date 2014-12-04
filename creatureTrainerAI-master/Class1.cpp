//  Class1.cpp
//  Helper Class1
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#include "Class1.h"
#include <fstream>
using namespace std;

DexEntry::DexEntry(){
	name = -1;
	attackType = -1;
	strengthType = -1;
	weaknessType = -1;
	for (int i = 0; i < 6; i++){
		neutralTypes[i] = -1;
	}
	neutralTypesSize = 0;
	baseAttack = -1;
	baseHP = -1;
	restHPGain = -1;
	attackScaling = -1;
	HPScaling = -1;
}

const string CREATURE_NAMES[] = { "Axolotyl", "Bittern", "C-lacanth",
		"Dugong", "Echidna", "Fossa", "Guanaco", "Honeybdgr", "Ibex", "Jackal",
		"Kiwi", "Loris", "Megapode", "Narwhal", "Ovenbird", "Pika", "Quagga",
		"RockHyrax", "Solenodon", "Tuatara", "Uromastyx", "Vaquita", "Wolverine",
		"Xenops", "Yaminon", "Zebu" };

const string ELEMENT_NAMES[] = { "Air", "Bombast", "Cool", "Dark",
		"Earth", "Funk", "Gamma", "Hammer" };

const int NUM_CREATURES = 26;
const int NUM_ELEMENTS = 8;

CreatureDex::CreatureDex(){
	for (int i = 0; i < NUM_CREATURES; i++){
		creatureDex[i].name = i;
	}
}

void CreatureDex::printDex(){
	ofstream fout;
	fout.open("creatureDex.txt");
	for (int i = 0; i < NUM_CREATURES; i++){
		string attackTypeName;
		string strengthTypeName;
		string weaknessTypeName;

		if (creatureDex[i].attackType == -1){
			attackTypeName = "Unknown";
		}
		else{
			attackTypeName = ELEMENT_NAMES[creatureDex[i].attackType];
		}
		if (creatureDex[i].strengthType == -1){
			strengthTypeName = "Unknown";
		}
		else{
			strengthTypeName = ELEMENT_NAMES[creatureDex[i].strengthType];
		}
		if (creatureDex[i].weaknessType == -1){
			weaknessTypeName = "Unknown";
		}
		else{
			weaknessTypeName = ELEMENT_NAMES[creatureDex[i].weaknessType];
		}

		fout << CREATURE_NAMES[creatureDex[i].name]   << endl
			 << "Attack Type: "   << attackTypeName   << endl
			 << "Strength Type: " << strengthTypeName << endl
			 << "Weakness Type: " << weaknessTypeName << endl
			 << "Neutral Types: ";

		for (int j = 0; j < creatureDex[i].neutralTypesSize; j++){
			fout << ELEMENT_NAMES[creatureDex[i].neutralTypes[j]] << " ";
		}
		fout << endl;
		fout << "Base Attack: " << creatureDex[i].baseAttack << endl
			 << "Base HP: " << creatureDex[i].baseHP << endl
			 << "Rest HP Gain: " << creatureDex[i].restHPGain << endl;
		fout << endl;
	}
	fout.close();
}

int CreatureDex::findCreatureName(string name){
	for (int i = 0; i < NUM_CREATURES; i++){
		if (name == CREATURE_NAMES[i]){
			return i;
		}
	}
	return -1;
}

int CreatureDex::findElementName(string name){
	for (int i = 0; i < NUM_ELEMENTS; i++){
		if (name == ELEMENT_NAMES[i]){
			return i;
		}
	}
	return -1;
}

