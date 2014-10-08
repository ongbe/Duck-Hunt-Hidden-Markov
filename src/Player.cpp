#include "Player.hpp"
#include "HiddenMarkovModel.hpp"
#include "BaumWelch.hpp"
#include <cstdlib>
#include "Matrix.hpp"
#include <iostream>

namespace ducks
{

	Player::Player() {}

	Action Player::shoot(const GameState &pState, const Deadline &pDue) {
		int N = pState.getNumBirds();
		int M = pState.getBird(0).getSeqLength();			// Keep an eye on! Could be wrong.

		cerr << "Round: " <<  pState.getRound() << " nBirds: " << N << " nSequences: " << M << " myScore: " << pState.myScore() << endl;

		if(M < 25) {
			return cDontShoot;
		}

		int npo = 4 + COUNT_MOVE - 1;

		Matrix<> iniA(npo, npo, 1.0/COUNT_MOVE);
		Matrix<> iniB(npo, COUNT_MOVE, 1E-100);

		for(int i = 0; i < COUNT_MOVE; i++) {
			if(i == MOVE_STOPPED) continue;
			if(i < MOVE_STOPPED) {
				iniB[i][i] = 1;
			} else {
				iniB[i-1][i] = 1;
			}
		}

		iniB[COUNT_MOVE + 1 -2][MOVE_UP_LEFT] = 0.5;
		iniB[COUNT_MOVE + 1 -2][MOVE_UP_RIGHT] = 0.5;
		iniB[COUNT_MOVE + 2 -2][MOVE_DOWN] = 1.0/3;
		iniB[COUNT_MOVE + 2 -2][MOVE_RIGHT] = 1.0/3;
		iniB[COUNT_MOVE + 2 -2][MOVE_LEFT] = 1.0/3;
		iniB[COUNT_MOVE + 3 -2] = vector<double>(COUNT_MOVE, 1.0/COUNT_MOVE);
		iniB[COUNT_MOVE + 4 -2] = vector<double>(COUNT_MOVE, 1.0/COUNT_MOVE);

		vector<double> quality;
		vector<HMM> hmms;
		for(int i = 0; i < N; i++) {
			Bird bird = pState.getBird(i);
			Matrix<> q(1, npo, 1.0/npo);
			HMM model(iniA, iniB, q);
			HMM::Sequence observations;
			for(int j = 0; j < M; j++) {
				observations.push_back(bird.getObservation(j));
			}
			BaumWelch bw(&model, observations);
			double h = bw.train();
			quality.push_back(h);
			hmms.push_back(model);
		}

		int bestbird = -1;
		double max = 0;
		for(int i = 0; i < quality.size(); i++) {
			if(quality[i] > max) {
				max = quality[i];
				bestbird = i;
			}
		}

		Matrix<> next = hmms[bestbird].nextemission();
		max = 0;
		int bestmove = -1;
		for(int i = 0; i < next.m(); i++) {
			if(next[0][i] > max) {
				max = next[0][i];
				bestmove = i;
			}
		}

		return Action(bestbird, (EMovement) bestmove);
	}

	std::vector<ESpecies> Player::guess(const GameState &pState, const Deadline &pDue) {
		/*
		 * Here you should write your clever algorithms to guess the species of each bird.
		 * This skeleton makes no guesses, better safe than sorry!
		 */
		cerr << "Question" << endl;
		std::vector<ESpecies> lGuesses(pState.getNumBirds(), SPECIES_UNKNOWN);
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
	}


}
