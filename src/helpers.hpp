#include <iostream>
#include <vector>
using namespace std;

#ifndef Helpers_h
#define Helpers_h


namespace Helpers {

	template <class T>
	vector<T> readfromstdin() {
		vector<T> v;
		T n;
		cin >> n;
		for(int j = 0; j < n; j++) {
			int cell;
			cin >> cell;
			v.push_back(cell);
		}
		return v;
	}

}

#endif