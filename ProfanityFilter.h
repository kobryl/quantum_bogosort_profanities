#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <utility>
#include <algorithm>
#include "MaskFactory.h"

/**
 * The `ProfanityFilter` class provides a way to filter out profanity from a string by performing several
 * transformations on the string, such as collapsing repeating characters, removing special characters and digits,
 * and converting the string to lower case. It also generates possible variations
 * of each character in the string. This allows the filter to identify and censor profanity even if it has been
 * transformed in some way.
 */

class ProfanityFilter {
private:

    /**
     * An array of profanity words.
     */
    std::vector<std::string> profanitiesArray;

    /**
     * An array of words to filter for profanity.
     */
    std::vector<std::string> sourceArray;

    /**
     * An array of the original data before it was filtered.
     */
    std::vector<std::string> originalData;

    /**
     * An array of words that are not considered profanity, even if they share part of its spelling with profanity.
     */
    std::vector<std::string> whitelistArray;

    /**
     * An array of arrays containing the possible character variations for each word in the `sourceArray`.
     */
    std::vector<std::vector<std::pair<std::pair<int, int>, char>>> processedArray;

    /**
     * An array containing the number  of allowed characters between the characters that can spell profanity
     */
    std::vector<int> allowedCharactersBetweenWordsArray;

    /**
     * An array containing pairs of the number of allowed whitelist characters before and after a word.
     */
    std::vector<std::pair<int, int>> allowedWhitelistCharactersBeforeAndAfterWord;

    /**
     * An array containing pairs of the number of allowed profanity characters before and after a word.
     */
    std::vector<std::pair<int, int>> allowedProfanityCharactersBeforeAndAfterWord;

    /**
     * An array indicating whether a given word in the `sourceArray` has been censored or not.
     */
    std::vector<bool> isCensoredArray;

    /**
     * An array indicating whether a given word in the `sourceArray` is on the whitelist or not.
     */
    std::vector<bool> isWhitelistedArray;

    /**
     * A factory to generate and check masks for improving censoring speed.
     */
    MaskFactory wordMaskFactory;

    /*
    * For debug purposes only
    */
    int originalIndex;

    /**
     * Collapses repeated characters in the given array of strings.
     *
     * For example, the string "hello" would become "helo".
     *
     * @param sourceArray The array of strings to collapse the repeated characters in.
     */
    void collapseLetters(std::vector<std::string>& sourceArray);


    /**
     * Replaces special characters and digits from the given array of strings.
     *
     * For example, the string "hello@123" would become "helloaize".
     *
     * @param sourceArray The array of strings to remove the special characters and digits from.
     */
    void removeSpecialCharactersAndDigits(std::vector<std::string>& sourceArray);

    /**
     * Converts the given array of strings to lower case.
     *
     * @param sourceArray The array of strings to convert to lower case.
     */
    void toLowerCases(std::vector<std::string>& sourceArray);

    /**
     * Replaces certain diacritical marks from the given array of strings to characters that are easier to handle in code.
     *
     * @param sourceArray The array of strings to remove the diacritical marks from.
     */
    void removeUnambiguousDiactrics(std::vector<std::string>& sourceArray);

    /**
     * Generates possible variations of each character in the given array of strings.
     * For example, word `all` would have 4 variations: `all`, `ali`, `ail`, `aii`
     *
     * @param sourceArray The array of strings to generate the character variations for.
     * @param outputArray An array to store the generated character variations in.
     */
    void generatePossibleVariationsOfLetters(std::vector<std::string>& sourceArray,
        std::vector<std::vector<std::pair<std::pair<int, int>, char>>>& outputArray);

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
    bool containsSubstring(std::string& checkedWord, std::string& substringWord, int possibleCharactersBetweenInWord,
        int allowedCharactersInFront, int allowedCharactersInBack, int skip = 0);

    /**
     * Determines if the given `potentialProfanityWord` is a profanity, taking into account the list of profanities
     * and the number of characters to skip.
     *
     * @param potentialProfanityWord The string to check if it is a profanity.
     * @param profanitiesArray The list of profanities to compare against.
     * @param skip The number of characters to skip when checking for the `potentialProfanityWord` in the `profanitiesArray`.
     * @returns `true` if the `potentialProfanityWord` is a profanity, `false` otherwise.
     */
    bool isProfanity(std::string& potentialProfanityWord, std::vector<std::string>& profanitiesArray, int skip = -1);

    /**
     * Determines if the given `potentialProfanityWord` is on the whitelist, taking into account the number of characters
     * to skip.
     *
     * @param potentialProfanityWord The string to check if it is on the whitelist.
     * @param skip The number of characters to skip when checking for the `potentialProfanityWord` in the whitelist.
     * @returns `true` if the `potentialProfanityWord` is on the whitelist, `false` otherwise.
     */
    bool isOnWhitelist(std::string& potentialProfanityWord, int skip = 0);

    /**
     * Determines if the given `inputArray` contains any profanity, by checking all possible words that are possible to generate
     * from given array.
     *
     * @param inputArray The array of character variations to generate words and check for profanity in.
     * It contains information which character can be used on which index
     * @param index The index of the current character in the `inputArray` being checked.
     * @param arrayLocationIndex The index of the current character of `inputArray` being checked.
     * @param currentWord A pointer to the current word being checked.
     * @returns `true` if the `inputArray` contains profanity, `false` otherwise.
     */
    bool findProfanityInAllPossibleWords(std::vector<std::pair<std::pair<int, int>, char>>& inputArray,
        int index, int arrayLocationIndex, std::string* currentWord);

    /**
     * Loads the list of profanities from a file.
     */
    void loadProfanities();

    /**
     * Loads the whitelist from a file.
     */
    void loadWhitelist();
public:
    /**
     * An array containing the censored version of the inputted text after using censorInputtedText() method.
     */
    std::vector<std::string> outputArray;

    /**
     * Filters the inputted text to censor any profanity.
     */
    void censorInputtedText();

    /**
     * Loads the inputted text and the list of profanities and the whitelist from files.
     */
    void loadData();

    /**
     * Shows the censored version of the inputted text.
     */
    void showCensoredText();
};