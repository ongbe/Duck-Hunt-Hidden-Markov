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
		Matrix last = Matrix(q.m(), 1);
		cerr << this->str() << endl;
		for(int i = 0; i < q.n(); i++) {
			last[i][0] = q[0][i] * B[emission[0]][0];
		}
		for(int i = 1; i < emission.size(); i++) {
			Matrix next = Matrix(q.m(), 1);
			for(int k = 0; k < q.m(); k++) {
				double sum = 0;
				for(int j = 0; j < q.m(); j++) {
					sum += last[j][0] * A[j][k];
				}
				next[k][0] = sum;
			}
			cerr << "last\n" << last.str() << endl;
			last = next;
		}
		double chance = 0;
		for(int i = 0; i < last.n(); i++) {
			chance += last[i][0];
		}
		return chance;
	}

};