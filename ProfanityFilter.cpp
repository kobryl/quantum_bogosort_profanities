#include "ProfanityFilter.h"
#include "MaskFactory.h"


void ProfanityFilter::collapseLetters(std::vector<std::string>& sourceArray) {
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
}


void ProfanityFilter::removeSpecialCharactersAndDigits(std::vector<std::string>& sourceArray) {
    std::map<char, char> specialCharactersSwapMap = {
        {'0', 'o'},
        {'1', 'i'},
        {'2', 'z'},
        {'3', 'e'},
        {'4', 'a'},
        {'5', 's'},
        {'6', 'b'},
        {'7', 't'},
        {'8', 'b'},
        {'!', 'i'},
        {'@', 'a'},
        {'$', 's'},
        {'#', 'h'},
        {'|', 'i'}
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
}


void ProfanityFilter::toLowerCases(std::vector<std::string>& sourceArray) {
    for (int i = 0; i < sourceArray.size(); i++) {
        for (int j = 0; j < sourceArray[i].size(); j++) {
            char currentCharacter = tolower(sourceArray[i][j]);
            sourceArray[i][j] = currentCharacter;
        }
    }
}


void ProfanityFilter::removeUnambiguousDiactrics(std::vector<std::string>& sourceArray) {
    std::map<std::pair<int, int>, char> diactricsSwapMap = {
        {{-60, -123}, 'a'},
        {{-60, -103}, 'e'},
        {{-59, -101}, 's'},
        {{-59, -126}, 'l'},
        {{-59, -68}, 'z'},
        {{-59, -70}, 'z'},
        {{-61, -77}, '�'},
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
}


void ProfanityFilter::generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
    std::vector<std::vector<std::pair<std::pair<int, int>, char>>>& outputArray) {
    std::map<char, std::vector<char>> possibleSingleCharacterSwaps = {
        {'i', {'l', 'i'}},
        {'l', {'i', 'l'}},
        {'�', {'o', 'u'}},
        {'v', {'v', 'u', 'w'}},
        {'z', {'z', 's'}},
        {'s', {'s', 'z'}},
        {'u', {'u', 'v', 'w'}}
    };
    std::map<std::pair<char, char>, std::vector<char>> possibleDoubleCharacterSwaps = {
        {{'|', '<'}, {'k', 'e'}},
        {{'|', '_'}, {'l'}},
        {{'/', '\\'}, {'a'}},
        {{'\\', '/'}, {'v', 'u'}},
        {{'|', ')'}, {'d'}},
        {{'(', ')'}, {'o', 'u'}}
    };
    outputArray.clear();

    //single characters
    for (int i = 0; i < sourceArray.size(); i++) {
        std::vector<std::pair<std::pair<int, int>, char>> possibleLettersInWord;
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
        std::vector<std::pair<std::pair<int, int>, char>> possibleDoubleLettersInWord;
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


void ProfanityFilter::loadData() {
    std::string inputWord;
    while (std::cin >> inputWord) {
        sourceArray.push_back(inputWord);
        originalData.push_back(inputWord);
    }
}


bool ProfanityFilter::containsSubstring(std::string& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord) {
    for (int i = 0; i < (int)checkedWord.size() - (int)substringWord.size() + 1; i++) {
        int currentAllowedCharactersBetweenInWord = possibleCharactersBetweenInWord;
        int idx = i;
        bool found = true;
        for (int j = 0; j < substringWord.size(); j++) {
            while (idx + j < checkedWord.size() && substringWord[j] != checkedWord[idx + j]) {
                idx++;
                currentAllowedCharactersBetweenInWord--;
                if (currentAllowedCharactersBetweenInWord < 0) {
                    found = false;
                    break;
                }
            }
            if (currentAllowedCharactersBetweenInWord < 0 || idx + j > checkedWord.size()) {
                found = false;
                break;
            }
        }
        if (found)
            return true;
    }
    return false;
}


bool ProfanityFilter::isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray) {
    //if (isOnWhitelist(potentialProfanityWord)) todo
    //    return false;
    for (int i = 0; i < profanitiesArray.size(); i++) {
        std::string& currentProfanity = profanitiesArray[i];
        if (!wordMaskFactory.canBeProfanity(potentialProfanityWord, i))
            continue;
        if (containsSubstring(potentialProfanityWord, currentProfanity, 2))
            return true;

        //If profanity has two adjacent letters swapped
        for (int i = 0; i < potentialProfanityWord.size() - 1; i++) {
            std::swap(potentialProfanityWord[i], potentialProfanityWord[i + 1]);
            if (containsSubstring(potentialProfanityWord, currentProfanity, 0))
                return true;
            std::swap(potentialProfanityWord[i], potentialProfanityWord[i + 1]);
        }
    }
    return false;

}


bool ProfanityFilter::findProfanityInAllPossibleWords(std::vector<std::pair<std::pair<int, int>, char>>& inputArray,
    int index, int arrayLocationIndex, std::string* currentWord) {
    if (index > inputArray.back().first.first) {
        return isProfanity((*currentWord), profanitiesArray);
    }
    bool foundProfanity = false;
    while (arrayLocationIndex < inputArray.size() && inputArray[arrayLocationIndex].first.first < index)
        arrayLocationIndex++;
    while (arrayLocationIndex < inputArray.size() && inputArray[arrayLocationIndex].first.first == index) {
        (*currentWord) += inputArray[arrayLocationIndex].second;
        bool isProfanityFound = findProfanityInAllPossibleWords(inputArray,
            index + inputArray[arrayLocationIndex].first.second, arrayLocationIndex, currentWord);
        if (isProfanityFound)
            foundProfanity = true;
        (*currentWord).pop_back();
        arrayLocationIndex++;
    }
    return foundProfanity;
}


void ProfanityFilter::censorInputtedText() {
    wordMaskFactory = MaskFactory();

    removeUnambiguousDiactrics(sourceArray);
    removeSpecialCharactersAndDigits(sourceArray);
    toLowerCases(sourceArray);
    collapseLetters(sourceArray);
    generatePossibleVariationsOfLetters(sourceArray, processedArray);

    profanitiesArray = { "kurwa", "kutas"};

    for (int i = 0; i < processedArray.size(); i++) {
        std::string tmp = "";
        bool isCensored = findProfanityInAllPossibleWords(processedArray[i], 0, 0, &tmp);
        if (isCensored)
            outputArray.push_back(std::string(originalData[i].size(), '*'));
        else
            outputArray.push_back(originalData[i]);
    }
}


void ProfanityFilter::showCensoredText() {
    for (std::string word : outputArray) {
        std::cout << word << " ";
    }
}
