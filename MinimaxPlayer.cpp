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

typedef tuple<int, int> move; //col, row
typedef vector<move> move_vector; //vector of moves (col, row)


MinimaxPlayer::MinimaxPlayer(char symb) :
		Player(symb) {

}

MinimaxPlayer::~MinimaxPlayer() {

}


int MinimaxPlayer::min_value(OthelloBoard* currentBoard){

	int minValue = std::numeric_limits<int>::max();

	for( auto x : this->getSuccessorStates(currentBoard)){ //go through all possible moves after this
		minValue = min(minValue, this->max_value(&x));
	}

	return minValue;
}


int MinimaxPlayer::max_value(OthelloBoard* currentBoard){

	int maxValue = std::numeric_limits<int>::lowest();

	for( auto x : this->getSuccessorStates(currentBoard)){  //go through all possible moves after this
		maxValue = max(maxValue, this->min_value(&x));
	}

	return maxValue;
} 

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

	//Pretty much max function that keeps track of the first-level of moves so we can pass those back

	OthelloBoard currentBoard((OthelloBoard) *b);
	int bestScore = std::numeric_limits<int>::lowest();
	int currentScore;
	move bestMove;
	for( move x : b->get_possible_moves(this->symbol)){
		currentBoard = *b;
		currentScore = this->min_value(&currentBoard);
		if(currentScore > bestScore){
			bestScore = currentScore;
			bestMove  = x;
		}
	}
	col = get<0>(bestMove);
	row = get<1>(bestMove);
	currentBoard.play_move(col, row, this->symbol);
	return; 
}






vector<OthelloBoard> MinimaxPlayer::getSuccessorStates(OthelloBoard* currentBoard){
	
	move_vector possibleMoves = currentBoard->get_possible_moves(this->symbol);
	vector<OthelloBoard> successorStates;

	for(auto&& x : possibleMoves){ //Iterate through all the tuples in the possible moves
		OthelloBoard tempBoard(*currentBoard);
		tempBoard.play_move(get<0>(x), get<1>(x), this->symbol); 
		successorStates.push_back (tempBoard);
	}
	return successorStates;
}

int MinimaxPlayer::getGoodness(OthelloBoard* b) {
	int goodness = 0;

	if(this->symbol == 'X'){
		goodness = (b->count_score('O') - b->count_score('X'));
		if(!b->has_legal_moves_remaining(this->symbol) && goodness < 1){ //if minimax lost, subtract points
			goodness -= 4;
		}
			
	} else {	
		goodness = (b->count_score('X') - b->count_score('O'));
		if(!b->has_legal_moves_remaining(this->symbol) && goodness > 0){ //if minimax won, add points
			goodness += 4;
		}
	}

	return goodness;
} 




MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
