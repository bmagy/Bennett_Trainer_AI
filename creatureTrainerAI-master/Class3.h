//
//  Class3.h
//  Class3
//
//  These class heaaders are for use in your Trainer class. If you wish to 
//  write helper classes, write them in these 5 classes. 
//  Use of these classes is optional. You may use none or any or all of them. 
//  You still may NOT #include any.h files we provide you
//  (Battle, Creature, CreatureType, EECSRandom, Party, PrintHelper)
//

#ifndef __CreatureTrainer__Class3__
#define __CreatureTrainer__Class3__

// vvvvvv Your implementations go between this line�
class ActualCreature{
	public:
		ActualCreature();
		int name;
		int currentHP;
		int damageTaken;
		bool isAlive;
};

class MyParty{
	public:
		MyParty();
		int findAlive();
		int countAlive();
		int findLargestHP();

		ActualCreature party[4];
		int currentCreatureIndex;
		int partySize;
};





// ^^^^^^ And this line. That's it!

#endif /* defined(__CreatureTrainer__Class3__) */
