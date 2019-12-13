#include "Labyrinth.h"

int main() {
	//seed the random
	srand(time(0));
	ifstream f("G://files/Lab.txt");
	vector<Labyrinth> labs;
	while (f.peek() != EOF) {
		Labyrinth lab(f);
		if (lab.isValid()) {
			labs.push_back(lab);
		}
	}
	sort(labs.begin(), labs.end(), compareLabyrinths);
	Player * p = nullptr;
	char hero;
	cout << "Please choose a hero! ('s' for succubus, 'm' for mage)" << endl;
	do {
		cin >> hero;
	} while (hero != 's' && hero != 'm');
	switch (hero) {
	case 's':
		p = new Succubus;
		break;
	case 'm':
		p = new Mage;
		break;
	}
	for (int i = 0; i < labs.size(); i++) {
		labs[i].printLab();
		unsigned blockedCount;
		do {
			cout << "Enter a number of positions to block! (upper bound: free spaces - number of monsters - 2)" << endl;
			cin >> blockedCount;
		} while (blockedCount > labs[i].getFreeBlocks() - labs[i].getMonstCount() - 2);
		if (blockedCount > 0) {
			Labyrinth temp;
			do {
				temp = labs[i];
				cout << "Enter " << blockedCount << " pairs of y-x coordinates corresponding to blocked squares. Do not close off your exit!" << endl;
				for (int j = 0; j < blockedCount; j++) {
					unsigned y, x;
					cin >> y >> x;
					if (!temp.blockSquare(y, x)) {
						cout << "That is not a valid position to block!" << endl;
						j--;
					}
				}
			} while (!temp.isValidAfterBlock());
			labs[i] = temp;
		}
		labs[i].addMonsters();
		if (!p->canPassMaze(labs[i])) {
			cout << "Game over! Failed to pass this labyrinth.." << endl;
			return 0;
		}
	}
	cout << "Congratulations, you have passed all labyrinths! You win!";
	return 0;
}