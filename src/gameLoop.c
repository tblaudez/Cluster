//
// Created by tblaudez on 3/10/22.
//

#include "cluster.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#ifndef __APPLE__

#include <bsd/stdlib.h>
#include <stdnoreturn.h>

#else
#include <stdlib.h>
#endif


static t_color *getTokens(t_player player) {
	static t_color colors[2] = {EMPTY};
	size_t offset = player * 2 + 1;


	if (gameData.tokens[offset] == 0 && gameData.tokens[offset + 1] == 0)
		playerWins(!player);
	else if (gameData.tokens[offset] == 0) {
		colors[0] = offset + 1;
		colors[1] = EMPTY;
	} else if (gameData.tokens[offset+1] == 0) {
		colors[0] = offset;
		colors[1] = EMPTY;
	} else {
		colors[0] = arc4random_uniform(2) + offset;
		colors[1] = arc4random_uniform(2) + offset;
	}

	return colors;
}

void gameStep(int i) {
	t_player player = i % 2;
	t_color *availableColors = getTokens(player);

	system("clear");
	printf("Turn %d | %s | Available colors : %s & %s | Remaining tokens : %zu | Gravity : %s\n", i + 1,
		   getPlayerString(player), getColorString(availableColors[0]), getColorString(availableColors[1]),
		   getRemainingTokens(player), getGravityString());
	fflush(stdout);

	if (arc4random_uniform(10) < 9) {
		t_color color = availableColors[0];
		int row = (int) arc4random_uniform(gameData.maxLine);
		while (!insertToken(row, color))
			row = (int) arc4random_uniform(gameData.maxLine);

		printf("ACTION -> PLAY %d %s\n", row + 1, getColorString(color));
	} else {
		int amount = (int) arc4random_uniform(6);
		printf("ACTION -> ROTATE %d\n", amount);
		rotateGameGrid(amount);
	}

	/*char input[INPUT_SIZE + 1];
	ssize_t nbyte;

	if ((nbyte = read(STDIN_FILENO, input, INPUT_SIZE)) <= 0) {
		fputs("Error while reading standard input\n", stderr);
		exit(EXIT_FAILURE);
	}
	input[nbyte - 1] = '\0';

	if (!strncmp(input, "PLAY ", 5)) {
		int row = atoi(input + 5);
		if (row < 1 || row > gameData.maxLine)
			illegalInstruction();
		row--;
		if (player == PLAYER_ONE && !strcmp(input + 7, "BLUE") &&
			(availableColors[0] == BLUE || availableColors[1] == BLUE))
			insertToken(row, BLUE);
		else if (player == PLAYER_ONE && !strcmp(input + 7, "CYAN") &&
					(availableColors[0] == CYAN || availableColors[1] == CYAN))
			insertToken(row, CYAN);
		else if (player == PLAYER_TWO && !strcmp(input + 7, "RED") &&
					(availableColors[0] == RED || availableColors[1] == RED))
			insertToken(row, RED);
		else if (player == PLAYER_TWO && !strcmp(input + 7, "ORANGE") &&
					(availableColors[0] == ORANGE || availableColors[1] == ORANGE))
			insertToken(row, ORANGE);
		else illegalInstruction();
	} else if (!strncmp(input, "ROTATE ", 7)) {
		int amount = atoi(input + 7);
		gameData.tokens[availableColors[0]]--;
		gameData.tokens[availableColors[1]]--;
		rotateGameGrid(amount);
	} else illegalInstruction();*/

	displayArray();

	if ((player = getWinner()) != NONE)
		playerWins(player);
}

noreturn void gameLoop() {
	for (int i = 0;; i++) {
		gameStep(i);
		usleep(8000);
	}
}
