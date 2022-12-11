#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <utility>
#include <algorithm>
#include "MaskFactory.h"

class ProfanityFilter {
private:
    std::vector<std::string> profanitiesArray, sourceArray, originalData, whitelistArray;
    std::vector<std::vector<std::pair<std::pair<int, int>, char>>> processedArray;
    std::vector<int> allowedCharactersBetweenWordsArray;
    std::vector<std::pair<int, int>> allowedWhitelistCharactersBeforeAndAfterWord;
    std::vector<std::pair<int, int>> allowedProfanityCharactersBeforeAndAfterWord;
    std::vector<bool> isCensoredArray, isWhitelistedArray;
    MaskFactory wordMaskFactory;
    int originalIndex;

    void collapseLetters(std::vector<std::string>& sourceArray);

    void removeSpecialCharactersAndDigits(std::vector<std::string>& sourceArray);

    void toLowerCases(std::vector<std::string>& sourceArray);

    void removeUnambiguousDiactrics(std::vector<std::string>& sourceArray);

    void generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
        std::vector<std::vector<std::pair<std::pair<int, int>, char>>>& outputArray);

    bool containsSubstring(std::string& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord,
        int allowedCharactersInFront, int allowedCharactersInBack, int skip = 0);

    bool isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray, int skip = -1);

    bool isOnWhitelist(std::string& potentialProfanityWord, int skip = 0);

    bool findProfanityInAllPossibleWords(std::vector<std::pair<std::pair<int, int>, char>>& inputArray,
        int index, int arrayLocationIndex, std::string* currentWord);

    void loadProfanities();

    void loadWhitelist();
public:
    std::vector<std::string> outputArray;

    void censorInputtedText();

    void loadData();

    void showCensoredText();
};