#include "chess/internal/pawn_gen.hpp"
#include "chess/internal/bit_shifts.hpp"

U8 PawnGen::GetQuietMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned short startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, PieceType::Pawn);

    while (pieces) {
        U64 lsb = Utilities::LSB_Pop(&pieces);

        // Single push
        if (!(board->GetOccupiedBB() & singleMove[(int) lsb]))
            if (board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | BitShifts::Shift(C64(lsb), up, 1))) {
                // If Promotion
                if (C64(lsb) & (U64) promotionRank) {
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::BPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::NPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::QPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::RPromotion, (Square) lsb, (Square) (lsb + (int) up)));
                } else
                    AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::Quiet, (Square) lsb, (Square) (lsb + (int) up)));
            }

        // Double push
        if (C64(lsb) & (U64) doubleRank && !(board->GetOccupiedBB() & doubleMove[(int) lsb]))
            if (board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | BitShifts::Shift(C64(lsb), up, 2)))
                AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::DoublePawnPush, (Square) lsb, (Square) (lsb + (int) up * 2)));
    }

    return moveCount;
}

U8 PawnGen::GetAttackMoves(std::array<Move, MAXMOVECOUNT> &moves, const std::shared_ptr<Board> &board, unsigned short startIndex) {
    U8 moveCount = 0;
    U64 pieces = board->GetPiecePos(color, PieceType::Pawn);

    while (pieces) {
        U64 lsb = Utilities::LSB_Pop(&pieces);

        // Diagonal
        U64 captures = board->GetColorBB(oppColor) & PawnAttacks[(int) color][(int) lsb];
        while (captures) {
            U64 capturePiece = Utilities::LSB_Pop(&captures);
            if (board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb)) | C64(capturePiece), board->GetColorBB(oppColor) ^ C64(capturePiece))) {
                // If Promotion
                if (C64(lsb) & (U64) promotionRank) {
                    AppendMove(moves, startIndex + moveCount, moveCount,
                               Move(MoveType::QPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                    AppendMove(moves, startIndex + moveCount, moveCount,
                               Move(MoveType::RPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                    AppendMove(moves, startIndex + moveCount, moveCount,
                               Move(MoveType::BPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                    AppendMove(moves, startIndex + moveCount, moveCount,
                               Move(MoveType::NPromotionCapture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                } else {
                    AppendMove(moves, startIndex + moveCount, moveCount,
                               Move(MoveType::Capture, (Square) lsb, (Square) capturePiece,
                                    board->GetType((Square) capturePiece)));
                }
            }
        }

        // En Passant
        captures = (U64) board->GetEP() & PawnAttacks[(int) color][(int) lsb] & (U64) enPassantRank;
        while (captures) {
            U64 toSquare = Utilities::LSB_Pop(&captures);
            U64 capturePiece = Utilities::LSB( BitShifts::Shift(C64(toSquare), down, 1));
            if (board->IsKingSafe((board->GetOccupiedBB() ^ C64(lsb) ^ C64(capturePiece)) | C64(toSquare),  board->GetColorBB(oppColor) ^ C64(capturePiece) | C64(toSquare)))
                AppendMove(moves, startIndex + moveCount, moveCount, Move(MoveType::EPCapture, (Square) lsb, (Square) toSquare, PieceType::Pawn));
        }
    }

    return moveCount;
}

void PawnGen::GeneratePawnMoves() {
    for (int i = 0; i < 64; ++i) {
        U64 bit = C64(i);
        if (bit & (U64) notPromotionRank) {
            U64 upOne = BitShifts::Shift(bit, up, 1);
            singleMove[i] |= BitShifts::Shift(bit, up, 1);
            if (bit & (U64) doubleRank) {
                doubleMove[i] |= upOne;
                doubleMove[i] |= BitShifts::Shift(bit, up, 2);
            }
        }
    }
}
