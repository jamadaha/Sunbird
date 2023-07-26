#include <chess/import.hpp>
#include <chess/internal/utilities.hpp>

namespace Chess::Import {
// HACK: Refactor this
Board MoveSequence(std::string moves) {
    Board board;
    board.Initialize();
    std::string move;
    for (int i = 0; i < moves.length(); i++) {
        if (moves[i] != ' ')
            move.push_back(moves[i]);

        if (moves[i] == ' ' || i == (int)moves.length() - 1) {
            Square fromSquare = Utilities::GetSquare(move[0], move[1]);
            Square toSquare = Utilities::GetSquare(move[2], move[3]);
            PieceType fromType = board.GetType(fromSquare);
            PieceType toType = board.GetType(toSquare);
            Color fromColor = board.GetColor(fromSquare);
            MoveType type;
            // Is castling?
            //// Is kingside castle
            if (fromType == PieceType::King && (move == "e1g1" || move == "e8g8"))
                type = MoveType::KingCastle;
            //// Is queenside castle
            else if (fromType == PieceType::King && (move == "e1c1" || move == "e8c8"))
                type = MoveType::QueenCastle;

            else if (fromType == PieceType::Pawn) {
                // If double pawn push
                if (std::abs((int)Utilities::GetRowIndex(fromSquare) -
                             (int)Utilities::GetRowIndex(toSquare)) == 2)
                    type = MoveType::DoublePawnPush;
                // If promotion
                else if (Utilities::GetRow(toSquare) ==
                         ((fromColor == Color::White) ? Row::Row8 : Row::Row1)) {
                    // If none capture promotion
                    if (toType == PieceType::None) {
                        if (moves.length() == 5) {
                            if (move[4] == 'r')
                                type = MoveType::RPromotion;
                            else if (move[4] == 'n')
                                type = MoveType::NPromotion;
                            else if (move[4] == 'b')
                                type = MoveType::BPromotion;
                        } else
                            type = MoveType::QPromotion;
                    } else {
                        // Capture promotion
                        if (moves.length() == 5) {
                            if (move[4] == 'r')
                                type = MoveType::RPromotionCapture;
                            else if (move[4] == 'n')
                                type = MoveType::NPromotionCapture;
                            else if (move[4] == 'b')
                                type = MoveType::BPromotionCapture;
                        } else
                            type = MoveType::QPromotionCapture;
                    }
                } else if (toType != PieceType::None)
                    type = MoveType::Capture;
                else {
                    if (Utilities::GetColumn(fromSquare) != Utilities::GetColumn(toSquare))
                        type = MoveType::EPCapture;
                    else
                        type = MoveType::Quiet;
                }
            } else if (toType != PieceType::None)
                type = MoveType::Capture;
            else
                type = MoveType::Quiet;
            Move tempMove;
            if (type == MoveType::Capture || type == MoveType::RPromotionCapture ||
                type == MoveType::NPromotionCapture || type == MoveType::BPromotionCapture ||
                type == MoveType::QPromotionCapture)
                tempMove = Move(type, fromSquare, toSquare, board.GetType(toSquare));
            else
                tempMove = Move(type, fromSquare, toSquare);
            board.DoMove(tempMove);
            move = "";
        }
    }
    return board;
}
} // namespace Import