#include "Mask.hpp"

/*
*	Implementation of MaskFactory class.
*	
*	Provides an effictient method for preliminary checking whether a word could be a profanity or not, by matching the letters the words consist of.
*	When a profanity list is modified, a new cache file is created for the class to use. When an instance is initialized, the file is read.
*/

// Default constructor: initializes masks vector.
// Creates a new cache file to read if the old one is not recent.
MaskFactory::MaskFactory() {
	if (!isCacheFileRecent()) createCacheFile();
	readMaskCacheFile();
}


// Returns true if two masks match, false otherwise.
// Masks are considered equal when two words (from which the masks were constructed) consist of the same letters.
bool MaskFactory::doMasksMatch(int firstMask, int secondMask) {
	int comparisonMask = (firstMask & secondMask);
	if (comparisonMask == firstMask || comparisonMask == secondMask) return true;
	return false;
}


// Returns a mask generated from a given word.
// A mask is a numerical representation of the letters a word consists of. 
// It's created by setting specific bits in an int to '1', based on indices computed from the given word (e.g. 'a' = LSB, 'z' = 25th bit from LSB).
int MaskFactory::parseStringToMask(std::string& word) {
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


// Reads the cache file and inserts masks into the mask vector.
void MaskFactory::readMaskCacheFile() {
	std::ifstream list(CACHE_FILE_NAME);
	std::string textFromFile;
	std::stringstream sstream;
	int number;

	std::getline(list, textFromFile);	// Skip the first line, which is a timestamp
	while (std::getline(list, textFromFile)) {
		std::stringstream sstream2;
		sstream2 << textFromFile;
		sstream2 >> number;
		profanityMasks.push_back(number);
	}

	list.close();
}


// Creates a cache file and writes masks computed from the profanity list file.
void MaskFactory::createCacheFile() {
	std::ofstream cache(CACHE_FILE_NAME);
	std::ifstream list(PROFANITY_LIST_NAME);
	std::string textFromFile;

	cache << getProfanityListModificationTime();
	
	while (std::getline(list, textFromFile)) {
		cache << "\n";
		cache << parseStringToMask(textFromFile);
	}

	cache.close();
	list.close();
}


// Returns the profanity list's modification time.
time_t MaskFactory::getProfanityListModificationTime() {
	time_t time;
	struct stat attrib;
	__int64 ltime;
	_time64(&ltime);

	stat(PROFANITY_LIST_NAME, &attrib);
	time = attrib.st_mtime;

	return time;
}


// Checks if the cache file is recent. Returns true or false depending on the outcome.
// This is determined by comparing the list's modification time and the recorded modification time in the cache file.
bool MaskFactory::isCacheFileRecent() {
	std::ifstream list(CACHE_FILE_NAME);
	std::string textFromFile;
	std::stringstream sstream;
	time_t recordedTime;

	std::getline(list, textFromFile);
	sstream << textFromFile;
	sstream >> recordedTime;
	list.close();

	if (recordedTime != getProfanityListModificationTime()) return false;
	return true;
}


// Checks if the provided word can be a profanity based on it's mask. Returns true or false depending on the outcome.
// A mask is a numerical representation of the letters a word consists of. If two words' masks match, then they consist of the same letters.
// Whether a word could be a profanity is determined by comparing a profanity mask with the given word's mask.
bool MaskFactory::canBeProfanity(std::string& word, int profanityIndex) {
	int mask = parseStringToMask(word);

	return doMasksMatch(mask, profanityMasks[profanityIndex]);
}


// Debug functions

// Returns a pointer to a vector of all of the masks.
std::vector<int>* MaskFactory::getMasks() {
	return &profanityMasks;
}


// Prints all masks in format: index: binary rep. = letters = int rep.
// e.g. 30: 00000000010100100000010000000000 = kruw = 5374976
void MaskFactory::printMasks() {
	int i = 0;
	for (int mask : profanityMasks) {
		std::bitset<32> bitMask = std::bitset<32>(mask);
		std::cout << i++ << ": " << bitMask << " = ";
		for (int i = 0; i < 32; i++) {
			if (bitMask.test(i)) std::cout << (char)('a' + i);
		}
		std::cout << " = " << mask << "\n";
	}
}