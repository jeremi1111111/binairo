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
	this->cols_y = new std::bitset<32>[width] {};
	this->rows_x = new std::bitset<32>[height] {};
	this->rows_y = new std::bitset<32>[height] {};
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
				rows_y[i].set(j, true);
				cols_y[j].set(i, true);
			}
		}
	}
}

void board::print()
{
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (rows_x[i].test(j))
				std::cout << 'x';
			else if (rows_y[i].test(j))
				std::cout << 'o';
			else
				std::cout << '.';
		}
		std::cout << std::endl;
	}
}
