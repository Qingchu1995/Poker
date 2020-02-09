#include "CardCompute.h"

double CardCompute::compute_prob_flop(Card &c1)
{
	clock_t start, end;
	start = clock();
	double N = 0;
	int c1_ct = 0;
	int c2_ct = 0;
	int out;
	double p = 0.5;
	bool pF = false;

	while (sqrt(p*(1 - p) / N)>0.0071 || N < 1000) {
		out = c1.Perform_river_turn_replace(pF);
		if (out != 0) {
			if (out == 1)  c1_ct += 1;
			++N;
			p = c1_ct / N;
		}
	}
	end = clock();
	double time_taken = static_cast<double>(end - start) / double(CLOCKS_PER_SEC);
	std::cout << "Time taken by program is : " << std::fixed
		<< time_taken << std::setprecision(5);
	std::cout << " sec " << std::endl;
	std::cout << " hands1 (on the flop) wins" << c1_ct << "/" << N << "(" << p << ")." << std::endl;
	return p;
}
