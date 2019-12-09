#include <iostream>
#include <list>
using namespace std;

struct Block {
	char sym;
	unsigned x;
	unsigned y;
	Block(char tsym = NULL, unsigned ty = NULL, unsigned tx = NULL) : sym(tsym), y(ty), x(tx) {}
	Block(unsigned ty, unsigned tx) : sym(NULL), y(ty), x(tx) {}
	bool operator==(char t) {
		if (sym == t) {
			return true;
		}
		return false;
	}
	bool operator!=(char t) {
		if (sym == t) {
			return false;
		}
		return true;
	}
	friend ostream& operator<<(ostream& os, const Block &b);
};
ostream& operator<<(ostream& os, const Block &b) {
	os << b.sym;
	return os;
}