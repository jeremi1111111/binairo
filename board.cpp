#include "board.h"

board::board(std::string file_name)
{
	std::fstream file;
	file.open(file_name);
	if (file.bad())
		return;
	file >> this->width;
	file >> this->height;
	this->cols_x = new std::bitset<32>[width] {};
	this->cols_o = new std::bitset<32>[width] {};
	this->rows_x = new std::bitset<32>[height] {};
	this->rows_o = new std::bitset<32>[height] {};
	// picking row
	for (int i = 0; i < height; i++)
	{
		// picking column
		for (int j = 0; j < width; j++)
		{
			char c;
			file >> c;
			if (c == 'x')
			{
				rows_x[i].set(j, true);
				cols_x[j].set(i, true);
			}
			else if (c == 'o')
			{
				rows_o[i].set(j, true);
				cols_o[j].set(i, true);
			}
		}
	}
}

void board::print()
{
	std::cout << std::endl;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (rows_x[i].test(j))
				std::cout << 'x';
			else if (rows_o[i].test(j))
				std::cout << 'o';
			else
				std::cout << '.';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void board::set(int mode, int n1, int n2)
{
	switch (mode)
	{
	case 1:
		rows_o[n1].set(n2);
		cols_o[n2].set(n1);
		break;
	case 2:
		cols_x[n1].set(n2);
		rows_x[n2].set(n1);
		break;
	case 3:
		cols_o[n1].set(n2);
		rows_o[n2].set(n1);
		break;
	default:
		rows_x[n1].set(n2);
		cols_x[n2].set(n1);
		break;
	}
}

bool board::check_if_solved()
{
	for (int i = 0; i < height; i++) {
		if (rows_o[i].count() + rows_x[i].count() < width)
			return 1;
	}
	return 0;
}

void board::solve(int mode)
{
	// modes:
	// 0 - test rows_o
	// 1 - test rows_x
	// 2 - test cols_o
	// 3 - test cols_x

	std::bitset<32>* p1; // test bitsets
	std::bitset<32>* p2; // other sign bitsets
	int b1; // rows/cols quantity
	int b2; // row/col boundary
	switch (mode)
	{
	case 1:
		p1 = rows_x;
		p2 = rows_o;
		b1 = height;
		b2 = width;
		break;
	case 2:
		p1 = cols_o;
		p2 = cols_x;
		b1 = width;
		b2 = height;
		break;
	case 3:
		p1 = cols_x;
		p2 = cols_o;
		b1 = width;
		b2 = height;
		break;
	default:
		p1 = rows_o;
		p2 = rows_x;
		b1 = height;
		b2 = width;
		break;
	}
	for (int i = 0; i < b1; i++)
	{
		// test mask '11'
		for (int j = 0; j < b2 - 1; j++) {
			if (p1[i].test(j) && p1[i].test(j + 1)) {
				if (j - 1 >= 0)
					set(mode, i, j - 1);
				if (j + 2 < b2)
					set(mode, i, j + 2);
			}
		}
		// test mask '101'
		for (int j = 0; j < b2 - 2; j++)
			if (p1[i].test(j) && p1[i].test(j + 2))
				set(mode, i, j + 1);
		// test counters
	}
	if (mode < 3)
		solve(mode + 1);
	//if (mode == 0 && check_if_solved() == 1)
	//	solve(0);
}
