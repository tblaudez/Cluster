//
// Created by tblaudez on 3/8/22.
//

#include <stdio.h>
#include "cluster.h"
#include <stdnoreturn.h>

void displayArray(void) {
	for (int r = 0; r < MAX_LINE; r++) {
		for (int q = 0; q < MAX_LINE; q++) {
			if (!IS_VALID(q, r))
				fputs(" X ", stdout);
			else printf("%2d ", gameGrid[r][q]);
		}
		putc('\n', stdout);
	}
	putc('\n', stdout);

}

noreturn void illegalInstruction(void) {
	fputs("ILLEGAL INSTRUCTION\n", stderr);
	exit(EXIT_FAILURE);
}

t_hex **createNewBoard(void) {
	t_hex **board = calloc(MAX_LINE, sizeof(t_hex *));
	for (int i = 0; i < MAX_LINE; i++)
		board[i] = calloc(MAX_LINE, sizeof(t_hex));

	return board;
}

int getNonEmptyHexagonsTopDown(int q, t_hex buffer[MAX_LINE]) {
	int offset = 0;

	for (int r = 0; r < MAX_LINE; r++) {
		if (!IS_VALID(q, r))
			continue;
		if (gameGrid[r][q] != EMPTY)
			buffer[offset++] = gameGrid[r][q];
	}

	return offset;
}

int getNonEmptyHexagonsLeftRight(int r, t_hex buffer[MAX_LINE]) {
	int offset = 0;

	for (int q = 0; q < MAX_LINE; q++) {
		if (!IS_VALID(q, r))
			continue;
		if (gameGrid[r][q] != EMPTY)
			buffer[offset++] = gameGrid[r][q];
	}

	return offset;
}

int getNonEmptyHexagonsDiagonalRight(int q, int r, t_hex buffer[MAX_LINE]) {
	int offset = 0;

	if (!IS_VALID(r, q))
		return 0;

	while (q < MAX_LINE && r >= 0)
		buffer[offset++] = gameGrid[r--][q++];

	return offset;
}

void swapInt(int *a, int *b) {
	int tmp = *a;
	*a = *b;
	*b = tmp;
}