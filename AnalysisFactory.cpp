#include "AnalysisFactory.h"

int AnalysisFactory::play_2p(Card &c1, bool unequalF, bool printF) {
	//Card c1;
	//bool printF = false;
	c1 = Card();
	c1.shuffle(false);

	//initialize the stacks
	c1.add_pool(1000, player1);
	c1.add_pool(1000, player2);

	//blind
	c1.set_sbID(player1);
	c1.add_pocket(5, player1);

	std::vector<int> hands1;
	c1.pop_rand(2, hands1, printF);
	c1.sethands1(hands1);

	std::vector<int> hands2;
	c1.pop_rand(2, hands2, printF);
	c1.sethands2(hands2);

	bool pF_hands = false;
	bool pF_flop = false;
	bool pF_river = false;
	bool betF = true;
	int out = c1.Perform_poker_2p(printF, pF_hands, pF_flop, pF_river, betF);

	if (unequalF) {
		if (out == 0) out = play_2p(c1, unequalF, printF);
	}
	if (out == 1) {
		std::cout << "player 1 wins!\n";
	}
	else if (out == 0) {
		std::cout << "even!\n";
	}
	else {
		std::cout << "player 2 wins!\n";
	}
		return out;
}

void AnalysisFactory::play(Card & c1, bool printF)
{
	//Card c1;
	//bool printF = false;
	c1 = Card();
	c1.shuffle(false);

	//initialize the stacks
	for (unsigned i = 0; i < playernum; ++i) {
		c1.add_pool(100, i);
	}
	//blind
	c1.set_sbID(player1);
	c1.add_pocket(5, player1);
	c1.add_pocket(10, player2);//big blind
	c1.initialize_hands(printF);

	bool pF_hands = false;
	bool pF_flop = false;
	bool pF_river = false;
	bool betF = true;
	std::vector<int> out;
	c1.Perform_poker(out,printF, pF_hands, pF_flop, pF_river, betF);

	for (unsigned i = 0; i < out.size(); ++i) {
		std::cout << "player " << out[i] << ", ";
	}
	std::cout << " wins!";
}

int AnalysisFactory::play_spec_cards1_2p(Card &c1, std::vector<std::string> &cards1_str, bool unequalF) {
	//Card c1;
	c1 = Card();
	bool printF = false;
	c1.shuffle(printF);
	std::vector<int> hands1;
	c1.pop_spec(cards1_str, hands1, printF);
	c1.sethands1(hands1);

	std::vector<int> hands2;
	c1.pop_rand(2, hands2, printF);
	c1.sethands2(hands2);
	bool pF_hands = true;
	bool pF_flop = true;
	bool pF_river = true;
	int out = c1.Perform_poker_2p(printF, pF_hands, pF_flop, pF_river);
	if (unequalF) {
		if (out == 0) out = play_spec_cards1_2p(c1, cards1_str,unequalF);
	}
	return out;
}

int AnalysisFactory::play_spec_two_cards_2p(Card & c1, std::vector<std::string>& cards1_str, std::vector<std::string>& cards2_str, bool unequalF)
{
	//Card c1;
	c1 = Card();
	bool printF = true;
	c1.shuffle(printF);
	std::vector<int> hands1;
	c1.pop_spec(cards1_str, hands1, printF);
	c1.sethands1(hands1);

	std::vector<int> hands2;
	c1.pop_spec(cards2_str, hands2, printF);
	c1.sethands2(hands2);

	bool pF_hands = true;
	bool pF_flop = true;
	bool pF_river = true;
	int out = c1.Perform_poker_2p(printF, pF_hands, pF_flop, pF_river);
	if (unequalF) {
		if (out == 0) out = play_spec_two_cards_2p(c1, cards1_str, cards2_str, unequalF);
	}
	return out;
}

double  AnalysisFactory::launch_play_spec_cards1_2p(Card &c1, std::vector<std::string> &cards1_str) {
	clock_t start, end;
	start = clock();
	double N = 0;
	int c1_ct = 0;
	int c2_ct = 0;
	int out;
	double p = 0.5;
	bool unequalF = true;
	//std::vector<std::string> cards1_str = { "h2","s3" };
	while (sqrt(p*(1 - p) / N)>0.0071 || N < 1000) {
		out = play_spec_cards1_2p(c1, cards1_str, unequalF);
		if (out == 1)  c1_ct += 1;
		++N;
		p = c1_ct / N;
	}
	end = clock();
	double time_taken = static_cast<double>(end - start) / double(CLOCKS_PER_SEC);
	std::cout << "Time taken by program is : " << std::fixed
		<< time_taken << std::setprecision(5);
	std::cout << " sec " << std::endl;
	std::cout << cards1_str[0] << " " << cards1_str[1] << "wins" << c1_ct << "/" << N << "(" << p << ")." << std::endl;
	//std::cout << c1_ct << std::endl;
	return p;
}

void AnalysisFactory::first_2hands_prob_analysis_2p(Card &c1, std::string fn)
{
	std::vector<double> p(13 + 13 * 12);//13 paired, 78  non-pair non-suited, 78  non-pair suited, 
	std::filebuf fb;
	//std::string fn = "p.txt";
	fb.open(fn, std::ios::out);
	std::ostream os(&fb);
	os << "cards1 cards2 p\n";

	int p_i = 0;
	// spade, heart 
	std::vector<std::string> cards1_str(2);
	// test pair hands
	std::string face = "234567890JQKA";
	/*auto tmp =typeid(face[0]);*/
	for (unsigned i = 0; i < 13; ++i) {
		cards1_str[0] = "s"; cards1_str[1] = "h";
		cards1_str[0] += face[i];
		cards1_str[1] += face[i];
		p[p_i] = launch_play_spec_cards1_2p(c1, cards1_str);
		os << cards1_str[0] << " " << cards1_str[1] << " " << p[p_i] << "\n";
		p_i++;
	}
	// test non-paired suited
	std::string bitmask(2, 1); // K leading 1's
	bitmask.resize(13, 0); // N-K trailing 0's
	do {
		cards1_str[0] = "s"; cards1_str[1] = "s";
		int j = 0;
		for (int i = 0; i < 13; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) {
				std::cout << i << "\t";
				cards1_str[j] += face[i];
				++j;
			}
		}
		std::cout << "\n";
		p[p_i] = launch_play_spec_cards1_2p(c1, cards1_str);
		os << cards1_str[0] << " " << cards1_str[1] << " " << p[p_i] << "\n";
		p_i++;
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));
	
	// test non-paired non-suited
	bitmask.clear();
	bitmask = "11"; // K leading 1's
	bitmask.resize(13, 0); // N-K trailing 0's
	do {
		cards1_str[0] = "s"; cards1_str[1] = "h";
		int j = 0;
		for (int i = 0; i < 13; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) {
				std::cout << i << "\t";
				cards1_str[j] += face[i];
				++j;
			}
		}
		std::cout << "\n";
		p[p_i] = launch_play_spec_cards1_2p(c1, cards1_str);
		os << cards1_str[0] << " " << cards1_str[1] << " " << p[p_i] << "\n";
		p_i++;
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

	fb.close();
}
