#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include<stdio.h> 
#include<string.h>
#include <sstream>

using namespace std;

int main(int argc, char const *argv[]) {
	string name = string(argv[1]) + ".satoutput";
	ifstream minisat;
	minisat.open(name);

	ofstream mapping;
	mapping.open(string(argv[1]) + ".mapping");

	string out_string;
	getline(minisat, out_string);
	if (out_string == "SAT") {
		getline(minisat, out_string);
	}
	else if (out_string == "UNSAT") {
		mapping << 0;
		return 0;
	}

	ifstream graph;
	graph.open(string(argv[1]) + ".map");

	int v1, v2, v3;
	int j = 0;

	vector<int> result;
	istringstream l_(out_string);
	l_ >> v1;
	result.push_back(v1);
	while (true) {
		l_ >> v1;
		if (v1 == 0) {
			break;
		}
		result.push_back (v1);
	}

	vector<int>::iterator itr;
	for(itr = result.begin(); itr!= result.end(); itr++) {
		v1 = *itr;
		if (v1 < 0) {
			continue;
		}
		else {
			while (getline(graph, out_string)) {
				// cout << out_string << endl;
				istringstream l__(out_string);
				l__ >> j;
				if (j == 0) {
					l__ >> v2; l__ >> v3;
					mapping << v2 << " " << v3 << endl;
				}
				else if (v1!= j) {
					continue;
				}
				else
				{
					l__ >> v2; l__ >> v3;
					mapping << v2 << " " << v3 << endl;
					break;
				}
			}
			
		}
	}
}