// Binairo solver

#include "board.h"

int main()
{
	board b1{ "board10x10easy.txt" };
	b1.print();
	b1.solve();
	b1.print();
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