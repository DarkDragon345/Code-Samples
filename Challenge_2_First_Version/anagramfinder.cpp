#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <unordered_map>

using namespace std;

//PROTOTYPES
bool is_anagram(string one, string two);
int usage(char* pname);

int usage(char* pname) {
	cout << "Usage is: " << pname << " <filename>\n";
	exit(1);
}

int main(int argc, char* argv[]) {


	string input;
	vector <string> angrms;
	//check if commandline input is valid
	if (argc < 1) 
		usage(argv[1]);

	cout << "Welcome to the Anagram Finder" << endl;
	cout << "-----------------------------" << endl;

	ifstream in (argv[1]);

	cout << "Enter word you want to find Anagram of: ";
	cin >> input;

	auto start = chrono::steady_clock::now();

	string str;
	while (getline(in, str)) {
		if (is_anagram(input, str)) 
			angrms.push_back(str);
	}
	
	cout << "List of Anagrams in file: ";
	for (auto i : angrms) {
		cout << i + ",";
	}
	cout << endl;

	auto end = chrono::steady_clock::now();
	auto diff = end - start;

	cout << chrono::duration <double, milli>(diff).count() << " ms" << endl;

	return 0;
}

bool is_anagram(string one, string two) {

	unordered_map<char, int> m;

	//check if strings are same length
	if (one.size() != two.size())
		return false;
	//add char counts from string one into map
	for (int i = 0; i < one.size(); i++)
		m[one[i]]++;
	//logic to determine if anagram
	for (int i = 0; i < two.size(); i++) {
		if (m.find(two[i]) == m.end())
			return false;
		else {
			m[two[i]]--;
			if (m[two[i]] == 0)
				m.erase(two[i]);
		}
	}
	
	return (m.size() == 0);
}