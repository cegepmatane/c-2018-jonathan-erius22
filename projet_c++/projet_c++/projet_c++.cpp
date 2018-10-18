// projet_c++.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include "pch.h"
#include <iostream>
#include <iostream>
#include <fstream>
#include "Character/Character.h"
#include "Character/Barbarian.h"
#include "Character/Mage.h"
#include "Character/Archer.h"
#include "spell/Spell.h"
#include "spell/DancingLight.h"
#include "spell/smash.h"
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include "conio.h"
using namespace std;


int main() {

	enum keyConfiguration : int {exit = 'q'};

	Spell *spell = new Spell("Spell", 50);
	Spell *dancingLight = new DancingLight();
	Spell *smash = new Smash();

	vector<Character*> listCharacter;



	//import init

	ifstream sourcePersonnages;
	sourcePersonnages.open("../data/Personnages.csv");
	string ligne;

	int indexPersonnage = 0;
	bool firstLine = true;
	while (!sourcePersonnages.eof())
	{
		if (firstLine)
		{
			getline(sourcePersonnages, ligne);
			firstLine = false;
			cout << "FIRST" << endl;

		}

		else
		{
			getline(sourcePersonnages, ligne);

			unsigned int positionDebut = 0;
			unsigned int positionFin = 0; //unsigned int fait en sorte que -1 donne la valeur maximum d'un int
			int indexColum = 0;

			do {
				positionFin = ligne.find(";", positionDebut);


				string value = ligne.substr(positionDebut, positionFin - positionDebut);
				//cout << value << endl;

				switch (indexColum) {

				case 0:

					if (value == "Barbarian")
					{
						listCharacter.push_back(new Barbarian());
					}
					else if (value == "Mage") {
						listCharacter.push_back(new Mage());
					}
					else if (value == "Archer") {
						listCharacter.push_back(new Archer());
					}


					break;

				case 1:
					listCharacter[indexPersonnage]->setName(value);
					break;

				case 2:
					listCharacter[indexPersonnage]->setHp(strtof((value).c_str(), 0));
					break;

				case 3:
					listCharacter[indexPersonnage]->setDamageBasicAttaque(strtof((value).c_str(), 0));
					break;

				case 4:

					if (value == "Smash")
					{
						listCharacter[indexPersonnage]->addSpell(smash);
					}
					else if (value == "DancingLight")
					{
						listCharacter[indexPersonnage]->addSpell(dancingLight);
					}

					break;

				default:

					break;

				}

				positionDebut = positionFin + 1;
				indexColum++;
			} while (positionDebut != 0);

			indexPersonnage++;
		}// fin boucle ligne


	}//fin lecture document


	//fin import init





	//creation donne test

	Character *barbarian = new Barbarian();
	barbarian->setHp(100);
	barbarian->setName("Theo");

	Character *mage = new Mage();
	mage->setHp(50);
	mage->setName("Bob");
	mage->addSpell(dancingLight);
	mage->addSpell(spell);

	//fin creation donne test


	//test interaction
	cout << barbarian->exporter() << endl;
	cout << mage->exporter() << endl;

	cout << "barbarian attack mage for " << barbarian->getDamageBasicAttaque() << endl;
	barbarian->attackTarget(*mage);
	cout << mage->exporter() << endl;

	cout << "mage launch spell Dancing light to barbarian for " << mage->findSpell(dancingLight->getName()).getDamage() << endl;

	mage->launchSpellAtTarget(*barbarian, mage->findSpell(dancingLight->getName()));
	cout << barbarian->exporter() << endl;
	//fin test interaction



	//exportation fichier

	string const nomFichier("../data/Export.xml");
	ofstream monFlux(nomFichier.c_str());

	if (monFlux)
	{
		monFlux << barbarian->exporter() << endl;
		monFlux << mage->exporter() << endl;

		for (unsigned int i = 0; i < listCharacter.size(); i++)
		{
			monFlux << listCharacter[i]->exporter() << endl;
		}
	}
	else
	{
		cout << "ERREUR: Impossible d'ouvrir le fichier." << endl;
	}

	//fin exportation


	//debut boucle de jeu

	int frame = 0;
	int key;
	bool gameIsRunning = true;

	while (gameIsRunning)
	{
		// PREMIER PROBLEME-SOLUTION : dormir sans occuper les ressources
		this_thread::sleep_for(chrono::milliseconds(1000 / 60));
		// SECOND PROBLEME-SOLUTION : lire les entrées sans bloquer la boucle
		if (_kbhit())
		{
			key = _getch();
			cout << "key " << key << " " << frame << endl;
			switch (key)
			{
			case keyConfiguration::exit :
				gameIsRunning = false;
				break;
			}
		}
		while (_kbhit()) _getch();


		frame++;
	}

	//fin boucle de jeu


	delete barbarian;
	delete mage;

	for (int i = 0; i < listCharacter.size(); i++)
	{
		delete listCharacter[i];
	}

	delete spell;
	delete dancingLight;

	return 0;
}