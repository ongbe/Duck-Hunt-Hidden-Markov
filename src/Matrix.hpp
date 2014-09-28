#include <vector>
#include <sstream>
#include <iostream>


using namespace std;

class Matrix {
	vector<vector<double>> data;

public:

	Matrix() {}

	Matrix(int n, int m) {
		for(int i = 0; i < n; i++) {
			vector<double> row;
			for(int j = 0; j < m; j++) {
				row.push_back(0);
			}
			data.push_back(row);
		}
	}

	int n() {
		return data.size();
	}

	int m() {
		return data.at(0).size();
	}

	string str() {
		stringstream ss;
		for(int i = 0; i < n(); i++) {
			for(int j = 0; j < m(); j++) {
				ss << data[i][j] << " ";
			}
			ss << "\n";
		}
		return ss.str();
	}

	string kattis() {
		stringstream ss;
		ss << n() << " " << m() << " ";
		for(int i = 0; i < n(); i++) {
			for(int j = 0; j < m(); j++) {
				ss << data[i][j];
				if(!(i == n()-1 && j == m()-1)) {
					ss << " ";
				}
			}
		}
		return ss.str();
	}

	vector<double> &operator[](int i)
    {
        return data.at(i);
    }

    Matrix operator*(Matrix other) {
    	Matrix product = Matrix(n(), other.m());
    	for(int i = 0; i < product.n(); i++) {
    		for(int j = 0; j < product.m(); j++) {
    			product[i][j] = 0;
    			for(int k = 0; k < product.m()+1; k++) {
    				product[i][j] += data[i][k] * other[k][j];
    			}
    		}
    	}
    	return product;
    }

	static Matrix readfromstdin() {
		Matrix matrix = Matrix();
		int n;
		int m;
		cin >> n;
		cin >> m;
		matrix.data = vector<vector<double>>();
		for(int i = 0; i < n; i++) {
			vector<double> row = vector<double>();
			for(int j = 0; j < m; j++) {
				double cell;
				cin >> cell;
				row.push_back(cell);
			}
			matrix.data.push_back(row);
		}
		return matrix;
	}
};