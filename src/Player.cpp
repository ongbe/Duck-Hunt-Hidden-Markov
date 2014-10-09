#include "Player.hpp"
#include "HiddenMarkovModel.hpp"
#include "BaumWelch.hpp"
#include <cstdlib>
#include "Matrix.hpp"
#include <iostream>
#include "../lib/Constants.hpp"


namespace ducks
{
	vector<bool> knownSpecies;
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
			knownSpecies.push_back(false);
		}
	}

	HMM::Sequence observe(GameState pState, int i) {
		Bird bird = pState.getBird(i);
		HMM::Sequence sequence;
		for(int j = 0; j < bird.getSeqLength(); j++) {
			sequence.push_back(bird.getObservation(j));
		}
		return sequence;
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
		std::vector<ESpecies> lGuesses;
		for(int i = 0; i < pState.getNumBirds(); i++) {
			vector<double> chance(COUNT_SPECIES, -1);
			for(int j = 0; j < COUNT_SPECIES; j++) {
				if(knownSpecies[j]) {
					chance[j] = hmms[j].test(observe(pState, i));
				}
			}
			int argmax = (int) SPECIES_PIGEON;
			double max = 0;
			for(int i = 0; i < chance.size(); i++) {
				if(chance[i] > max) {
					max = chance[i];
					argmax = i;
				}
			}
			lGuesses.push_back((ESpecies) argmax);
		}
		cerr << "I guess: ";
		for(int i = 0; i < lGuesses.size(); i++) {
			cerr << lGuesses[i] << " ";
		}
		cerr << endl;
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
		cerr << "It was   ";
		for(int i = 0; i < pSpecies.size(); i++) {
			cerr << pSpecies[i] << " ";
			knownSpecies[pSpecies[i]] = true;
			BaumWelch bw(&hmms[pSpecies[i]], observe(pState, i));
			bw.train();
		}
		cerr << endl;
	}


}
