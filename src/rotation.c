//
// Created by tblaudez on 3/9/22.
//

#include <string.h>
#include "cluster.h"

static void gravityBottom(void) {
	for (int q = 0; q < gameData.maxLine; q++) {
		size_t size = getNonEmptyHexagonsTopDown(q, gameData.buffer);
		for (int r = (int)gameData.maxLine - 1; r >= 0; r--) {
			if (!areCoordinatesValid(q, r))
				continue;
			gameData.gameGrid[r][q] = size ? gameData.buffer[--size] : EMPTY;
		}
	}

	gameData.gravity = BOTTOM;
}

static void gravityBottomRight(void) {
	for (int r = 0; r < gameData.maxLine; r++) {
		size_t size = getNonEmptyHexagonsLeftRight(r, gameData.buffer);
		for (int q = (int)gameData.maxLine - 1; q >= 0; q--) {
			if (!areCoordinatesValid(q, r))
				continue;
			gameData.gameGrid[r][q] = size ? gameData.buffer[--size] : EMPTY;
		}
	}

	gameData.gravity = BOTTOM_RIGHT;
}

static void gravityTopRightSlide(int q, int r) {
	size_t size = getNonEmptyHexagonsDiagonalRight(q, r, gameData.buffer);
	swapInt(&r, &q);
	while (q >= 0 && r < gameData.maxLine)
		gameData.gameGrid[r++][q--] = size ? gameData.buffer[--size] : EMPTY;
}

static void gravityTopRight(void) {
	for (int r = 0, q = 0; r < gameData.maxLine; r++) {
		if (!areCoordinatesValid(q, r))
			continue;
		gravityTopRightSlide(q, r);
	}

	bzero(gameData.buffer, gameData.maxLine);

	for (int r = (int)gameData.maxLine - 1, q = 1; q < gameData.maxLine; q++) {
		if (!areCoordinatesValid(q, r))
			continue;
		gravityTopRightSlide(q, r);
	}

	gameData.gravity = TOP_RIGHT;
}

static void gravityTop(void) {
	for (int q = 0; q < gameData.maxLine; q++) {
		size_t size = getNonEmptyHexagonsTopDown(q, gameData.buffer);
		for (int r = 0, offset = 0; r < gameData.maxLine; r++) {
			if (!areCoordinatesValid(q, r))
				continue;
			gameData.gameGrid[r][q] = offset < size ? gameData.buffer[offset++] : EMPTY;
		}
	}

	gameData.gravity = TOP;
}

static void gravityTopLeft(void) {
	for (int r = 0; r < gameData.maxLine; r++) {
		size_t size = getNonEmptyHexagonsLeftRight(r, gameData.buffer);
		for (int q = 0, offset = 0; q < gameData.maxLine; q++) {
			if (!areCoordinatesValid(q, r))
				continue;
			gameData.gameGrid[r][q] = offset < size ? gameData.buffer[offset++] : EMPTY;
		}
	}

	gameData.gravity = TOP_LEFT;
}

static void gravityBottomLeftSlide(int q, int r) {
	size_t size = getNonEmptyHexagonsDiagonalRight(q, r, gameData.buffer);
	int offset = 0;
	while (q < gameData.maxLine && r >= 0)
		gameData.gameGrid[r--][q++] = offset < size ? gameData.buffer[offset++] : EMPTY;
}

static void gravityBottomLeft(void) {
	for (int r = 0, q = 0; r < gameData.maxLine; r++) {
		if (!areCoordinatesValid(q, r))
			continue;
		gravityBottomLeftSlide(q, r);
	}

	for (int r = (int)gameData.maxLine - 1, q = 1; q < gameData.maxLine; q++) {
		if (!areCoordinatesValid(q, r))
			continue;
		gravityBottomLeftSlide(q, r);
	}

	gameData.gravity = BOTTOM_LEFT;
}

void rotateGameGrid(int amount) {
	static void (*rotationFunctions[6])(void) = {
			gravityBottom, gravityBottomRight, gravityTopRight, gravityTop, gravityTopLeft, gravityBottomLeft};

	if (amount < 0 || amount > 5)
		illegalInstruction();

	bzero(gameData.buffer, gameData.maxLine * sizeof(t_hex));
	rotationFunctions[(gameData.gravity + amount) % 6]();
}