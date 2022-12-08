// profanities.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <utility>
#include <algorithm>
#include "Mask.hpp"

void collapseLetters(std::vector<std::string>& sourceArray) {
    std::string tmpString;
    for (int i = 0; i < sourceArray.size(); i++) {
        for (int j = 0; j < sourceArray[i].size(); j++) {
            if (j > 0 && sourceArray[i][j - 1] == sourceArray[i][j])
                continue;
            else
                tmpString += sourceArray[i][j];
        }
        sourceArray[i] = tmpString;
        tmpString = "";
    }
    return;
}

void removeSpecialCharactersAndDigits(std::vector<std::string>& sourceArray) {
    std::map<char, char> specialCharactersSwapMap = {
        {'0', 'o'},
        {'1', 'i'},
        {'2', 'z'},
        {'3', 'e'},
        {'4', 'a'},
        {'5', 's'},
        {'7', 'z'},
        {'!', 'i'},
        {'@', 'a'},
        {'$', 's'},
    };
    std::string tmpString;
    for (int i = 0; i < sourceArray.size(); i++) {
        for (int j = 0; j < sourceArray[i].size(); j++) {
            char currentCharacter = sourceArray[i][j];
            if ((specialCharactersSwapMap.find(currentCharacter) != specialCharactersSwapMap.end()))
                tmpString += specialCharactersSwapMap[currentCharacter];
            else
                tmpString += sourceArray[i][j];
        }
        sourceArray[i] = tmpString;
        tmpString = "";
    }
    return;
}

void toLowerCases(std::vector<std::string>& sourceArray) {
    for (int i = 0; i < sourceArray.size(); i++) {
        for (int j = 0; j < sourceArray[i].size(); j++) {
            char currentCharacter = tolower(sourceArray[i][j]);
            sourceArray[i][j] = currentCharacter;
        }
    }
}

void removeUnambiguousDiactrics(std::vector<std::string>& sourceArray) {
    std::map<std::pair<int, int>, char> diactricsSwapMap = {
        {{-60, -123}, 'a'},
        {{-60, -103}, 'e'},
        {{-59, -101}, 's'},
        {{-59, -126}, 'l'},
        {{-59, -68}, 'z'},
        {{-59, -70}, 'z'},
        {{-61, -77}, 'ó'},
        {{-60, -121}, 'c'},
        {{-59, -124}, 'n'}
    };

    std::string tmpString;
    for (int i = 0; i < sourceArray.size(); i++) {
        for (int j = 0; j < sourceArray[i].size(); j++) {
            std::pair<int, int> potentialWideCharacter = { sourceArray[i][j], sourceArray[i][j + 1] };
            if ((diactricsSwapMap.find(potentialWideCharacter) != diactricsSwapMap.end())) {
                tmpString += diactricsSwapMap[potentialWideCharacter];
                j++;
            }
            else
                tmpString += sourceArray[i][j];
        }
        sourceArray[i] = tmpString;
        tmpString = "";
    }
    return;
}

void printArray(std::vector<std::string>& sourceArray) {
    for (int i = 0; i < sourceArray.size(); i++) {
        std::cout << sourceArray[i] << "\n";
    }
}

void generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
    std::vector < std::vector<std::pair<std::pair<int, int>, char>>>& outputArray) {
    std::map<char, std::vector<char>> possibleSingleCharacterSwaps = {
        {'i', {'l', 'i'}},
        {'l', {'i', 'l'}},
        {'ó', {'o', 'u'}},
        {'v', {'v', 'u', 'w'}},
        {'z', {'z', 's'}},
        {'s', {'s', 'z'}},
        {'u', {'u', 'v', 'w'}}
    };
    std::map<std::pair<char, char>, std::vector<char>> possibleDoubleCharacterSwaps = {
        {{'|', '<'}, {'k', 'e'}}
    };
    outputArray.clear();

    //single characters
    for(int i = 0; i < sourceArray.size(); i++){
        std::vector < std::pair<std::pair<int, int>, char>> possibleLettersInWord;
        for (int j = 0; j < sourceArray[i].size(); j++) {
            char currentCharacterInWord = sourceArray[i][j];
            if (possibleSingleCharacterSwaps.find(currentCharacterInWord) != possibleSingleCharacterSwaps.end()) {
                for (char letter : possibleSingleCharacterSwaps[currentCharacterInWord]) {
                    possibleLettersInWord.push_back({ {j, 1}, letter });
                }
            }
            else
                possibleLettersInWord.push_back({ {j, 1}, currentCharacterInWord });
        }

        //double characters
        std::vector < std::pair<std::pair<int, int>, char>> possibleDoubleLettersInWord;
        for (int j = 1; j < sourceArray[i].size(); j++) {
            std::pair<char, char> currentCharactersInWord = { sourceArray[i][j - 1], sourceArray[i][j] };
            if (possibleDoubleCharacterSwaps.find(currentCharactersInWord) != possibleDoubleCharacterSwaps.end()) {
                for (char letter : possibleDoubleCharacterSwaps[currentCharactersInWord]) {
                    possibleLettersInWord.push_back({ {j - 1, 2}, letter });
                }
            }
        }
        std::sort(possibleLettersInWord.begin(), possibleLettersInWord.end());
        outputArray.push_back(possibleLettersInWord);
    }
}

void loadInputDataToArray(std::string fileName, std::vector<std::string>& sourceArray) {
    std::ifstream inputFile(fileName);
    std::string inputWord;
    while (inputFile >> inputWord)
        sourceArray.push_back(inputWord);
}

void generateAllPossibleWords(std::vector<std::pair<std::pair<int, int>, char>>& inputArray,
    int index, int arrayLocationIndex, std::string currentWord) {
    if (index > inputArray.back().first.first) {
        std::cout << currentWord << "\n";
        return;
    }
    while (arrayLocationIndex < inputArray.size() && inputArray[arrayLocationIndex].first.first < index)
        arrayLocationIndex++;
    while (arrayLocationIndex < inputArray.size() && inputArray[arrayLocationIndex].first.first == index) {
        generateAllPossibleWords(inputArray, index + inputArray[arrayLocationIndex].first.second, arrayLocationIndex, currentWord + inputArray[arrayLocationIndex].second);
        arrayLocationIndex++;
    }
}

const int possibleDifferencesInWord = 2;

bool containsSubstring(std::string& checkedWord, std::string& substringWord) {
    for (int i = 0; i < checkedWord.size() - substringWord.size() + 1; i++) {
        int currentPossibleDifferences = possibleDifferencesInWord;
        int idx = i;
        bool found = true;
        for (int j = 0; j < substringWord.size(); j++) {
            while (idx + j < checkedWord.size() && substringWord[j] != checkedWord[idx + j]) {
                idx++;
                currentPossibleDifferences--;
                if (currentPossibleDifferences < 0) {
                    found = false;
                    break;
                }
            }
            if (currentPossibleDifferences < 0 || idx + j > checkedWord.size()) {
                found = false;
                break;
            }
        }
        if (found)
            return true;
    }
    return false;
}

bool isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray) {
    //if (isOnWhitelist(potentialProfanityWord)) todo
    //    return false;
    for (int i = 0; i < profanitiesArray.size(); i++) {
        std::string& currentProfanity = profanitiesArray[i];
        //TODO
        //if (!maskFactory.canBeProfanity(i, potentialProfanityWord))  
        //    return false;
        if (containsSubstring(potentialProfanityWord, currentProfanity))
            return true;
    }
    return false;

}

int main() {
    //std::vector<std::string> sourceArray;
    std::vector<std::string> profanitiesArray = { "kurwa" };
    std::vector < std::vector<std::pair<std::pair<int, int>, char>>> outputArray;
    loadInputDataToArray("dane.txt", sourceArray);
    removeUnambiguousDiactrics(sourceArray);
    removeSpecialCharactersAndDigits(sourceArray);
    toLowerCases(sourceArray);
    collapseLetters(sourceArray);
    //printArray(sourceArray);
    generatePossibleVariationsOfLetters(sourceArray, outputArray);
    
    for (int i = 0; i < outputArray.size(); i++) {
        generateAllPossibleWords(outputArray[i], 0, 0, "");
    }

    std::string word1 = "tokurwato", word2 = "tokurewato", word3="uwu luv";
    std::cout << isProfanity(word1, profanitiesArray) << "\n";
    std::cout << isProfanity(word2, profanitiesArray) << "\n";
    std::cout << isProfanity(word3, profanitiesArray) << "\n";
}