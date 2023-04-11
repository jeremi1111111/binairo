#include "board.h"

int main()
{
	board b1{ "board.txt" };
	b1.print();

	// bonus task: make class "board" a template class, so that it can dynamically allocate bitsets 
}