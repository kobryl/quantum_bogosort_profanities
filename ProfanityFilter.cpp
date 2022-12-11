#include "ProfanityFilter.h"
#include "MaskFactory.h"

/**
 * The `ProfanityFilter` class provides a way to filter out profanity from a string by performing several
 * transformations on the string, such as collapsing repeating characters, removing special characters and digits,
 * and converting the string to lower case. It also generates possible variations
 * of each character in the string. This allows the filter to identify and censor profanity even if it has been
 * transformed in some way.
 */

 /**
  * Collapses repeated characters in the given array of strings.
  *
  * For example, the string "hello" would become "helo".
  *
  * @param sourceArray The array of strings to collapse the repeated characters in.
  */
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

/**
 * Replaces special characters and digits from the given array of strings.
 *
 * For example, the string "hello@123" would become "helloaize".
 *
 * @param sourceArray The array of strings to remove the special characters and digits from.
 */
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

/**
 * Converts the given array of strings to lower case.
 *
 * @param sourceArray The array of strings to convert to lower case.
 */
void ProfanityFilter::toLowerCases(std::vector<std::string>& sourceArray) {
    for (int i = 0; i < sourceArray.size(); i++) {
        for (int j = 0; j < sourceArray[i].size(); j++) {
            char currentCharacter = tolower(sourceArray[i][j]);
            sourceArray[i][j] = currentCharacter;
        }
    }
}

/**
 * Replaces certain diacritical marks from the given array of strings to characters that are easier to handle in code.
 *
 * @param sourceArray The array of strings to remove the diacritical marks from.
 */
void ProfanityFilter::removeUnambiguousDiactrics(std::vector<std::string>& sourceArray) {
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
}

/**
 * Generates possible variations of each character in the given array of strings.
 * For example, word `all` would have 4 variations: `all`, `ali`, `ail`, `aii`
 *
 * @param sourceArray The array of strings to generate the character variations for.
 * @param outputArray An array to store the generated character variations in.
 */
void ProfanityFilter::generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
    std::vector<std::vector<std::pair<std::pair<int, int>, char>>>& outputArray) {
    std::map<char, std::vector<char>> possibleSingleCharacterSwaps = {
        {'i', {'l', 'i'}},
        {'l', {'i', 'l'}},
        {'v', {'v', 'u', 'w'}},
        {'z', {'z', 's'}},
        {'s', {'s', 'z'}},
        {'u', {'u', 'v', 'o'}},
        {'ó', {'o', 'u'}}
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

bool ProfanityFilter::isGivenCharacterPossibleVariation(std::vector<std::pair<std::pair<int, int>, char>>& checkedWord, int characterIndex, int startIndex, char character) {
    while (startIndex < checkedWord.size() && checkedWord[startIndex].first.first < characterIndex)
        startIndex++;
    int originalIndex = startIndex;
    while (startIndex < checkedWord.size() && checkedWord[startIndex].first.first == characterIndex
        && checkedWord[startIndex].second != character)
        startIndex++;
    if (startIndex < checkedWord.size() && checkedWord[startIndex].second == character
        && checkedWord[startIndex].first.first == checkedWord[originalIndex].first.first)
        return true;
    return false;
}

/**
 * Loads the inputted text and the list of profanities and the whitelist from files.
 */
void ProfanityFilter::loadData() {
    std::string inputWord;
    while (std::cin >> inputWord) {
        sourceArray.push_back(inputWord);
        originalData.push_back(inputWord);
    }
}



/**
 * Determines if the given `checkedWord` contains the given `substringWord`, taking into account the number of allowed
 * characters between the words, and the allowed characters in front and back of the word.
 *
 * @param checkedWord The string to check for the presence of the given `substringWord`.
 * @param substringWord The string to search for in the `checkedWord`.
 * @param possibleCharactersBetweenInWord The number of allowed characters between the characters that can create `substringWord`.
 * @param allowedCharactersInFront The number of allowed characters in front of the word.
 * @param allowedCharactersInBack The number of allowed characters in back of the word.
 * @param skip The number of characters to skip when checking for the `substringWord` in the `checkedWord`.
 * @returns `true` if the `checkedWord` contains the `substringWord`, `false` otherwise.
 */
bool ProfanityFilter::containsSubstring(std::vector<std::pair<std::pair<int, int>, char>>& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord,
    int allowedCharactersInFront, int allowedCharactersInBack, int skip) {
    int offset = checkedWord[skip].first.first;
    int checkedWordSize = checkedWord[checkedWord.size() - 1].first.first - offset + 1;
    int startIndex = std::max(0,  int(checkedWordSize - substringWord.size() * 2 - possibleCharactersBetweenInWord - allowedCharactersInBack));
    int endIndex = std::min(checkedWordSize - (int)substringWord.size() / 2 + 1, allowedCharactersInFront + 1);
    int compressedCharacters = 0;
    for (int i = startIndex; i < endIndex; i++) {
        int currentAllowedCharactersBetweenInWord = possibleCharactersBetweenInWord;
        int idx = i;
        int checkedWordIt = i + skip;  //checkedWord[checkedWord.size() - 1].first.first + 1 > checkedWord.size() - skip
        while (checkedWordIt < checkedWord.size() && checkedWord[checkedWordIt].first.first - offset < i)
            checkedWordIt++;
        bool found = true;
        for (int j = 0; j < substringWord.size(); j++) {
            while (idx + j < endIndex + substringWord.size() 
            && !isGivenCharacterPossibleVariation(checkedWord, i + idx + j + offset, checkedWordIt, substringWord[j])) {
                currentAllowedCharactersBetweenInWord--;
                if (currentAllowedCharactersBetweenInWord < 0) {
                    found = false;
                    break;
                }
                idx++;
                while (checkedWordIt < checkedWord.size() && checkedWord[checkedWordIt].first.first - offset < i + idx)
                    checkedWordIt++;
                
            }
            if (currentAllowedCharactersBetweenInWord < 0 || idx + j >= endIndex + substringWord.size()) {
                found = false;
                break;
            }
            else {
                while (checkedWordIt < checkedWord.size() && checkedWord[checkedWordIt].first.first - offset < i + idx + j + 1){
                    if (checkedWord[checkedWordIt].second == substringWord[j]) {
                        compressedCharacters += checkedWord[checkedWordIt].first.second - 1;
                        idx += checkedWord[checkedWordIt].first.second - 1;
                        break;
                    }
                    checkedWordIt++;
                }
            }
        }
        if (found)
            return true;
    }
    return false;
}

/**
 * Determines if the given `potentialProfanityWord` is on the whitelist, taking into account the number of characters
 * to skip.
 *
 * @param potentialProfanityWord The string to check if it is on the whitelist.
 * @param skip The number of characters to skip when checking for the `potentialProfanityWord` in the whitelist.
 * @returns `true` if the `potentialProfanityWord` is on the whitelist, `false` otherwise.
 */
bool ProfanityFilter::isOnWhitelist(std::vector<std::pair<std::pair<int, int>, char>>& potentialProfanityWord, int skip) {
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

/**
 * Determines if the given `potentialProfanityWord` is a profanity, taking into account the list of profanities
 * and the number of characters to skip.
 *
 * @param potentialProfanityWord The string to check if it is a profanity.
 * @param profanitiesArray The list of profanities to compare against.
 * @param skip The number of characters to skip when checking for the `potentialProfanityWord` in the `profanitiesArray`.
 * @returns `true` if the `potentialProfanityWord` is a profanity, `false` otherwise.
 */
bool ProfanityFilter::isProfanity(std::vector<std::pair<std::pair<int, int>, char>>& potentialProfanityWord, std::vector<std::string>& profanitiesArray, int skip) {
    if (skip == potentialProfanityWord.size())
        return false;
    wordMaskFactory.setCurrentWordMask(potentialProfanityWord, skip);
    for (int i = 0; i < profanitiesArray.size(); i++) {
        std::string& currentProfanity = profanitiesArray[i];
        int charactersBeforeWord = allowedProfanityCharactersBeforeAndAfterWord[i].first;
        int charactersAfterWord = allowedProfanityCharactersBeforeAndAfterWord[i].second;
        int characterBetweenWord = allowedCharactersBetweenWordsArray[i];
        int potentialProfanityWordSize = potentialProfanityWord[potentialProfanityWord.size() - 1].first.first;
        
        if (skip != -1) {
            characterBetweenWord = 0;
        }
        if (!wordMaskFactory.canBeProfanity(i))
            continue;
        if (charactersAfterWord + charactersBeforeWord + currentProfanity.size() * 2 + std::max(skip, 0) < potentialProfanityWordSize)
            continue;
        if (containsSubstring(potentialProfanityWord, currentProfanity, characterBetweenWord,
            charactersBeforeWord, charactersAfterWord, std::max(skip, 0))) {
            std::cout << "WULGARYZM!! " << currentProfanity << " originalnie:" << originalData[originalIndex] << "\n";
            return true;
        }

        if (skip != -1)
            continue;
        //If profanity has two adjacent letters swapped
        // TO DO
        //for (int i = 0; i < potentialProfanityWordSize - 1; i++) {
        //    std::swap(potentialProfanityWord[i], potentialProfanityWord[i + 1]);
        //    if (containsSubstring(potentialProfanityWord, currentProfanity, 0, charactersBeforeWord,
        //        charactersAfterWord, 0)) {
        //        //std::cout << potentialProfanityWord << "WULGARYZM!! " << currentProfanity << " originalnie:" << originalData[originalIndex] << "\n";
        //        return true;
        //    }
        //    std::swap(potentialProfanityWord[i], potentialProfanityWord[i + 1]);
        //}
    }
    return false;

}

/**
 * Loads the whitelist from a file.
 */
void ProfanityFilter::loadWhitelist() {
    std::ifstream whitelistFile("whitelist.txt");
    std::string whitelistWord;
    int charactersBeforeWord, charactersAfterWord;
    while (whitelistFile >> whitelistWord  >> charactersBeforeWord >> charactersAfterWord) {
        whitelistArray.push_back(whitelistWord);
        allowedWhitelistCharactersBeforeAndAfterWord.push_back({ charactersBeforeWord, charactersAfterWord });
    }
}

/**
 * Loads the list of profanities from a file.
 */
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

/**
 * Filters the inputted text to censor any profanity.
 */
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
        if (isOnWhitelist(processedArray[i]))
            isWhitelistedArray.push_back(true);
        else
            isWhitelistedArray.push_back(false);
    }

    for (int i = 0; i < processedArray.size(); i++) {
        std::string tmp = "";
        bool isCensored = false;
        originalIndex = i;
        if (!isWhitelistedArray[i]) {
            isCensored = isProfanity(processedArray[i], profanitiesArray);
        }
        if (isCensored)
            isCensoredArray.push_back(true);
        else
            isCensoredArray.push_back(false);
    }
    int maxLengthOfWord = 0;
    for (int i = 0; i < profanitiesArray.size(); i++) {
        maxLengthOfWord = std::max(maxLengthOfWord, (int)profanitiesArray[i].size() * 2);
    }
    std::cout << "Profanities with spaces: \n";
    std::vector<std::pair<std::pair<int, int>, char>> currentWordWithSkippedSpaces;
    int currentFrontIndex = 0, skip = 0, currentWordLength = 0;
    for (int i = 0; i < processedArray.size(); i++) {
         originalIndex = i;
        if (isCensoredArray[i]) {
            continue;
        }
        int offset = 0;
        if(currentWordWithSkippedSpaces.size() > 0)
            offset = currentWordWithSkippedSpaces[currentWordWithSkippedSpaces.size() - 1].first.first + 1;
        for(int j = 0; j < processedArray[i].size(); j++){
            std::pair<int, int> newIndex = { processedArray[i][j].first.first + offset, processedArray[i][j].first.second };
            currentWordWithSkippedSpaces.push_back({newIndex, processedArray[i][j].second});
        }
        currentWordLength += processedArray[i][processedArray[i].size() - 1].first.first + 1;
        //to do
        //if (currentWordWithSkippedSpaces.size() > 10000){
        //    currentWordWithSkippedSpaces = currentWordWithSkippedSpaces.substr(std::max(skip, 0));
        //    skip = 0;
        //}

        while (currentWordLength > maxLengthOfWord && i - currentFrontIndex + 1 > 2) {
            if (!isCensoredArray[currentFrontIndex]) {
                skip += processedArray[currentFrontIndex].size();
                currentWordLength -= processedArray[currentFrontIndex][processedArray[currentFrontIndex].size() - 1].first.first + 1;
            }
            currentFrontIndex++;
        }
        if (currentFrontIndex == i)
            continue;
        if (isWhitelistedArray[i]) {
            currentFrontIndex = i + 1;
            skip = currentWordWithSkippedSpaces.size();
            currentWordLength = 0;
            continue;
        }
        bool isCensored = false;
        if (!isOnWhitelist(currentWordWithSkippedSpaces, skip))
            isCensored = isProfanity(currentWordWithSkippedSpaces, profanitiesArray, skip);
        if (isCensored) {
            while (isProfanity(currentWordWithSkippedSpaces, profanitiesArray, skip)) {
                skip += processedArray[currentFrontIndex].size();
                currentWordLength -= processedArray[currentFrontIndex][processedArray[currentFrontIndex].size() - 1].first.first + 1;
                currentFrontIndex++;
            }
            currentFrontIndex--;
            skip -= processedArray[currentFrontIndex].size();
            currentWordLength += processedArray[currentFrontIndex][processedArray[currentFrontIndex].size() - 1].first.first + 1;
            std::cout << "PRAWDZIWY wulgaryzm ze spacja: ";
            for (int j = currentFrontIndex; j <= i; j++) {
                std::cout << sourceArray[j];
            }
            std::cout << "\n";
            while (currentFrontIndex <= i) {
                if (!isCensoredArray[currentFrontIndex]) {
                    isCensoredArray[currentFrontIndex] = true;
                    skip += processedArray[currentFrontIndex].size();
                    currentWordLength -= processedArray[currentFrontIndex][processedArray[currentFrontIndex].size() - 1].first.first + 1;
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

/**
 * Shows the censored version of the inputted text.
 */
void ProfanityFilter::showCensoredText() {
    for (std::string word : outputArray) {
        std::cout << word << " ";
    }
}
