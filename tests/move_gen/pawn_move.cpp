#include <stdexcept>
#include <string>
#include <stdlib.h>

#include "constants.hh"
#include "board.hh"
#include "board_importer.hh"
#include "move_gen.hh"
#include "pawn_gen.hh"

int main(int, char* argv[]) {
    BitShifts::Init();
    Board board = BoardImporter::ImportFEN((std::string) argv[2]);
    PawnGen moveGens[2] = { PawnGen(Color::White), PawnGen(Color::Black) };
    
    std::array<Move, MAXMOVECOUNT> moves;
    int moveCount = moveGens[(int) board.GetColor()].GetALlMoves(moves, std::make_shared<Board>(board), 0);

    if (moveCount == std::atoi(argv[1]))
        exit(EXIT_SUCCESS);
    else
        throw std::logic_error("Incorrect move count | Expected " + (std::string)argv[1] + " - Actual " + std::to_string(moveCount));
}