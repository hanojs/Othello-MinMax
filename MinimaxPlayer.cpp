/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
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


int MinimaxPlayer::min_value(OthelloBoard* currentBoard, char player){
	if(!currentBoard->has_legal_moves_remaining(player)){
		return getGoodness(currentBoard, player);	
	}
	int minValue = std::numeric_limits<int>::max();

	for( auto x : this->getSuccessorStates(currentBoard, player)){ //go through all possible moves after this
		minValue = min(minValue, this->max_value(&x));
	}

	return minValue;
}


int MinimaxPlayer::max_value(OthelloBoard* currentBoard){
	char opponent;
	if (this->get_symbol() == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}

	if(!currentBoard->has_legal_moves_remaining(this->get_symbol() )){
		return getGoodness(currentBoard, this->get_symbol() );	
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
	
	move_vector possibleMoves = currentBoard->get_possible_moves(player);
	vector<OthelloBoard> successorStates;

	for(auto&& x : possibleMoves){ //Iterate through all the tuples in the possible moves
		OthelloBoard tempBoard(*currentBoard);
		tempBoard.play_move(get<0>(x), get<1>(x), player); 
		successorStates.push_back (tempBoard);
	}
	return successorStates;
}


/***********************************
 * Utility function
 * Returns the goodness of a terminal node. 
 * ********************************/
int MinimaxPlayer::getGoodness(OthelloBoard* b, char player) {
	int goodness = 0;
	char opponent;

	if (this->get_symbol() == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}

	if(player == this->get_symbol()){
		goodness = (b->count_score(this->get_symbol()) - b->count_score(opponent));
	} else {	
		goodness = (b->count_score(opponent) - b->count_score(this->get_symbol()));
	}

	return goodness;
} 




MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
