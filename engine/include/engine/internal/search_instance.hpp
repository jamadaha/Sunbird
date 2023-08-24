#ifndef CHESS_ENGINE_SEARCH_INSTANCE
#define CHESS_ENGINE_SEARCH_INSTANCE

#include <chess/internal/move_list.hpp>
#include <chess/board.hpp>
#include <chrono>
#include <csetjmp>
#include <engine/internal/pv.hpp>
#include <optional>
#include <unordered_map>

namespace Chess::Engine::Search {
class Instance {
public:
    struct Result {
        PV _pv;
        Move _move;
        int _score;
    };
    Instance(int depth);
    Instance(int depth, int time, std::optional<Result> prior);
    Result Begin(Board &board, std::jmp_buf *jmpBuf = nullptr);

private:
    const int _rootDepth;
    const std::chrono::steady_clock::time_point _time;
    const PV _priorPV;

    bool IsTime();

    /*
     * From a given position, searches all non-quiet moves
     */
    int Quiesce(Board &board, int alpha, int beta);
    /*
     * Finds optimal move for a given position, or until the limit is reached
     */
    int Negamax(Board &board, int alpha, int beta, int depth, PV &pv,
                std::jmp_buf *jmpBuf);
};
} // namespace Chess::Engine::Search

#endif
