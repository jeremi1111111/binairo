#include "board.h"

board::board(std::string file_name)
{
	// file format:
	// height width '\n'
	// 'x' / 'o' / '.' - free space / '\n'
	// or
	// 'x' / 'o' / "%d" - free spaces to next x/o
	// or
	// 'x' / 'o' / '%d' - free spaces to next x/o or '\n' / '\n'
	std::fstream file;
	file.open(file_name);
	if (file.bad()) {
		file.close();
		return;
	}
	file >> this->height;
	file >> this->width;
	this->rows_x = new std::bitset<32>[height] {};
	this->rows_o = new std::bitset<32>[height] {};
	this->cols_x = new std::bitset<32>[width] {};
	this->cols_o = new std::bitset<32>[width] {};
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
			else if (isdigit(c))
			{
				file.unget();
				int s;
				file >> s;
				s += j - 1;
				j = s % width;
				i += s / height;
			}
		}
	}
	file.close();
}

board::~board()
{
	delete[] rows_o;
	delete[] rows_x;
	delete[] cols_o;
	delete[] cols_x;
}

void board::print()
{
	std::cout << std::endl;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			if (rows_x[i].test(j))
				std::cout << "x ";
			else if (rows_o[i].test(j))
				std::cout << "o ";
			else
				std::cout << ". ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void board::print_bitset_overview()
{
	std::cout << "rows_o" << std::endl;
	for (int i = 0; i < height; i++)
	{
		std::cout << rows_o[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "rows_x" << std::endl;
	for (int i = 0; i < height; i++)
	{
		std::cout << rows_x[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "cols_o" << std::endl;
	for (int i = 0; i < width; i++)
	{
		std::cout << cols_o[i] << std::endl;
	}
	std::cout << std::endl;
	std::cout << "cols_x" << std::endl;
	for (int i = 0; i < width; i++)
	{
		std::cout << cols_x[i] << std::endl;
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

void board::set(int mode, std::bitset<32> mask, int n)
{
	if (mask.none())
		return;
	std::cout << mask << std::endl;
	std::bitset<32>* p1; // masked bitsets
	std::bitset<32>* p2; // other direction bitsets
	int b; // other direction boundary
	switch (mode)
	{
	case 1:
		p1 = rows_o;
		p2 = cols_o;
		b = height;
		break;
	case 2:
		p1 = cols_x;
		p2 = rows_x;
		b = width;
		break;
	case 3:
		p1 = cols_o;
		p2 = rows_o;
		b = width;
		break;
	default:
		p1 = rows_x;
		p2 = cols_x;
		b = height;
		break;
	}
	p1[n] |= mask;
	for (int i = 0; i < b; i++)
		if (mask.test(i))
			p2[i].set(n);
	//print();
}

int board::count()
{
	int count = 0;
	for (int i = 0; i < height; i++)
		count += rows_o[i].count() + rows_x[i].count();
	return count;
}

bool board::check_if_solved()
{
	for (int i = 0; i < height; i++)
		if (rows_o[i].count() + rows_x[i].count() < width)
			return 1;
	return 0;
}

void board::solve(int mode)
{
	// modes:
	// 0 - test rows, assign x
	// 1 - test rows, assign o
	// 2 - test cols, assign x
	// 3 - test cols, assign o

	std::bitset<32>* p1; // primary sign bitsets
	std::bitset<32>* p2; // opposite sign bitsets
	std::bitset<32> gm; // guarding mask
	gm.set();
	int b1; // rows/cols quantity
	int b2; // row/col boundary
	int c = count();
	switch (mode)
	{
	case 1:
		p1 = rows_x;
		p2 = rows_o;
		gm >>= 32 - width;
		b1 = height;
		b2 = width;
		break;
	case 2:
		p1 = cols_o;
		p2 = cols_x;
		gm >>= 32 - height;
		b1 = width;
		b2 = height;
		break;
	case 3:
		p1 = cols_x;
		p2 = cols_o;
		gm >>= 32 - height;
		b1 = width;
		b2 = height;
		break;
	default:
		p1 = rows_o;
		p2 = rows_x;
		gm >>= 32 - width;
		b1 = height;
		b2 = width;
		break;
	}
	for (int i = 0; i < b1; i++)
	{
		std::bitset<32> mask1; // primary mask
		std::bitset<32> mask2; // secondary mask
		// test mask "11"
		mask1 = 0b11;
		mask2 = 0b1001;
		for (int j = 0; j < b2 - 1; j++)
			if ((p1[i] & (mask1 << j)) == (mask1 << j))
				set(mode, ((mask2 << j) >> 1) & (~p2[i] & gm), i);
		// test mask "101"
		mask1 = 0b101;
		mask2 = 0b10;
		for (int j = 0; j < b2 - 2; j++)
			if ((p1[i] & (mask1 << j)) == (mask1 << j))
				set(mode, (mask2 << j) & (~p2[i] & gm), i);
		// test mask "1001", testing opposite sign! (the point is to reach max count of one sign)
		// test mask p1:"1000", p2:"0001" and vice versa
		mask1 = 0b1;
		mask2 = 0b110;
		for (int j = 0; j < b2 - 3; j++)
			if ((p1[i] & (mask1 << j)) == (mask1 << j) && (p2[i] & (mask1 << j + 3)) == (mask1 << j + 3))
				if (p1[i].count() + 1 == b2 / 2)
					set(mode, (~p1[i] ^ (mask2 << j)) & (~p2[i] & gm), i);
		// test counter
		if (p1[i].count() == b2 / 2)
			set(mode, ~p1[i] & (~p2[i] & gm), i);
	}
	if (mode < 3)
		solve(mode + 1);
	if (mode == 0 && count() != c && count() < height * width)
		solve(0);
}
