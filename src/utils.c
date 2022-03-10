//
// Created by tblaudez on 3/8/22.
//

#include <stdio.h>
#include "cluster.h"
#include <stdnoreturn.h>
#include <stdbool.h>

void displayArray(void) {
	for (int r = 0; r < gameData.maxLine; r++) {
		for (int q = 0; q < gameData.maxLine; q++) {
			if (!areCoordinatesValid(q, r))
				fputs(" X ", stdout);
			else printf("%2d ", gameData.gameGrid[r][q]);
		}
		putc('\n', stdout);
	}
	putc('\n', stdout);
}

noreturn inline void illegalInstruction(void) {
	fputs("ILLEGAL INSTRUCTION\n", stderr);
	exit(EXIT_FAILURE);
}

t_hex **createNewBoard(void) {
	t_hex **board = calloc(gameData.maxLine, sizeof(t_hex *));
	for (int i = 0; i < gameData.maxLine; i++)
		board[i] = calloc(gameData.maxLine, sizeof(t_hex));

	return board;
}

void deleteGameBoard(t_hex **gameBoard) {
	for (int i = 0; i < gameData.maxLine; i++)
		free(gameBoard[i]);
	free(gameBoard);
}

size_t getNonEmptyHexagonsTopDown(int q, t_hex buffer[gameData.maxLine]) {
	size_t offset = 0;

	for (int r = 0; r < gameData.maxLine; r++) {
		if (!areCoordinatesValid(q, r))
			continue;
		if (gameData.gameGrid[r][q] != EMPTY)
			buffer[offset++] = gameData.gameGrid[r][q];
	}

	return offset;
}

size_t getNonEmptyHexagonsLeftRight(int r, t_hex buffer[gameData.maxLine]) {
	register size_t offset = 0;

	for (int q = 0; q < gameData.maxLine; q++) {
		if (!areCoordinatesValid(q, r))
			continue;
		if (gameData.gameGrid[r][q] != EMPTY)
			buffer[offset++] = gameData.gameGrid[r][q];
	}

	return offset;
}

size_t getNonEmptyHexagonsDiagonalRight(int q, int r, t_hex buffer[gameData.maxLine]) {
	if (!areCoordinatesValid(q, r))
		return 0;

	register size_t offset = 0;
	while (q < gameData.maxLine && r >= 0)
		buffer[offset++] = gameData.gameGrid[r--][q++];

	return offset;
}

inline void swapInt(int *a, int *b) {
	register int tmp = *a;
	*a = *b;
	*b = tmp;
}

inline bool areCoordinatesValid(int q, int r) {
	register int sum = q + r;
	return (sum >= gameData.gridSize - 1) && (sum < gameData.maxLine * 2 - gameData.gridSize);
}

t_player getPlayerForHex(t_hex hex)
{
	if (hex == 0)
		return NONE;
	return (hex - 1) / 2;
}