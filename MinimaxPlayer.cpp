/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 * 
 * Student edits: Jonah Spencer
 * 	
 */

#include <tuple>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>    // std::max
#include "MinimaxPlayer.h"
#include "OthelloBoard.h"

using std::vector;
using std::max;
using std::min;
using std::tuple;
using std::get;
using std::cout;

typedef tuple<int, int> move; //col, row
typedef vector<move> move_vector; //vector of moves (col, row)


MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}

//min part of minimax
int MinimaxPlayer::min_value(OthelloBoard* currentBoard, char player){
	if(!currentBoard->has_legal_moves_remaining(player)){
		return utility(currentBoard, player);	
	}
	int minValue = std::numeric_limits<int>::max();

	for( auto x : this->getSuccessorStates(currentBoard, player)){ //go through all possible moves after this
		minValue = min(minValue, this->max_value(&x));
	}

	return minValue;
}

//Max part of minimax
int MinimaxPlayer::max_value(OthelloBoard* currentBoard){
	char opponent;
	if (this->get_symbol() == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}

	if(!currentBoard->has_legal_moves_remaining(this->get_symbol() )){
		return utility(currentBoard, this->get_symbol() );	
	}
	int maxValue = std::numeric_limits<int>::lowest();

	for( auto x : this->getSuccessorStates(currentBoard, this->get_symbol() )){  //go through all possible moves after this
		maxValue = max(maxValue, this->min_value(&x, opponent));
	}

	return maxValue;
} 

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

	//Pretty much max function that keeps track of the first-level of moves so we can pass those back

	OthelloBoard currentBoard((OthelloBoard) *b);
	int bestScore = std::numeric_limits<int>::lowest();
	int currentScore;
	move bestMove;
	char opponent;
	if (this->get_symbol()  == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}
	//initial maximize function -- keeps track of the best move
	for( auto x : b->get_possible_moves(this->get_symbol())){
		currentBoard = *b;
		currentScore = this->min_value(&currentBoard, opponent);
		if(currentScore > bestScore){
			bestScore = currentScore;
			bestMove  = x;
		}
	}
	col = get<0>(bestMove);
	row = get<1>(bestMove);
	currentBoard.play_move(col, row, this->get_symbol());
	return; 
}





//Gets all possible states within one move of the current board
vector<OthelloBoard> MinimaxPlayer::getSuccessorStates(OthelloBoard* currentBoard, char player){
	
	vector<OthelloBoard> successorStates;

	for(auto x : currentBoard->get_possible_moves(player)){ //Iterate through all the moves possible

		OthelloBoard tempBoard(*currentBoard);
		tempBoard.play_move(get<0>(x), get<1>(x), player); 
		successorStates.push_back (tempBoard);

	}
	return successorStates;
}


/***********************************
 * Utility function
 * Returns the goodness of a terminal node for the minimax player. 
 * ********************************/
int MinimaxPlayer::utility(OthelloBoard* b, char player) {

	char opponent;

	if (this->get_symbol() == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}

	return (b->count_score(this->get_symbol()) - b->count_score(opponent));

} 




MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}