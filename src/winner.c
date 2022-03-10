#include "cluster.h"

#include <stdio.h>

static int getLengthX(int q, int r)
{
	int len = 0;

	t_hex piece = gameData.gameGrid[q][r];
	for (; q < gameData.maxLine; q++)
	{
		if (gameData.gameGrid[q][r] != piece)
			break;
		len++;
	}
	return len;
}

static int getLengthY(int q, int r)
{
	int len = 0;

	t_hex piece = gameData.gameGrid[q][r];
	for (; r < gameData.maxLine; r++)
	{
		if (gameData.gameGrid[q][r] != piece)
			break;
		len++;
	}
	return len;
}

static int getLengthXY(int q, int r)
{
	int len = 0;

	t_hex piece = gameData.gameGrid[q][r];
	for (; q < gameData.maxLine && r < gameData.maxLine; q++, r++)
	{
		if (gameData.gameGrid[q][r] != piece)
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

	for (int q = 0; q < gameData.maxLine; q++)
		for (int r = 0; r < gameData.maxLine; r++)
		{
			if (!areCoordinatesValid(q, r))
				continue;
			t_hex piece = gameData.gameGrid[q][r];
			if (piece == EMPTY)
				continue;
			t_player playerForPiece = (piece - 1) / 2;	// TODO: use a getPlayerForHex(t_hex) function
			addLength(getLengthX(q, r), &maxLengths[playerForPiece], &counts[playerForPiece]);
			addLength(getLengthY(q, r), &maxLengths[playerForPiece], &counts[playerForPiece]);
			addLength(getLengthXY(q, r), &maxLengths[playerForPiece], &counts[playerForPiece]);
		}

	int					winner = compare(maxLengths[PLAYER_ONE], maxLengths[PLAYER_TWO]);	// Choose the one with the longest length
	if (winner == 0)	winner = compare(    counts[PLAYER_ONE],     counts[PLAYER_TWO]);	// If its a tie, choose the one with the most number of said length
	
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