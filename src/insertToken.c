//
// Created by tblaudez on 3/9/22.
//

#include <stdbool.h>
#include <stdio.h>
#include "cluster.h"

static bool insertTokenBottom(int q, t_hex token) {
	for (int r = (int)gameData.maxLine - 1; r >= 0; r--) {
		if (!areCoordinatesValid(q, r) || gameData.gameGrid[r][q] != EMPTY)
			continue;
		gameData.gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static bool insertTokenTop(int q, t_hex token) {
	for (int r = 0; r < gameData.maxLine; r++) {
		if (!areCoordinatesValid(q, r) || gameData.gameGrid[r][q] != EMPTY)
			continue;
		gameData.gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static bool insertTokenBottomRight(int r, t_hex token) {
	for (int q = (int)gameData.maxLine - 1; q >= 0; q--) {
		if (!areCoordinatesValid(q, r) || gameData.gameGrid[r][q] != EMPTY)
			continue;
		gameData.gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static bool insertTokenTopLeft(int r, t_hex token) {
	for (int q = 0; q < gameData.maxLine; q++) {
		if (!areCoordinatesValid(q, r) || gameData.gameGrid[r][q] != EMPTY)
			continue;
		gameData.gameGrid[r][q] = token;
		return true;
	}

	return false;
}

static void insertTokenBottomLeft(int q, int r, t_hex token) {
	if (!areCoordinatesValid(q, r) || gameData.gameGrid[r][q] != EMPTY)
		illegalInstruction();

	while (r < gameData.maxLine && q >= 0 && gameData.gameGrid[r++][q--] == EMPTY);
	gameData.gameGrid[r - 1][q + 1] = token;
}

static void insertTokenTopRight(int q, int r, t_hex token) {
	if (!areCoordinatesValid(q, r) || gameData.gameGrid[r][q] != EMPTY)
		illegalInstruction();

	while (r >= 0 && q < gameData.maxLine && gameData.gameGrid[r--][q++] == EMPTY);
	gameData.gameGrid[r + 1][q - 1] = token;
}

void insertToken(int index, t_hex token) {
	bool valid = true;

	if (gameData.tokens[token] == 0)
		illegalInstruction();
	else gameData.tokens[token] -= 1;

	switch (gameData.gravity) {
		case BOTTOM:
			valid = insertTokenBottom(index, token);
			break;
		case BOTTOM_RIGHT:
			valid = insertTokenBottomRight(index, token);
			break;
		case TOP_RIGHT: {
			int q = MAX(0, gameData.gridSize - 1 - index);
			int r = MIN(gameData.maxLine - 1, gameData.maxLine - 1 + gameData.gridSize - 1 - index);
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
			int q = MIN(gameData.gridSize - 1 + index, gameData.maxLine - 1);
			int r = MAX(0, index - (gameData.gridSize - 1));
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