// Binairo solver

#include "board.h"

int main()
{
	board b1{ "board8x8hard.txt" };
	b1.print();
	b1.solve();
	b1.print();
	//b1.print_bitset_overview();
}

// Ideas:
// - more complex conditions
// - skip completed rows/cols
// - UI
// - stop solving condition (if no changes were made)
// - color initial board (for solver)
// - generator (based on solver)
// ! deduce using hypothetical solution (when specific cells have to be filled with exact amount of x's and/or o's, but their placement is not yet known)
// - check if rows/cols are unique
// - continue solving in row and/or col of latest added cell
// - allocate bitsets according to board size (template class board)