#include <iostream>

#include "../classes/headers/board.hh"
#include "../classes/headers/board_importer.hh"
#include "../classes/headers/minimax.hh"
#include "../classes/headers/move_gen.hh"
#include "../classes/headers/perft.hh"

int main(int argc, char* argv[]) {
    BitShifts::Init();
    Board board = Board();
    BoardImporter::ImportMoveSequence(&board, "");
    MiniMax max = MiniMax(&board);
    Move move = max.GetBestMove();
    printf("Move found %s\n", move.ToString().c_str());
    printf("Moves made %llu\n", board.GetStats().totalMoves);
}
