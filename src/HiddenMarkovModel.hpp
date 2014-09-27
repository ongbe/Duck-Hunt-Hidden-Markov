#include "./Matrix.hpp"


using namespace std;

class HMM {

	Matrix A;
	Matrix B;
	Matrix q;

public:

	HMM(Matrix A, Matrix B, Matrix q) {		this->A = A;
		this->B = B;
		this->q = q;
	}

	string str() {
		stringstream ss;
		ss << "A =\n" << A.str() << "\nB = \n" << B.str() << "\nq = \n" << q.str() << "\n";
		return ss.str();
	}

	Matrix transitions() {
		double max = 0;
		int index;
		for(int i = 0; i < q.m(); i++) {
			if(q[0][i] > max) {
				max = q[0][i];
				index = i;
			}
		}
		Matrix transisions = Matrix(1, A.m());
		transisions[0] = A[index];
		return transisions;
	}

	Matrix nextemission() {
		return transitions() * A;
	}

	static HMM readfromstdin() {
		Matrix A = Matrix::readfromstdin();
		Matrix B = Matrix::readfromstdin();
		Matrix q = Matrix::readfromstdin();
		return HMM(A, B, q);
	}

};