//
//  Class1.h
//  Class1
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#ifndef __CreatureTrainer__Class1__
#define __CreatureTrainer__Class1__
#include <string>
using namespace std;
// vvvvvv Your implementations go between this line…
class DexEntry{
	public:
		DexEntry();
		int name;
		int attackType;
		int strengthType;
		int weaknessType;
		int neutralTypes[6];
		int neutralTypesSize;
		int baseAttack;
		int baseHP;
		int restHPGain;
		int attackScaling;
		int HPScaling;
};

class CreatureDex{
	public:
		CreatureDex();
		void printDex();
		int findCreatureName(string name);
		int findElementName(string name);
		DexEntry creatureDex[26];	
};




// ^^^^^^ And this line. That's it!

#endif /* defined(__CreatureTrainer__Class1__) */
