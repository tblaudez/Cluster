//
// Created by tblaudez on 3/7/22.
//

#include <stdlib.h> // abs

#pragma once

#define GRID_SIZE 4
#define HEX_ON_GRID (1 + 6 * ((GRID_SIZE * (GRID_SIZE - 1)) / 2))
#define MAX_LINE (GRID_SIZE * 2 -1)

#define IS_VALID(q, r) ((q + r >= GRID_SIZE - 1) && (q + r < (MAX_LINE * 2 - GRID_SIZE)))
#define EMPTY ((t_hex)0)

typedef u_char t_hex;

extern t_hex **gameGrid;

void displayArray(void);