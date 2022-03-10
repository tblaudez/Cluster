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
				fputs("\033[31m X \033[0m", stdout);
			else printf("\033[%dm%2d \033[0m", gameData.gameGrid[r][q] + 32, gameData.gameGrid[r][q]);
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

char *getColorString(t_color color) {
	static char *colors[4] = {"BLUE", "CYAN", "RED", "ORANGE"};
	return colors[color - 1];
}

char *getPlayerString(t_player player) {
	static char *players[2] = {"PLAYER ONE", "PLAYER TWO"};
	return players[player];
}

size_t getRemainingTokens(t_player player) {
	if (player == PLAYER_ONE)
		return gameData.tokens[BLUE] + gameData.tokens[CYAN];
	else if (player == PLAYER_TWO)
		return gameData.tokens[RED] + gameData.tokens[ORANGE];
	__builtin_unreachable();
}

void freeMemory(void) {
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

t_player getPlayerForHex(t_hex hex)
{
	if (hex == 0)
		return NONE;
	return (hex - 1) / 2;
}