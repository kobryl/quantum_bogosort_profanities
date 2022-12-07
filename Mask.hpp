#pragma once
#ifndef Mask_hpp
#define Mask_hpp

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

bool canBeProfanity(std::string& word);
bool doBinaryMasksMatch(int mask);
int parseStringToMask(std::string& word);
void readMaskCacheFile();
void writeMaskCacheFile();


#endif
