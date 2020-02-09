#include "Card.h"

Card::Card() {
	deck.resize(52);
	for (int i = 0; i < 52; ++i) deck[i] = i;
	face = "234567890JQKA";
	suit.resize(4);
	suit[0] = "spade"; suit[1] = "heart"; suit[2] = "club"; suit[3] = "diamond";
	hands.resize(playernum);
	/*pot = { 0,0,0,0,0 };
	pocket = { 0,0,0,0,0 };
	pool = { 1000,1000,1000,1000,1000 };*/
	pot.resize(playernum);
	pocket.resize(playernum);
	pool.resize(playernum);
	p_status.resize(playernum);
	for (unsigned i = 0; i < playernum; ++i) {
		pot[i] = 0;
		pocket[i] = 0;
		pool[i] = 0;
		p_status[i] = true;
	}
	avail_pnum = playernum;
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
	if (printF) print_card(cards);

}

void Card::initialize_hands(bool printF)
{
	for (unsigned i = 0; i < playernum; ++i) {
		pop_rand(2, hands[i], printF);
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

int Card::compare_5cards_2p(std::vector<int>& cards1, std::vector<int>& cards2, bool pF)
{
	std::vector<std::string> name = {"	royal_flush","straight_flush","four_of_a_kind","full_house","flush","straight","three_of_a_kind","two_pairs","one_pair","high_cards"};

	int hierarchy1 = classify_5cards(cards1);
	int hierarchy2 = classify_5cards(cards2);
	int out; //0:equal, 1: cards1 > cards2 , -1: cards1 < cards2
	if (hierarchy1 == hierarchy2) {
		if (pF) std::cout << "Two hands are in the same hierarchy: "<<name[hierarchy1]<<".\n\n";
		out = compare_5cards_in_hierarchy(cards1, cards2, hierarchy1);
	}
	else {
		if(pF){
			std::cout << "Card1 is in the hierarchy: " << name[hierarchy1] << ".\n";
			std::cout << "Card2 is in the hierarchy: " << name[hierarchy2] << ".\n\n";
		}
		
		hierarchy1 < hierarchy2 ? out = 1 : out = -1;
	}
	return out;
}

void Card::compare_5cards(std::vector<std::vector<int>>& cards_5, bool pF,std::vector<int> &out)
{
	for (unsigned i = 0; i < playernum; ++i) {
		print_card(cards_5[i]);
	}
	
	int max_i = 0;
	int id1 = -1;//-1 means no first player index
	std::vector<int> id1_equ = {-1};//
	int id2 = -1;
	int out_2p;
	for (unsigned i = 0; i < playernum; ++i) {
		if (get_status(i)) {
			if (id1 == -1) {
				id1 = i;//just for the first true status
				id1_equ[0] = i;
			}
			else if (id2 == -1) {
				id2 = i;
			}
		}
		if (id1 != -1 && id2 != -1) {//both players being selected
			out_2p = compare_5cards_2p(cards_5[id1], cards_5[id2], true);
			if (out_2p == 1) {//first player win
				id2 = -1;
			}
			else if (out_2p == -1) {//second player win
				id1 = id2;
				id2 = -1;
				id1_equ.resize(1);
				id1_equ[0] = id1;
			}
			else { //even out_2p=0
				id1_equ.insert(id1_equ.end(), id2);
				id2 = -1;//clear id2;

			}
		}
	}
	out = id1_equ;
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

void Card::sethands1(std::vector<int>& hands)
{
	hands1 = hands;
}

void Card::sethands2(std::vector<int>& hands)
{
	hands2 = hands;
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
		if (compare_5cards_2p(out_tmp, out,false) == 1) out = out_tmp;
	} while (std::prev_permutation(bitmask.begin(), bitmask.end()));

}

int Card::Perform_poker_2p(bool pF, bool pF_hands, bool pF_flop, bool pF_river, bool betF)
{
	int out;
	if (pF_hands) {
		compute_prob_hands();
	}
	if (betF) {
		betting(true,true);
		if (get_availpnum() == 1) {
			if (get_status(player1)) {
				pot2pool(player1);
				out = 1;
			}
			else {
				pot2pool(player2);
				out = -1;
			}
			print_betinfo();
			return out;
		}
	}
	on_the_flop(pF);

	if (pF_flop) {
		compute_prob_flop();
	}
	if (betF) {
		betting(true, false);
		if (get_availpnum() == 1) {
			if (get_status(player1)) {
				pot2pool(player1);
				out = 1;
			}
			else {
				pot2pool(player2);
				out = -1;
			}
			print_betinfo();
			return out;
		}
	}
	on_the_river(pF);
	if (pF_river) {
		compute_prob_river();
	}
	if (betF) {
		betting(true, false);
		if (get_availpnum() == 1) {
			if (get_status(player1)) {
				pot2pool(player1);
				out = 1;
			}
			else {
				pot2pool(player2);
				out = -1;
			}
			print_betinfo();
			return out;
		}
	}

	on_the_turn(pF);

	if (betF) {
		betting(true, false);
		if (get_availpnum() == 1) {
			if (get_status(player1)) {
				pot2pool(player1);
				out = 1;
			}
			else {
				pot2pool(player2);
				out = -1;
			}
			print_betinfo();
			return out;
		}
	}

	public_card = flop_cards;
	public_card.insert(public_card.end(), river_cards[0]);
	public_card.insert(public_card.end(), turn_cards[0]);

	std::vector<int> cards1 = hands1;
	std::vector<int> cards2 = hands2;

	cards1.insert(cards1.end(), public_card.begin(), public_card.end());
	cards2.insert(cards2.end(), public_card.begin(), public_card.end());
	std::vector<int> cards1_5(5);
	std::vector<int> cards2_5(5);

	select_5_from_7cards(cards1, cards1_5);
	select_5_from_7cards(cards2, cards2_5);
	out = compare_5cards_2p(cards1_5, cards2_5, pF);
	if (betF) {
		if (out == 1) {
			pot2pool(player1);
		}
		else if (out == -1) {
			pot2pool(player2);
		}
		else {
			std::vector<int>pID = { player1,player2 };
			pot2pool(pID);
		}
		print_betinfo();
	}
	return out;
}

void Card::Perform_poker(std::vector<int> &out, bool pF, bool pF_hands, bool pF_flop, bool pF_river, bool betF)
{
	out = {-1};//unreferenced player index
	if (pF_hands) {
		//compute_prob_hands();
	}
	if (betF) {
		betting(true, true);
		if (get_availpnum() == 1) {
			for (unsigned i = 0; i < playernum; ++i) {
				if (get_status(i)) {
					pot2pool(i);
					out[0] = i;
					return;
				}
			}
			print_betinfo();
		}
	}
	on_the_flop(pF);

	if (pF_flop) {
		//compute_prob_flop();
	}
	if (betF) {
		betting(true, false);
		if (get_availpnum() == 1) {
			for (unsigned i = 0; i < playernum; ++i) {
				if (get_status(i)) {
					pot2pool(i);
					out[0] = i;
					return;
				}
			}
			print_betinfo();
		}
	}
	on_the_river(pF);
	if (pF_river) {
		//compute_prob_river();
	}
	if (betF) {
		betting(true, false);
		if (get_availpnum() == 1) {
			for (unsigned i = 0; i < playernum; ++i) {
				if (get_status(i)) {
					pot2pool(i);
					out[0] = i;
					return;
				}
			}
			print_betinfo();
		}
	}

	on_the_turn(pF);

	if (betF) {
		betting(true, false);
		if (get_availpnum() == 1) {
			for (unsigned i = 0; i < playernum; ++i) {
				if (get_status(i)) {
					pot2pool(i);
					out[0] = i;
					return;
				}
			}
			print_betinfo();
		}
	}

	public_card = flop_cards;
	public_card.insert(public_card.end(), river_cards[0]);
	public_card.insert(public_card.end(), turn_cards[0]);

	std::vector<std::vector<int>> cards = hands;
	std::vector<std::vector<int>> cards_5(playernum);
	//cards_5.resize(playernum);
	for (unsigned i = 0; i < playernum; ++i) {
		if (get_status(i)) {
			cards[i].insert(cards[i].end(), public_card.begin(), public_card.end());
			cards_5[i].resize(5);
			select_5_from_7cards(cards[i], cards_5[i]);
		}
	}
	compare_5cards(cards_5, pF, out);
	if (betF) {
		if (out.size() == 1) {
			pot2pool(out[0]);
		}
		else {
			pot2pool(out);
		}
		print_betinfo();
	}
}
void Card::on_the_flop(bool printF)
{
	pop_rand(3, flop_cards, printF);
}

void Card::on_the_river(bool printF)
{
	pop_rand(1, river_cards, printF);
}

void Card::on_the_turn(bool printF)
{
	pop_rand(1, turn_cards, printF);
}

void Card::return_river()
{
	deck.insert(deck.end(), river_cards[0]);
}

//return the turn card into the deck
void Card::return_turn()
{
	deck.insert(deck.end(), turn_cards[0]);
}

//has to be performed before the flop cards are given
int Card::Perform_public_replace(bool pF)
{
	on_the_flop(pF);
	on_the_river(pF);
	on_the_turn(pF);

	public_card = flop_cards;
	public_card.insert(public_card.end(), river_cards[0]);
	public_card.insert(public_card.end(), turn_cards[0]);

	std::vector<int> cards1 = hands1;
	std::vector<int> cards2 = hands2;

	cards1.insert(cards1.end(), public_card.begin(), public_card.end());
	cards2.insert(cards2.end(), public_card.begin(), public_card.end());
	std::vector<int> cards1_5(5);
	std::vector<int> cards2_5(5);

	select_5_from_7cards(cards1, cards1_5);
	select_5_from_7cards(cards2, cards2_5);
	int out = compare_5cards_2p(cards1_5, cards2_5, pF);

	//return river and turn
	deck.insert(deck.end(), flop_cards.begin(),flop_cards.end());
	deck.insert(deck.end(), river_cards[0]);
	deck.insert(deck.end(), turn_cards[0]);
	shuffle(pF);

	return out;
}

int Card::Perform_river_turn_replace(bool pF)
{
	on_the_river(pF);
	on_the_turn(pF);

	public_card = flop_cards;
	public_card.insert(public_card.end(), river_cards[0]);
	public_card.insert(public_card.end(), turn_cards[0]);

	std::vector<int> cards1 = hands1;
	std::vector<int> cards2 = hands2;

	cards1.insert(cards1.end(), public_card.begin(), public_card.end());
	cards2.insert(cards2.end(), public_card.begin(), public_card.end());
	std::vector<int> cards1_5(5);
	std::vector<int> cards2_5(5);

	select_5_from_7cards(cards1, cards1_5);
	select_5_from_7cards(cards2, cards2_5);
	int out = compare_5cards_2p(cards1_5, cards2_5, pF);

	//return river and turn
	deck.insert(deck.end(), river_cards[0]);
	deck.insert(deck.end(), turn_cards[0]);
	shuffle(pF);

	return out;
}

int Card::Perform_turn_replace(bool pF)
{
	on_the_turn(pF);

	public_card = flop_cards;
	public_card.insert(public_card.end(), river_cards[0]);
	public_card.insert(public_card.end(), turn_cards[0]);

	std::vector<int> cards1 = hands1;
	std::vector<int> cards2 = hands2;

	cards1.insert(cards1.end(), public_card.begin(), public_card.end());
	cards2.insert(cards2.end(), public_card.begin(), public_card.end());
	std::vector<int> cards1_5(5);
	std::vector<int> cards2_5(5);

	select_5_from_7cards(cards1, cards1_5);
	select_5_from_7cards(cards2, cards2_5);
	int out = compare_5cards_2p(cards1_5, cards2_5, pF);

	//return river and turn
	deck.insert(deck.end(), turn_cards[0]);
	shuffle(pF);

	return out;
}

void Card::add_pool(double pl, int pID)
{
	pool[pID] += pl;
}

void Card::add_pocket(double pk, int pID)
{
	pocket[pID] += pk;
	pool[pID] -= pk;
}

void Card::fold_player(int pID)
{
	p_status[pID] = false;
}

double Card::get_pocket(int pID)
{
	return pocket[pID];
}

double Card::get_pool(int pID)
{
	return pool[pID];
}

bool Card::get_status(int pID)
{
	return p_status[pID];
}

double Card::get_pot(int pID)
{
	return pot[pID];
}

int Card::get_availpnum()
{
	return avail_pnum;	
}

void Card::pocket2pot(int pID)
{
	pot[pID] += pocket[pID];
	pocket[pID] = 0;
}

void Card::pot2pool(std::vector<int> pID)
{
	int num = pID.size();
	for (int i = 0; i < playernum; ++i) {
		for (int j = 0; j < pID.size(); ++j) {
			pool[pID[j]] += pot[i] / num;
		}
		pot[i] = 0;		
	}
}

void Card::pot2pool(int pID)
{
	for (int i = 0; i < playernum; ++i) {
		pool[pID] += pot[i];
		pot[i] = 0;
	}
}

void Card::betting(bool debugF,bool preflopF)
{
	if (debugF) {
		std::cout << "pool: ";
		for (int i = 0; i < playernum; ++i) {
			std::cout << get_pool(i) << " ";
		}
		std::cout << "\n";
		std::cout << "pocket: ";
		for (int i = 0; i < playernum; ++i) {
			std::cout << get_pocket(i) << " ";
		}
		std::cout << "\n";
		std::cout << "pot: ";
		for (int i = 0; i < playernum; ++i) {
			std::cout << get_pot(i) << " ";
		}
		std::cout << "\n";
		std::cout << "status: ";
		for (int i = 0; i < playernum; ++i) {
			std::cout << get_status(i) << " ";
		}
		std::cout << "\n";

	}

	bool done = false;
	int cpid;
	preflopF ? cpid = sb_ID + 2 : cpid = sb_ID;
	if (cpid == playernum) cpid = 0;
	//int cpid = sb_ID;//current id actually differ between preflop bet and other bets
	int action = 0;//count the consecutive fold or call
	int lr_ID;// = sb_ID + 1;//last raise id (initially the big blind)
	preflopF ? lr_ID = sb_ID + 1 : lr_ID = sb_ID;
	while (!done) {
		int action;
		if (get_status(cpid))
		{
			std::cout << "player " << cpid + 1 << " fold (0), call (1) or raise (2)?  ";
			std::cin >> action;
			if (action == 0) {
				pocket2pot(cpid);
				fold_player(cpid);
				avail_pnum -= 1;
			}
			else if (action == 1) {
				add_pocket(get_pocket(lr_ID) - get_pocket(cpid), cpid);
			}
			else {
				double raise;
				std::cout << "player " << cpid + 1 << " how much?  ";
				std::cin >> raise;
				while (raise > get_pool(cpid) || raise <= (get_pocket(lr_ID) - get_pocket(cpid))) {
					std::cout << "Invalid input\n";
					std::cout << "player " << cpid + 1 << " how much?  ";
					std::cin >> raise;
				}
				add_pocket(raise, cpid);
				lr_ID = cpid;
			}
		}
		if (debugF) {
			std::cout << "pool: ";
			for (int i = 0; i < playernum; ++i) {
				std::cout << get_pool(i) << " ";
			}
			std::cout << "\n";
			std::cout << "pocket: ";
			for (int i = 0; i < playernum; ++i) {
				std::cout << get_pocket(i) << " ";
			}
			std::cout << "\n";
			std::cout << "pot: ";
			for (int i = 0; i < playernum; ++i) {
				std::cout << get_pot(i) << " ";
			}
			std::cout << "\n";
			std::cout << "status: ";
			for (int i = 0; i < playernum; ++i) {
				std::cout << get_status(i) << " ";
			}
			std::cout << "\n";
		}
		++cpid;
		if (cpid == playernum) cpid = 0;
		if (lr_ID == cpid || avail_pnum==1) {
			done = true;
			for (int i = 0; i < playernum; ++i) {
				pocket2pot(i);
			}
		}

	}
}

void Card::print_betinfo()
{
	std::cout << "pool: ";
	for (int i = 0; i < playernum; ++i) {
		std::cout << get_pool(i) << " ";
	}
	std::cout << "\n";
	std::cout << "pocket: ";
	for (int i = 0; i < playernum; ++i) {
		std::cout << get_pocket(i) << " ";
	}
	std::cout << "\n";
	std::cout << "pot: ";
	for (int i = 0; i < playernum; ++i) {
		std::cout << get_pot(i) << " ";
	}
	std::cout << "\n";

	std::cout << "status: ";
	for (int i = 0; i < playernum; ++i) {
		std::cout << get_status(i) << " ";
	}
	std::cout << "\n";

}

void Card::test_bet()
{
	betting(true,true);
}

void Card::set_sbID(int pID)
{
	sb_ID = pID;
}

void Card::convertStr2Num(std::vector<std::string> &in, std::vector<int> &out)
{
	if (out.size() != in.size()) out.resize(in.size());
	for (unsigned i = 0; i < in.size(); ++i) {
		if (in[i][0] == 'S' || in[i][0] == 's') {
			out[i] = static_cast<int>(face.find(in[i][1]));
		}
		else if (in[i][0] == 'H' || in[i][0] == 'h') {
			out[i] = static_cast<int>(face.find(in[i][1])) + 13;
		}
		else if (in[i][0] == 'C' || in[i][0] == 'c') {
			out[i] = static_cast<int>(face.find(in[i][1])) + 26;
		}
		else if (in[i][0] == 'D' || in[i][0] == 'd') {
			out[i] = static_cast<int>(face.find(in[i][1])) + 39;
		}
		else { std::cout << "unknown card\n"; break; }
	}
}

double Card::compute_prob_hands()
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
		out = Perform_public_replace(pF);
		if (out != 0) {
			if (out == 1)  c1_ct += 1;
			++N;
			p = c1_ct / N;
		}
	}
	end = clock();
	double time_taken = static_cast<double>(end - start) / double(CLOCKS_PER_SEC);
	//std::cout << "Time taken by program is : " << std::fixed
	//	<< time_taken << std::setprecision(5);
	//std::cout << " sec " << std::endl;
	std::cout << " hands1 (on the hands) wins " << c1_ct << "/" << N << "(" << p << ")." << std::endl;
	return p;
}

double Card::compute_prob_flop()
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
		out = Perform_river_turn_replace(pF);
		if (out != 0) {
			if (out == 1)  c1_ct += 1;
			++N;
			p = c1_ct / N;
		}
	}
	end = clock();
	double time_taken = static_cast<double>(end - start) / double(CLOCKS_PER_SEC);
	//std::cout << "Time taken by program is : " << std::fixed
	//	<< time_taken << std::setprecision(5);
	//std::cout << " sec " << std::endl;
	std::cout << " hands1 (on the flop) wins " << c1_ct << "/" << N << "(" << p << ")." << std::endl;
	return p;
}

double Card::compute_prob_river()
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
		out = Perform_turn_replace(pF);
		if (out != 0) {
			if (out == 1)  c1_ct += 1;
			++N;
			p = c1_ct / N;
		}
	}
	end = clock();
	double time_taken = static_cast<double>(end - start) / double(CLOCKS_PER_SEC);
	//std::cout << "Time taken by program is : " << std::fixed
	//	<< time_taken << std::setprecision(5);
	//std::cout << " sec " << std::endl;
	std::cout << " hands1 (on the river) wins " << c1_ct << "/" << N << "(" << p << ")." << std::endl;
	return p;
}