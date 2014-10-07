#include <iostream>
#include "./HiddenMarkovModel.hpp"
#include "./BaumWelch.hpp"
#include "./helpers.hpp"
using namespace std;


int main() {
	HMM model = HMM::readfromstdin();
	HMM::Sequence sequence = Helpers::readfromstdin<int>();
	BaumWelch bw = BaumWelch(&model, sequence);
	bw.train();
	cout << model.kattis();
}