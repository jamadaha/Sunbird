<div align="center">

<h1>Sunbird</h1>

<img src="https://github.com/jamadaha/Sunbird/actions/workflows/ctest.yml/badge.svg">
<img src="https://img.shields.io/github/commits-since/jamadaha/Sunbird/latest">

[![lichess-bullet](https://lichess-shield.vercel.app/api?username=sun_bird&format=bullet)](https://lichess.org/@/sun_bird/perf/bullet)
[![lichess-blitz](https://lichess-shield.vercel.app/api?username=sun_bird&format=blitz)](https://lichess.org/@/sun_bird/perf/blitz)
[![lichess-rapid](https://lichess-shield.vercel.app/api?username=sun_bird&format=rapid)](https://lichess.org/@/sun_bord/perf/rapid)

</div>

A UCI chess engine written in CPP.

Play against it [here](https://lichess.org/@/sun_bird).

## Table of Contents
1. [Features](#Features)
2. [Benchmark](#Benchmark)
3. [How to Build](#How-To-Build)
4. [Comparison](#Comparison)

## Features
### State
* 16 bit move [[wiki](https://www.chessprogramming.org/Encoding_Moves)]
* 80 byte state (I.e. pieces on board, castling rights, EP rights, and hash)
  * Bitboards for pieces [[wiki](https://www.chessprogramming.org/Bitboard_Board-Definition)]
  * Zobrist hash [[wiki](https://www.chessprogramming.org/Zobrist_Hashing)]
* Make/unmake move through stack [[wiki](https://www.chessprogramming.org/Board_Representation)]
  * Make move clones state, then pushes modified state to stack
  * Unmake simply pops from stack
* Custom move generation
  * Moves for sliding pieces are generated in a piecewise manner from a series of expanding rings.

### Search
* Negamax [[wiki](https://www.chessprogramming.org/Negamax)]
* Alpha-Beta Pruning [[wiki](https://www.chessprogramming.org/Alpha-Beta)]
* Iterative Deepening [[wiki](https://www.chessprogramming.org/Iterative_Deepening)]
* Quiescence Search [[wiki](https://www.chessprogramming.org/Quiescence_Search)]
* PVS [[wiki](https://www.chessprogramming.org/Principal_Variation_Search)]
* Move Ordering
  * MVV-LVA [[wiki](https://www.chessprogramming.org/MVV-LVA)]
  * PV [[wiki](https://www.chessprogramming.org/PV-Move)]
  * Killer Heuristic [[wiki](https://www.chessprogramming.org/Killer_Heuristic)]
* Transposition Table [[wiki](https://www.chessprogramming.org/Transposition_Table)]
    
### Evaluation
* Material Point Value [[wiki](https://www.chessprogramming.org/Material)]
* Piece-Square Tables [[wiki](https://www.chessprogramming.org/Piece-Square_Tables)]
* Pawn Structure
    * Doubled pawns
    * Passed pawns
* Tapered by game phase [[wiki](https://www.chessprogramming.org/Tapered_Eval)]

## Benchmark
These are run on a i5-13600k with a single thread.

For the purposes here, a "node" is a move done.

### Perft
**52m nodes/s**

### Search
**10m nodes/s**
## How to Build
Navigate to root folder of project, then run the following to build engine.
```
mkdir build &&\
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release -DIGNORE_TEST=true &&\
cmake --build build
```
The executable can then be found in `chess_engine/build/engine/`.

## Comparison
| # PLAYER          | : | RATING | POINTS | PLAYED | (%) |
|-------------------|---|--------|--------|--------|-----|
| 1 Sunbird v1.2.3  | : | 240.9  | 801.5  | 1280   | 63  |
| 2 Sunbird v1.2.2  | : | 176.9  | 577.5  | 1254   | 46  |
| 3 Sunbird v1.2.1  | : | 95.0   | 127.0  | 205    | 62  |
| 4 Sunbird v1.2.0  | : | 58.0   | 117.0  | 205    | 57  |
| 5 Sunbird v1.1.2  | : | 30.8   | 109.5  | 205    | 53  |
| 6 Sunbird v1.1.1  | : | 14.6   | 105.0  | 205    | 51  |
| 7 Sunbird v1.1.0  | : | 9.2    | 103.5  | 205    | 50  |
| 8 Sunbird v1.0.6  | : | -61.3  | 83.5   | 201    | 42  |
| 9 Sunbird v1.0.5  | : | -163.1 | 57.5   | 200    | 29  |
| 10 Sunbird v1.0.4 | : | -193.6 | 50.5   | 200    | 25  |
| 11 Sunbird v1.0.0 | : | -207.2 | 47.5   | 200    | 24  |
