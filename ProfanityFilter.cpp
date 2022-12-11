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
        {{-61, -77}, 'u'},
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
        {'v', {'v', 'u', 'w'}},
        {'z', {'z', 's'}},
        {'s', {'s', 'z'}},
        {'u', {'u', 'v', 'o'}}
    };
    std::map<std::pair<char, char>, std::vector<char>> possibleDoubleCharacterSwaps = {
        {{'i', '<'}, {'k', 'e'}},
        {{'i', '_'}, {'l'}},
        {{'/', '\\'}, {'a'}},
        {{'\\', '/'}, {'v', 'u'}},
        {{'i', ')'}, {'d'}},
        {{'(', ')'}, {'o', 'u'}}
    };
    processedArray.clear();

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


bool ProfanityFilter::containsSubstring(std::string& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord,
    int allowedCharactersInFront, int allowedCharactersInBack, int skip) {
    int startIndex = std::max(skip,  int((int)checkedWord.size() - substringWord.size() - allowedCharactersInBack));
    int endIndex = std::min((int)checkedWord.size() - (int)substringWord.size() + 1, allowedCharactersInFront + skip + 1);
    for (int i = startIndex; i < endIndex; i++) {
        int currentAllowedCharactersBetweenInWord = possibleCharactersBetweenInWord;
        int idx = i;
        bool found = true;
        for (int j = 0; j < substringWord.size(); j++) {
            while (idx + j < endIndex + substringWord.size() && substringWord[j] != checkedWord[idx + j]) {
                idx++;
                currentAllowedCharactersBetweenInWord--;
                if (currentAllowedCharactersBetweenInWord < 0) {
                    found = false;
                    break;
                }
            }
            if (currentAllowedCharactersBetweenInWord < 0 || idx + j >= endIndex + substringWord.size()) {
                found = false;
                break;
            }
        }
        if (found)
            return true;
    }
    return false;
}


bool ProfanityFilter::isOnWhitelist(std::string& potentialProfanityWord, int skip) {
    wordMaskFactory.setCurrentWordMask(potentialProfanityWord, skip);
    for (int i = 0; i < whitelistArray.size(); i++) {
        std::string& currentWhitelistedWord = whitelistArray[i];
        int charactersBeforeWord = allowedWhitelistCharactersBeforeAndAfterWord[i].first;
        int charactersAfterWord = allowedWhitelistCharactersBeforeAndAfterWord[i].second;
        if (!wordMaskFactory.canBeWhitelisted(i))
            continue;
        if (charactersAfterWord + charactersBeforeWord + currentWhitelistedWord.size() < potentialProfanityWord.size())
            continue;
        if (containsSubstring(potentialProfanityWord, currentWhitelistedWord, 0, charactersBeforeWord, charactersAfterWord))
            return true;
    }
    return false;
}


bool ProfanityFilter::isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray, int skip) {
    wordMaskFactory.setCurrentWordMask(potentialProfanityWord, skip);
    for (int i = 0; i < profanitiesArray.size(); i++) {
        std::string& currentProfanity = profanitiesArray[i];
        int charactersBeforeWord = allowedProfanityCharactersBeforeAndAfterWord[i].first;
        int charactersAfterWord = allowedProfanityCharactersBeforeAndAfterWord[i].second;
        int characterBetweenWord = allowedCharactersBetweenWordsArray[i];
        if (skip != -1) {
            characterBetweenWord = 0;
        }
        if (!wordMaskFactory.canBeProfanity(i))
            continue;
        if (charactersAfterWord + charactersBeforeWord + currentProfanity.size() + skip < potentialProfanityWord.size())
            continue;
        if (containsSubstring(potentialProfanityWord, currentProfanity, characterBetweenWord,
            charactersBeforeWord, charactersAfterWord, std::max(skip, 0))) {
            std::cout << potentialProfanityWord.substr(std::max(skip, 0)) << "WULGARYZM!! " << currentProfanity << " originalnie:"<<originalData[originalIndex] << "\n";
            return true;
        }

        if (skip != -1)
            continue;
        //If profanity has two adjacent letters swapped
        for (int i = 0; i < potentialProfanityWord.size() - 1; i++) {
            std::swap(potentialProfanityWord[i], potentialProfanityWord[i + 1]);
            if (containsSubstring(potentialProfanityWord, currentProfanity, 0, charactersBeforeWord,
                charactersAfterWord, 0)) {
std::cout << potentialProfanityWord << "WULGARYZM!! " << currentProfanity << " originalnie:" << originalData[originalIndex] << "\n";
                return true;
            }
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
            return true;
        (*currentWord).pop_back();
        arrayLocationIndex++;
    }
    return foundProfanity;
}


void ProfanityFilter::loadWhitelist() {
    std::ifstream whitelistFile("whitelist.txt");
    std::string whitelistWord;
    int charactersBeforeWord, charactersAfterWord;
    while (whitelistFile >> whitelistWord  >> charactersBeforeWord >> charactersAfterWord) {
        whitelistArray.push_back(whitelistWord);
        allowedWhitelistCharactersBeforeAndAfterWord.push_back({ charactersBeforeWord, charactersAfterWord });
    }
}


void ProfanityFilter::loadProfanities() {
    std::ifstream profanitiesListFile("profanity_list.txt");
    std::string profinityWord;
    int allowedCharactersBetween;
    int charactersBeforeWord, charactersAfterWord;
    while (profanitiesListFile >> profinityWord >> allowedCharactersBetween >> charactersBeforeWord >> charactersAfterWord) {
        profanitiesArray.push_back(profinityWord);
        allowedCharactersBetweenWordsArray.push_back(allowedCharactersBetween);
        allowedProfanityCharactersBeforeAndAfterWord.push_back({ charactersBeforeWord, charactersAfterWord });
    }
}


void ProfanityFilter::censorInputtedText() {
    wordMaskFactory = MaskFactory();

    loadProfanities();
    loadWhitelist();

    removeUnambiguousDiactrics(sourceArray);
    removeSpecialCharactersAndDigits(sourceArray);
    toLowerCases(sourceArray);
    collapseLetters(sourceArray);
    generatePossibleVariationsOfLetters(sourceArray, processedArray);

    for (int i = 0; i < sourceArray.size(); i++) {
        if (isOnWhitelist(sourceArray[i]))
            isWhitelistedArray.push_back(true);
        else
            isWhitelistedArray.push_back(false);
    }

    for (int i = 0; i < processedArray.size(); i++) {
        std::string tmp = "";
        bool isCensored = false;
        originalIndex = i;
        if (!isWhitelistedArray[i]) {
            isCensored = findProfanityInAllPossibleWords(processedArray[i], 0, 0, &tmp);
        }
        if (isCensored)
            isCensoredArray.push_back(true);
        else
            isCensoredArray.push_back(false);
    }
    int maxLengthOfWord = 0;
    for (int i = 0; i < profanitiesArray.size(); i++) {
        int charactersBeforeWord = allowedProfanityCharactersBeforeAndAfterWord[i].first;
        int charactersAfterWord = allowedProfanityCharactersBeforeAndAfterWord[i].second;
        maxLengthOfWord = std::max(maxLengthOfWord, (int)profanitiesArray[i].size() + charactersBeforeWord + charactersAfterWord);
    }
    std::cout << "Profanities with spaces: \n";
    std::string currentWordWithSkippedSpaces = "";
    int currentFrontIndex = 0, skip = 0, currentWordLength = 0;
    for (int i = 0; i < processedArray.size(); i++) {

        if (isCensoredArray[i]) {
            continue;
        }
        currentWordWithSkippedSpaces += sourceArray[i];
        currentWordLength += sourceArray[i].size();
        if (currentWordWithSkippedSpaces.size() > 10000){
            currentWordWithSkippedSpaces = currentWordWithSkippedSpaces.substr(std::max(skip, 0));
            skip = 0;
        }
        while (currentWordLength > maxLengthOfWord * 2) {
            if (!isCensoredArray[currentFrontIndex]) {
                skip += sourceArray[currentFrontIndex].size();
                currentWordLength -= sourceArray[currentFrontIndex].size();
            }
            currentFrontIndex++;
        }
        if (currentFrontIndex == i)
            continue;
        if (isWhitelistedArray[i]) {
            currentFrontIndex = i;
            skip = currentWordWithSkippedSpaces.size();
            currentWordLength = 0;
            continue;
        }
        bool isCensored = false;
        if (!isOnWhitelist(currentWordWithSkippedSpaces, skip))
            isCensored = isProfanity(currentWordWithSkippedSpaces, profanitiesArray, skip);
        if (isCensored) {
            while (isProfanity(currentWordWithSkippedSpaces, profanitiesArray, skip)) {
                skip += sourceArray[currentFrontIndex].size();
                currentWordLength -= sourceArray[currentFrontIndex].size();
                currentFrontIndex++;
            }
            currentFrontIndex--;
            skip -= sourceArray[currentFrontIndex].size();
            currentWordLength += sourceArray[currentFrontIndex].size();
            while (currentFrontIndex <= i) {
                if (!isCensoredArray[currentFrontIndex]) {
                    isCensoredArray[currentFrontIndex] = true;
                    skip += sourceArray[currentFrontIndex].size();
                    currentWordLength -= sourceArray[currentFrontIndex].size();
                }
                currentFrontIndex++;
            }
        }
    }

    for (int i = 0; i < isCensoredArray.size(); i++) {
        if (isCensoredArray[i])
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
