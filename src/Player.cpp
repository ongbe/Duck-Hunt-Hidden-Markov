#include "Player.hpp"
#include "HiddenMarkovModel.hpp"
#include "BaumWelch.hpp"
#include <cstdlib>
#include "Matrix.hpp"
#include <iostream>
#include <utility>


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

	pair<int, double> maximum(vector<double> v) {
		int argmax = -1;
		double max = 0;
		for(int i = 0; i < v.size(); i++) {
			if(v[i] > max) {
				max = v[i];
				argmax = i;
			}
		}
		return pair<int, double>(argmax, max);
	}

	pair<ESpecies, double> mostLiklySpecies(GameState pState, int ibird) {
		vector<double> chance(COUNT_SPECIES, -1);
		for(int j = 0; j < COUNT_SPECIES; j++) {
			if(knownSpecies[j]) {
				chance[j] = hmms[j].test(observe(pState, ibird));
			}
		}
		auto data = maximum(chance);
		double prob = data.second;
		ESpecies species;
		if(data.first == -1) {
			species = SPECIES_UNKNOWN;
		} else {
			species = (ESpecies) data.first;
		}
		return pair<ESpecies, double>(species, prob);
	}

	Action Player::shoot(const GameState &pState, const Deadline &pDue) {
		if(round != pState.getRound()) {
			cerr << "==== Round " << pState.getRound() << " with " << pState.getNumBirds() << " birds" << endl;
			round = pState.getRound();
		}
		if(observe(pState, 0).size() > 1000) {
			vector<double> ideaGoodness(pState.getNumBirds(), 0);
			vector<ESpecies> isa(pState.getNumBirds());
			for(int i = 0; i < pState.getNumBirds(); i++) {
				auto data = mostLiklySpecies(pState, i);
				if(!pState.getBird(i).isDead() && data.first != SPECIES_BLACK_STORK) {
					isa[i] = data.first;
					ideaGoodness[i] = data.second;
				}
			}
			auto data = maximum(ideaGoodness);
			int bird = data.first;
			int chance = data.second;
			if(bird != -1) {
				ESpecies species = isa[bird];
				Matrix<> whatnext = hmms[species].nextemission();
				data = maximum(whatnext[0]);
				cerr << "Die " << bird << " there is a " << chance << "\% you are " << data.first << " and should die!" << endl;
				return Action(bird, (EMovement) data.first);
			}
		}
		return cDontShoot;
	}

	std::vector<ESpecies> lGuesses;

	std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue) {
		/*
		 * Here you should write your clever algorithms to guess the species of each bird.
		 * This skeleton makes no guesses, better safe than sorry!
		 */

		lGuesses = vector<ESpecies>();
		for(int i = 0; i < pState.getNumBirds(); i++) {
			ESpecies species = mostLiklySpecies(pState, i).first;
			if(species == SPECIES_UNKNOWN && pState.getRound() != 9) {
				species = SPECIES_PIGEON;
			}
			lGuesses.push_back(species);
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
		int correct = 0;
		for(int i = 0; i < pSpecies.size(); i++) {
			cerr << pSpecies[i] << " ";
			if(lGuesses[i] == pSpecies[i]) {
				correct++;
			}
			if(pSpecies[i] != -1) {
				knownSpecies[pSpecies[i]] = true;
				BaumWelch bw(&hmms[pSpecies[i]], observe(pState, i));
				bw.train();
			}
		}
		cerr << endl << correct << "/" << pState.getNumBirds() << "=" << (double) correct / pState.getNumBirds() << endl << endl;
	}


}
