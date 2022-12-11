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
* 
*	A mask is a numerical representation of the letters a word consists of. 
*	It's created by setting specific bits in an int to '1', based on indices computed from the given word (e.g. 'a' = LSB, 'z' = 25th bit from LSB).
*/


class MaskFactory {
private:
	std::vector<int> profanityMasks;
	std::vector<int> whitelistMasks;
	int currentWordMask;			// currently set mask, which is used in canBeProfanity/Whitelisted to increase performance.

	// Compares two masks and returns true if either mask is a subset of the other.
	bool doMasksMatch(int firstMask, int secondMask);

	// Converts a string to a mask, ignoring any characters that are not lowercase letters. 
	int parseStringToMask(std::string& word, int skip = 0);

	// Converts a processed word to a mask, ignoring any characters that are not lowercase letters. 
	int parseProcessedWordToMask(std::vector<std::pair<std::pair<int, int>, char>>& word, int skip = 0);

	// Reads a mask cache file and stores the masks in a vector.
	void readMaskCacheFile(const char* cacheName, std::vector<int>* masks);

	// Creates a cache file from a given list file.
	void createCacheFile(const char* cacheName, const char* listName, int irrelevantParamsAfterWord);

	// Returns the modification time for a list specified by fileName.
	time_t getListModificationTime(const char* fileName);

	// Determines whether a cache file is recent or not.
	bool isCacheFileRecent(const char* cacheName, const char* listName);


public:
	// Default constructor: initializes masks vector.
	MaskFactory();

	// Checks whether the currently set word's mask in currentWordMask could be a profanity.
	bool canBeProfanity(int profanityIndex);

	// Checks whether the currently set word's mask in currentWordMask could be whitelisted.
	bool canBeWhitelisted(int whitelistIndex);

	// Sets the currentWordMask field to the mask generated from a given processed word.
	void setCurrentWordMask(std::vector<std::pair<std::pair<int, int>, char>>& word, int skip = 0);


	// Debug functions

	// Prints the profanity and whitelist masks.
	void printMasks();
};


#endif
