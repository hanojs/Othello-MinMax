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




// Utility function that returns the goodness of a terminal node for the minimax player. 
int MinimaxPlayer::utility(OthelloBoard* b) {

	char opponent;
	if (this->get_symbol() == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}

	return (b->count_score(this->get_symbol()) - b->count_score(opponent));

} 


//Chooses a move and plays that
void MinimaxPlayer::get_move(OthelloBoard* b, int& col, int& row) {

	//Pretty much max function that keeps track of the first-level of moves so we can pass those back

	OthelloBoard currentBoard((OthelloBoard) *b);
	int bestScore = std::numeric_limits<int>::lowest();
	int currentScore;
	move bestMove;


	//initial maximize function -- keeps track of the best move, and goes into the minimax functions
	for( auto x : b->get_possible_moves(this->get_symbol())){
		currentBoard = *b;
		currentScore = this->min_value(&currentBoard);
		if(currentScore > bestScore){
			bestScore = currentScore;
			bestMove  = x;
		}
	}

	currentBoard.play_move(get<0>(bestMove), get<1>(bestMove), this->get_symbol());
	return; 
}

//Min_value utility function for minimax (get_move)
int MinimaxPlayer::min_value(OthelloBoard* currentBoard, char player){
	
	//gets the opponent's symbol
	char opponent;
	if (this->get_symbol() == 'X') {
		opponent = 'O';
	}else {
		opponent = 'X';
	}
	
	if(!currentBoard->has_legal_moves_remaining(opponent)){
		return utility(currentBoard);	
	}
	int minValue = std::numeric_limits<int>::max();

	//go through all possible moves after this
	for( auto x : this->getSuccessorStates(currentBoard, opponent)){ 
		minValue = min(minValue, this->max_value(&x));
	}

	return minValue;
}

//Max_value utility function for minimax (get_move)
int MinimaxPlayer::max_value(OthelloBoard* currentBoard){
	if(!currentBoard->has_legal_moves_remaining(this->get_symbol() )){
		return utility(currentBoard, this->get_symbol() );	
	}
	int maxValue = std::numeric_limits<int>::lowest();

	//go through all possible moves after this
	for( auto x : this->getSuccessorStates(currentBoard, this->get_symbol() )){  
		maxValue = max(maxValue, this->min_value(&x, opponent));
	}

	return maxValue;
} 




//Returns all possible game states from all possible moves (1 deep) 
vector<OthelloBoard> MinimaxPlayer::getSuccessorStates(OthelloBoard* currentBoard, char player){
	
	vector<OthelloBoard> successorStates;

	//Iterate through all the moves possible
	for(auto x : currentBoard->get_possible_moves(player)){ 
		OthelloBoard tempBoard(*currentBoard);
		tempBoard.play_move(get<0>(x), get<1>(x), player); 
		successorStates.push_back(tempBoard);
	}
	//returns all possible game states
	return successorStates; 
}




MinimaxPlayer* MinimaxPlayer::clone() {
	MinimaxPlayer* result = new MinimaxPlayer(symbol);
	return result;
}
