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
		if (word[i] < 'a' || word[i] > 'z')
			continue;
		letterNumber = word[i] - 'a';
		mask |= 1 << letterNumber;
	}

	return mask;
}


void readMaskCacheFile() {
	std::ifstream file("cache");
	std::string textFromFile;
	std::stringstream sstream;
	int mask = 0;

	while (std::getline(file, textFromFile)) {
		sstream << textFromFile;
		sstream >> mask;
		profanityMasks.push_back(mask);
	}

	file.close();
}


void writeMaskCacheFile() {
	std::ofstream file("cache");

	for (int mask : profanityMasks)
		file << mask << "\n";

	file.close();
}
