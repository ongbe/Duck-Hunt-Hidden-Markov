#include <iostream>
#include "./HiddenMarkovModel.hpp"
using namespace std;


int main() {
	HMM model = HMM::readfromstdin();
	Emission emission = Emission::readfromstdin();
	
}