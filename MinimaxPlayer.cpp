/*
 * MinimaxPlayer.cpp
 *
 *  Created on: Apr 17, 2015
 *      Author: wong
 */

#include <tuple>
#include <iostream>
#include <assert.h>
#include <vector>
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


int MinimaxPlayer::min_value(OthelloBoard* currentBoard){
	if(!currentBoard->has_legal_moves_remaining(this->symbol)){
		return getGoodness(currentBoard);
	}
	int value = 999999;

	for( auto x : this->getSuccessorStates(currentBoard)){
		value = min(value, this->max_value(&x));
	}

	return value;
}


int MinimaxPlayer::max_value(OthelloBoard* currentBoard){
	if(!currentBoard->has_legal_moves_remaining(this->symbol)){
		return getGoodness(currentBoard);
	}
	int value = -999999;

	for( auto x : this->getSuccessorStates(currentBoard)){
		value = max(value, this->min_value(&x));
	}

	return value;
} 

void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

	//Pretty much max function that keeps track of the first-level of moves so we can pass those back

	OthelloBoard currentBoard((OthelloBoard) *b);
	int bestScore = -999999, currentScore;
	move bestMove;
	cout << "Getting best move... \n" << std::endl;
	for( auto x : b->get_possible_moves(this->symbol)){
		cout << "possibleMoves: " << get<0>(x) << " and " <<  get<1>(x) << std::endl;
		currentBoard = *b;
		currentBoard.play_move(get<0>(x), get<1>(x), this->symbol);
		currentScore = this->min_value(&currentBoard);
		if(currentScore > bestScore){
			bestScore = currentScore;
			bestMove  = x;
		}
	}
	col = get<0>(bestMove);
	row = get<1>(bestMove);
	cout << "Col: " << col;
	cout << "row: " << row;
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
	
	if (b->get_cell(0,0) == this->symbol)
		goodness += 10;
	else if(!b->is_cell_empty(0, 0))
		goodness -= 10;

	if (b->get_cell(0,3) == this->symbol)
		goodness += 10;
	else if(!b->is_cell_empty(3, 3))
		goodness -= 10;

	if (b->get_cell(3,3) == this->symbol)
		goodness += 10;
	else if(!b->is_cell_empty(3, 3))
		goodness -= 10;
	
	if (b->get_cell(3,0) == this->symbol)
		goodness += 10;
	else if(!b->is_cell_empty(3, 3))
		goodness -= 10;


	if(this->symbol == 'O')
		goodness += (b->count_score('O') - b->count_score('X')) * 2;
	else
		goodness += (b->count_score('X') - b->count_score('O')) * 2;

	return goodness;
} 




MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
