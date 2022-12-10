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
    MaskFactory wordMaskFactory;
    int originalIndex;

    void collapseLetters(std::vector<std::string>& sourceArray);

    void removeSpecialCharactersAndDigits(std::vector<std::string>& sourceArray);

    void toLowerCases(std::vector<std::string>& sourceArray);

    void removeUnambiguousDiactrics(std::vector<std::string>& sourceArray);

    void generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
        std::vector<std::vector<std::pair<std::pair<int, int>, char>>>& outputArray);

    bool containsSubstring(std::string& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord,
        int allowedCharactersInFront, int allowedCharactersInBack);

    bool isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray);

    bool isOnWhitelist(std::string& potentialProfanityWord);

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