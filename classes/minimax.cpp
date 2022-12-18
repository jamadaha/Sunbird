#include "headers/minimax.hh"

Move MiniMax::GetBestMove(int depth) {
    return NegaMax(depth);
}

Move MiniMax::NegaMax(int depth) {
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attacks[2] = { 0, 0 };
    U8 moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, *board, &attacks);

    Move bestMove = Move(MoveType::Quiet);
    int bestScore = -(int) PieceValue::Inf;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(&moves[i]);
        int score = -NegaMax(depth - 1, -(int) PieceValue::Inf, (int) PieceValue::Inf, attacks);
        board->UndoMove(moves[i]);

        printf("%s %d\n", moves[i].ToString().c_str(), score);
        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    return bestMove;
}

int MiniMax::NegaMax(int depth, int alpha, int beta, U64 attackedSquares[2]) {
    if (depth == 0)
        return Quiesce(alpha, beta, attackedSquares);
    
    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, *board, &attackSquares);

    for (int moveType = 0; moveType < 2; moveType++) 
        for (int i = 0; i < moveCount; i++) {
            // Do capture before quiet
            if (moves.at(i).IsCapture()) {
                if (moveType == 1)
                    continue;
            } else {
                if (moveType == 0)
                    continue;
            }
            board->DoMove(&moves[i]);
            int score = -NegaMax(depth - 1, -beta, -alpha, attackSquares);
            board->UndoMove(moves[i]);

            if( score >= beta )
                return beta;   //  fail hard beta-cutoff
            if( score > alpha )
                alpha = score; // alpha acts like max in MiniMax
        }
        
    return alpha;
}

int MiniMax::Quiesce(int alpha, int beta, U64 attackedSquares[2]) {
    int standPat = evaluator.Evalute(*board);
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    std::array<Move, MAXMOVECOUNT> moves;
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()].GetAllMoves(&moves, *board, &attackSquares);    

    for (int i = 0; i < moveCount; i++) {
        if (!moves[i].IsCapture())
            continue;

        board->DoMove(&moves[i]);
        int score = -Quiesce(-beta, -alpha, attackedSquares);
        board->UndoMove(moves[i]);

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    return alpha;
}
