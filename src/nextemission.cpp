#include <iostream>
#include "HiddenMarkovModel.hpp"


int main() {
	HMM model = HMM::readfromstdin();
	cerr << model.str();
	cout << model.nextemission().kattis() << endl;
}