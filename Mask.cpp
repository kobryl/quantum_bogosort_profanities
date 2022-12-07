#include "Mask.hpp"

std::vector<int> profanityMasks;
bool canBeProfanity(std::string& word) {

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


void writeMaskCacheFile() {
	std::ofstream file("cache");

	for (int mask : profanityMasks)
		file << mask << "\n";

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
