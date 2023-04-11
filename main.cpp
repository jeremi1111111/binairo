#include "board.h"

int main()
{
	board b1{ "board.txt" };
	b1.print();
	b1.solve();
	b1.print();
}