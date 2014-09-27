#include <iostream>
#include <vector>
#include <sstream>


using namespace std;

class Matrix {
	vector<vector<double>> data;

public:
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

	vector<double> &operator[](int i)
    {
        return data.at(i);
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