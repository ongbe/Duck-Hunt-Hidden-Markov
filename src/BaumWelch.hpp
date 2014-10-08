#ifndef BaumWelch_H
#define BaumWelch_H

#include "Matrix.hpp"
#include "HiddenMarkovModel.hpp"
#include <math.h>
#include <utility>


class BaumWelch {

	vector<double> c;
	int N, T;
	HMM::Sequence sequence;
	HMM * hmm;

	Matrix<> forward() {
		Matrix<> alpha(N, T);
		c[0] = 0;
		for(int i = 0; i < N; i++) {
			alpha[i][0] = hmm->q[0][i] * hmm->B[i][sequence[0]];
			c[0] += alpha[i][0];
		}
		c[0] = 1.0 / c[0];
		for(int i = 0; i < N; i++) {
			alpha[i][0] = c[0] * alpha[i][0];
		}
		for(int t = 1; t < T; t++) {
			c[t] = 0;
			for(int i = 0; i < N; i++) {
				alpha[i][t] = 0;
				for(int j = 0; j < N; j++) {
					alpha[i][t] += alpha[j][t - 1] * hmm->A[j][i];
				}
				alpha[i][t] *= hmm->B[i][sequence[t]];
				c[t] += alpha[i][t];
			}
			c[t] = 1.0 / c[t];
			for(int i = 0; i < N; i++) {
				alpha[i][t] = c[t] * alpha[i][t];
			}
		}
		return alpha;
	}

	Matrix<> backwards() {
		Matrix<> beta(N, T);
		for(int i = 0; i < N; i++) {
			beta[i][T - 1] = c[T - 1];
		}
		for(int t = T - 2; t >= 0; t--) {
			for(int i = 0; i < N; i++) {
				beta[i][t] = 0;
				for(int j = 0; j < N; j++) {
					double val = hmm->A[i][j];
					val *= hmm->B[j][sequence[t + 1]];
					val *= beta[j][t + 1];
					beta[i][t] += val;
				}
				beta[i][t] *= c[t];
			}
		}
		return beta;
	}

	typedef vector<vector<vector<double>>> DiGamma;

	pair<Matrix<>, DiGamma> gammaDiGamma(Matrix<> alpha, Matrix<> beta) {
		Matrix<> gamma(N, T);
		DiGamma digamma;
		for(int i = 0; i < N; i++) {
			vector<vector<double>> row;
			for(int j = 0; j < N; j++) {
				vector<double> column;
				for(int w = 0; w < T; w++) {
					column.push_back(0);
				}
				row.push_back(column);
			}
			digamma.push_back(row);
		}
		for(int t = 0; t < T - 1; t++) {
			double b = 0;
			for(int i = 0; i < N; i++) {
				for(int j = 0; j < N; j++) {
					double val = alpha[i][t];
					val *= hmm->A[i][j];
					val *= hmm->B[j][sequence[t + 1]];
					val *= beta[j][t + 1];
					b += val;
				}
			}
			for(int i = 0; i < N; i++) {
				gamma[i][t] = 0;
				for(int j = 0; j < N; j++) {
					double val = alpha[i][t];
					val *= hmm->A[i][j];
					val *= hmm->B[j][sequence[t + 1]];
					val *= beta[j][t + 1];
					val /= b;
					digamma[i][j][t] = val;
					gamma[i][t] += digamma[i][j][t];
				}
			}
		}
		return pair<Matrix<>, DiGamma>(gamma, digamma);
	}

	void reEstimate(Matrix<> gamma, DiGamma digamma) {
		for(int i = 0; i < N; i++) {
			hmm->q[0][i] = gamma[i][0];
		}
		for(int i = 0; i < N; i++) {
			for(int j = 0; j < N; j++) {
				double a = 0;		// TODO better variable name
				double b = 0;
				for(int t = 0; t < T - 1; t++) {
					a += digamma[i][j][t];
					b += gamma[i][t];
				}
				hmm->A[i][j] = a / b;
			}
		}
		for (int i = 0; i < N; i++) {
	        for (int j = 0; j < hmm->B.m(); j++) {
	            double a = 0;
	            double b = 0;
	            for (int t = 0; t < T - 1; t++) {
	                if (sequence[t] == j) {
	                    a = a + gamma[i][t];
	                }
	                b = b + gamma[i][t];
	            }
	            hmm->B[i][j] = a / b;
	        }
	    }
	}

	double sum() {
		double prob = 0;
	    for (int i = 0; i < T; i++) {
	        prob += c[i];
	    }
	    return prob;
	}

	double compute() {
		Matrix<> alpha = forward();
		Matrix<> beta = backwards();
		pair<Matrix<>, DiGamma> pair = gammaDiGamma(alpha, beta);
		auto gamma = pair.first;
		auto digamma = pair.second;
		reEstimate(gamma, digamma);
		return sum();
	}

public:

	int maxIterations = 25;

	BaumWelch(HMM * hmm, HMM::Sequence sequence) {
		this->hmm = hmm;
		this->sequence = sequence;
		N = hmm->K();		// TODO Rename to K to be consistant
		T = sequence.size();
		c = vector<double>(T);
	}

	double train() {
		double stability;
		for(int i = 0; i < maxIterations; i++) {
			stability = compute();
		}
		return stability;
	}

};

#endif