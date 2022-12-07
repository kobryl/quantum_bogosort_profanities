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
		letterNumber = word[i] - 'a';
		mask |= 1UL << letterNumber;
	}

	return mask;
}


void readMaskCacheFile() {
	ifstream file("cache");
	string textFromFile;
	stringstream sstream;
	int number;
	time_t time;

	getline(file, textFromFile);
	sstream << textFromFile;
	sstream >> time;
	if (time != getProfanityListModificationTime()) createCacheFile();

	while (getline(file, textFromFile)) {
		stringstream sstream2;
		sstream2 << textFromFile;
		sstream2 >> number;
		profanityMasks.push_back(number);
	}

	file.close();
}


void createCacheFile() {
	ofstream cache("cache");
	ifstream file("profanity_list.txt");
	string textFromFile;

	cache << getProfanityListModificationTime();
	
	while (getline(file, textFromFile)) {
		cache << "\n";
		cache << parseStringToMask(textFromFile);
	}
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