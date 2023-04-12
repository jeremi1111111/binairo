#pragma once
#include <string>
#include <fstream>
#include <bitset>
#include <iostream>

class board {
	int width;
	int height;
	std::bitset<32>* rows_x;
	std::bitset<32>* rows_o;
	std::bitset<32>* cols_x;
	std::bitset<32>* cols_o;
public:
	// import from file
	board(std::string);
	~board();
	// random board generator
	// board(int, int);

	void print();
	void print_bitset_overview();
	void set(int, int, int);
	void set(int, std::bitset<32>, int);
	int count();
	bool check_if_solved();
	void solve(int = 0);
};