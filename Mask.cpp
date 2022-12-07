#include "Mask.hpp"

std::vector<int> profanityMasks;
bool canBeProfanity(std::string& word) {
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


int parseStringToMask(std::string& word) {
	int mask = 0;
	int letterNumber;
	for (int i = 0; i < word.length(); i++) {
		letterNumber = word[i] - 'a';
		mask |= 1UL << letterNumber;
	}

	return mask;
}


void readMaskCacheFile() {
	std::ifstream file("cache");
	std::string textFromFile;
	std::stringstream sstream;
	int number;

	while (std::getline(file, textFromFile)) {
		sstream << textFromFile;
		sstream >> number;
		profanityMasks.push_back(number);
	}

	file.close();
}
