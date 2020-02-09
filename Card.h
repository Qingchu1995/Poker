#ifndef CARD_H
#define CARD_H
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>    // std::random_shuffle, sort
#include <cstdlib>      // std::rand, std::srand
#include <map>			// std::map
#include <utility>		// std::pair
#include <random>
#include <time.h>
#include <iomanip>
#include <iostream>
#include "preparams.h"


class Card {
public:
	Card();

	void print_card(std::vector<int>);
	void print_card();
	void shuffle(bool printF=true);
	void pop_rand(int num, std::vector<int> &Poped_cards, bool printF=true);
	void pop_spec(std::vector<std::string> cards_str, std::vector<int> &cards, bool printF);

	void initialize_hands(bool printF);

	int classify_5cards(std::vector<int> &cards);
	int compare_5cards_2p(std::vector<int> &cards1, std::vector<int> &cards2, bool pF);
	void compare_5cards(std::vector<std::vector<int>>& cards_5, bool pF, std::vector<int> &out);
	int compare_5cards_in_hierarchy(std::vector<int> &cards1, std::vector<int> &cards2, int hierarchy);

	//set hands;
	void sethands1(std::vector<int> &hands);
	void sethands2(std::vector<int> &hands);
	
	void select_5_from_7cards(std::vector<int> &in, std::vector<int> &out);

	//play poker
	int Perform_poker_2p(bool pF = false, bool pF_hands = false,bool pF_flop = false, bool pF_river = false, bool betF=false);
	void Perform_poker(std::vector<int> &out, bool pF = false, bool pF_hands = false, bool pF_flop = false, bool pF_river = false, bool betF = false);

	void convertStr2Num(std::vector<std::string> &in, std::vector<int> &out);
	double compute_prob_hands();
	double compute_prob_flop();
	double compute_prob_river();
	
	//flop 1-3
	void on_the_flop(bool printF = false);
	//river 4
	void on_the_river(bool printF = false);
	//turn 5
	void on_the_turn(bool printF = false);

	void return_river();
	void return_turn();

	int Perform_public_replace(bool pF);

	//analyze the 2hands + on the flop
	int Perform_river_turn_replace(bool pF);
	int Perform_turn_replace(bool pF);
	
	void add_pool(double pl, int pID);
	void add_pocket(double pk, int pID);
	void fold_player(int pID);
	double get_pocket(int pID);
	double get_pool(int pID);
	bool get_status(int pID);
	double get_pot(int pID);
	int get_availpnum();
	void pocket2pot(int pID);
	void pot2pool(std::vector<int> pID);//win by pID
	void pot2pool(int pID);//win by pID
	void set_sbID(int pID);

	void betting(bool debugF,bool preflopF);
	void print_betinfo();
	void test_bet();

private:
	std::vector<int> deck;
	std::string face = "1234567890JQK";
	std::vector<std::string> suit;//heart spade diamond club

	std::vector<int> flop_cards;//3
	std::vector<int> river_cards;//1
	std::vector<int> turn_cards;//1
	std::vector<int> public_card; //5

	std::vector<std::vector<int>> hands;
	std::vector<int> hands1;//2
	std::vector<int> hands2;//2
	std::vector<double> pot;//2
	std::vector<double> pocket;//2
	std::vector<double> pool;//2
	std::vector<bool> p_status;//2 whether they are still in the game (fold or not)
	int sb_ID;
	int bb_ID;
	int avail_pnum;

};


#endif // !CARD_H
