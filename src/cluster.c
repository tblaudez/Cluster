#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <getopt.h>
#include "cluster.h"
#include "math.h"

mlx_errno_t mlx_errno;    // WHY

t_data gameData = {
		.gameGrid = NULL,
		.buffer = NULL,
		.gravity = BOTTOM,
		.gridSize = 5,
		.minimalConnect = 4,
		.window_size = 0
};

static void parse_arguments(int argc, char **argv) {
	int ch;

	while ((ch = getopt(argc, argv, "s:c:d:h")) >= 0) {
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
			case 'd':
				if ((gameData.window_size = atoi(optarg)) <= 0) {
					fprintf(stderr, "%s: bad window size\n", argv[0]);
					exit(EXIT_FAILURE);
				}
				break;
			case 'h':
				fprintf(stdout, "Usage: %s [-h] [-s size] [-c connect] [-d display size]\n", argv[0]);
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

static void initMLX() {
	gameData.mlx = mlx_init(gameData.window_size, gameData.window_size, "Cluster", false);
	gameData.img = mlx_new_image(gameData.mlx, gameData.mlx->width, gameData.mlx->height);
	gameData.img_index = mlx_image_to_window(gameData.mlx, gameData.img, 0, 0);
	DrawHexagons(gameData.img, GetBaseAngle());
}

static void mlx_hook(void *_) {
	static float angle = 0;
	static int i = 0;
	static float time = 0;

	float targetAngle = GetBaseAngle();

	float diff = targetAngle - angle;
	if (diff > M_PI) diff -= M_PI * 2;
	else if (diff < -M_PI) diff += M_PI * 2;

	angle += diff * MIN(gameData.mlx->delta_time * 7, 1);

	time -= gameData.mlx->delta_time;
	if (time < 0) {
		time += 0.2f;
		gameStep(i++);
	}

	DrawHexagons(gameData.img, angle);
}

int main(int argc, char **argv) {
	atexit(freeMemory);

	parse_arguments(argc, argv);
	initGameData();
	displayArray();

	if (gameData.window_size > 0) {
		initMLX();
		mlx_loop_hook(gameData.mlx, mlx_hook, NULL);
		mlx_loop(gameData.mlx);
	} else gameLoop();
}
