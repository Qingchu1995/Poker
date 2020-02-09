#include <time.h>
#include <iomanip>
#include <math.h>
#include <iostream>
//#include <omp.h>
#include "Card.h"
#include "AnalysisFactory.h"
#include "CardCompute.h"
//#include <fstream>

//int main() {
//	Card c1;
//	AnalysisFactory AF;
//	std::string fn = "test2.txt";
//	AF.first_2hands_prob_analysis(c1,fn);
//}

//int main() {
//	Card c1;
//	c1.test_bet();
//}

int main() {
	Card c1;
	AnalysisFactory af1;
	//std::vector<std::string> cards1_str = { "h2","s3" };
	//std::vector<std::string> cards2_str = { "hA","sA" };
	//CardCompute cc1;
	af1.play(c1, true);
	//c1.return_river();
	//c1.return_turn();
	//cc1.compute_prob_flop(c1);
	//std::cout << out;
	return 0;

}
