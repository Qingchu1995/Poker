#include "Card.h"

Card::Card() {
	deck.resize(52);
	for (int i = 0; i < 52; ++i) deck[i] = i;
	face = "234567890JQKA";
	suit.resize(4);
	suit[0] = "spade"; suit[1] = "heart"; suit[2] = "club"; suit[3] = "diamond";
}

//overload function for default deck print
void Card::print_card(){
	print_card(deck);
}

void Card::print_card(std::vector<int> deck)
{
	//std::cout << "The existed cards in the deck are\n";
	for (unsigned i = 0; i < deck.size(); i++) {
		int a = deck[i] / 13;
		switch (a) {
		case 0:
			std::cout << suit[0] << " " << face[deck[i]] << '\t';
			break;
		case 1:
			std::cout << suit[1] << " " << face[deck[i]-13] << '\t';
			break;
		case 2:
			std::cout << suit[2] << " " << face[deck[i]-26] << '\t';
			break;
		case 3:
			std::cout << suit[3] << " " << face[deck[i]-39] << '\t';
			break;
		default:
			std::cout << "deck[i]>52\n";
		}
		
	}
	std::cout << "\n";
}

void Card::shuffle(bool printF)
{
	std::random_device device;
	std::mt19937 generator(device());
	std::shuffle(deck.begin(), deck.end(), generator);
	if (printF) {
		print_card();
	}
}

void Card::pop_rand(int num, std::vector<int> &Poped_cards, bool printF)
{
	Poped_cards.resize(num);
	//int *Poped_cards = new int[num];
	for (unsigned i = 0; i < num; i++) {
		Poped_cards[i] = deck.back(); 
		deck.pop_back();
	}
	if(printF) print_card(Poped_cards);
}

void Card::pop_spec(std::vector<std::string> cards_str, std::vector<int> &cards, bool printF)
{
	convertStr2Num(cards_str, cards);
	
	for (unsigned i = 0; i < cards.size(); i++) {
		deck.erase(std::remove(deck.begin(), deck.end(), cards[i]), deck.end());
	}
	if (printF) {
		print_card();
	}

}

//please only insert 5 cards
int Card::classify_5cards(std::vector<int>& cards)
{
	int hierarchy;

	//1. decide if there is any pair
	std::vector<int> c_tmp = cards;
	for (unsigned i = 0; i < cards.size(); i++) {
		c_tmp[i] = cards[i] - 13*(cards[i] / 13);
	}
	// Create a map to store the frequency of each element in vector
	std::map<int, int> countMap;
	// Iterate over the vector and store the frequency of each element in map
	for (auto & elem : c_tmp)
	{
		auto result = countMap.insert(std::pair<int, int>(elem, 1));
		if (result.second == false)
			result.first->second++;
	}
	if (countMap.size() == 2) {//1+4 or 2+3
		if (countMap[c_tmp[0]] == 1 | countMap[c_tmp[0]] == 4) {
			hierarchy = four_of_a_kind;
		}
		else {//2+3
			hierarchy = full_house;
		}
		return hierarchy;
	}
	else if (countMap.size() == 3) {
		for (unsigned i = 0; i < cards.size(); i++) {
			if (countMap[c_tmp[i]] == 3) {
				hierarchy = three_of_a_kind;
				break;
			}
			else if (countMap[c_tmp[i]] == 2) {
				hierarchy = two_pairs;
				break;
			}
		}
		return hierarchy;
	}
	else if (countMap.size() == 4) {
		hierarchy = one_pair;
		return hierarchy;
	}
	
	//2.1 whether straight
	bool straight_J = true;
	bool royal_J;
	std::sort(c_tmp.begin(), c_tmp.end());
	for (unsigned i = 0; i < cards.size() - 1; i++) {
		if (c_tmp[i + 1] - c_tmp[i] != 1) {
			straight_J = false;
			break;
		}
	}
	if (c_tmp.back() == 12) {
		royal_J = true;
	}
	else {
		royal_J = false;
	}


	//2.2 whether flush
	for (unsigned i = 0; i < cards.size(); i++) {
		c_tmp[i] = cards[i] / 13;
	}
	bool flush_J=true;
	for (unsigned i = 0; i < cards.size()-1; i++) {
		flush_J = flush_J && (c_tmp[i]== c_tmp[i+1]);
		if (!flush_J) break;
	}

	if (flush_J && (!straight_J)) {
		hierarchy = flush;
	}
	else if ((!flush_J) && straight_J) {
		hierarchy = straight;
	}
	else if (flush_J && straight_J) {
		if (royal_J) {
			hierarchy = royal_flush;
		}
		else {
			hierarchy = straight_flush;
		}
		
	}
	else {
		hierarchy = high_cards;
	}
	return hierarchy;

}

int Card::compare_5cards(std::vector<int>& cards1, std::vector<int>& cards2, bool pF)
{
	std::vector<std::string> name = {"	royal_flush","straight_flush","four_of_a_kind","full_house","flush","straight","three_of_a_kind","two_pairs","one_pair","high_cards"};

	int hierarchy1 = classify_5cards(cards1);
	int hierarchy2 = classify_5cards(cards2);
	int out; //0:equal, 1: cards1 > cards2 , -1: cards1 < cards2
	if (hierarchy1 == hierarchy2) {
		if (pF) std::cout << "Two hands are in the same hierarchy: "<<name[hierarchy1]<<".\n";
		out = compare_5cards_in_hierarchy(cards1, cards2, hierarchy1);
	}
	else {
		if(pF){
			std::cout << "Card1 is in the hierarchy: " << name[hierarchy1] << ".\n";
			std::cout << "Card2 is in the hierarchy: " << name[hierarchy2] << ".\n";
		}
		
		hierarchy1 < hierarchy2 ? out = 1 : out = -1;
	}
	return out;
}

int compare_pair_num(std::vector<int>& c_tmp1, std::vector<int>& c_tmp2, int pairnum, int numOfpair) {
	std::map<int, int> cM1, cM2;
	// Iterate over the vector and store the frequency of each element in map
	for (auto & elem : c_tmp1)
	{
		auto result = cM1.insert(std::pair<int, int>(elem, 1));
		if (result.second == false)
			result.first->second++;
	}
	for (auto & elem : c_tmp2)
	{
		auto result = cM2.insert(std::pair<int, int>(elem, 1));
		if (result.second == false)
			result.first->second++;
	}
	std::vector<int> c1(numOfpair);
	std::vector<int> c2(numOfpair);
	int i = 0;
	for (auto & elem : cM1)
	{
		if (elem.second == pairnum)
		{
			c1[i] = elem.first;
			++i;
		}
	}
	std::sort(c1.begin(), c1.end());
	i = 0;
	for (auto & elem : cM2)
	{
		if (elem.second == pairnum)
		{
			c2[i] = elem.first;
			++i;
		}
	}
	std::sort(c2.begin(), c2.end());
	int out;
	for (unsigned i = 0; i < numOfpair; ++i) {
		if (c1.back() > c2.back()) {
			out = 1;
			return out;
		}
		else if (c1.back() < c2.back()) {
			out = -1;
			return out;
		}
		else if ((c1.back() == c2.back()) && c1.size() == 1) {
			out = 0;
			return out;
		}
		c1.pop_back();
		c2.pop_back();
	}
}

int Card::compare_5cards_in_hierarchy(std::vector<int>& cards1, std::vector<int>& cards2, int hierarchy)
{
	int out; //0:equal, 1: cards1 > cards2 , -1: cards1 < cards2
	std::vector<int>c_tmp1 = cards1;
	std::vector<int>c_tmp2 = cards2;
	switch (hierarchy)
	{
	case royal_flush:
		out = 0;
		break;
	case straight_flush:
		std::sort(c_tmp1.begin(), c_tmp1.end());
		std::sort(c_tmp2.begin(), c_tmp2.end());
		if (c_tmp1.back() > c_tmp2.back()) {
			out = 1;
		}
		else if(c_tmp1.back() < c_tmp2.back()) {
			out = -1;
		}
		else {
			out = 0;
		}
		break;

	case four_of_a_kind:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 4,1);
		if (out == 0) {
			out = compare_pair_num(c_tmp1, c_tmp2, 1,1);
		}
		break;
	}
	case full_house:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 3,1);
		if (out == 0) {
			out = compare_pair_num(c_tmp1, c_tmp2, 2,1);
		}
		break;
	}
	case flush:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 1, 5);
		break;
	}

	case straight:
		std::sort(c_tmp1.begin(), c_tmp1.end());
		std::sort(c_tmp2.begin(), c_tmp2.end());
		if (c_tmp1.back() > c_tmp2.back()) {
			out = 1;
		}
		else if (c_tmp1.back() < c_tmp2.back()) {
			out = -1;
		}
		else {
			out = 0;
		}
		break;
	case three_of_a_kind:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 3, 1);
		if (out == 0) {
			out = compare_pair_num(c_tmp1, c_tmp2, 1, 2);
		}
		break;
	}
	case two_pairs:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 2, 2);
		if (out == 0) {
			out = compare_pair_num(c_tmp1, c_tmp2, 1, 1);
		}
		break;
	}
	case one_pair:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 2, 1);
		if (out == 0) {
			out = compare_pair_num(c_tmp1, c_tmp2, 1, 3);
		}
		break;
	}
	case high_cards:
	{
		for (unsigned i = 0; i < cards1.size(); i++) {
			c_tmp1[i] = cards1[i] - 13 * (cards1[i] / 13);
			c_tmp2[i] = cards2[i] - 13 * (cards2[i] / 13);
		}
		out = compare_pair_num(c_tmp1, c_tmp2, 1, 5);
		break;
	}
	default:
		break;
	}
	return out;
}

void Card::select_5_from_7cards(std::vector<int>& in, std::vector<int>& out)
{
	out[0] = in[0]; out[1] = in[1]; out[2] = in[2]; out[3] = in[3]; out[4] = in[4];
	std::vector<int> out_tmp = out;
	std::string bitmask(5, 1); // K leading 1's
	bitmask.resize(7, 0); // N-K trailing 0's
	// print integers and permute bitmask
	do {
		int j = 0;
		for (int i = 0; i < 7; ++i) // [0..N-1] integers
		{
			if (bitmask[i]) {
				out_tmp[j] = in[i];
				++j;
			}
		}
		if (compare_5cards(out_tmp, out,false) == 1) out = out_tmp;
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

}

int Card::Perform_poker(std::vector<int>& cards1, std::vector<int>& cards2,bool pF)
{
	std::vector<int> public_card;
	pop_rand(5, public_card, pF);
	cards1.insert(cards1.end(), public_card.begin(), public_card.end());
	cards2.insert(cards2.end(), public_card.begin(), public_card.end());
	std::vector<int> cards1_5(5);
	std::vector<int> cards2_5(5);

	select_5_from_7cards(cards1, cards1_5);
	select_5_from_7cards(cards2, cards2_5);
	int out = compare_5cards(cards1_5, cards2_5, pF);
	return out;
}

void Card::convertStr2Num(std::vector<std::string> &in, std::vector<int> &out)
{
	if (out.size() != in.size()) out.resize(in.size());
	for (unsigned i = 0; i < in.size(); ++i) {
		if (in[i][0] == 'S' || in[i][0] == 's') {
			out[i] = static_cast<int>(face.find(in[i][1]));
		}
		else if (in[i][0] == 'H' || in[i][0] == 'h') {
			out[i] = static_cast<int>(face.find(in[i][1]))+13;
		}
		else if (in[i][0] == 'C' || in[i][0] == 'c') {
			out[i] = static_cast<int>(face.find(in[i][1])) + 26;
		}
		else if (in[i][0] == 'D' || in[i][0] == 'd') {
			out[i] = static_cast<int>(face.find(in[i][1])) + 39;
		}
		else { std::cout << "unknown card\n"; break;}
	}
}
