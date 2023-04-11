#pragma once
#include <string>
#include <fstream>
#include <bitset>
#include <iostream>

class board {
	int width;
	int height;
	std::bitset<32>* rows_x;
	std::bitset<32>* rows_y;
	std::bitset<32>* cols_x;
	std::bitset<32>* cols_y;
public:
	// import from file
	board(std::string);
	// random board generator
	// board(int, int);

	void print();
};