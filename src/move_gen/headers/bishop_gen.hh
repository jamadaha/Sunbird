#ifndef BISHOP_GEN_HH
#define BISHOP_GEN_HH

#include "piece_gen.hh"

class BishopGen : public PieceGen {
public:
    explicit BishopGen(Color color) : PieceGen(color) {};
    inline U8 GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
    inline U8 GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) final;
};

U8 BishopGen::GetQuietMoves (std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    return GetSlidingMoves(moves, board, PieceType::Bishop, isKingSafe, startIndex, attackedSquares);
}

U8 BishopGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> *moves, Board *board, U64 attackedSquares, bool isKingSafe, U8 startIndex) {
    return GetSlidingAttacks(moves, board, PieceType::Bishop, isKingSafe, startIndex, attackedSquares);
}

#endif //BISHOP_GEN_HH
