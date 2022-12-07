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

bool canBeProfanity(std::string& word);
int parseStringToMask(std::string& word);
void readMaskCacheFile();
bool doBinaryMasksMatch(int mask);
void createCacheFile();
time_t getProfanityListModificationTime();


#endif
