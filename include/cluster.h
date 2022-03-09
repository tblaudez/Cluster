//
// Created by tblaudez on 3/7/22.
//

#include <stdlib.h> // abs
#include <stdint.h> // uint8_t
#include <stdint-gcc.h>

#pragma once

#define GRID_SIZE 25
#define HEX_ON_GRID (1 + 6 * ((GRID_SIZE * (GRID_SIZE - 1)) / 2))
#define MAX_LINE (GRID_SIZE * 2 -1)

#define IS_VALID(q, r) ((q + r >= GRID_SIZE - 1) && (q + r < (MAX_LINE * 2 - GRID_SIZE)))
#define EMPTY ((t_hex)0)

#define MAX(x, y) ((x) > (y) ? (x) : (y))
#define MIN(x, y) ((x) < (y) ? (x) : (y))

typedef enum {
	BOTTOM, BOTTOM_RIGHT, TOP_RIGHT, TOP, TOP_LEFT, BOTTOM_LEFT
} t_gravity;

typedef uint8_t t_hex;

extern t_hex **gameGrid;
extern t_gravity gravity;


// cluster.c
void insertToken(int row, t_hex token);

// utils.c
void displayArray(void);

void illegalInstruction(void);

t_hex **createNewBoard(void);

int getNonEmptyHexagonsTopDown(int q, t_hex buffer[MAX_LINE]);

int getNonEmptyHexagonsLeftRight(int r, t_hex buffer[MAX_LINE]);

int getNonEmptyHexagonsDiagonalRight(int q, int r, t_hex buffer[MAX_LINE]);

void swapInt(int *a, int *b);

void rotateGameGrid(int amount);