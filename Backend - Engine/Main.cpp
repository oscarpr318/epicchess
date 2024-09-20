#include <Windows.h>
#include <iostream>
#include <fstream>
#include <bitset>
#include <string>
#include <vector>
#include "common.h"

bool WHITE_CAN_CASTLE = true;
bool BLACK_CAN_CASTLE = true;
bool WHITE_CAN_CASTLE_search = true;
bool BLACK_CAN_CASTLE_search = true;

byte board[8][8] = { {BLACKROOK,BLACKKNIGHT,BLACKBISHOP,BLACKQUEEN,BLACKKING,BLACKBISHOP,BLACKKNIGHT,BLACKROOK},
					 {BLACKPAWN,BLACKPAWN,BLACKPAWN,BLACKPAWN,BLACKPAWN,BLACKPAWN,BLACKPAWN,BLACKPAWN},
					 {0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0},
					 {0,0,0,0,0,0,0,0},
					 {WHITEPAWN,WHITEPAWN,WHITEPAWN,WHITEPAWN,WHITEPAWN,WHITEPAWN,WHITEPAWN,WHITEPAWN},
					 {WHITEROOK,WHITEKNIGHT,WHITEBISHOP,WHITEQUEEN,WHITEKING,WHITEBISHOP,WHITEKNIGHT,WHITEROOK} };

// A copy of the board is used in functions used during searching so the actual board is not changed during a search
byte _board[8][8];

int check_if_in_check(byte colour, byte __board[8][8]) {

	int dst_row, dst_col, x;
	if (colour) {

		// Find the white king
		for (dst_row = 0; dst_row < 8; dst_row++) {
			for (dst_col = 0; dst_col < 8; dst_col++) {
				if (__board[dst_row][dst_col] == WHITEKING) {
					break;
				}
			}
			if (dst_col != 8) { break; }
		}

		// Check row to the left of the king
		for (int x = dst_col - 1; x >= 0; x--) {
			if (IS_WHITE(__board[dst_row][x]) || __board[dst_row][x] == BLACKPAWN || __board[dst_row][x] == BLACKBISHOP || __board[dst_row][x] == BLACKKNIGHT) {
				break;
			}

			if (x == dst_col - 1 && __board[dst_row][x] == BLACKKING) {
				return 1;
			}

			if (__board[dst_row][x] == BLACKROOK || __board[dst_row][x] == BLACKQUEEN) {
				return 1;
			}
		}

		// Check row to the right of the king
		for (int x = dst_col + 1; x < 8; x++) {
			if (IS_WHITE(__board[dst_row][x]) || __board[dst_row][x] == BLACKPAWN || __board[dst_row][x] == BLACKBISHOP || __board[dst_row][x] == BLACKKNIGHT) {
				break;
			}

			if (x == dst_col + 1 && __board[dst_row][x] == BLACKKING) {
				return 1;
			}

			if (__board[dst_row][x] == BLACKROOK || __board[dst_row][x] == BLACKQUEEN) {
				return 1;
			}
		}

		// Check column above the king
		for (int x = dst_row - 1; x >= 0; x--) {
			if (IS_WHITE(__board[x][dst_col]) || __board[x][dst_col] == BLACKPAWN || __board[x][dst_col] == BLACKBISHOP || __board[x][dst_col] == BLACKKNIGHT) {
				break;
			}

			if (x == dst_row - 1 && __board[x][dst_col] == BLACKKING) {
				return 1;
			}

			if (__board[x][dst_col] == BLACKROOK || __board[x][dst_col] == BLACKQUEEN) {
				return 1;
			}
		}

		// Check column below the king
		for (int x = dst_row + 1; x < 8; x++) {
			if (IS_WHITE(__board[x][dst_col]) || __board[x][dst_col] == BLACKPAWN || __board[x][dst_col] == BLACKBISHOP || __board[x][dst_col] == BLACKKNIGHT) {
				break;
			}

			if (x == dst_row + 1 && __board[x][dst_col] == BLACKKING) {
				return 1;
			}

			if (__board[x][dst_col] == BLACKROOK || __board[x][dst_col] == BLACKQUEEN) {
				return 1;
			}
		}

		int i = 1;
		int col = dst_col, row = dst_row;

		// Check the diagonals from the king for black bishops, queens, pawns or the king

		for (i = 1; i < 8; i++) {
			col = dst_col + i;
			row = dst_row + i;

			if (col == 8 || row == 8) {
				break;
			}

			if (i == 1 && __board[row][col] == BLACKKING) {
				return 1;
			}

			if (__board[row][col] == BLACKBISHOP || __board[row][col] == BLACKQUEEN) {
				return 1;
			}

			if (IS_WHITE(__board[row][col]) || __board[row][col] == BLACKPAWN || __board[row][col] == BLACKKNIGHT) {
				break;
			}
		}

		for (i = 1; i < 8; i++) {
			col = dst_col + i;
			row = dst_row - i;

			if (col == 8 || row == -1) {
				break;
			}

			if (i == 1 && (__board[row][col] == BLACKPAWN || __board[row][col] == BLACKKING)) {
				return 1;
			}

			if (__board[row][col] == BLACKBISHOP || __board[row][col] == BLACKQUEEN) {
				return 1;
			}

			if (IS_WHITE(__board[row][col]) || __board[row][col] == BLACKKNIGHT) {
				break;
			}
		}

		for (i = 1; i < 8; i++) {
			col = dst_col - i;
			row = dst_row + i;

			if (col == -1 || row == 8) {
				break;
			}

			if (i == 1 && __board[row][col] == BLACKKING) {
				return 1;
			}

			if (__board[row][col] == BLACKBISHOP || __board[row][col] == BLACKQUEEN) {
				return 1;
			}

			if (IS_WHITE(__board[row][col]) || __board[row][col] == BLACKKNIGHT || __board[row][col] == BLACKPAWN) {
				break;
			}
		}

		for (i = 1; i < 8; i++) {
			col = dst_col - i;
			row = dst_row - i;

			if (col == -1 || row == -1) {
				break;
			}

			if (i == 1 && (__board[row][col] == BLACKPAWN || __board[row][col] == BLACKKING)) {
				return 1;
			}

			if (__board[row][col] == BLACKBISHOP || __board[row][col] == BLACKQUEEN) {
				return 1;
			}

			if (IS_WHITE(__board[row][col]) || __board[row][col] == BLACKKNIGHT) {
				break;
			}
		}

		std::vector<std::pair<int, int>> knights;

		for (int c = 0; c < 8; c++) {
			for (int d = 0; d < 8; d++) {
				if (__board[c][d] == BLACKKNIGHT) {

					std::pair<int, int> pos;
					pos.first = c;
					pos.second = d;
					knights.push_back(pos);
				}
			}
		}
		for (int q = 0; q < knights.size(); q++) {

			if ((abs(knights[q].first - dst_row) == 1 && abs(knights[q].second - dst_col) == 2) || (abs(knights[q].first - dst_row) == 2 && abs(knights[q].second - dst_col) == 1)) {
				return 1;
			}
		}
	}
	else {
		

		
		for (dst_row = 0; dst_row < 8; dst_row++) {
			for (dst_col = 0; dst_col < 8; dst_col++) {
				if (__board[dst_row][dst_col] == BLACKKING) {
					break;
				}
			}
			if (dst_col != 8) { break; }
		}

		for (int x = dst_col - 1; x >= 0; x--) {
			if (IS_BLACK(__board[dst_row][x]) || __board[dst_row][x] == WHITEPAWN || __board[dst_row][x] == WHITEBISHOP || __board[dst_row][x] == WHITEKNIGHT)  {
				break;
			}

			if (x == dst_col - 1 && __board[dst_row][x] == WHITEKING) {
				return 1;
			}

			if (__board[dst_row][x] == WHITEROOK || __board[dst_row][x] == WHITEQUEEN) {
				return 1;
			}
		}

		for (int x = dst_col + 1; x < 8; x++) {
			if (IS_BLACK(__board[dst_row][x]) || __board[dst_row][x] == WHITEPAWN || __board[dst_row][x] == WHITEBISHOP || __board[dst_row][x] == WHITEKNIGHT) {
				break;
			}

			if (x == dst_col + 1 && __board[dst_row][x] == WHITEKING) {
				return 1;
			}

			if (__board[dst_row][x] == WHITEROOK || __board[dst_row][x] == WHITEQUEEN) {
				return 1;
			}
		}

		for (int x = dst_row - 1; x >= 0; x--) {
			if (IS_BLACK(__board[x][dst_col]) || __board[x][dst_col] == WHITEPAWN || __board[x][dst_col] == WHITEBISHOP || __board[x][dst_col] == WHITEKNIGHT) {
				break;
			}

			if (x == dst_row - 1 && __board[x][dst_col] == WHITEKING) {
				return 1;
			}

			if (__board[x][dst_col] == WHITEROOK || __board[x][dst_col] == WHITEQUEEN) {
				return 1;
			}
		}

		for (int x = dst_row + 1; x < 8; x++) {
			if (IS_BLACK(__board[x][dst_col]) || __board[x][dst_col] == WHITEPAWN || __board[x][dst_col] == WHITEBISHOP || __board[x][dst_col] == WHITEKNIGHT) {
				break;
			}

			if (x == dst_row + 1 && __board[x][dst_col] == WHITEKING) {
				return 1;
			}

			if (__board[x][dst_col] == WHITEROOK || __board[x][dst_col] == WHITEQUEEN) {
				return 1;
			}
		}

		int i = 1;
		int col = dst_col, row = dst_row;

		for (i = 1; i < 8; i++) {
			col = dst_col + i;
			row = dst_row + i;

			if (col == 8 || row == 8) {
				break;
			}

			if (i == 1 && (__board[row][col] == WHITEKING || __board[row][col] == WHITEPAWN)) {
				return 1;
			}

			if (__board[row][col] == WHITEBISHOP || __board[row][col] == WHITEQUEEN) {
				return 1;
			}

			if (IS_BLACK(__board[row][col])) {
				break;
			}
		}

		for (i = 1; i < 8; i++) {
			col = dst_col + i;
			row = dst_row - i;

			if (col == 8 || row == -1) {
				break;
			}

			if (i == 1 && __board[row][col] == WHITEKING) {
				return 1;
			}

			if (__board[row][col] == WHITEBISHOP || __board[row][col] == WHITEQUEEN) {
				return 1;
			}

			if (IS_BLACK(__board[row][col])) {
				break;
			}
		}

		for (i = 1; i < 8; i++) {
			col = dst_col - i;
			row = dst_row + i;

			if (col == -1 || row == 8) {
				break;
			}

			if (i == 1 && (__board[row][col] == WHITEKING || __board[row][col] == WHITEPAWN)) {
				return 1;
			}

			if (__board[row][col] == WHITEBISHOP || __board[row][col] == WHITEQUEEN) {
				return 1;
			}

			if (IS_BLACK(__board[row][col])) {
				break;
			}
		}

		for (i = 1; i < 8; i++) {
			col = dst_col - i;
			row = dst_row - i;

			if (col == -1 || row == -1) {
				break;
			}

			if (i == 1 && __board[row][col] == WHITEKING) {
				return 1;
			}

			if (__board[row][col] == WHITEBISHOP || __board[row][col] == WHITEQUEEN) {
				return 1;
			}

			if (IS_BLACK(__board[row][col])) {
				break;
			}
		}

		std::vector<std::pair<int, int>> knights;

		for (int c = 0; c < 8; c++) {
			for (int d = 0; d < 8; d++) {
				if (__board[c][d] == WHITEKNIGHT) {

					std::pair<int, int> pos;
					pos.first = c;
					pos.second = d;
					knights.push_back(pos);
				}
			}
		}

		for (int q = 0; q < knights.size(); q++) {

			if ((abs(knights[q].first - dst_row) == 1 && abs(knights[q].second - dst_col) == 2) || (abs(knights[q].first - dst_row) == 2 && abs(knights[q].second - dst_col) == 1)) {
				return 1;
			}
		}
	}
	return 0;
}

double check_if_game_end(byte __board[8][8], byte p, bool search) {

	int white_in_check = check_if_in_check(1, __board);
	std::vector<move_data> whites_moves = get_possible_moves(1, __board, true);

	// If white has no valid moves left
	if (!whites_moves.size()) {

		// If white is in check, black has won by checkmate
		if (white_in_check) {
			if (!p) {

				// If the current player is black, he has a positive score
				return 1.0;
			}
			else {
				return 0.0;
			}
		}

		// If white is not in check, game must be a draw
		else {
			return 0.5;
		}
	}

	bool v = true;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

			// If the only pieces left on the board are knights and/or bishops the game cannot be won by either player so result is a draw
			if (__board[x][y] == WHITEPAWN || __board[x][y] == BLACKPAWN || __board[x][y] == WHITEQUEEN || __board[x][y] == BLACKQUEEN || __board[x][y] == WHITEROOK || __board[x][y] == BLACKROOK) {
				v = false;
				break;
			}
		}
		if (!v) { break; }
	}
	if (v) {
		return 0.5;
	}


	int black_in_check = check_if_in_check(0, __board);
	std::vector<move_data> blacks_moves = get_possible_moves(0, __board, true);

	// If black has no valid moves left
	if (!blacks_moves.size()) {

		// If black is in check, white has won by checkmate
		if (black_in_check) {
			if (!p) {

				return 0.0;
			}
			else {
				// If the current player is white, he has a positive score
				return 1.0;
			}
		}
		else {
			// If black is not in check, game must be a draw
			return 0.5;
		}
	}

	return -1;
}

int castle(int dst_col, byte colour, byte __board[8][8], bool search) {

	int r = 2;
	if (colour) {
		if (search) {
			if (!WHITE_CAN_CASTLE_search) {
				r = 1;
			}
		}
		else {
			if (!WHITE_CAN_CASTLE) {
				r = 1;
			}
		}
		if (r == 2) {

			if (check_if_in_check(1, __board) == 1) {
				return 1;
			}

			if (dst_col == 5) {
				if (__board[7][6] || __board[7][5]) {
					return 1;
				}
			}

			if (dst_col == 2) {
				if (__board[7][1] || __board[7][2] || __board[7][3]) {
					return 1;
				}
			}
			if (search) {
				WHITE_CAN_CASTLE_search = false;
			}
			else {
				WHITE_CAN_CASTLE = false;
			}
		}

	}
	else {

		if (search) {
			if (!BLACK_CAN_CASTLE_search) {
				r = 1;
			}
		}
		else {
			if (!BLACK_CAN_CASTLE) {
				r = 1;
			}
		}
		if (r == 2)
		{

			if (check_if_in_check(0, __board) == 1) {
				return 1;
			}

			if (dst_col == 5) {
				if ( __board[0][5] || __board[0][6]) {
					return 1;
				}
			}

			if (dst_col == 2) {
				if (__board[0][1] || __board[0][2] || __board[0][3]) {
					return 1;
				}

			}
			if (search) {
				BLACK_CAN_CASTLE_search = false;
			}
			else {
				BLACK_CAN_CASTLE = false;
			}
		}

	}
	return r;
}

std::vector<move_data> get_possible_moves(byte player, byte __board[8][8], bool search) {

	std::vector<move_data> moves;
	std::vector<move_data> valid_moves;

	int row, col;
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 8; y++) {

			if (player) {

				switch (__board[x][y]) {

				case (WHITEPAWN):
					if (x > 0) {
						moves.push_back(move_data{ x,y,x - 1,y,WHITEPAWN });
					}
					if (x == 6) {
						moves.push_back(move_data{ x,y,x - 2,y,WHITEPAWN });
					}
					if (y != 7) {
						moves.push_back(move_data{ x,y,x - 1,y + 1,WHITEPAWN });
					}
					if (y != 0) {
						moves.push_back(move_data{ x,y,x - 1,y - 1,WHITEPAWN });
					}

					break;

				case (WHITEKNIGHT):
					moves.push_back(move_data{ x,y,abs(x - 1),abs(y - 2),WHITEKNIGHT });
					moves.push_back(move_data{ x,y,abs(x - 2),abs(y - 1),WHITEKNIGHT });

					if (x < 7 && y < 6) {
						moves.push_back(move_data{ x,y,x + 1,y + 2,WHITEKNIGHT });
					}
					if (y < 7 && x < 6) {
						moves.push_back(move_data{ x,y,x + 2,y + 1,WHITEKNIGHT });
					}
					break;

				case (WHITEBISHOP):
					row = x;
					col = y;
					while (row > 0) {
						row--;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,WHITEBISHOP });
						}
					}
					row = x, col = y;
					while (row > 0) {
						row--;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,WHITEBISHOP });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,WHITEBISHOP });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,WHITEBISHOP });
						}
					}

					break;

				case (WHITEKING):

					moves.push_back(move_data{ x,y,7,0,WHITEKING });
					moves.push_back(move_data{ x,y,7,7,WHITEKING });

					if (x != 7) {
						moves.push_back(move_data{ x,y,x + 1,y,WHITEKING });
					}
					if (x != 0) {
						moves.push_back(move_data{ x,y,x - 1,y,WHITEKING });
					}
					if (y != 7) {
						moves.push_back(move_data{ x,y,x,y + 1,WHITEKING });
					}
					if (y != 0) {
						moves.push_back(move_data{ x,y,x,y - 1,WHITEKING });
					}

					if (x != 7 && y != 7) {
						moves.push_back(move_data{ x,y,x + 1,y + 1,WHITEKING });
					}
					if (x != 7 && y != 0) {
						moves.push_back(move_data{ x,y,x + 1,y - 1,WHITEKING });
					}
					if (x != 0 && y != 7) {
						moves.push_back(move_data{ x,y,x - 1,y + 1,WHITEKING });
					}
					if (x != 0 && y != 0) {
						moves.push_back(move_data{ x,y,x - 1,y - 1,WHITEKING });
					}

					break;

				case (WHITEQUEEN):
					row = x;
					col = y;
					while (row > 0) {
						row--;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
						}
					}
					row = x, col = y;
					while (row > 0) {
						row--;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
						}
					}

					row = x, col = y;
					while (row < 7) {
						row++;
						moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
					}

					row = x, col = y;
					while (row > 0) {
						row--;
						moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
					}

					row = x, col = y;
					while (col < 7) {
						col++;
						moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
					}

					row = x, col = y;
					while (col > 0) {
						col--;
						moves.push_back(move_data{ x,y,row,col,WHITEQUEEN });
					}
					break;

				case (WHITEROOK):
					row = x, col = y;
					while (row < 7) {
						row++;
						moves.push_back(move_data{ x,y,row,col,WHITEROOK });
					}

					row = x;
					while (row > 0) {
						row--;
						moves.push_back(move_data{ x,y,row,col,WHITEROOK });
					}

					col = y, row = x;
					while (col < 7) {
						col++;
						moves.push_back(move_data{ x,y,row,col,WHITEROOK });
					}

					col = y, row = x;
					while (col > 0) {
						col--;
						moves.push_back(move_data{ x,y,row,col,WHITEROOK });
					}
					break;
				}
			}

			else {

				switch (__board[x][y]) {

				case (BLACKPAWN):

					if (x < 7) {
						moves.push_back(move_data{ x,y,x + 1,y,BLACKPAWN });
					}
					if (x == 1) {
						moves.push_back(move_data{ x,y,x + 2,y,BLACKPAWN });
					}
					if (y != 7) {
						moves.push_back(move_data{ x,y,x + 1,y + 1,BLACKPAWN });
					}
					if (y != 0) {
						moves.push_back(move_data{ x,y,x + 1,y - 1,BLACKPAWN });
					}

					break;

				case (BLACKKNIGHT):
					moves.push_back(move_data{ x,y,abs(x - 1),abs(y - 2),BLACKKNIGHT });
					moves.push_back(move_data{ x,y,abs(x - 2),abs(y - 1),BLACKKNIGHT });

					if (x < 7 && y < 6) {
						moves.push_back(move_data{ x,y,x + 1,y + 2,BLACKKNIGHT });
					}
					if (y < 7 && x < 6) {
						moves.push_back(move_data{ x,y,x + 2,y + 1,BLACKKNIGHT });
					}
					break;

				case (BLACKBISHOP):
					row = x;
					col = y;
					while (row > 0) {
						row--;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,BLACKBISHOP });
						}
					}
					row = x, col = y;
					while (row > 0) {
						row--;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,BLACKBISHOP });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,BLACKBISHOP });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,BLACKBISHOP });
						}
					}

					break;

				case (BLACKKING):

					moves.push_back(move_data{ x,y,0,0,BLACKKING });
					moves.push_back(move_data{ x,y,0,7,BLACKKING });

					if (x != 7) {
						moves.push_back(move_data{ x,y,x + 1,y,BLACKKING });
					}
					if (x != 0) {
						moves.push_back(move_data{ x,y,x - 1,y,BLACKKING });
					}
					if (y != 7) {
						moves.push_back(move_data{ x,y,x,y + 1,BLACKKING });
					}
					if (y != 0) {
						moves.push_back(move_data{ x,y,x,y - 1,BLACKKING });
					}

					if (x != 7 && y != 7) {
						moves.push_back(move_data{ x,y,x + 1,y + 1,BLACKKING });
					}
					if (x != 7 && y != 0) {
						moves.push_back(move_data{ x,y,x + 1,y - 1,BLACKKING });
					}
					if (x != 0 && y != 7) {
						moves.push_back(move_data{ x,y,x - 1,y + 1,BLACKKING });
					}
					if (x != 0 && y != 0) {
						moves.push_back(move_data{ x,y,x - 1,y - 1,BLACKKING });
					}

					break;

				case (BLACKQUEEN):
					row = x;
					col = y;
					while (row > 0) {
						row--;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
						}
					}
					row = x, col = y;
					while (row > 0) {
						row--;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col < 7) {
							col++;
							moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
						}
					}
					row = x, col = y;
					while (row < 7) {
						row++;
						while (col > 0) {
							col--;
							moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
						}
					}

					row = x, col = y;
					while (row < 7) {
						row++;
						moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
					}

					row = x, col = y;
					while (row > 0) {
						row--;
						moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
					}

					row = x, col = y;
					while (col < 7) {
						col++;
						moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
					}

					row = x, col = y;
					while (col > 0) {
						col--;
						moves.push_back(move_data{ x,y,row,col,BLACKQUEEN });
					}
					break;

				case (BLACKROOK):
					row = x, col = y;
					while (row < 7) {
						row++;
						moves.push_back(move_data{ x,y,row,col,BLACKROOK });
					}

					row = x, col = y;
					while (row > 0) {
						row--;
						moves.push_back(move_data{ x,y,row,col,BLACKROOK });
					}

					row = x, col = y;
					while (col < 7) {
						col++;
						moves.push_back(move_data{ x,y,row,col,BLACKROOK });
					}

					row = x, col = y;
					while (col > 0) {
						col--;
						moves.push_back(move_data{ x,y,row,col,BLACKROOK });
					}
					break;
				}
			}
		}
	}

	for (int a = 0; a < moves.size(); a++) {

		for (int b = 0; b < 8; b++) {
			for (int c = 0; c < 8; c++) {
				_board[b][c] = __board[b][c];
			}
		}

		if (move(moves[a].src_row, moves[a].src_col, moves[a].dst_row, moves[a].dst_col, moves[a].piece, _board, true) != 1) {
			valid_moves.push_back(moves[a]);
		}
	}

	return valid_moves;
}

byte move(int src_row, int src_col, int dst_row, int dst_col, byte piece, byte __board[8][8], bool search) {

	if ((__board[dst_row][dst_col] == WHITEKING && piece != WHITEROOK) || (__board[dst_row][dst_col] == BLACKKING && piece != BLACKROOK)) {
		return 1;
	}

	if ((IS_WHITE(piece)) && (IS_WHITE(__board[dst_row][dst_col])) && piece != WHITEROOK && piece != WHITEKING) {
		return 1;
	}

	if ((IS_BLACK(piece)) && (IS_BLACK(__board[dst_row][dst_col])) && piece != BLACKROOK && piece != BLACKKING) {
		return 1;
	}

	if (src_row == dst_row && src_col == dst_col) {
		return 1;
	}

	switch (piece) {

	case (WHITEPAWN):

		if (abs(dst_col - src_col) == 1 && (src_row - dst_row != 1 || !__board[dst_row][dst_col]) || abs(dst_col - src_col) > 1 || (src_row == 6 && (src_row - dst_row > 2 || __board[5][src_col])) || (src_row != 6 && src_row - dst_row > 1) || src_row - dst_row < 0 || src_col == dst_col && __board[dst_row][src_col]) {
			return 1;
		}

		break;

	case (BLACKPAWN):
		if (abs(dst_col - src_col) == 1 && (dst_row - src_row != 1 || !__board[dst_row][dst_col]) || abs(dst_col - src_col) > 1 || (src_row == 1 && (dst_row - src_row > 2 || __board[2][src_col])) || (src_row != 1 && dst_row - src_row > 1) || dst_row - src_row < 0 || src_col == dst_col && __board[dst_row][src_col]) {
			return 1;
		}
		break;

	case (WHITEKNIGHT):
		if ((abs(src_row - dst_row) == 1 && abs(src_col - dst_col) == 2) || (abs(src_row - dst_row) == 2 && abs(src_col - dst_col) == 1)) {
			break;
		}
		else {
			return 1;
		}

	case (BLACKKNIGHT):
		if ((abs(src_row - dst_row) == 1 && abs(src_col - dst_col) == 2) || (abs(src_row - dst_row) == 2 && abs(src_col - dst_col) == 1)) {
			break;
		}
		else {
			return 1;
		}

	case (WHITEKING):

		if (__board[dst_row][dst_col] == WHITEROOK) {

			if (dst_col == 0) {
				int r = castle(2, 1, __board, search);
				if (r == 2) {
					__board[7][0] = 0;
					__board[7][2] = WHITEKING;
					__board[7][3] = WHITEROOK;
					__board[7][4] = 0;
				}
				return r;
			}
			if (dst_col == 7) {
				int r = castle(5, 1, __board, search);
				if (r == 2) {
					__board[7][7] = 0;
					__board[7][6] = WHITEKING;
					__board[7][5] = WHITEROOK;
					__board[7][4] = 0;
				}
				return r;

			}
			else {
				return 1;
			}
		}

		if (IS_WHITE(__board[dst_row][dst_col])) {
			return 1;
		}

		if (abs(dst_row - src_row) > 1 || abs(dst_col - src_col) > 1) {
			return 1;;
		}
		if (search) {
			WHITE_CAN_CASTLE_search = false;
		}
		else {
			WHITE_CAN_CASTLE = false;
		}
		break;

	case (BLACKKING):

		if (__board[dst_row][dst_col] == BLACKROOK) {
			if (dst_col == 0) {
				int r = castle(2, 0, __board, search);

				if (r == 2) {
					__board[0][0] = 0;
					__board[0][2] = BLACKKING;
					__board[0][3] = BLACKROOK;
					__board[0][4] = 0;
				}
				return r;
			}
			if (dst_col == 7) {
				int r = castle(5, 0, __board, search);

				if (r == 2) {
					__board[0][7] = 0;
					__board[0][6] = BLACKKING;
					__board[0][5] = BLACKROOK;
					__board[0][4] = 0;
				}
				return r;
			}
			else {
				return 1;
			}
		}

		if (IS_BLACK(__board[dst_row][dst_col])) {
			return 1;
		}

		if (abs(dst_row - src_row) > 1 || abs(dst_col - src_col) > 1) {
			return 1;;
		}

		if (search) {
			BLACK_CAN_CASTLE_search = false;
		}

		else {
			BLACK_CAN_CASTLE = false;
		}

		break;

	case (WHITEROOK):


		if (src_col != dst_col && src_row != dst_row) {
			return 1;
		}
		printf("1");
		if (__board[dst_row][dst_col] == WHITEKING) {
			

			if (src_col == 0) {
				int r = castle(2, 1, __board, search);
				if (r == 2) {
					__board[7][0] = 0;
					__board[7][2] = WHITEKING;
					__board[7][3] = WHITEROOK;
					__board[7][4] = 0;
				}
				return r;
			}
			if (src_col == 7) {
				int r = castle(5, 1, __board, search);
				if (r == 2) {
					__board[7][7] = 0;
					__board[7][6] = WHITEKING;
					__board[7][5] = WHITEROOK;
					__board[7][4] = 0;
				}
				return r;

			}
			else {
				return 1;
			}
		}

		if (IS_WHITE(piece) && IS_WHITE(__board[dst_row][dst_col])) {
			return 1;
		}

		if (src_col == dst_col) {
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
		}
		else if (src_row == dst_row) {
			if (src_col > dst_col) {
				for (int y = src_col - 1; y > dst_col; y--) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_col + 1; y < dst_col; y++) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
		}

		break;

	case (BLACKROOK):

		if (src_col != dst_col && src_row != dst_row) {
			return 1;
		}

		if (__board[dst_row][dst_col] == BLACKKING) {

			if (src_col == 0) {
				int r = castle(2, 0, __board, search);
				if (r == 2) {
					__board[0][0] = 0;
					__board[0][2] = BLACKKING;
					__board[0][3] = BLACKROOK;
					__board[0][4] = 0;
				}
				return r;
			}
			if (src_col == 7) {
				int r = castle(5, 0, __board, search);
				if (r == 2) {
					__board[0][7] = 0;
					__board[0][6] = BLACKKING;
					__board[0][5] = BLACKROOK;
					__board[0][4] = 0;
				}
				return r;
			}
			else {
				return 1;
			}
		}

		if (IS_BLACK(piece) && IS_BLACK(__board[dst_row][dst_col])) {
			return 1;
		}

		if (src_col == dst_col) {
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
		}
		else if (src_row == dst_row) {
			if (src_col > dst_col) {
				for (int y = src_col - 1; y > dst_col; y--) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_col + 1; y < dst_col; y++) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
		}
		
		break;

	case (WHITEQUEEN):

		if (src_row == dst_row && src_col != dst_col) {
			if (src_col > dst_col) {
				for (int y = src_col - 1; y > dst_col; y--) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_col + 1; y < dst_col; y++) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
		}

		else if (src_col == dst_col && src_row != dst_row) {
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
		}

		else if (abs(src_col - dst_col) == abs(src_row - dst_row)) {
			int i = 1;
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
		}

		else {
			return 1;
		}

		break;

	case (BLACKQUEEN):

		if (src_row == dst_row && src_col != dst_col) {
			if (src_col > dst_col) {
				for (int y = src_col - 1; y > dst_col; y--) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_col + 1; y < dst_col; y++) {
					if (__board[dst_row][y]) {
						return 1;
					}
				}
			}
		}

		else if (src_col == dst_col && src_row != dst_row) {
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (__board[y][dst_col]) {
						return 1;
					}
				}
			}
		}

		else if (abs(src_col - dst_col) == abs(src_row - dst_row)) {
			int i = 1;
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
		}

		else {
			return 1;
		}

		break;

	case (WHITEBISHOP):

		if (abs(src_col - dst_col) == abs(src_row - dst_row)) {
			int i = 1;
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
		}

		else {
			return 1;
		}

		break;

	case (BLACKBISHOP):

		if (abs(src_col - dst_col) == abs(src_row - dst_row)) {
			int i = 1;
			if (src_row > dst_row) {
				for (int y = src_row - 1; y > dst_row; y--) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
			else {
				for (int y = src_row + 1; y < dst_row; y++) {
					if (dst_col > src_col) {
						if (__board[y][src_col + i]) {
							return 1;
						}
					}
					else {
						if (__board[y][src_col - i]) {
							return 1;
						}
					}
					i++;
				}
			}
		}

		else {
			return 1;
		}

		break;
	}

	byte dst_piece = __board[dst_row][dst_col];
	__board[dst_row][dst_col] = piece;
	__board[src_row][src_col] = 0;

	if (IS_WHITE(piece)) {
		if (check_if_in_check(1, __board) == 1) {
			__board[dst_row][dst_col] = dst_piece;
			__board[src_row][src_col] = piece;
			return 1;
		}
	}
	else {
		if (check_if_in_check(0, __board) == 1) {
			__board[dst_row][dst_col] = dst_piece;
			__board[src_row][src_col] = piece;
			return 1;
		}
	}
	return 0;
}

void get_pipe_data(HANDLE pipe, byte* output, int size) {
	unsigned long r = 0;
	int i = 0;

	while (i < size) {
		ReadFile(pipe, output + i, size - i, &r, 0);
		i += r;
	}
}

void write_pipe_data(HANDLE pipe, byte* data, int size) {

	unsigned long r = 0;
	int i = 0;

	while (i < size) {
		WriteFile(pipe, data + i, size - i, &r, 0);
		i += r;
	}
}

int main() {

	HANDLE pipe = CreateFileW(TEXT("\\\\.\\pipe\\p"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
	byte colour;
	get_pipe_data(pipe, &colour, 1);
	colour -= 48;

	byte current_player = 1;
	byte mv[5];
	DWORD temp;
	if (colour == 2) {
		while (1) {
			get_pipe_data(pipe, mv, 5 * sizeof(byte)); 

			move_data m = search(current_player);
			mv[0] = m.src_row;
			if (mv[0] != 255) {
				mv[1] = m.src_col;
				mv[2] = m.dst_row;
				mv[3] = m.dst_col;
				mv[4] = m.piece; 

				write_pipe_data(pipe, (byte*)&m, 5 * sizeof(byte));
			}
			move(mv[0], mv[1], mv[2], mv[3], mv[4], board, true);
			current_player ^= 1;
		}
	}

	else {
		while (1) {
			if (current_player != colour) {
				get_pipe_data(pipe, mv, 5);
				printf("PLAYER: %d %d %d %d %d\n", mv[0], mv[1], mv[2], mv[3], mv[4]);
				byte r = move(mv[0], mv[1], mv[2], mv[3], mv[4], board, false); 
				write_pipe_data(pipe, &r, sizeof(byte));
				if (r != 1) {
					current_player ^= 1;
				}
			}
			else {
				move_data m = search(colour); 
				mv[0] = m.src_row;
				if (mv[0] != 255) {
					mv[1] = m.src_col;
					mv[2] = m.dst_row;
					mv[3] = m.dst_col;
					mv[4] = m.piece;
					printf("ENGINE: %d %d %d %d %d\n", mv[0], mv[1], mv[2], mv[3], mv[4]);
					move(mv[0], mv[1], mv[2], mv[3], mv[4], board, false);
					write_pipe_data(pipe, mv, 5 * sizeof(byte));
					current_player ^= 1;
				}
				else {
					printf("Player has won!\n");
					byte w = 8;
					for (int x = 0; x < 5; x++) {
						write_pipe_data(pipe, &w, sizeof(byte));
					}
					get_pipe_data(pipe, &w, sizeof(byte));
					break;
				}
			}
		}
	}

	CloseHandle(pipe);

	return 0;
}