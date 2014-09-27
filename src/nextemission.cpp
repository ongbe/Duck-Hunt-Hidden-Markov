#include <iostream>
#include "./HiddenMarkovModel.hpp"


int main() {
	HMM model = HMM::readfromstdin();
	cerr << model.str();
	cerr << model.nextemission().str() << endl;
}