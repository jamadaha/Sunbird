#include <iostream>

#include "classes/headers/bit_board.hh"
#include "classes/headers/evaluator.hh"
#include "classes/headers/board_importer.hh"
#include "classes/headers/move_gen.hh"

int main(int, char**) {
    BitBoard board = BitBoard();
    BoardImporter::ImportFEN(&board, "8/8/8/8/4R3/8/8/8 w KQkq - 0 1");
    MoveGen moveGen = MoveGen();

    std::vector<Move> moves = moveGen.GetRookMoves(board.GetColor(), board);
}
