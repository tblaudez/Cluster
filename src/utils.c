//
// Created by tblaudez on 3/8/22.
//

#include <stdio.h>
#include "cluster.h"
#include <stdnoreturn.h>
#include <stdbool.h>

void displayArray(void) {
	static char *colors[5] = {"\033[0m", "\033[34m", "\033[36m", "\033[31m", "\033[33m"};

	for (int r = 0; r < gameData.maxLine; r++) {
		for (int q = 0; q < gameData.maxLine; q++) {
			if (!areCoordinatesValid(q, r))
				printf("%s X %s", "\033[30m", colors[EMPTY]);
			else printf("%s%2d %s", colors[gameData.gameGrid[r][q]], gameData.gameGrid[r][q], colors[EMPTY]);
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
	while (q < gameData.maxLine && r >= 0) {
		if (gameData.gameGrid[r][q] != EMPTY)
			buffer[offset++] = gameData.gameGrid[r][q];
		r--;
		q++;
	}

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

char *getColorString(t_color color) {
	static char *colors[5] = {"(NONE)", "BLUE", "CYAN", "RED", "ORANGE"};
	return colors[color];
}

char *getPlayerString(t_player player) {
	static char *players[2] = {"PLAYER ONE", "PLAYER TWO"};
	return players[player];
}

size_t inline getRemainingTokens(t_player player) {
	size_t offset = player * 2 + 1;
	return gameData.tokens[offset] + gameData.tokens[offset + 1];
}

char *getGravityString(void) {
	static char *gravity[6] = {"BOTTOM", "BOTTOM RIGHT", "TOP RIGHT", "TOP", "TOP LEFT", "BOTTOM LEFT"};
	return gravity[gameData.gravity];
}

void freeMemory(void) {
	if (gameData.window_size > 0)
		mlx_terminate(gameData.mlx);

	for (int i = 0; i < gameData.maxLine; i++)
		free(gameData.gameGrid[i]);
	free(gameData.gameGrid);
	free(gameData.buffer);
}

noreturn void playerWins(t_player player) {
	printf("%s WINS !!\n", getPlayerString(player));
	exit(EXIT_SUCCESS);
}

t_player getPlayerForHex(t_hex hex) {
	if (hex == 0)
		return NONE;
	return (hex - 1) / 2;
}