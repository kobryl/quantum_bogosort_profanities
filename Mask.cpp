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
	int number;
	time_t time;

	std::getline(file, textFromFile);
	sstream << textFromFile;
	sstream >> time;
	if (time != getProfanityListModificationTime()) createCacheFile();

	while (std::getline(file, textFromFile)) {
		std::stringstream sstream2;
		sstream2 << textFromFile;
		sstream2 >> number;
		profanityMasks.push_back(number);
	}

	file.close();
}


void createCacheFile() {
	std::ofstream cache("cache");
	std::ifstream file("profanity_list.txt");
	std::string textFromFile;

	cache << getProfanityListModificationTime();
	
	while (std::getline(file, textFromFile)) {
		cache << "\n";
		cache << parseStringToMask(textFromFile);
	}

	cache.close();
	file.close();
}


time_t getProfanityListModificationTime() {
	time_t time;
	struct stat attrib;
	__int64 ltime;
	_time64(&ltime);

	stat("profanity_list.txt", &attrib);
	time = attrib.st_mtime;

	return time;
}
