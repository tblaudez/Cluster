//
// Created by tblaudez on 3/8/22.
//

#include <stdio.h>
#include "cluster.h"

void displayArray(void) {
	printf("Grid Size : %d\nHexagons on grid : %d\nMax line : %d\n\n", GRID_SIZE, HEX_ON_GRID, MAX_LINE);

	for (int r = 0; r < MAX_LINE; r++) {
		for (int q = 0; q < MAX_LINE; q++) {
			if (!IS_VALID(q, r))
				fputs("X\t", stdout);
			else printf("%d\t", gameGrid[r][q]);
		}
		putc('\n', stdout);
	}
}