#ifndef common
#define common

#define WHITEKING 1
#define WHITEQUEEN 2
#define WHITEBISHOP 3
#define WHITEKNIGHT 4
#define WHITEROOK 5
#define WHITEPAWN 6
#define BLACKKING 7
#define BLACKQUEEN 8
#define BLACKBISHOP 9
#define BLACKKNIGHT 10
#define BLACKROOK 11
#define BLACKPAWN 12

#define IS_WHITE(p) (p < 7 && p)
#define IS_BLACK(p) (p > 6 && p < 13)

struct move_data {
    int src_row;
    int src_col;
    int dst_row;
    int dst_col;
    byte piece;
};

struct GameState {
	byte _board[8][8];
};

struct Node {
	int visits;
	double score;
	std::vector<Node*> children;
	Node* parent;
	GameState state;
	move_data md;
	Node() : visits(0), score(0.0), parent(nullptr) {}
};

move_data search(byte player);
void get_pipe_data(HANDLE pipe, byte* output, int size);
void write_pipe_data(HANDLE pipe, byte* data, int size);
byte move(int src_row, int src_col, int dst_row, int dst_col, byte piece, byte __board[8][8], bool search);
std::vector<move_data> get_possible_moves(byte player, byte __board[8][8], bool search);
double check_if_game_end(byte __board[8][8], byte p, bool search); 

extern byte board[8][8]; 

#endif