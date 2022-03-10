#include "cluster.h"

#include <stdio.h>

static int getLengthX(int x, int y)
{
	int len = 0;

	t_hex piece = gameData.gameGrid[x][y];
	for (; x < gameData.maxLine; x++)
	{
		if (gameData.gameGrid[x][y] != piece)
			break;
		len++;
	}
	return len;
}

static int getLengthY(int x, int y)
{
	int len = 0;

	t_hex piece = gameData.gameGrid[x][y];
	for (; y < gameData.maxLine; y++)
	{
		if (gameData.gameGrid[x][y] != piece)
			break;
		len++;
	}
	return len;
}

static int getLengthXY(int x, int y)
{
	int len = 0;

	t_hex piece = gameData.gameGrid[x][y];
	for (; x < gameData.maxLine && y < gameData.maxLine; x++, y++)
	{
		if (gameData.gameGrid[x][y] != piece)
			break;
		len++;
	}
	return len;
}

static void addLength(int GotLength, int* Length, int* Count)
{
	if (GotLength > *Length)
	{
		*Length = GotLength;
		*Count = 1;
	}
	else if (GotLength == *Length)
		(*Count)++;
}

static int compare(int a, int b)
{
	if (a > b)
		return 1;
	else if (a < b)
		return -1;
	return 0;
}

t_player getWinner()
{
	int maxLengths[2] = { 0, 0 };
	int counts[2] = { 0, 0 };

	for (int x = 0; x < gameData.maxLine; x++)
		for (int y = 0; y < gameData.maxLine; y++)
		{
			if (!areCoordinatesValid(x, y))
				continue;
			t_hex piece = gameData.gameGrid[x][y];
			if (piece == EMPTY)
				continue;
			t_player playerForPiece = (piece - 1) / 2;	// TODO: use a getPlayerForHex(t_hex) function
			addLength(getLengthX(x, y), &maxLengths[playerForPiece], &counts[playerForPiece]);
			addLength(getLengthY(x, y), &maxLengths[playerForPiece], &counts[playerForPiece]);
			addLength(getLengthXY(x, y), &maxLengths[playerForPiece], &counts[playerForPiece]);
		}

	int					winner = compare(maxLengths[PLAYER_ONE], maxLengths[PLAYER_TWO]);	// Choose the one with the longest length
	if (winner == 0)	winner = compare(counts[PLAYER_ONE],     counts[PLAYER_TWO]    );	// If its a tie, choose the one with the most number of said length
	
	// winner == -1:	PLAYER_TWO wins
	// winner == 0:		NONE wins
	// winner == 1:		PLAYER_ONE wins

	if (winner == 0)
		return NONE;

	t_player possibeWinner = winner == 1 ? PLAYER_ONE : PLAYER_TWO;

	if (maxLengths[possibeWinner] < gameData.minimalConnect)	// Ony counts as a win if its long enough
		return NONE;
	return possibeWinner;
}