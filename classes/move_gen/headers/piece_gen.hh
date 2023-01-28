#ifndef PIECE_HH
#define PIECE_HH

#include "../../headers/constants.hh"
#include "../../headers/move.hh"
#include "../../headers/board.hh"
#include "../../headers/utilities.hh"


class PieceGen {
public:
    explicit PieceGen(Color color) : color(color), oppColor(Utilities::GetOppositeColor(color)) {};
    U8 GetALlMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex);
    virtual U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) = 0;
    virtual U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) = 0;
protected:
    Color color;
    Color oppColor;
    U8 GetSlidingMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 pieces, Direction direction, bool isKingSafe, U8 startIndex);
    U8 GetSlidingAttacks(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 pieces, Direction direction, bool isKingSafe, U8 startIndex);

    static inline void AppendMove(std::array<Move, MAXMOVECOUNT> *moves, int index, U8* moveCount, Move move);
};

inline void PieceGen::AppendMove(std::array<Move, MAXMOVECOUNT> *moves, int index, U8* moveCount, Move move) {
    moves->at(index) = move;
    ++(*moveCount);
}

#endif //PIECE_HH
