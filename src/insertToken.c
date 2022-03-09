//
// Created by tblaudez on 3/9/22.
//

#include <stdbool.h>
#include <stdio.h>
#include "cluster.h"

static bool insertTokenBottom(int q, t_hex token) {
	for (int r = MAX_LINE - 1; r >= 0; r--) {
		if (!IS_VALID(q, r) || gameGrid[r][q] != EMPTY)
			continue;
		gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static bool insertTokenTop(int q, t_hex token) {
	for (int r = 0; r < MAX_LINE; r++) {
		if (!IS_VALID(q, r) || gameGrid[r][q] != EMPTY)
			continue;
		gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static bool insertTokenBottomRight(int r, t_hex token) {
	for (int q = MAX_LINE - 1; q >= 0; q--) {
		if (!IS_VALID(q, r) || gameGrid[r][q] != EMPTY)
			continue;
		gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static bool insertTokenTopLeft(int r, t_hex token) {
	for (int q = 0; q < MAX_LINE; q++) {
		if (!IS_VALID(q, r) || gameGrid[r][q] != EMPTY)
			continue;
		gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static void insertTokenBottomLeft(int q, int r, t_hex token) {
	if (!IS_VALID(q, r) || gameGrid[r][q] != EMPTY)
		illegalInstruction();

	while (r < MAX_LINE && q >= 0 && gameGrid[r++][q--] == EMPTY);
	gameGrid[r - 1][q + 1] = token;
}

static void insertTokenTopRight(int q, int r, t_hex token) {
	if (!IS_VALID(q, r) || gameGrid[r][q] != EMPTY)
		illegalInstruction();

	while (r >= 0 && q < MAX_LINE && gameGrid[r--][q++] == EMPTY);
	gameGrid[r + 1][q - 1] = token;
}

void insertToken(int index, t_hex token) {
	bool valid = true;

	switch (gravity) {
		case BOTTOM:
			valid = insertTokenBottom(index, token);
			break;
		case BOTTOM_RIGHT:
			valid = insertTokenBottomRight(index, token);
			break;
		case TOP_RIGHT: {
			int q = MAX(0, GRID_SIZE - 1 - index);
			int r = MIN(MAX_LINE - 1, MAX_LINE - 1 + GRID_SIZE - 1 - index);
			insertTokenTopRight(q, r, token);
			break;
		}
		case TOP:
			valid = insertTokenTop(index, token);
			break;
		case TOP_LEFT:
			valid = insertTokenTopLeft(index, token);
			break;
		case BOTTOM_LEFT: {
			int q = MIN(GRID_SIZE - 1 + index, MAX_LINE - 1);
			int r = MAX(0, index - (GRID_SIZE - 1));
			insertTokenBottomLeft(q, r, token);
			break;
		}
		default:
			fputs("Gravity is messed up !\n", stderr);
			break;
	}

	if (!valid)
		illegalInstruction();
}