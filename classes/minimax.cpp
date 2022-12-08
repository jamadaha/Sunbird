#include "headers/minimax.hh"

MiniMax::MiniMax(Board* board) : board(board), evaluator(Evaluator(board->GetColor())) {
    moveGens[(U8) Color::White] = new MoveGen(Color::White);
    moveGens[(U8) Color::Black] = new MoveGen(Color::Black);
}

Move MiniMax::GetBestMove(int depth) {
    return NegaMax(depth);
}

Move MiniMax::NegaMax(int depth) {
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attacks[2] = { 0, 0 };
    U8 moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attacks);

    Move bestMove = Move(MoveType::Quiet);
    int bestScore = -(int) PieceValue::Inf;

    for (int i = 0; i < moveCount; i++) {
        board->DoMove(&moves[i]);
        int score = -NegaMax(depth - 1, -(int) PieceValue::Inf, (int) PieceValue::Inf, attacks);
        board->UndoMove(moves[i]);

        if (score > bestScore) {
            bestScore = score;
            bestMove = moves[i];
        }
    }

    free(moves);
    return bestMove;
}

int MiniMax::NegaMax(int depth, int alpha, int beta, U64 attackedSquares[2]) {
    if (depth == 0)
        return Quiesce(alpha, beta, attackedSquares);
    
    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);

    int score = -(int) PieceValue::Inf;
    for (int i = 0; i < moveCount; i++) {
        board->DoMove(&moves[i]);
        score = std::max(score, -NegaMax(depth - 1, -beta, -alpha, attackSquares));
        board->UndoMove(moves[i]);

        alpha = std::max(alpha, score);
        if (alpha >= beta)
            break;
    }

    free(moves);
    return score;
}

int MiniMax::Quiesce(int alpha, int beta, U64 attackedSquares[2]) {
    int standPat = evaluator.Evalute(*board);
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    Move* moves = (Move*) calloc(MAXMOVECOUNT, sizeof(Move));
    U64 attackSquares[2] = { attackedSquares[0], attackedSquares[1] };
    int moveCount = moveGens[(int) board->GetColor()]->GetAllMoves(moves, *board, &attackSquares);    

    for (int i = 0; i < moveCount; i++) {
        if (!moves[i].IsCapture())
            continue;

        board->DoMove(&moves[i]);
        int score = Quiesce(-beta, -alpha, attackedSquares);
        board->UndoMove(moves[i]);

        if (score >= beta)
            return beta;
        if (score > alpha)
            alpha = score;
    }
    free(moves);
    return alpha;
}
