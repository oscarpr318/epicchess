#include <windows.h>
#include <vector>
#include <mutex> 
#include <string> 
#include <random> 
#include <unordered_map>
#include "common.h"

struct PositionCache {
	std::unordered_map<unsigned long, Node*> positions;

	// Function to get a node from the cache
	Node* getNode(GameState& state) {
		std::hash<byte*> h;
		auto hash = h._Do_hash(reinterpret_cast<byte*>(&state));
		auto it = positions.find(hash);
		return (it != positions.end()) ? it->second : nullptr;
	}

	// Function to add a node to the cache
	void addNode(GameState& state, Node* node) {
		std::hash<byte*> h;
		auto hash = h._Do_hash(reinterpret_cast<byte*>(&state));
		positions[hash] = node;
	}
};

const int BOARD_SIZE = 8;
const int MAX_ITERATIONS = 10;
const double EXPLORATION_PARAM = 1.4;

GameState copy;
PositionCache cache;

void backpropagate(Node* node, double score) {

	// Propagate through the nodes parent nodes until root is reached
	while (node != nullptr) {
		node->score += score;
		node->visits++;
		// New node is the parent of the current node
		node = node->parent;
	}
}

Node* selectBestChild(Node* node) {
	double bestUCT = -std::numeric_limits<double>::infinity();
	Node* bestChild = nullptr;
	// Iterate through child nodes of current node
	for (const auto& child : node->children) {
		// Calculate value of this node

		double exploration = std::sqrt(std::log(node->visits) / child->visits);
		double uctValue = child->score / child->visits + EXPLORATION_PARAM * exploration;

		if (uctValue > bestUCT) {
			bestUCT = uctValue;
			bestChild = child;
		}
	}

	return bestChild;
}

double simulateRandomPlayout(Node* node, byte player) {

	byte p = player;
	std::vector<move_data> md;

	// Use a copy variable for the gamestate so the nodes state is not changed
	GameState currentState;
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			currentState._board[x][y] = node->state._board[x][y];
		}
	}
	double result = 0;

	while (true) {

		// Make sure the game has not finished
		// If it has, break out of the while loop
		if ((result = check_if_game_end(currentState._board, player, true)) != -1) {
			break;
		}

		// Check if the current position has been reached, if so get the node that was used previously from this current position
		Node* bestNode = cache.getNode(currentState);

		// If the previous node exists and its score is greater than the current nodes score, use that
		if (bestNode != nullptr) {
			if (bestNode->score > node->score) {
				move(bestNode->md.src_row, bestNode->md.src_col, bestNode->md.dst_row, bestNode->md.dst_col, bestNode->md.piece, currentState._board, true);
			}
		}

		// If it doesnt exist, choose a different move
		else {

			md = get_possible_moves(p, currentState._board, true);
			int r = rand() % md.size();
			move(md[r].src_row, md[r].src_col, md[r].dst_row, md[r].dst_col, md[r].piece, currentState._board, true);
		}

		// Change the player, XOR is used to change 1 to 0 or 0 to 1
		p ^= 1;
	}

	return result;
}

void expandNode(Node* node, GameState state, byte player) {

	std::vector<move_data> possibleMoves = get_possible_moves(player, state._board, true);
	for (const auto& mov : possibleMoves) {
		Node* child = new Node();
		child->parent = node;
		child->md = mov;
		child->state = state;
		node->children.push_back(child);
	}
}

Node* performMCTSearch(GameState initialState, byte player) {

	// Set random seed to vlaue of GetTickCount64() which is number of milliseconds since computer has been turned on
	// This ensures when a random value is chosen for the index of the child nodes vector rand() gives a different value
	// So the same move is unlikely to be chosen again
	srand(GetTickCount64());

	// Create new empty root node for the start of the search
	Node* root = new Node();
	root->visits = 1;
	root->score = 0.0;

	for (int x = 0; x < MAX_ITERATIONS; x++) {

		Node* currentNode = root;

		// Selection phase
		while (!currentNode->children.empty()) {
			currentNode = selectBestChild(currentNode);
		}
		// Expansion phase
		if (currentNode->visits > 0) {
			// From the currentNode, generate possible child nodes 
			expandNode(currentNode, initialState, player);
			if (!currentNode->children.empty()) {
				// Select random child node from current node
				int r = rand() % currentNode->children.size();
				currentNode = currentNode->children[r];
			}
		}

		// Simulation phase
		double score = simulateRandomPlayout(currentNode, player);

		// Backpropagation phase
		cache.addNode(currentNode->state, currentNode);
		backpropagate(currentNode, score);
	}


	Node* bestNode = nullptr;
	double bestScore = -std::numeric_limits<double>::infinity();
	for (const auto& child : root->children) {
		double childScore = child->score / child->visits;
		if (childScore > bestScore) {
			bestScore = childScore;
			bestNode = child;
		}
	}

	return bestNode;
}

move_data search(byte player) {

	// Set inital state of search to the current board position
	GameState initialState = { };
	for (int x = 0; x < BOARD_SIZE; x++) {
		for (int y = 0; y < BOARD_SIZE; y++) {
			initialState._board[x][y] = board[x][y];
		}
	}

	Node* bestMove = performMCTSearch(initialState, player);

	if (bestMove == nullptr) {
		// nullptr is returned if the current player is in checkmate since no move can be found
		return move_data{ -1,-1,-1,-1,0 };
	}
	return bestMove->md;
}