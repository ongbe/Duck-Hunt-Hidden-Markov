#include "./Matrix.hpp"


class Emission {

	vector<int> sequence;
	
public:

	int &operator[](int i) {
		return sequence[i];
	}
	
	static Emission readfromstdin() {
		Emission emission = Emission();
		int n;
		cin >> n;
		for(int j = 0; j < n; j++) {
			int cell;
			cin >> cell;
			emission.sequence.push_back(cell);
		}
		return emission;
	}

	int size() {
		return sequence.size();
	}

};

using namespace std;

class HMM {

	Matrix A;
	Matrix B;
	Matrix q;

public:

	HMM(Matrix A, Matrix B, Matrix q) {
		this->A = A;
		this->B = B;
		this->q = q;
	}

	string str() {
		stringstream ss;
		ss << "A =\n" << A.str() << "\nB = \n" << B.str() << "\nq = \n" << q.str() << "\n";
		return ss.str();
	}

	Matrix nextemission() {
		return q * A * B;
	}

	int K() {
		return q.m();
	}

	static HMM readfromstdin() {
		Matrix A = Matrix::readfromstdin();
		Matrix B = Matrix::readfromstdin();
		Matrix q = Matrix::readfromstdin();
		return HMM(A, B, q);
	}

	double test(Emission emission) {
		vector<double> last(K(), 0);
		for(int i = 0; i < K(); i++) {
			last[i] = q[0][i] * B[i][emission[0]];
		}
		for(int i = 1; i < emission.size(); i++) {
			vector<double> next(K(), 0);
			for(int k = 0; k < K(); k++) {
				double sum = 0;
				for(int j = 0; j < K(); j++) {
					sum += last[j] * A[j][k];
				}
				next[k] = sum * B[k][emission[i]];
			}
			last = next;
		}
		double chance = 0;
		for(int i = 0; i < last.size(); i++) {
			chance += last[i];
		}
		return chance;
	}

	int mostLiklyState(vector<double> options) {
		int best = 0;
		double max = 0;
		cerr << endl;
		for(int i = 0; i < options.size(); i++) {
			if(options[i] > max) {
				max = options[i];
				best = i;
			}
		}
		return best;
	}


	vector<int> viterbi(Emission emission) {
		// https://en.wikipedia.org/wiki/Viterbi_algorithm
		int T = emission.size();
		Matrix T_1(K(), T);
		int T_2[K()][T];
		for(int i = 0; i < K(); i++) {
			T_1[i][0] = q[0][i];
			T_2[i][0] = 0;
		}
		for(int i = 1; i < T; i++) {
			for(int j = 0; j < K(); j++) {
				double max = -1;
				int argmax = -1;
				for(int k = 0; k < K(); k++) {
					double tmp = T_1[k][i - 1] * A[k][j] * B[k][emission[i - 1]];
					if(tmp > max) {
						max = tmp;
						argmax = k;
					}
				}
				T_1[j][i] = max;
				T_2[j][i] = argmax;
			}
		}
		vector<int> Z(T);
		vector<int> X(T);
		double max = -1;
		for(int k = 0; k < K(); k++) {
			double tmp = T_1[k][T - 1];
			if(tmp > max) {
				max = tmp;
				Z[T - 1] = k;
			}
		}
		X[T - 1] = Z[T - 1];
		for(int i = T - 1; i > 0; i--) {
			Z[i - 1] = T_2[Z[i]][i];
			X[i - 1] = Z[i - 1];
		}
		return X;
	}

};