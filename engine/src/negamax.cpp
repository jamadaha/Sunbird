#include "engine/internal/values.hpp"
#include <chess/internal/constants.hpp>
#include <chess/move_gen.hpp>
#include <chrono>
#include <engine/evaluation.hpp>
#include <engine/negamax.hpp>
#include <engine/internal/opening_book.hpp>
#include <optional>

namespace Chess::Engine::Negamax {
namespace {
int Quiesce(Board &board, int alpha, int beta) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::Attack>(board, board.GetColor());

    if (moves.empty() && !board.IsKingSafe()) {
        moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
        if (moves.empty())
            return Evaluation::EvalNoMove(false);
    }

    int standPat = EVAL_FUNCTION(board);
    if (standPat >= beta)
        return beta;
    if (alpha < standPat)
        alpha = standPat;

    for (auto move : moves) {
        board.DoMove(move);
        int score = -Quiesce(board, -beta, -alpha);
        board.UndoMove(move);

        if (score >= beta)
            return beta; //  fail hard beta-cutoff
        if (score > alpha)
            alpha = score; // alpha acts like max in MiniMax
    }

    return alpha;
}

int Negamax(Board &board, int depth, int alpha, int beta) {
    if (depth == 0)
        return Quiesce(board, alpha, beta);

    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());

    if (moves.size() == 0)
        return Evaluation::EvalNoMove(board.IsKingSafe());
    if (board.IsThreefoldRep() || board.GetPly() > 150)
        return 0;

    for (auto move : moves) {
        board.DoMove(move);
        int score = -Negamax(board, depth - 1, -beta, -alpha);
        board.UndoMove(move);

        if (score >= beta)
            return beta; //  fail hard beta-cutoff
        if (score > alpha) {
            alpha = score; // alpha acts like max in MiniMax
        }
    }

    return alpha;
}
} // namespace

std::pair<std::optional<Move>, int> GetBestMove(Board &board, int depth) {
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    if (moves.empty())
        return {{}, Evaluation::EvalNoMove(board.IsKingSafe())};
    std::array<int, MAXMOVECOUNT> scores{0};

    int workingDepth = 1;
    do {
        for (int i = 0; i < moves.size(); i++) {
            Move &move = moves[i];
            board.DoMove(move);
            const int score =
                -Negamax(board, workingDepth, -MaterialValue::Inf, MaterialValue::Inf);
            board.UndoMove(move);
            scores[i] = score;
        }
        moves.sort(scores);
    } while (workingDepth++ < depth);

    return {moves[0], scores[0]};
}

std::pair<std::optional<Move>, int> GetBestMoveTime(Board &board, int timeLimit) {
    std::optional<Move> bookMove = OpeningBook::GetMove(board.GetHash());
    if (bookMove.has_value())
        return {bookMove, MaterialValue::Inf};
    MoveList moves = MoveGen::GenerateMoves<MoveGen::GenType::All>(board, board.GetColor());
    if (moves.empty())
        return {{}, Evaluation::EvalNoMove(board.IsKingSafe())};
    std::array<int, MAXMOVECOUNT> scores{0};

    U64 totalTime = 0;

    int workingDepth = 1;
    do {
        for (int i = 0; i < moves.size(); i++) {
            auto t0 = std::chrono::steady_clock::now();
            Move &move = moves[i];
            board.DoMove(move);
            const int score =
                -Negamax(board, workingDepth, -MaterialValue::Inf, MaterialValue::Inf);
            board.UndoMove(move);
            scores[i] = score;
            auto t1 = std::chrono::steady_clock::now();
            U64 time = (U64)std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
            totalTime += time;
            if (totalTime + time > timeLimit ||
                (i == moves.size() - 1 && totalTime + 20 * time > timeLimit)) {
                totalTime = 999999;
                break;
            }
        }
        moves.sort(scores);
    } while (workingDepth++ < 1000 && totalTime < timeLimit);

    return {moves[0], scores[0]};
}

int EvaluateMove(Board &board, Move move, int timeLimit) {
    board.DoMove(move);
    auto bestMove = GetBestMoveTime(board, timeLimit);
    board.UndoMove(move);
    return -bestMove.second;
}
} // namespace Chess::Engine::Negamax
