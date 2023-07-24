#include <chess/board.hpp>
#include <chess/internal/bit_shift.hpp>

using namespace Chess;

void Board::Initialize() {
    for (U8 x = 0; x < WIDTH; ++x) {
        PlacePiece(Utilities::GetSquare(x, PAWNROWWHITE), PieceType::Pawn, Color::White);
        PlacePiece(Utilities::GetSquare(x, PAWNROWBLACK), PieceType::Pawn, Color::Black);
    }

    PlacePiece(Square::A1, PieceType::Rook, Color::White);
    PlacePiece(Square::H1, PieceType::Rook, Color::White);
    PlacePiece(Square::A8, PieceType::Rook, Color::Black);
    PlacePiece(Square::H8, PieceType::Rook, Color::Black);

    PlacePiece(Square::B1, PieceType::Knight, Color::White);
    PlacePiece(Square::G1, PieceType::Knight, Color::White);
    PlacePiece(Square::B8, PieceType::Knight, Color::Black);
    PlacePiece(Square::G8, PieceType::Knight, Color::Black);

    PlacePiece(Square::C1, PieceType::Bishop, Color::White);
    PlacePiece(Square::F1, PieceType::Bishop, Color::White);
    PlacePiece(Square::C8, PieceType::Bishop, Color::Black);
    PlacePiece(Square::F8, PieceType::Bishop, Color::Black);

    PlacePiece(Square::D1, PieceType::Queen, Color::White);
    PlacePiece(Square::D8, PieceType::Queen, Color::Black);

    PlacePiece(Square::E1, PieceType::King, Color::White);
    PlacePiece(Square::E8, PieceType::King, Color::Black);

    for (U8 i = 0; i < COLORCOUNT; ++i)
        for (U8 i2 = 0; i2 < 2; ++i2)
            castlingAllowed[i][i2] = true;

    turn = Color::White;
    oppColor = Color::Black;
}

void Board::DoMove(Move &move) {
    PieceType fromType;
    if (move.GetType() == MoveType::KingCastle) [[unlikely]] {
        if (turn == Color::White) {
            PlacePiece(Square::G1, PieceType::King, Color::White);
            PlacePiece(Square::F1, PieceType::Rook, Color::White);
            RemovePiece(Square::E1, PieceType::King, Color::White);
            RemovePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            PlacePiece(Square::G8, PieceType::King, Color::Black);
            PlacePiece(Square::F8, PieceType::Rook, Color::Black);
            RemovePiece(Square::E8, PieceType::King, Color::Black);
            RemovePiece(Square::H8, PieceType::Rook, Color::Black);
        }
        fromType = PieceType::King;
    } else if (move.GetType() == MoveType::QueenCastle) [[unlikely]] {
        if (turn == Color::White) {
            PlacePiece(Square::C1, PieceType::King, Color::White);
            PlacePiece(Square::D1, PieceType::Rook, Color::White);
            RemovePiece(Square::E1, PieceType::King, Color::White);
            RemovePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            PlacePiece(Square::C8, PieceType::King, Color::Black);
            PlacePiece(Square::D8, PieceType::Rook, Color::Black);
            RemovePiece(Square::E8, PieceType::King, Color::Black);
            RemovePiece(Square::A8, PieceType::Rook, Color::Black);
        }
        fromType = PieceType::King;
    } else [[likely]] {
        fromType = GetType(move.GetFrom());
        RemovePiece(move.GetFrom(), fromType, turn);

        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) [[unlikely]] {
                auto captureSquare = (Square)((turn == Color::White) ? (int)move.GetTo() - 8
                                                                     : (int)move.GetTo() + 8);
                RemovePiece(captureSquare, PieceType::Pawn, oppColor);
            } else [[likely]]
                RemovePiece(move.GetTo(), move.GetCapturedPiece(), oppColor);

            if (move.GetCapturedPiece() == PieceType::Rook &&
                move.GetTo() == initRookPos[(U8)oppColor][(U8)Castling::King])
                if (castlingAllowed[(U8)oppColor][(U8)Castling::King])
                    DisableCastling(move, oppColor, Castling::King);
            if (move.GetCapturedPiece() == PieceType::Rook &&
                move.GetTo() == initRookPos[(U8)oppColor][(U8)Castling::Queen])
                if (castlingAllowed[(U8)oppColor][(U8)Castling::Queen])
                    DisableCastling(move, oppColor, Castling::Queen);
        }
        if (move.IsPromotion()) [[unlikely]] {
            if (move.GetType() == MoveType::RPromotion ||
                move.GetType() == MoveType::RPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Rook, turn);
            else if (move.GetType() == MoveType::BPromotion ||
                     move.GetType() == MoveType::BPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Bishop, turn);
            else if (move.GetType() == MoveType::NPromotion ||
                     move.GetType() == MoveType::NPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Knight, turn);
            else if (move.GetType() == MoveType::QPromotion ||
                     move.GetType() == MoveType::QPromotionCapture)
                PlacePiece(move.GetTo(), PieceType::Queen, turn);
        } else
            PlacePiece(move.GetTo(), fromType, turn);
    }

    // En passant
    //// Check if there is old
    if (EP != Column::None) [[unlikely]]
        move.SetDisableEnPassant(EP);
    //// Set new
    if (move.GetType() == MoveType::DoublePawnPush) [[unlikely]] {
        SetEnPassant(Utilities::GetColumn(move.GetFrom()));
    } else
        SetEnPassant(Column::None);

    // Castling rights
    if (fromType == PieceType::King) [[unlikely]] {
        if (castlingAllowed[(U8)turn][(U8)Castling::King])
            DisableCastling(move, turn, Castling::King);
        if (castlingAllowed[(U8)turn][(U8)Castling::Queen])
            DisableCastling(move, turn, Castling::Queen);
    } else if (fromType == PieceType::Rook) [[unlikely]] {
        if (castlingAllowed[(U8)turn][(U8)Castling::King] &&
            move.GetFrom() == (turn == Color::White ? Square::H1 : Square::H8))
            DisableCastling(move, turn, Castling::King);
        else if (castlingAllowed[(U8)turn][(U8)Castling::Queen] &&
                 move.GetFrom() == (turn == Color::White ? Square::A1 : Square::A8))
            DisableCastling(move, turn, Castling::Queen);
    }

    SwitchTurn();
    zobrist.IncHash();
    ++ply;
}

void Board::UndoMove(Move move) {
    zobrist.DecHash();
    PieceType toType;

    // Check if old en passant
    bool priorEP = false;
    if (move.IsDEP()) {
        SetEnPassant(move.GetDEP());
        priorEP = true;
    }

    if (move.GetType() == MoveType::KingCastle) [[unlikely]] {
        if (turn == Color::Black) {
            RemovePiece(Square::G1, PieceType::King, Color::White);
            RemovePiece(Square::F1, PieceType::Rook, Color::White);
            PlacePiece(Square::E1, PieceType::King, Color::White);
            PlacePiece(Square::H1, PieceType::Rook, Color::White);
        } else {
            RemovePiece(Square::G8, PieceType::King, Color::Black);
            RemovePiece(Square::F8, PieceType::Rook, Color::Black);
            PlacePiece(Square::E8, PieceType::King, Color::Black);
            PlacePiece(Square::H8, PieceType::Rook, Color::Black);
        }
    } else if (move.GetType() == MoveType::QueenCastle) [[unlikely]] {
        if (turn == Color::Black) {
            RemovePiece(Square::C1, PieceType::King, Color::White);
            RemovePiece(Square::D1, PieceType::Rook, Color::White);
            PlacePiece(Square::E1, PieceType::King, Color::White);
            PlacePiece(Square::A1, PieceType::Rook, Color::White);
        } else {
            RemovePiece(Square::C8, PieceType::King, Color::Black);
            RemovePiece(Square::D8, PieceType::Rook, Color::Black);
            PlacePiece(Square::E8, PieceType::King, Color::Black);
            PlacePiece(Square::A8, PieceType::Rook, Color::Black);
        }
    } else {
        toType = GetType(move.GetTo());
        if (move.IsPromotion()) [[unlikely]] {
            RemovePiece(move.GetTo(), toType, oppColor);
            PlacePiece(move.GetFrom(), PieceType::Pawn, oppColor);
        } else [[likely]] {
            RemovePiece(move.GetTo(), toType, oppColor);
            PlacePiece(move.GetFrom(), toType, oppColor);
            if (move.GetType() == MoveType::DoublePawnPush) [[unlikely]]
                if (!priorEP)
                    SetEnPassant(Column::None);
        }

        if (move.IsCapture()) {
            if (move.GetType() == MoveType::EPCapture) [[unlikely]] {
                auto captureSquare = (Square)((turn == Color::Black) ? (int)move.GetTo() - 8
                                                                     : (int)move.GetTo() + 8);
                PlacePiece(captureSquare, PieceType::Pawn, turn);
            } else [[likely]]
                PlacePiece(move.GetTo(), move.GetCapturedPiece(), turn);
        }
    }
    EnableCastling(move);
    SwitchTurn();
    --ply;
}

void Board::EnableCastling(Move &move) {
    auto enableCastling = [&](Color color, Castling castling) {
        castlingAllowed[(int)color][(int)castling] = true;
        zobrist.FlipCastling(color, castling);
    };
    if (move.IsDC()) [[unlikely]] {
        if (move.IsDC(Color::White, Castling::King))
            enableCastling(Color::White, Castling::King);
        if (move.IsDC(Color::White, Castling::Queen))
            enableCastling(Color::White, Castling::Queen);
        if (move.IsDC(Color::Black, Castling::King))
            enableCastling(Color::Black, Castling::King);
        if (move.IsDC(Color::Black, Castling::Queen))
            enableCastling(Color::Black, Castling::Queen);
    }
}

void Board::DisableCastling(Move &move, Color color, Castling side) {
    zobrist.FlipCastling(color, side);
    castlingAllowed[(U8)color][(U8)side] = false;
    move.SetDisableCastle(color, side);
}

U64 Board::GenerateAttackSquares(Color color) const {
    U64 attacks = 0;

    U64 tempPieces[PIECECOUNT];
    for (U8 pIndex = 0; pIndex < PIECECOUNT; pIndex++)
        tempPieces[pIndex] = GetPiecePos(color, (PieceType)pIndex);

    while (tempPieces[(U8)PieceType::Pawn])
        attacks |= PawnAttacks[(int)color][Utilities::LSB_Pop(&tempPieces[(U8)PieceType::Pawn])];

    for (U8 pIndex = 1; pIndex < PIECECOUNT; ++pIndex)
        while (tempPieces[pIndex]) {
            unsigned short pos = Utilities::LSB_Pop(&tempPieces[pIndex]);
            U64 attacks1 = BitShift::MOVES[pIndex][pos];

            for (U64 b = occupiedBB & BitShift::BB[pIndex][pos]; b != 0; b &= (b - 1))
                attacks1 &= ~BitShift::XRAYS[pos][Utilities::LSB(b)];

            attacks |= attacks1;
        }

    return attacks;
}

bool Board::IsKingSafe(U64 tempOccuracyBoard, U64 tempEnemyBoard, U64 tempKingBoard) const {
    if (tempKingBoard == 0)
        return true;
    U64 kingPosIndex = Utilities::LSB_Pop(&tempKingBoard);

    U64 enemyRooks =
        (GetPiecePos(PieceType::Rook) | GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyBishops =
        (GetPiecePos(PieceType::Bishop) | GetPiecePos(PieceType::Queen)) & tempEnemyBoard;
    U64 enemyKnights = GetPiecePos(PieceType::Knight) & tempEnemyBoard;
    U64 enemyPawns = GetPiecePos(PieceType::Pawn) & tempEnemyBoard;

    // clang-format off
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::North] & enemyRooks) [[unlikely]]
        if (Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::North] & enemyRooks) == Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::North] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::East] & enemyRooks) [[unlikely]]
        if (Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::East] & enemyRooks) == Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::East] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::South] & enemyRooks) [[unlikely]]
        if (Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::South] & enemyRooks) == Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::South] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::West] & enemyRooks) [[unlikely]]
        if (Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::West] & enemyRooks) == Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::West] & tempOccuracyBoard))
            return false;

    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthEast] & enemyBishops) [[unlikely]]
        if (Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthEast] & enemyBishops) == Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthEast] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthWest] & enemyBishops) [[unlikely]]
        if (Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthWest] & enemyBishops) == Utilities::LSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::NorthWest] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthEast] & enemyBishops) [[unlikely]]
        if (Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthEast] & enemyBishops) == Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthEast] & tempOccuracyBoard))
            return false;
    if (BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthWest] & enemyBishops) [[unlikely]]
        if (Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthWest] & enemyBishops) == Utilities::MSB(BitShift::RAYS[kingPosIndex][(int)DirectionIndex::SouthWest] & tempOccuracyBoard))
            return false;
    // clang-format on

    if (BitShift::MOVES[(int)PieceType::Knight][kingPosIndex] & enemyKnights) [[unlikely]]
        return false;

    if (PawnAttacks[(int)turn][kingPosIndex] & enemyPawns) [[unlikely]]
        return false;

    return true;
}

void Board::SetEnPassant(Column col) {
    if (EP != Column::None)
        zobrist.FlipEnPassant(EP);
    if (col != Column::None)
        zobrist.FlipEnPassant(col);
    EP = col;
}
