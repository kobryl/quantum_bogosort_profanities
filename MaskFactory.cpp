#include "MaskFactory.h"

/*
*	Implementation of MaskFactory class.
*	
*	Provides an effictient method for preliminary checking whether a word could be a profanity or not, by matching the letters the words consist of.
*	When a word list is modified, a new cache file is created for the class to use. When an instance is initialized, the file is read.
*/

// Default constructor: initializes masks vector.
// Creates a new cache file to read if the old one is not recent.
MaskFactory::MaskFactory() {
	// Profanity masks initialization (cache creation if needed)
	if (!isCacheFileRecent(PROFANITY_LIST_CACHE_NAME, PROFANITY_LIST_NAME))
		// 3 parameters: allowed characters between letters of a word, allowed characters before word and after a word
		createCacheFile(PROFANITY_LIST_CACHE_NAME, PROFANITY_LIST_NAME, 3);	
	readMaskCacheFile(PROFANITY_LIST_CACHE_NAME, &profanityMasks);

	// Whitelist masks initialization (cache creation if needed)
	if (!isCacheFileRecent(WHITELIST_CACHE_NAME, WHITELIST_NAME))
		// 2 parameters: allowed characters before a word and after a word
		createCacheFile(WHITELIST_CACHE_NAME, WHITELIST_NAME, 2);
	readMaskCacheFile(WHITELIST_CACHE_NAME, &whitelistMasks);
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


// Reads the given list's cache file and inserts masks into the given mask list.
void MaskFactory::readMaskCacheFile(const char* cacheName, std::vector<int>* masks) {
	std::ifstream list(cacheName);
	std::string str;
	int number = 0;

	std::getline(list, str);	// Skip the first line, which is a timestamp
	while (list >> number) {
		(*masks).push_back(number);
	}

	list.close();
}


// Creates a given list's cache file and writes masks computed from the given word file.
void MaskFactory::createCacheFile(const char* cacheName, const char* listName, int irrelevantParamsAfterWord) {
	std::ofstream cache(cacheName);
	std::ifstream list(listName);
	std::string textFromFile;

	cache << getListModificationTime(listName);
	
	int temp;
	while (list >> textFromFile) {
		cache << "\n";
		cache << parseStringToMask(textFromFile);
		for (int i = 0; i < irrelevantParamsAfterWord; i++)
			list >> temp;		// Skip an irrelevant parameter, such as allowed characters between word letters.
	}

	cache.close();
	list.close();
}


// Returns the given list's modification time.
time_t MaskFactory::getListModificationTime(const char* fileName) {
	time_t time;
	struct stat attrib;
	__int64 ltime;
	_time64(&ltime);

	stat(fileName, &attrib);
	time = attrib.st_mtime;

	return time;
}


// Checks if the given list's cache file is recent. Returns true or false depending on the outcome.
// This is determined by comparing the list's modification time and the recorded modification time in the cache file.
bool MaskFactory::isCacheFileRecent(const char* cacheName, const char* listName) {
	std::ifstream cache(cacheName);
	std::string textFromFile;
	time_t recordedTime;

	cache >> recordedTime;
	cache.close();

	if (recordedTime != getListModificationTime(listName)) return false;
	return true;
}


// Checks if the provided word can be a profanity based on it's mask. Returns true or false depending on the outcome.
// A mask is a numerical representation of the letters a word consists of. If two words' masks match, then they consist of the same letters.
bool MaskFactory::canBeProfanity(std::string& word, int profanityIndex) {
	int mask = parseStringToMask(word);

	return doMasksMatch(mask, profanityMasks[profanityIndex]);
}


// Checks if the provided word can be a whitelisted based on it's mask. Returns true or false depending on the outcome.
// A mask is a numerical representation of the letters a word consists of. If two words' masks match, then they consist of the same letters.
bool MaskFactory::canBeWhitelisted(std::string& word, int whitelistIndex) {
	int mask = parseStringToMask(word);

	return doMasksMatch(mask, whitelistMasks[whitelistIndex]);
}


// Debug functions

// Returns a pointer to a vector of all of the masks.
std::vector<int>* MaskFactory::getMasks() {
	return &profanityMasks;
}


// Prints all masks in format: index: binary rep. = letters = int rep.
// e.g. 30: 00000000010100100000010000000000 = kruw = 5374976
void MaskFactory::printMasks() {
	std::cout << "Profanity masks:\n";
	int i = 0;
	for (int mask : profanityMasks) {
		std::bitset<32> bitMask = std::bitset<32>(mask);
		std::cout << i++ << ": " << bitMask << " = ";
		for (int i = 0; i < 32; i++) {
			if (bitMask.test(i)) std::cout << (char)('a' + i);
		}
		std::cout << " = " << mask << "\n";
	}

	std::cout << "\nWhitelist masks:\n";
	i = 0;
	for (int mask : whitelistMasks) {
		std::bitset<32> bitMask = std::bitset<32>(mask);
		std::cout << i++ << ": " << bitMask << " = ";
		for (int i = 0; i < 32; i++) {
			if (bitMask.test(i)) std::cout << (char)('a' + i);
		}
		std::cout << " = " << mask << "\n";
	}
}