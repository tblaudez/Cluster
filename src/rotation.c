//
// Created by tblaudez on 3/9/22.
//

#include <string.h>
#include "cluster.h"

static t_hex buffer[MAX_LINE];

static void gravityBottom(void) {
	for (int q = 0; q < MAX_LINE; q++) {
		int size = getNonEmptyHexagonsTopDown(q, buffer);
		for (int r = MAX_LINE - 1; r >= 0; r--) {
			if (!IS_VALID(q, r))
				continue;
			gameGrid[r][q] = size ? buffer[--size] : EMPTY;
		}
	}

	gravity = BOTTOM;
}

static void gravityBottomRight(void) {
	for (int r = 0; r < MAX_LINE; r++) {
		int size = getNonEmptyHexagonsLeftRight(r, buffer);
		for (int q = MAX_LINE - 1; q >= 0; q--) {
			if (!IS_VALID(q, r))
				continue;
			gameGrid[r][q] = size ? buffer[--size] : EMPTY;
		}
	}

	gravity = BOTTOM_RIGHT;
}

static void gravityTopRightSlide(int q, int r) {
	int size = getNonEmptyHexagonsDiagonalRight(q, r, buffer);
	swapInt(&r, &q);
	while (q >= 0 && r < MAX_LINE)
		gameGrid[r++][q--] = size ? buffer[--size] : EMPTY;
}

static void gravityTopRight(void) {
	for (int r = 0, q = 0; r < MAX_LINE; r++) {
		if (!IS_VALID(q, r))
			continue;
		gravityTopRightSlide(q, r);
	}

	bzero(buffer, MAX_LINE);

	for (int r = MAX_LINE - 1, q = 1; q < MAX_LINE; q++) {
		if (!IS_VALID(q, r))
			continue;
		gravityTopRightSlide(q, r);
	}

	gravity = TOP_RIGHT;
}

static void gravityTop(void) {
	for (int q = 0; q < MAX_LINE; q++) {
		int size = getNonEmptyHexagonsTopDown(q, buffer);
		for (int r = 0, offset = 0; r < MAX_LINE; r++) {
			if (!IS_VALID(q, r))
				continue;
			gameGrid[r][q] = offset < size ? buffer[offset++] : EMPTY;
		}
	}

	gravity = TOP;
}

static void gravityTopLeft(void) {
	for (int r = 0; r < MAX_LINE; r++) {
		int size = getNonEmptyHexagonsLeftRight(r, buffer);
		for (int q = 0, offset = 0; q < MAX_LINE; q++) {
			if (!IS_VALID(q, r))
				continue;
			gameGrid[r][q] = offset < size ? buffer[offset++] : EMPTY;
		}
	}

	gravity = TOP_LEFT;
}

static void gravityBottomLeftSlide(int q, int r) {
	int size = getNonEmptyHexagonsDiagonalRight(q, r, buffer);
	int offset = 0;
	while (q < MAX_LINE && r >= 0)
		gameGrid[r--][q++] = offset < size ? buffer[offset++] : EMPTY;
}

static void gravityBottomLeft(void) {
	for (int r = 0, q = 0; r < MAX_LINE; r++) {
		if (!IS_VALID(q, r))
			continue;
		gravityBottomLeftSlide(q, r);
	}

	for (int r = MAX_LINE - 1, q = 1; q < MAX_LINE; q++) {
		if (!IS_VALID(q, r))
			continue;
		gravityBottomLeftSlide(q, r);
	}

	gravity = BOTTOM_LEFT;
}

void rotateGameGrid(int amount) {
	static void (*rotationFunctions[6])(void) = {
			gravityBottom, gravityBottomRight, gravityTopRight, gravityTop, gravityTopLeft, gravityBottomLeft};

	if (amount < 0 || amount > 5)
		illegalInstruction();

	bzero(buffer, MAX_LINE);
	rotationFunctions[(gravity + amount) % 6]();
}