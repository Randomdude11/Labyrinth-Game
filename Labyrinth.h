#include "Functions.h"
#include "Block.h"
#include <fstream>
#include <queue>
#include <stack>
using namespace std;

class Labyrinth {
	friend class Mage;
	friend class Succubus;
	unsigned N, M, monstCount, freeBlocks = 0;
	Block* monsters;
	Block** pos;
public:
	Labyrinth();
	Labyrinth(ifstream &);
	Labyrinth(const Labyrinth&);
	Labyrinth& operator=(const Labyrinth&);
	~Labyrinth();
	bool operator<(const Labyrinth&) const;
	void copyLab(const Labyrinth&);
	void deleteLab();
	void inputLab(ifstream&);
	void printLab() const;
	bool isValid() const;
	bool isValidAfterBlock() const;
	bool BFScheck() const;
	unsigned getSize() const;
	unsigned getMonstCount() const;
	unsigned getFreeBlocks() const;
	bool blockSquare(unsigned, unsigned);
	void addMonsters();
	bool moveMonstersEatPlayer(const Block &);
};

Labyrinth::Labyrinth() : monstCount(0), N(0), M(0), pos(nullptr), monsters(nullptr) {}
Labyrinth::Labyrinth(ifstream & f) {
	inputLab(f);
}
Labyrinth::Labyrinth(const Labyrinth& other) {
	copyLab(other);
}
Labyrinth& Labyrinth::operator=(const Labyrinth &other) {
	deleteLab();
	copyLab(other);
	return *this;
}
Labyrinth::~Labyrinth() {
	deleteLab();
}
void Labyrinth::copyLab(const Labyrinth &other) {
	if (this != &other) {
		N = other.N;
		M = other.M;
		monstCount = other.monstCount;
		freeBlocks = other.freeBlocks;
		pos = new Block*[N];
		for (int i = 0; i < N; i++) {
			pos[i] = new Block[M];
			for (int j = 0; j < M; j++) {
				pos[i][j] = Block(other.pos[i][j].sym, i, j);
			}
		}
		monsters = new Block[monstCount];
		for (int i = 0; i < monstCount; i++) {
			monsters[i] = other.monsters[i];
		}
	}
}
void Labyrinth::deleteLab() {
	if (pos != nullptr) {
		for (int i = 0; i < N; i++) {
			delete[] pos[i];
		}
		delete[] pos;
		delete[] monsters;
		pos = nullptr;
		monsters = nullptr;
	}
}
void Labyrinth::inputLab(ifstream &f) {
	f >> N >> M;
	pos = new Block*[N];
	for (int i = 0; i < N; i++) {
		pos[i] = new Block[M];
		for (int j = 0; j < M; j++) {
			char tpos;
			f >> tpos;
			pos[i][j] = Block(tpos, i, j);
			if (tpos == '.') {
				freeBlocks++;
			}
		}
	}
	f >> monstCount;
	monsters = new Block[monstCount];
}
void Labyrinth::printLab() const {
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cout << pos[i][j];
		}
		cout << endl;
	}
	cout << "Number of monsters in this labyrinth: " << monstCount << endl;
}
bool Labyrinth::isValid() const {
	if (pos[0][0] == '#' || pos[N - 1][M - 1] == '#') {
		return false;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (pos[i][j] != '.' && pos[i][j] != '#') {
				return false;
			}
		}
	}
	if (!BFScheck()) {
		return false;
	}
	return true;
}
bool Labyrinth::isValidAfterBlock() const {
	if (pos[0][0] == '#' || pos[N - 1][M - 1] == '#') {
		return false;
	}
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			if (pos[i][j] != '.' && pos[i][j] != '#' && pos[i][j] != '!') {
				return false;
			}
		}
	}
	if (!BFScheck()) {
		return false;
	}
	return true;
}
bool Labyrinth::BFScheck() const {
	Labyrinth temp = *this;
	queue<Block> open;
	open.push(pos[0][0]);
	while (!open.empty()) {
		Block b = open.front();
		if ((b.y + 1 == N - 1 && b.x == M - 1) || (b.y == N - 1 && b.x + 1 == M - 1)) {
			return true;
		}
		open.pop();
		if (b.x > 0 && temp.pos[b.y][b.x - 1] == '.') {
			open.push(temp.pos[b.y][b.x - 1]);
			temp.pos[b.y][b.x - 1] = '*';
		}
		if (b.x < M - 1 && temp.pos[b.y][b.x + 1] == '.') {
			open.push(temp.pos[b.y][b.x + 1]);
			temp.pos[b.y][b.x + 1] = '*';
		}
		if (b.y > 0 && temp.pos[b.y - 1][b.x] == '.') {
			open.push(temp.pos[b.y - 1][b.x]);
			temp.pos[b.y - 1][b.x] = '*';
		}
		if (b.y < N - 1 && temp.pos[b.y + 1][b.x] == '.') {
			open.push(temp.pos[b.y + 1][b.x]);
			temp.pos[b.y + 1][b.x] = '*';
		}
	}
	return false;
}
unsigned Labyrinth::getSize() const {
	return N * M;
}
unsigned Labyrinth::getMonstCount() const {
	return monstCount;
}
unsigned Labyrinth::getFreeBlocks() const {
	return freeBlocks;
}
bool Labyrinth::operator<(const Labyrinth& other) const {
	return getSize() < other.getSize();
}
bool Labyrinth::blockSquare(unsigned y, unsigned x) {
	if (pos[y][x].sym == '.') {
		pos[y][x].sym = '!';
		return true;
	}
	return false;
}
void Labyrinth::addMonsters() {
	for (int i = 0; i < monstCount; i++) {
		monsters[i].sym = 'u';
		unsigned y, x;
		do {
			x = randUns(0, M);
			y = randUns(0, N);
		} while ((y == 0 && x == 0) || (y == N - 1 && x == M - 1) || pos[y][x] != '.');
		monsters[i].x = x;
		monsters[i].y = y;
	}
}
bool Labyrinth::moveMonstersEatPlayer(const Block &b) {
	for (int i = 0; i < monstCount; i++) {
		if ((monsters[i].x == 0 || pos[monsters[i].y][monsters[i].x - 1] == '#') && (monsters[i].x == M - 1 || pos[monsters[i].y][monsters[i].x + 1] == '#') && (monsters[i].y == 0 || pos[monsters[i].y - 1][monsters[i].x] == '#') && (monsters[i].y == N - 1 || pos[monsters[i].y + 1][monsters[i].x] == '#')) {
			continue;
		}
		bool movement = false;
		while (!movement) {
			if (monsters[i].sym == 'u') {
				if (monsters[i].y > 0 && pos[monsters[i].y - 1][monsters[i].x] != '#') {
					monsters[i].y--;
					movement = true;
				}
				else {
					monsters[i].sym = 'l';
				}
			}
			if (monsters[i].sym == 'l') {
				if (monsters[i].x > 0 && pos[monsters[i].y][monsters[i].x - 1] != '#') {
					monsters[i].x--;
					movement = true;
				}
				else {
					monsters[i].sym = 'd';
				}
			}
			if (monsters[i].sym == 'd') {
				if (monsters[i].y < N - 1 && pos[monsters[i].y + 1][monsters[i].x] != '#') {
					monsters[i].y++;
					movement = true;
				}
				else {
					monsters[i].sym = 'r';
				}
			}
			if (monsters[i].sym == 'r') {
				if (monsters[i].x < M - 1 && pos[monsters[i].y][monsters[i].x + 1] != '#') {
					monsters[i].x++;
					movement = true;
				}
				else {
					monsters[i].sym = 'u';
				}
			}
		}
		if (monsters[i].x == b.x && monsters[i].y == b.y) {
			return true;
		}
	}
	return false;
}
bool compareLabyrinths(Labyrinth &v1, Labyrinth &v2) {
	if (v1.getSize() != v2.getSize()) {
		return v1 < v2;
	}
	else {
		return  v1.getMonstCount() < v2.getMonstCount();
	}
}

class Player {
protected:
	unsigned x, y;
public:
	virtual bool canPassMaze(Labyrinth&) = 0;
};

class Mage : public Player {
public:
	virtual bool canPassMaze(Labyrinth &);
};

bool Mage::canPassMaze(Labyrinth &lab) {
	stack<Block> open;
	Block temp = lab.pos[0][0];
	lab.pos[0][0] = '*';
	if (lab.pos[1][0] == '.') {
		open.push(lab.pos[1][0]);
		lab.pos[1][0] = '*';
	}
	if (lab.pos[0][1] == '.') {
		open.push(lab.pos[0][1]);
		lab.pos[0][1] = '*';
	}
	while (open.top().x != lab.M - 1 || open.top().y != lab.N - 1) {
		Block temp = open.top();
		open.pop();
		x = temp.x;
		y = temp.y;
		if (x != 0 && lab.pos[y][x - 1] == '.') {
			open.push(lab.pos[y][x - 1]);
			lab.pos[y][x - 1] = '*';
		}
		if (y != 0 && lab.pos[y - 1][x] == '.') {
			open.push(lab.pos[y - 1][x]);
			lab.pos[y - 1][x] = '*';
		}
		if (x != lab.M - 1 && lab.pos[y][x + 1] == '.') {
			open.push(lab.pos[y][x + 1]);
			lab.pos[y][x + 1] = '*';
		}
		if (y != lab.N - 1 && lab.pos[y + 1][x] == '.') {
			open.push(lab.pos[y + 1][x]);
			lab.pos[y + 1][x] = '*';
		}
		if (lab.moveMonstersEatPlayer(temp)) {
			return false;
		}
		/*Labyrinth l(lab);
		for (int i = 0; i < l.monstCount; i++) {
			l.pos[l.monsters[i].y][l.monsters[i].x] = '@';
		}
		l.pos[y][x] = '%';
		l.printLab();*/
	}
	return true;
}
class Succubus : public Player {
private:
	list<Block> path;
public:
	virtual bool canPassMaze(Labyrinth &);
	void findPath(Labyrinth);
};

void Succubus::findPath(Labyrinth lab) {
	queue<BFSblock> open;
	open.push(BFSblock(0,0));
	while (open.front().x != lab.M - 1 || open.front().y != lab.N - 1) {
		BFSblock b = open.front();
		open.pop();
		if (b.x > 0 && lab.pos[b.y][b.x - 1] == '.') {
			open.push(BFSblock(b, b.y, b.x-1));
			lab.pos[b.y][b.x - 1] = '*';
		}
		if (b.x < lab.M - 1 && lab.pos[b.y][b.x + 1] == '.') {
			open.push(BFSblock(b, b.y, b.x+1));
			lab.pos[b.y][b.x + 1] = '*';
		}
		if (b.y > 0 && lab.pos[b.y - 1][b.x] == '.') {
			open.push(BFSblock(b, b.y-1, b.x));
			lab.pos[b.y - 1][b.x] = '*';
		}
		if (b.y < lab.N - 1 && lab.pos[b.y + 1][b.x] == '.') {
			open.push(BFSblock(b, b.y+1, b.x));
			lab.pos[b.y + 1][b.x] = '*';
		}
	}
	path = open.front().history;
	path.push_back(open.front());
}
bool Succubus::canPassMaze(Labyrinth &lab) {
	findPath(lab);
	list<Block>::iterator i = path.begin();
	++i;
	while (!lab.moveMonstersEatPlayer(*i)) {
		/*Labyrinth l(lab);
		for (int i = 0; i < l.monstCount; i++) {
			l.pos[l.monsters[i].y][l.monsters[i].x] = '@';
		}
		l.pos[i->y][i->x] = '%';
		l.printLab();*/
		++i;
		if (i->y == lab.N-1 && i->x == lab.M-1) {
			return true;
		}
	}
	return false;
}
