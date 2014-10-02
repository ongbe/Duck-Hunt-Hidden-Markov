#include <iostream>
#include "./HiddenMarkovModel.hpp"
using namespace std;


int main() {
	HMM model = HMM::readfromstdin();
	Emission emission = Emission::readfromstdin();
	vector<int> sequence = model.mostLiklySequence(emission);
	for(int i = 0; i < sequence.size() - 1; i++) {
		cout << sequence[i] << " ";
	}
	cout << sequence[sequence.size() - 1] << endl;
}