#ifndef MINIMAX
#define MINIMAX

#include <random>
#include <algorithm>
#include <iterator>

#include "constants.hh"
#include "board.hh"
#include "move_gen.hh"
#include "move.hh"
#include "evaluator.hh"

class MiniMax {
public:
    MiniMax(Board* board);
    Move GetBestMove(int depth);
    
private:
    Board* board;
    MoveGen* moveGens[2];
    Evaluator* evaluators[2];

    int NegaMax(bool original, Move* bestMove, int depth, int alpha, int beta, U64 attackedSquares[2]);
};
#endif
