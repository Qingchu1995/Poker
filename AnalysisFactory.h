#ifndef ANALYSISFACTORY_H
#define ANALYSISFACTORY_H
#include "Card.h"
#include <time.h>
#include <iomanip>
#include <fstream>
class AnalysisFactory {

public:
	//0: equal, 1: cards1 > cards2 , -1: cards1 < cards2
	int play_2p(Card &c1, bool unequalF, bool printF);// normal 2 people game
	void play(Card &c1, bool printF);
	int play_spec_cards1_2p(Card &c1, std::vector<std::string> &cards1_str, bool unequalF);
	int play_spec_two_cards_2p(Card &c1, std::vector<std::string> &cards1_str, std::vector<std::string> &cards2_str, bool unequalF);
	double  launch_play_spec_cards1_2p(Card &c1, std::vector<std::string> &cards1_str);
	void first_2hands_prob_analysis_2p(Card &c1, std::string fn);

	
private:


};


#endif // !ANALYSIS_FACTORY_H
