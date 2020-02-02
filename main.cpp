#include <time.h>
#include <iomanip>
#include <math.h>
//#include <omp.h>
#include "Card.h"
#include <fstream>



int play();
int play_spec_cards1(std::vector<std::string> &cards1_str);
double launch_play_spec_cards1(std::vector<std::string> &cards1_str);

int main() {
	std::vector<double> p(13 + 13 * 12);//13 paired, 78  non-pair non-suited, 78  non-pair suited, 
	std::filebuf fb;
	std::string fn = "p.txt";
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
		p[p_i] = launch_play_spec_cards1(cards1_str);
		os << cards1_str[0] << " " << cards1_str[1] << " " << p[p_i] <<"\n";
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
		p[p_i] = launch_play_spec_cards1(cards1_str);
		os << cards1_str[0] << " " << cards1_str[1] << " " << p[p_i] << "\n";
		p_i++;
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));


	// test non-paired non-suited
	bitmask.clear();
	bitmask="11"; // K leading 1's
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
		p[p_i] = launch_play_spec_cards1(cards1_str);
		os << cards1_str[0] << " " << cards1_str[1] << " " << p[p_i] << "\n";
		p_i++;
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

	fb.close();


	return 0;
}


double  launch_play_spec_cards1(std::vector<std::string> &cards1_str) {
	clock_t start, end;
	start = clock();
	double N = 0;
	int c1_ct = 0;
	int c2_ct = 0;
	int out;
	double p = 0.5;
	//std::vector<std::string> cards1_str = { "h2","s3" };
	while (sqrt(p*(1-p)/N)>0.0071 || N < 1000){
		out = play_spec_cards1(cards1_str);
		if (out == 1)  c1_ct += 1;
		++N;
		p = c1_ct / N;
	}
	end = clock();
	double time_taken = static_cast<double>(end - start) / double(CLOCKS_PER_SEC);
	std::cout << "Time taken by program is : " << std::fixed
		<< time_taken << std::setprecision(5);
	std::cout << " sec " << std::endl;
	std::cout << cards1_str[0]<<" "<< cards1_str[1]<<"wins" << c1_ct << "/"<<N<<"("<<p<<")."<<std::endl;
	//std::cout << c1_ct << std::endl;
	return p;
}

int play() {
	Card c1;
	bool printF = false;
	c1.shuffle(printF);
	std::vector<int> cards1;
	c1.pop_rand(2, cards1, printF);
	std::vector<int> cards2;
	c1.pop_rand(2, cards2, printF);

	int out = c1.Perform_poker(cards1, cards2,false);
	if (out == 0) out = play();
	return out;
}

int play_spec_cards1(std::vector<std::string> &cards1_str) {
	Card c1;
	bool printF = false;
	c1.shuffle(printF);
	std::vector<int> cards1;
	c1.pop_spec(cards1_str, cards1, printF);

	std::vector<int> cards2;
	c1.pop_rand(2, cards2, printF);

	int out = c1.Perform_poker(cards1, cards2, printF);
	if (out == 0) out = play();
	return out;
}

