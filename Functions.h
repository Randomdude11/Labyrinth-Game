#include <stdlib.h>     /* srand, rand */
#include <time.h>

unsigned randUns(unsigned s, unsigned e) {
	return rand() % (e - s) + s;
}
