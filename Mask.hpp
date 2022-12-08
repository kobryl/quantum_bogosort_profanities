#pragma once
#ifndef Mask_hpp
#define Mask_hpp

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <time.h>
#include <stdio.h>
#include <bitset>


#define PROFANITY_LIST_NAME "profanity_list.txt"
#define CACHE_FILE_NAME "cache"


/*
*	Declaration of MaskFactory class.
*
*	Provides an effictient method for preliminary checking whether a word could be a profanity or not, by matching the letters the words consist of.
*	When a profanity list is modified, a new cache file is created for the class to use. When an instance is initialized, the file is read.
*/


class MaskFactory {
private:
	std::vector<int> profanityMasks;

	// Returns true if two masks match, false otherwise.
	bool doMasksMatch(int firstMask, int secondMask);

	// Returns a mask generated from a given word.
	int parseStringToMask(std::string& word);

	// Reads the cache file and inserts masks into the mask vector.
	void readMaskCacheFile();

	// Creates a cache file and writes masks computed from the profanity list file.
	void createCacheFile();

	// Returns the profanity list's modification time.
	time_t getProfanityListModificationTime();

	// Checks if the cache file is recent. Returns true or false depending on the outcome.
	bool isCacheFileRecent();

public:
	// Default constructor: initializes masks vector.
	MaskFactory();

	// Checks if the provided word can be a profanity based on it's mask. Returns true or false depending on the outcome.
	bool canBeProfanity(std::string& word, int profanityIndex);

	// Debug functions
	// Returns a pointer to a vector of all of the masks.
	std::vector<int>* getMasks();

	// Prints all masks in format: binary rep. = letters = int rep.
	void printMasks();
};


#endif
