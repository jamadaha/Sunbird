#ifndef CHESS_MOVE_GEN
#define CHESS_MOVE_GEN

#include "move_list.hpp"
#include "position.hpp"
#include "types.hpp"

namespace MoveGen {
MoveList GenerateAll(const Position &pos, Color color);
MoveList GenerateAttack(const Position &pos, Color color);
} // namespace MoveGen

#endif
