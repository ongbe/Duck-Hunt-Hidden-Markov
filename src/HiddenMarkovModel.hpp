#include "./Matrix.hpp"


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

};