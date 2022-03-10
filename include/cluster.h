//
// Created by tblaudez on 3/7/22.
//

#include <stdlib.h> // abs
#include <stdint.h> // uint8_t
#include <stdbool.h> // bool

#pragma once

#define EMPTY ((t_hex)0)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef uint8_t t_hex;

typedef enum {
	BOTTOM, BOTTOM_RIGHT, TOP_RIGHT, TOP, TOP_LEFT, BOTTOM_LEFT
} t_gravity;

typedef enum {
	PLAYER_ONE = 0, PLAYER_TWO = 1, NONE
} t_player;

typedef enum {
	BLUE, CYAN, RED, ORANGE
} t_color;

typedef struct {
	t_hex *buffer, *tokens[2];
	t_hex **gameGrid;
	t_gravity gravity;
	size_t gridSize, hexOnGrid, maxLine, minimalConnect, tokenPerPlayer, tokenPerColor;
} t_data;

extern t_data gameData;

// cluster.c
void insertToken(int row, t_hex token);

// utils.c
void displayArray(void);

void deleteGameBoard(t_hex **gameBoard);

void illegalInstruction(void);

bool areCoordinatesValid(int q, int r);

t_hex **createNewBoard(void);

size_t getNonEmptyHexagonsTopDown(int q, t_hex buffer[gameData.maxLine]);

size_t getNonEmptyHexagonsLeftRight(int r, t_hex buffer[gameData.maxLine]);

size_t getNonEmptyHexagonsDiagonalRight(int q, int r, t_hex buffer[gameData.maxLine]);

void swapInt(int *a, int *b);

void rotateGameGrid(int amount);