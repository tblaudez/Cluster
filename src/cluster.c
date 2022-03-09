#include <stdlib.h>
#include <string.h>
#include "cluster.h"
#include <bsd/stdlib.h>

t_hex **gameGrid = NULL;
t_gravity gravity = BOTTOM;

int main() {
	gameGrid = createNewBoard();

	for (int i = 0; i < HEX_ON_GRID / 2; i++) {
		insertToken((int)arc4random_uniform(MAX_LINE - 1), i % 4);
	}

	displayArray();

	for (int i = 0; i < 6; i++) {
		rotateGameGrid(1);
		displayArray();
	}

	return EXIT_SUCCESS;
}
