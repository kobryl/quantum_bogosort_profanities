// profanities.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include "ProfanityFilter.h"

int main() {
	ProfanityFilter myFilter;
	myFilter.loadData("dane.txt");
	myFilter.censorInputedText();
	myFilter.showCensoredText();

}