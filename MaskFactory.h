#pragma once
#ifndef MaskFactory_h
#define MaskFactory_h

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <bitset>


#define PROFANITY_LIST_NAME "profanity_list.txt"
#define WHITELIST_NAME "whitelist.txt"
#define PROFANITY_LIST_CACHE_NAME "profanity_cache"
#define WHITELIST_CACHE_NAME "whitelist_cache"


/*
*	Declaration of MaskFactory class.
*
*	Provides an effictient method for preliminary checking whether a word could be a profanity or not, by matching the letters the words consist of.
*	When a word list is modified, a new cache file is created for the class to use. When an instance is initialized, the file is read.
*/


class MaskFactory {
private:
	std::vector<int> profanityMasks;
	std::vector<int> whitelistMasks;
	int currentWordMask;			// currently set mask, which is used in canBeProfanity/Whitelisted to increase performance.

	// Returns true if two masks match, false otherwise.
	bool doMasksMatch(int firstMask, int secondMask);

	// Returns a mask generated from a given word.
	int parseStringToMask(std::string& word, int skip = 0);

	// Reads the given list's cache file and inserts masks into the given mask list.
	void readMaskCacheFile(const char* cacheName, std::vector<int>* masks);

	// Creates a given list's cache file and writes masks computed from the given word file.
	void createCacheFile(const char* cacheName, const char* listName, int irrelevantParamsAfterWord);

	// Returns the given list's modification time.
	time_t getListModificationTime(const char* fileName);

	// Checks if the given list's cache file is recent. Returns true or false depending on the outcome.
	bool isCacheFileRecent(const char* cacheName, const char* listName);


public:
	// Default constructor: initializes masks vector.
	MaskFactory();

	// Checks if the word's mask set in currentWordMask could be a profanity based on it's mask. Returns true or false depending on the outcome.
	bool canBeProfanity(int profanityIndex);

	// Checks if the word's mask set in currentWordMask could be whitelisted based on it's mask. Returns true or false depending on the outcome.
	bool canBeWhitelisted(int whitelistIndex);

	// Sets the currentWordMask variable to a mask of the given word. Used in canBeProfanity/Whitelisted to increase performance.
	void setCurrentWordMask(std::string& word, int skip = 0);
	// Debug functions

	// Returns a pointer to a vector of all of the masks.
	std::vector<int>* getMasks();

	// Prints all masks in format: index: binary rep. = letters = int rep.
	void printMasks();
};


#endif
