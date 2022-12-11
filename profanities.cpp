// profanities.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "ProfanityFilter.h"

int main() {
	std::ios_base::sync_with_stdio(false);
	std::cin.tie(NULL);
	ProfanityFilter myFilter;
	myFilter.loadData();
	myFilter.censorInputtedText();
	myFilter.showCensoredText();

}