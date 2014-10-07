#include <iostream>
#include "./HiddenMarkovModel.hpp"
#include "./helpers.hpp"
using namespace std;


int main() {
	HMM model = HMM::readfromstdin();
	Sequence sequence = Helpers::readfromstdin<int>();
	model.learn(sequence);
	cout << model.kattis();
}