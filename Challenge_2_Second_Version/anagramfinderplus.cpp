#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <algorithm>
#include <unordered_map>

using namespace std;

//FUNCTION PROTOTYPES
bool is_anagram(string one, string two);
int usage(char* pname);
vector<string> load_dict(string file);

int usage(char* pname) {
	cout << "Usage is: " << pname << " <filename>\n";
	exit(1);
}

int main(int argc, char* argv[]) {
	string input;

	//check if commandline input is valid
	if (argc < 1)
		usage(argv[1]);

	cout << "Welcome to the Anagram Finder" << endl;
	cout << "-----------------------------" << endl;

	//load dictionary into hashtable
	auto start = chrono::steady_clock::now();
	string fn(argv[1]);
	vector<string> dict(load_dict(fn));
	auto end = chrono::steady_clock::now();
	auto diff = end - start;
	cout << "Dictionary loaded in " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
	cout << endl;

	//Generate Command Line
	string input2;
	cout << "AnagramFinder>";
	cin >> input2;
	cout << endl;

	string final;
	int count = 0;
	while (input2 != "exit") {
		bool first = true;
		for (auto i : dict) {
			if (is_anagram(i, input2)) {
				if (!first) 
					final += ", ";
				first = false;
				final += i; 
				count++;
			}
		}
		
		if (count == 0) {
			cout << "No anagrams found for " + input2 << " in " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
		}
		else {
			cout << count << " Anagrams found for " << input2 << " in " << chrono::duration <double, milli>(diff).count() << " ms" << endl;
			cout << final << endl;
		}
		
		final = "";
		count = 0;
		cout << endl;
		cout << "AnagramFinder>";
		cin >> input2;
		cout << endl;
	}
	return 0;
}

vector<string> load_dict(string file) {
	ifstream in(file);
	string str;
	vector<string> dict;
	while (getline(in, str)) 
		dict.push_back(str);
	return dict;
}

bool is_anagram(string one, string two) {
	unordered_map<char, int> char_count;

	//check if strings are same length
	if (one.size() != two.size())
		return false;
	//add char counts from string one into map
	for (int i = 0; i < one.size(); i++)
		char_count[one[i]]++;
	//logic to determine if anagram
	for (int i = 0; i < two.size(); i++) {
		if (char_count.find(two[i]) == char_count.end()) //check to see chars goes over 
			return false;
		else {
			char_count[two[i]]--; //subtract character count
			if (char_count[two[i]] == 0)
				char_count.erase(two[i]);
		}
	}

	return (char_count.size() == 0);
}