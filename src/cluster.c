#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "cluster.h"
#include "MLX42/MLX42.h"

t_hex **gameGrid = NULL;

/* Applies gravity to gameboard by making hexagons fall down.
 * The trick consists of duplicating a column, removing blank hexagons
 * and copy the duplicate from bottom to top
 * */
void gravityHappens(void) {
//	We loop over every column
	for (int q = 0; q < MAX_LINE; q++) {
		t_hex column[MAX_LINE] = {EMPTY};
		int offset = 0, offset2 = 0;

//		We loop over every hexagon of the current column from the bottom to the top
//		For every non-empty hexagon, we push it to `column` and make the hexagon empty
		for (int r = MAX_LINE - 1; r >= 0; r--) {
			if (gameGrid[r][q] != EMPTY) {
				column[offset++] = gameGrid[r][q];
				gameGrid[r][q] = EMPTY;
			}
		}

//		Now we copy `column` in the current column from the bottom to the top
		for (int r = MAX_LINE - 1; offset2 < offset; r--) {
			if (!IS_VALID(q, r))
				continue;
			gameGrid[r][q] = column[offset2++];
		}
	}
}

/* Insert token `token` on top of column `q` */
void insertToken(int q, t_hex token) {
//	Check if column is valid
	if (q < 0 || q >= MAX_LINE) {
		fputs("ILLEGAL INSTRUCTION\n", stderr);
		return;
	}

//	Check if column is full by looping to the first valid coordinates
	for (int r = 0; r < MAX_LINE; r++) {
		if (!IS_VALID(q, r))
			continue;
		if (gameGrid[r][q] != EMPTY) {
			fputs("ILLEGAL INSTRUCTION\n", stderr);
			return;
		} else break;
	}

//	Place a token on top of column `q`
//	We don't care if the coordinates are invalid since
//	gravity will make it fall down and we know the column isn't full
	gameGrid[0][q] = token;
	gravityHappens();
}

int main() {
//	 Allocate a MAX_LINE x MAX_LINE zero-initialized 2D array
	gameGrid = calloc(MAX_LINE, sizeof(t_hex *));
	for (int i = 0; i < MAX_LINE; i++)
		gameGrid[i] = calloc(MAX_LINE, sizeof(t_hex));

	insertToken(0, 1);
	insertToken(0, 1);
	insertToken(0, 1);

	displayArray();
	return EXIT_SUCCESS;
}
