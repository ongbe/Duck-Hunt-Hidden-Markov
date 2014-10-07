#include <iostream>
#include <vector>
using namespace std;


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