#include "Mask.hpp"

using namespace std;

vector<int> profanityMasks;
bool canBeProfanity(string& word) {
	int mask = parseStringToMask(word);
	readMaskCacheFile();

	return doBinaryMasksMatch(mask);
}


bool doBinaryMasksMatch(int mask) {
	int andMask = 0;
	for (int word : profanityMasks) {
		andMask = (word & mask);
		if (andMask == word) return true;
	}

	return false;
}


int parseStringToMask(string& word) {
	int mask = 0;
	int letterNumber;
	for (int i = 0; i < word.length(); i++) {
		if (word[i] < 'a' || word[i] > 'z')
			continue;
		letterNumber = word[i] - 'a';
		mask |= 1 << letterNumber;
	}

	return mask;
}


void readMaskCacheFile() {
	ifstream file("cache");
	string textFromFile;
	stringstream sstream;
	int number;

	while (getline(file, textFromFile)) {
		sstream << textFromFile;
		sstream >> number;
		profanityMasks.push_back(number);
	}

	file.close();
}
