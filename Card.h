#ifndef CARD_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // std::random_shuffle, sort
#include <cstdlib>      // std::rand, std::srand
#include <map>			// std::map
#include <utility>		// std::pair
#include <random>
#include "preparams.h"

class Card {
public:
	Card();

	void print_card(std::vector<int>);
	void print_card();
	void shuffle(bool printF=true);
	void pop_rand(int num, std::vector<int> &Poped_cards, bool printF=true);
	void pop_spec(std::vector<std::string> cards_str, std::vector<int> &cards, bool printF);

	int classify_5cards(std::vector<int> &cards);
	int compare_5cards(std::vector<int> &cards1, std::vector<int> &cards2, bool pF);
	int compare_5cards_in_hierarchy(std::vector<int> &cards1, std::vector<int> &cards2, int hierarchy);
	
	void select_5_from_7cards(std::vector<int> &in, std::vector<int> &out);

	int Perform_poker(std::vector<int> &cards1, std::vector<int> &cards2, bool pF);

	void convertStr2Num(std::vector<std::string> &in, std::vector<int> &out);


private:
	//void ConvertStr2Num(std::vector<std::string> &in, std::vector<int> &out);
	std::vector<int> deck;
	std::string face = "1234567890JQK";
	std::vector<std::string> suit;//heart spade diamond club

};


#endif // !CARD_H
