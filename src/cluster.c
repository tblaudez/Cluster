#include <stdlib.h>
#include <string.h>
#include "cluster.h"
#include <stdlib.h>
#include <stdio.h>
#include <getopt.h>
#include <unistd.h>

t_data gameData = {
		.gameGrid = NULL,
		.buffer = NULL,
		.gravity = BOTTOM,
		.gridSize = 5,
		.minimalConnect = 4,
};

static void parse_arguments(int argc, char **argv) {
	int ch;

	while ((ch = getopt(argc, argv, "s:c:h")) >= 0) {
		switch (ch) {
			case 's':
				if ((gameData.gridSize = atoi(optarg)) <= 1) {
					fprintf(stderr, "%s: bad grid size\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;
			case 'c':
				if ((gameData.minimalConnect = atoi(optarg)) <= 2) {
					fprintf(stderr, "%s: bad connect size\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;
			case 'h':
				fprintf(stdout, "Usage: %s [-h] [-s size] [-c connect]\n", argv[0]);
				exit(EXIT_SUCCESS);
			default:
				break;
		}
	}
}


static void initGameData(void) {
	gameData.maxLine = gameData.gridSize * 2 - 1;
	gameData.hexOnGrid = (1 + 6 * ((gameData.gridSize * (gameData.gridSize - 1)) / 2));

	gameData.tokenPerPlayer = gameData.hexOnGrid / 2;
	gameData.tokenPerColor = gameData.tokenPerPlayer / 2;
	for (int i = 0; i < 5; i++)
		gameData.tokens[i] = gameData.tokenPerColor;

	gameData.gameGrid = createNewBoard();
	gameData.buffer = calloc(1, gameData.maxLine * sizeof(t_hex));
}

static void initMLX()
{
	gameData.mlx = mlx_init(1024, 1024, "Cluster", false);
	gameData.img = mlx_new_image(gameData.mlx, gameData.mlx->width, gameData.mlx->height);
	gameData.img_index = mlx_image_to_window(gameData.mlx, gameData.img, 0, 0);

	DrawHexagons(gameData.img, 0);
}

static void mlx_hook(void* _)
{
	static float angle = 0;
	angle += (float)gameData.mlx->delta_time * 0.5f;
	DrawHexagons(gameData.img, angle);
}

static t_color *getTokens(t_player player) {
	static t_color colors[2];

	if (player == PLAYER_ONE) {
		if (gameData.tokens[BLUE] == 0 && gameData.tokens[CYAN] == 0) {
			playerWins(PLAYER_TWO);
			__builtin_unreachable();
		}
		if (gameData.tokens[BLUE] == 0 || gameData.tokens[CYAN] == 0)
			memset(colors, gameData.tokens[BLUE] == 0 ? CYAN : BLUE, 2);
		else {
			colors[0] = arc4random_uniform(2) + 1;
			colors[1] = arc4random_uniform(2) + 1;
		}
	} else if (player == PLAYER_TWO) {
		if (gameData.tokens[RED] == 0 && gameData.tokens[ORANGE] == 0) {
			playerWins(PLAYER_ONE);
			__builtin_unreachable();
		}
		if (gameData.tokens[RED] == 0 || gameData.tokens[ORANGE] == 0)
			memset(colors, gameData.tokens[RED] == 0 ? ORANGE : RED, 2);
		else {
			colors[0] = arc4random_uniform(2) + 3;
			colors[1] = arc4random_uniform(2) + 3;
		}
	}

	return colors;
}

int main(int argc, char **argv) {
	char input[INPUT_SIZE + 1];
	ssize_t nbyte;

	atexit(freeMemory);

	parse_arguments(argc, argv);
	initGameData();
	
	/*
	initMLX();

	mlx_loop_hook(gameData.mlx, mlx_hook, NULL);
	mlx_loop(gameData.mlx);
	*/
	
	displayArray();

	for (int i = 0; i < 1000; i++) {
		t_player player = i % 2;
		t_color *availableColors = getTokens(player);
		printf("Turn %d | %s | Available colors : %s & %s | Remaining tokens : %zu\n", i + 1,
			   getPlayerString(player),
			   getColorString(availableColors[0]), getColorString(availableColors[1]),
			   getRemainingTokens(player));
		fflush(stdout);

		/*if ((nbyte = read(STDIN_FILENO, input, INPUT_SIZE)) <= 0) {
			fputs("Error while reading standard input\n", stderr);
			exit(EXIT_FAILURE);
		}
		input[nbyte - 1] = '\0';*/

		if (arc4random_uniform(2) == 0)
			insertToken((int) arc4random_uniform(gameData.maxLine), availableColors[arc4random_uniform(2)]);
		else rotateGameGrid((int) arc4random_uniform(6));

		/*if (!strncmp(input, "PLAY ", 5)) {
			int row = atoi(input + 5);
			if (row < 1 || row >= gameData.maxLine)
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

		player = getWinner();
		if (player != NONE) {
			playerWins(player);
			__builtin_unreachable();
		}
	}

	return EXIT_SUCCESS;
}
