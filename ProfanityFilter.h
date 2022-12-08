#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <utility>
#include <algorithm>
#include "Mask.hpp"

class ProfanityFilter {
private:
    std::vector<std::string> profanitiesArray, sourceArray;
    std::vector < std::vector<std::pair<std::pair<int, int>, char>>> processedArray;

    void collapseLetters(std::vector<std::string>& sourceArray);

    void removeSpecialCharactersAndDigits(std::vector<std::string>& sourceArray);

    void toLowerCases(std::vector<std::string>& sourceArray);

    void removeUnambiguousDiactrics(std::vector<std::string>& sourceArray);

    void generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
        std::vector < std::vector<std::pair<std::pair<int, int>, char>>>& outputArray);


    bool containsSubstring(std::string& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord);

    bool isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray);

    bool generateAllPossibleWordsAndFindProfanity(std::vector<std::pair<std::pair<int, int>, char>>& inputArray,
        int index, int arrayLocationIndex, std::string* currentWord);

    void loadInputDataToArray(std::string fileName, std::vector<std::string>& sourceArray);

public:
    std::vector<std::string> outputArray;

    void censorInputedText();

    void loadData(std::string fileName);

    void showCensoredText();



};