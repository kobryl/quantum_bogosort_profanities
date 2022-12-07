#pragma once
#ifndef Mask_hpp
#define Mask_hpp

#include <string>
#include <fstream>
#include <vector>
#include <iostream>
#include <sstream>

bool canBeProfanity(std::string word);
int parseStringToMask(std::string* word);
void readMaskCacheFile();
bool checkIfCanBeProfanity(int mask);


#endif
