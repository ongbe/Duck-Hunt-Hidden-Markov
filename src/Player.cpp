#include "Player.hpp"
#include "HiddenMarkovModel.hpp"
#include "BaumWelch.hpp"
#include <cstdlib>
#include "Matrix.hpp"
#include <iostream>
#include "../lib/Constants.hpp"


namespace ducks
{

	vector<HMM> species;
	vector<int> species_map(COUNT_SPECIES);
	vector<int> known_birds;
	int round = -1;

	Player::Player() {
		Matrix<> iniA(COUNT_SPECIES, COUNT_SPECIES, 1.0/COUNT_SPECIES);		// TODO Randomize for better convergence
		Matrix<> iniB(COUNT_SPECIES, COUNT_MOVE, (double) 1 / COUNT_MOVE);
		Matrix<> iniq(1, COUNT_MOVE, 1.0/COUNT_MOVE);

		for(int i = 0; i < COUNT_SPECIES; i++) {
			HMM hmm(iniA, iniB, iniq);
			species.push_back(hmm);
		}
	}

	Action Player::shoot(const GameState &pState, const Deadline &pDue) {
		if(round != pState.getRound()) {
			cerr << "Round " << pState.getRound() << " with " << pState.getNumBirds() << " birds" << endl;
			known_birds = vector<int>(pState.getNumBirds(), 0);
		}
		return cDontShoot;
	}

	std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue) {
		/*
		 * Here you should write your clever algorithms to guess the species of each bird.
		 * This skeleton makes no guesses, better safe than sorry!
		 */
		cerr << "Question" << endl;
		std::vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_PIGEON);
		return lGuesses;
	}

	void Player::hit(const GameState &pState, int pBird, const Deadline &pDue) {
		/*
		 * If you hit the bird you are trying to shoot, you will be notified through this function.
		 */
		std::cerr << "HIT BIRD!!!" << std::endl;
	}

	void Player::reveal(const GameState &pState, const std::vector<ESpecies> &pSpecies, const Deadline &pDue) {
		/*
		 * If you made any guesses, you will find out the true species of those birds in this function.
		 */
		cerr << "REVeal!" << endl;
		for (int i = 0; i < pSpecies.size(); ++i) {
			cerr << pSpecies[i] << " ";
		}
	}


}
