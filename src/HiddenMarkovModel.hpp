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

	static HMM readfromstdin() {
		Matrix A = Matrix::readfromstdin();
		Matrix B = Matrix::readfromstdin();
		Matrix q = Matrix::readfromstdin();
		return HMM(A, B, q);
	}

	double test(Emission emission) {
		vector<double> last(q.m(), 0);
		for(int i = 0; i < q.m(); i++) {
			last[i] = q[0][i] * B[i][emission[0]];
		}
		for(int i = 1; i < emission.size(); i++) {
			vector<double> next(q.m(), 0);
			for(int k = 0; k < q.m(); k++) {
				double sum = 0;
				for(int j = 0; j < q.m(); j++) {
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

	vector<int> mostLiklySequence(Emission emission) {
		return vector<int>();
	}

};