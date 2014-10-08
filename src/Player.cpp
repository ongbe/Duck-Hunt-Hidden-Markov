#include "Player.hpp"
#include "HiddenMarkovModel.hpp"
#include "BaumWelch.hpp"
#include <cstdlib>
#include "Matrix.hpp"
#include <iostream>
#include "../lib/Constants.hpp"


namespace ducks
{

	vector<HMM> hmms;				// species -> hmm
	int round = -1;

	Player::Player() {
		Matrix<> iniA(COUNT_SPECIES, COUNT_SPECIES);
		Matrix<> iniB(COUNT_SPECIES, COUNT_MOVE);
		Matrix<> iniq(1, COUNT_SPECIES);

		for(int i = 0; i < COUNT_SPECIES; i++) {
			iniA.scramble();
			iniB.scramble();
			iniq.scramble();
			HMM hmm(iniA, iniB, iniq);
			hmms.push_back(hmm);
		}
	}

	Action Player::shoot(const GameState &pState, const Deadline &pDue) {
		if(round != pState.getRound()) {
			cerr << "Round " << pState.getRound() << " with " << pState.getNumBirds() << " birds" << endl;
			round = pState.getRound();
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
		for(int i = 0; i < pSpecies.size(); i++) {
			Bird bird = pState.getBird(i);
			HMM::Sequence sequence;
			for(int j = 0; j < bird.getSeqLength(); j++) {
				sequence.push_back(bird.getObservation(j));
			}
			cerr << i << " " << pSpecies[i] << " " << hmms[pSpecies[i]].A[0][0] << endl;
			BaumWelch bw(&hmms[pSpecies[i]], sequence);
			bw.train();
		}
	}


}
