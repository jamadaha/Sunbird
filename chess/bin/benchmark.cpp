#include <chrono>
#include <iostream>

#include <chess/board.hpp>
#include <chess/import.hpp>
#include <chess/perft.hpp>
#include <map>

std::vector<std::string> openings{
    "rnbqkbnr/ppp1pppp/8/3p4/2PP4/8/PP2PPPP/RNBQKBNR b KQkq - 0 2",
    "rnbqk2r/pppp1ppp/4pn2/8/1bPP4/2N5/PP2PPPP/R1BQKBNR w KQkq - 2 4",
    "rnbqkb1r/p2ppppp/5n2/1ppP4/2P5/8/PP2PPPP/RNBQKBNR w KQkq - 0 4",
    "r1bqkbnr/pppp1ppp/2n5/1B2p3/4P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3",
    "r1bqkbnr/pppp1ppp/2n5/4p3/2B1P3/5N2/PPPP1PPP/RNBQK2R b KQkq - 3 3"};

std::vector<std::string> testPositions{
    "4r1k1/pp1N1ppp/4qb2/2p5/2P1QP2/5P2/PP1R2PP/2K5 b - - 2 23",
    "3qk2r/3nbppp/p1r1p3/8/Q4B2/2N5/PP3PPP/R2R2K1 b k - 1 18",
    "8/5ppk/4pb2/1Q6/P5qP/4B3/5P1P/7K b - - 6 39",
    "r2r1nk1/np4pp/1q2p1b1/pP1pP3/Pb1P1PP1/4BN2/4B1K1/RN1Q1R2 b - - 2 21",
    "r4rk1/2qn1p1p/3p2p1/1ppPbP2/4P3/1PN4Q/3B2PP/4RRK1 w - - 0 20",
    "8/6k1/4Q1p1/2p4p/3q4/7P/6P1/7K w - - 0 35",
    "r1b2qk1/pp1n1rb1/8/3Q4/8/1N2B3/PP3PPP/R3R1K1 b - - 2 20",
    "r3kb1r/pp2q1pp/2p1p3/8/3Q4/8/PP3PPP/R1B1R1K1 b kq - 1 15",
    "1k6/1pr3p1/7p/p3q3/P6P/6P1/2R2P2/2Q3K1 b - - 4 39",
    "1k5r/1p4pp/2p5/p7/P7/4Q1P1/1q3P1P/3R2K1 b - - 0 30",
    "r4qk1/pp3rb1/5n2/5b2/3N3Q/4B3/PP3PPP/R3R1K1 b - - 10 24",
    "6k1/pp4b1/5nb1/6N1/5Q2/2B4P/PP3PPK/3q4 b - - 4 34",
    "r4rk1/pp4b1/2q2nb1/6N1/3B3Q/8/PP3PPP/3RR1K1 b - - 18 28",
    "6k1/pp4b1/5nb1/6N1/7Q/2B4P/PP2qPP1/6K1 w - - 1 33",
    "3q2k1/pp4b1/4Nnb1/8/5Q2/2B4P/PP3PPK/8 b - - 6 35",
    "5rk1/pp4b1/2q2nb1/6N1/7Q/2B5/PP3PPP/3Rr1K1 w - - 0 30",
    "r3kb1r/1b1nppp1/p4n1p/1p1pN3/3q1P1B/2NB4/PPP3PP/R2Q1R1K b kq - 1 12",
    "1k6/1p3rp1/1Qp4p/p4q2/P6P/6P1/1R3P2/6K1 w - - 0 37",
    "2Q5/6p1/1k5p/pP2q3/7P/6P1/5P1K/8 b - - 0 48",
    "2kr1b1r/pp4pp/2p1p3/3q4/Q7/4B3/PP3PPP/R3R1K1 b - - 7 18",
    "1k3r2/1p1q2pp/1Qp5/p7/P6P/6P1/5P2/1R4K1 w - - 5 34",
    "3Q4/6p1/k6p/pp2q3/P6P/6P1/5P1K/8 w - - 4 45",
    "4r1k1/pp3p1p/5p2/2p5/2P1PP2/8/PP1R2PP/2K5 b - - 0 25",
    "2r3k1/2qbppb1/1n1p2p1/1p6/4PPPp/1P2N2P/2P3BK/3QBR2 b - - 6 24",
    "r1bq1rk1/pp3ppp/4pn2/3p4/1bnN4/2N3P1/PPQ1PPBP/R1B2RK1 w - - 0 11",
    "8/3R2pk/1p6/p3pP2/6Pp/3K4/P1P4r/8 w - - 0 58",
    "3rr2k/1p3qbp/p2p1np1/P3p3/1PPn2Q1/2N1B1PP/3R1PBK/3R4 w - - 7 29",
    "1brr2k1/pp3pp1/4bn1p/3p3q/5P2/1PNBP2P/PB1Q2P1/2RR2K1 w - - 3 21",
    "7k/pp1R1p2/7p/5B2/P7/1r4bP/6P1/6K1 w - - 2 36",
    "3r1qk1/1b6/5p1p/1R3Np1/3p4/PPr2P1P/3Q2P1/2R3K1 b - - 0 32",
    "r1b1r1k1/pp3ppp/1q6/2nP1p2/2B2P2/P1Q2NP1/1PP4P/1R2K2R w K - 1 17",
    "3k4/5p1p/p1p3p1/Pr1B1p2/1NK2P1P/6P1/8/8 w - - 0 46",
    "r3k2r/1bq2ppp/p3p3/1pb1P3/P7/2PB4/3BQPPP/R3R1K1 b kq - 0 16",
    "r3k2r/5p1p/2q1p1p1/p1b1P3/6R1/2P5/3BQPPP/4R1K1 w kq - 0 21",
    "r3rbk1/pp1b1q2/2pn2p1/3pBpQ1/3P4/2P2BN1/PP2RPP1/4R1K1 b - - 1 25",
    "8/pp2k3/2p5/8/6p1/P1P3K1/1P6/8 w - - 2 62",
    "3r2k1/1b4pp/p7/1pb1q3/3Bp3/2P1N1P1/PP3Q1P/5RK1 b - - 1 24",
    "1r4k1/2b1qppp/2P2nn1/3pp3/Q7/B3PB1P/3N1PP1/2R3K1 b - - 2 22",
    "5rk1/1pbbqppp/2P2nn1/1P1pp3/Q7/4PN2/1B1NBPPP/2R3K1 b - - 0 18",
    "6k1/2b2ppp/2P2nn1/3pq3/Q3p3/B3P1PP/4BP2/1N4K1 b - - 0 26",
    "r2r1nk1/np4pp/1q2p1b1/pP1pP3/Pb1P1PP1/4BN2/4B1K1/RN1Q1R2 b - - 2 21",
    "5nk1/np4pp/1q2p1b1/pP1pP1N1/P2P1PP1/2r1B3/2r1BK2/2RQ1R2 b - - 10 28",
    "r1b1kb1r/ppp1qppp/8/3p4/3nN3/3B4/PPP2PPP/R1BQ1RK1 w kq - 0 9",
    "r2qkb1r/1b1npppp/p4n2/1pppN1B1/3P1P2/2NBP3/PPP3PP/R2QK2R b KQkq - 3 8",
    "2kr1b1r/pp4pp/2p1p3/3q4/Q7/4B3/PP3PPP/R3R1K1 b - - 7 18",
    "1k5r/1p4pp/2p5/p7/P7/4Q3/1q3PPP/3R2K1 w - - 0 30",
    "8/3Q2p1/1k5p/pp2q3/P6P/6P1/5P1K/8 w - - 2 44",
    "r1b2rk1/np4pp/1q2p3/pP1p4/Pb1P4/3nBN2/5KPP/RN1Q1R2 w - - 0 18",
    "r1b4r/ppk1b1pp/2p1np2/P3p3/2B1P3/1NP1B1P1/1P1R1P1P/2K4R b - - 2 16",
    "r7/p1kbb3/2p1n2p/Pp2P1p1/6B1/1NP1B2P/1P1R4/2K5 b - - 0 25",
    "8/p1k1r3/2p1n2p/PpN3p1/1P6/2P4P/1K1R4/8 w - - 4 34",
    "8/8/N1k4p/Pp4p1/1P1K4/7n/8/8 w - - 0 46",
    "8/k7/P7/1p4p1/1N5p/7K/8/8 b - - 6 59",
    "8/5pk1/4pbp1/p4r1p/Pp5P/qP2R1P1/3RQPK1/8 b - - 9 36",
    "1rb2rk1/1pnnqpbp/p2p2p1/P1pP4/2N2B2/2N1P2P/1P2BPP1/R2Q1RK1 b - - 4 15",
    "r1b2qk1/3pbp1p/pn4p1/2p1P3/2p3Q1/1B6/PP3PPP/RN2R1K1 w - - 0 16",
    "1rbqnrk1/1p1n1ppp/p2p4/2pPp1b1/P1P1P3/2NBB2P/1P1QNPP1/R4RK1 b - - 2 12",
    "1r3rk1/4q1pp/3p1p2/1NpPn3/4P3/6RP/1P1QN1P1/5RK1 b - - 0 23",
    "rnb1k2r/pp3ppp/2p2n2/4q3/1b1BP3/2N2P2/PP1Q2PP/R3KBNR b KQkq - 5 11",
    "3rk2r/pp1n1ppp/4b1q1/8/Pbp1PQ2/2N2P2/1PB1N1PP/R3K2R w KQk - 2 19",
    "5rk1/pp1r2pp/8/2bN4/P3p3/3R1PP1/1P4P1/R6K w - - 0 30",
    "q4rk1/3bppb1/3p1np1/1p1Nn3/4PNPp/5P1P/1PP3BK/2BQ1R2 b - - 1 18",
    "2r3k1/5p2/3P2p1/1p2PqPn/3B2Q1/1P5P/2P1R1K1/8 b - - 2 37",
    "3rr2k/1p3qbp/p2p1np1/P3p3/1PPn2Q1/2N1B1PP/3R1PBK/3R4 w - - 7 29",
    "2rq1rk1/1p2ppbp/p1npbnp1/3N4/P3P3/6PP/1PP1NPB1/R1BQ1RK1 b - - 2 12",
    "3rr2k/1p3qbp/p2p1np1/P3p3/1PPn2Q1/2N1B1PP/3R1PBK/3R4 w - - 7 29",
    "2rq1rk1/1b1nppb1/p2p1npp/1p4B1/3PP3/P1NB1N1P/1P3PP1/R2QR1K1 w - - 0 14",
    "3r1qk1/1b3p2/5b1p/1p3Np1/2rp4/P4P1P/1P1Q2PB/2R1R1K1 w - - 0 29",
    "r1b2rk1/pp3ppp/1q6/2nP1p2/2B2P2/P1Q2NP1/1PP4P/1R2K2R b K - 0 16",
    "2r1r3/1p1b1p1p/p4kp1/P2P1p2/2BN1P1P/1P4P1/2P2K2/3R4 w - - 1 27",
    "3k4/1p3p1p/p1P3p1/Pr1B1p2/1NK2P1P/6P1/8/8 b - - 0 45",
    "3N4/6kp/3K1pp1/3B1p2/5PrP/8/8/8 b - - 3 58",
    "8/8/5k1p/8/6BK/8/8/8 w - - 8 70",
    "r3rb2/pp1b3k/2p3p1/3pB3/3Pp3/2P2PN1/PP2R1P1/4R1K1 b - - 0 29",
    "4k3/pp6/2p5/8/6p1/P1P3K1/1P6/8 b - - 1 61",
    "r3r1k1/pppq2pp/2nbb3/3pp3/8/3P1NNP/PPP2PP1/R1BQR1K1 w - - 3 12",
    "3r1rk1/1bq3pp/p3pn2/1pbp4/3B1P2/2P1NBP1/PP1Q3P/R4RK1 b - - 0 19",
    "5rk1/2b1qppp/2P2nn1/3pp3/Q5b1/4PN1P/1B1NBPP1/2R3K1 b - - 0 20",
    "6k1/2b2ppp/2P1qnn1/3p4/Q3p3/B3P2P/4BPP1/1N4K1 b - - 0 25",
    "2rqnrk1/1p1nb1p1/p2p3p/3Ppp2/8/4N1PB/PPP2P1P/R1BQR1K1 b - - 1 16",
    "4rr2/1pq4k/p3Bbpp/3PR3/2P2p1n/1PQ5/PB3P2/4R1K1 w - - 4 32",
    "4q3/1p2P1k1/p3Q2p/3K2p1/1PP5/5p2/P7/8 w - - 1 53",
    "r1bq1rk1/1p3pp1/p4nnp/3pp3/8/1BPPNN2/PP2QPPP/R2R2K1 b - - 1 16",
    "2b1rnk1/rp4pp/p1pR2n1/4P3/1PP2P2/4N1P1/P1B3KP/R7 w - - 3 27",
    "3Rbrk1/1p2r1p1/p1pR4/2P3K1/1P4N1/6P1/7P/8 b - - 13 39",
    "r2q1rk1/pp1bppbp/2n2np1/8/3N4/2P1BN1P/PP2BPP1/R2Q1RK1 b - - 4 12",
    "3qn1k1/p4pbp/1p2p1p1/4N3/2P5/P1B4P/1P2QPP1/6K1 w - - 0 27",
    "r5k1/1p4pp/pqp2rn1/3p1P2/8/2P3Q1/PPN3PP/2R2R1K b - - 0 21",
    "r1bq1rk1/3n1ppp/p1pbpn2/1p6/3P4/2N1PN2/PPQ1BPPP/R1B2RK1 w - - 0 11",
    "r2qr1k1/1pp1bppp/p1p1b3/4P3/5B2/2N5/PPP2PPP/R2QR1K1 w - - 0 14",
    "3r1q1k/1pp2pp1/p1p1b2p/4P3/5B1P/P1Q5/1PP2PP1/3R2K1 w - - 1 23",
    "8/1pR4Q/pk2b3/2p1P3/2P4P/PP4P1/3q1PK1/3r4 b - - 0 43",
    "2kR4/1p2bppp/p1n2n2/4p1B1/8/1NN3P1/PPP2PKP/8 b - - 0 17",
    "8/7p/p2k2p1/1p3p2/3Np3/1PP3P1/P2K1P1P/2n5 b - - 2 34",
    "rnbqkb1r/pp2pppp/3p1n2/8/3NP3/8/PPP2PPP/RNBQKB1R w KQkq - 1 5",
    "r1bqk2r/ppppbppp/2n2n2/4p3/2B1P3/3P1N2/PPP2PPP/RNBQK2R w KQkq - 1 5",
    "r2q4/ppp2ppk/3p1b1p/3P4/P4BQ1/2P5/1P3PPP/R3r1K1 w - - 0 18",
    "r1bqkb1r/pppp1ppp/2n2n2/8/3QP3/2N5/PPP2PPP/R1B1KBNR w KQkq - 1 5",
    "1r3rk1/1b1p2pp/p2N1p2/1pR1n3/8/8/PPP3PP/2K2BR1 w - - 2 19",
    "3k4/r2p3N/2b3p1/1p3p2/8/P5PB/1PP4n/2KR4 w - - 0 32",
    "r1bbk2r/1p3ppp/p1n1pn2/8/2P5/2N1BN2/PP2BPPP/R4RK1 b kq - 1 12",
    "1r1rb1k1/2b3pp/4p3/2P2p2/4B3/4B1P1/5PKP/3R1R2 w - - 0 27",
    "1r5k/2p2pp1/p2q1n1p/3P1Q1P/1P2r3/PB4R1/2P2PP1/2KR4 b - - 1 25",
    "r6k/2p3p1/5p1p/P2P3P/1Q2P3/1B6/2PK2P1/6q1 w - - 3 38",
    "1r1q3k/6p1/2KP1p1p/P6P/1Q6/1B6/2P3P1/8 w - - 3 47",
    "r3kb1r/1p3ppp/p1n1pn2/2pP3q/2P3b1/4BN2/PP2BPPP/RN1Q1RK1 b kq - 0 10",
    "3rk2r/5p1p/p1p2p1q/2p5/8/4P1PP/PP6/R3QRK1 w k - 0 21",
    "8/5pk1/R6p/7P/p1r3K1/8/8/8 w - - 10 52",
    "r2qkb1r/1p2nppp/p1n1p3/3pP3/3P4/5B2/PP3PPP/RNBQ1RK1 w kq - 1 10",
    "rnbqkb1r/pppp1ppp/5n2/8/3pP3/2N5/PPP2PPP/R1BQKBNR w KQkq - 0 4",
    "r5k1/2pq1ppp/p2p2n1/2b3P1/4bB2/1P3N2/1PPQ3P/1K1R1R2 b - - 0 22",
    "r5k1/2p2ppp/1b1p2b1/6P1/1PQ5/1PB4q/pKP4P/2R2R2 b - - 1 31",
    "r2qk1nr/pppb1pbp/2np2p1/3Pp3/4P3/2N1B3/PPPQNPPP/R3KB1R b KQkq - 0 7",
    "r3k2r/2pqn3/3p3p/p2Pp2n/1p2PpN1/5P2/PPPQN2P/2KR2R1 b kq - 1 20",
    "rnbqkb1r/ppp2ppp/8/3pP3/4n3/2N5/PPPP2PP/R1BQKBNR w KQkq - 0 5",
    "r3k2r/p1p1q2p/1p2b1p1/4Pp2/3n3P/2NP2Q1/P1PKB1P1/1R5R w kq - 0 17",
    "r2qkbnr/p1pb1ppp/2pp4/8/3QP3/2N2N2/PPP2PPP/R1B1K2R b KQkq - 1 7",
    "rnbqk2r/pppp1ppp/4pn2/8/1bPP4/5N2/PP2PPPP/RNBQKB1R w KQkq - 2 4",
    "rn1qkb1r/ppp2ppp/4bn2/8/8/8/PPPPQPPP/RNB1KBNR w KQkq - 0 5",
    "2r1r1k1/ppq2ppp/8/4Nn1b/3P4/1R4PP/P1PQ1RBK/8 b - - 2 23"};

std::vector<std::string> endgamePositions{
    "6k1/5p2/6p1/8/7p/8/6PP/6K1 b - - 0 0",
    "3k4/2n2B2/1KP5/2B2p2/5b1p/7P/8/8 b - - 0 0",
    "r7/4R2P/3p4/3k1K2/2p5/8/8/8 b - - 0 0",
    "8/8/5p2/1P1K1k2/8/2r5/8/7R w - - 0 0",
    "5n2/R7/4pk2/8/5PK1/8/8/8 b - - 0 0",
    "3Q4/8/1k6/7p/p1p4P/2q3PB/7K/8 b - - 0 0",
    "4q3/2R4P/5R2/1p6/p3k3/P7/KP6/8 b - - 0 0",
    "R7/8/5rk1/5p2/1p5P/5KP1/P7/8 b - - 0 0",
    "3k4/5ppp/2q5/3p2r1/8/1Q3P2/P4P1P/3R3K w - - 0 1",
    "4R3/1k6/1p2P1p1/p7/4r3/1P1r4/1K6/2R5 w - - 0 0",
    "5k2/R7/3K4/4p3/5P2/8/8/5r2 w - - 0 0",
    "5k2/1R6/4p1p1/1pr3Pp/7P/1K6/8/8 w - - 0 0",
    "5k2/8/p7/4K1P1/P4R2/6r1/8/8 b - - 0 0",
    "8/8/8/p2r1k2/7p/PP1RK3/6P1/8 b - - 0 0",
    "8/8/8/1P4p1/5k2/5p2/P6K/8 b - - 0 0",
    "3b2k1/1p3p2/p1p5/2P4p/1P2P1p1/5p2/5P2/4RK2 w - - 0 0",
    "5k2/3R4/2K1p1p1/4P1P1/5P2/8/3r4/8 b - - 0 0",
    "6k1/6pp/5p2/8/5P2/P7/2K4P/8 b - - 0 0",
    "8/3R4/8/r3N2p/P1Pp1P2/2k2K1P/3r4/8 w - - 0 0",
    "6k1/8/6r1/8/5b2/2PR4/4K3/8 w - - 0 0",
    "8/1p3k2/3B4/8/3b2P1/1P6/6K1/8 b - - 0 0",
    "8/8/8/2p1k3/P6R/1K6/6rP/8 w - - 0 0",
    "6k1/5p1p/6p1/1P1n4/1K4P1/N6P/8/8 w - - 0 0",
    "8/k5r1/2N5/PK6/2B5/8/8/8 b - - 0 0",
    "6k1/8/5K2/8/5P1R/r6P/8/8 b - - 0 0",
    "8/8/4k1KP/p5P1/r7/8/8/8 w - - 0 0",
    "1R6/p2r4/2ppkp2/6p1/2PKP2p/P4P2/6PP/8 b - - 0 0",
    "8/7p/6p1/8/k7/8/2K3P1/8 b - - 0 0",
    "R7/8/8/6p1/4k3/3rPp1P/8/6K1 b - - 0 0",
    "8/7p/1p1k2p1/p1p2p2/8/PP2P2P/4KPP1/8 w - - 0 0",
    "8/p7/1P6/1r3p1k/7P/3R1KP1/8/8 b - - 0 0",
    "8/5p1p/pk1p2p1/2pP4/2P2P2/4K2P/1P4P1/8 w - - 0 0",
    "5k2/5p2/6p1/7p/P7/2K3P1/7P/8 b - - 0 0",
    "8/8/5K2/3kn3/6B1/7P/8/8 b - - 0 0",
    "8/8/7k/8/8/8/5q2/3B2RK b - - 0 1",
    "8/p6p/1p2p1k1/4pp2/2P5/8/PP1K1PPP/8 b - - 0 0",
    "8/8/4kp2/5p1p/8/3KP1P1/7P/8 b - - 0 0",
    "8/7p/5kp1/4p3/p3rPRP/2K3P1/8/8 w - - 0 0",
    "8/6k1/8/R7/7K/1P6/5r2/8 b - - 0 1",
    "3b1N2/8/3k4/5pp1/8/5K1P/8/8 w - - 0 0",
    "R7/P7/5p2/4pk1p/5p2/3K1PP1/r6P/8 b - - 0 0",
    "8/7p/6p1/5k2/7N/8/4KP2/8 b - - 0 0",
    "6k1/8/p7/1p6/3K4/8/PPr4P/4R3 w - - 0 1",
    "8/8/6R1/5p1p/5k2/7r/8/2K5 w - - 0 0",
    "8/6Rp/8/5k2/5p2/5K2/7r/8 b - - 0 0",
    "6k1/3R4/5Kp1/6r1/4P3/8/8/8 b - - 0 0",
    "6K1/8/5P1k/2R5/1r6/8/2p5/8 w - - 0 0",
    "r1b2k2/1pp4p/3p2p1/pP1P4/2PN4/8/P5PP/4R1K1 w - - 0 24",
    "8/4k1pp/2p2r2/1p6/1P6/2R1K2P/P5P1/8 w - - 0 32",
    "2r5/3r4/p3k1b1/1p1pp1pp/8/1PP1NPP1/PK1R2P1/4R3 b - - 0 26",
    "5k2/1R3p2/1p2r2p/8/5pPP/5K2/8/8 b - - 0 38",
    "8/1k6/8/5NP1/8/2p3K1/8/r7 w - - 0 51",
    "8/1r4k1/3R1ppp/1p6/2p4P/2P5/1P4PK/8 b - - 0 43",
    "1r6/8/p4kp1/P1KP3p/8/7P/4B1P1/8 b - - 0 43",
    "8/8/2R2pk1/3r3p/1P3P1K/8/7P/8 w - - 0 47",
    "3B4/K7/2k1b1p1/1p2Pp1p/3P3P/2P3P1/8/8 w - - 0 74",
    "8/8/p5rp/3k4/1P2R3/2P1K3/6P1/8 w - - 0 1",
    "8/5pkp/1n4p1/1P6/3K2P1/2N4P/8/8 w - - 0 70",
    "8/8/7B/8/8/3p4/6Kp/3k1n2 w - - 0 0",
    "8/5pk1/4pbp1/7p/2Bp1P2/1P3KP1/8/8 b - - 0 45",
    "2r1r3/5k2/3p3p/pp6/4P1PP/3P3Q/1P6/7K w - - 0 34",
    "8/pp2k3/2p3B1/3p2P1/3n2K1/8/PPP5/8 b - - 0 0",
    "3b4/6k1/4p1p1/1p5p/1q2B2P/5QP1/5P2/6K1 w - - 0 0",
    "8/p4pk1/4n1p1/1p2P2p/q4P1P/P4QP1/5BK1/8 w - - 0 1",
    "4R3/3q1ppk/p6p/P7/2pr4/7P/4QPP1/6K1 w - - 0 36",
    "3r3k/p3b1pp/2p5/2p1p3/2P5/BPNPP1P1/P6P/6K1 w - - 0 33",
    "8/6pk/5p1p/8/2b5/P1B2PP1/4r2P/3R2K1 w - - 0 31",
    "3k4/2p2p2/1p5p/p1p1P1p1/P1Pn2P1/1P3P1P/1B3K2/8 w - - 0 30",
    "8/1p3p1k/4b1p1/1PP4p/4Q2P/2q5/5PP1/5BK1 w - - 0 42",
    "6b1/6p1/8/5kPP/K7/P1P5/8/8 w - - 0 50",
    "8/3N4/1p2p2p/p1k1P3/4Rn2/P4r2/1KP4P/8 b - - 0 42",
    "4b3/8/1p4p1/p1k1np1p/P1PNp2P/2K1P1P1/4BP2/8 w - - 0 68",
    "4k3/2b5/6pN/2p4p/2B2p2/3P4/1P5P/3b2K1 w - - 0 41",
    "3b4/6k1/4pqp1/1B5p/7P/5QP1/5P2/6K1 w - - 0 41",
    "8/7B/8/2pkP2R/p6p/PbK5/6PP/3r4 b - - 0 45",
    "4r1k1/1q3pp1/3p3p/1p2p3/2pPP1Q1/2P1P2P/1P4PK/R7 w - - 0 29",
    "8/1kp5/1pp3p1/p1n1qp2/4P3/3P1QP1/PPP1N3/1K6 w - - 0 27",
    "6k1/1p3pp1/p2np2p/P7/2P2P2/1P5P/4N1P1/6K1 w - - 0 36",
    "1n6/4k2p/p3ppp1/1pPp4/3P1PP1/3NP3/P3K2P/8 w - - 0 27",
    "3q2k1/1p3p2/2p1b3/4p1p1/p1P1P1P1/1P3P1p/P3Q2P/3N3K b - - 0 37",
    "8/8/8/pp1k1p2/7p/1PK1PP1P/8/8 w - - 0 52",
    "6k1/R7/8/5pp1/6P1/N3r3/P5KP/2b5 b - - 0 44",
    "7r/8/8/6k1/R6p/6pK/8/8 w - - 0 52",
    "8/8/6pk/4Rp2/4p2P/6PK/1r6/8 b - - 3 57",
    "8/6k1/1p1r2pp/p7/P1P1K1P1/1P3R1P/8/8 b - - 0 42",
    "2r5/3k1pp1/p7/1p2P1P1/3PK3/8/P7/1R6 w - - 0 35",
    "8/6pp/1k1r1p2/8/1R2P3/4KP2/6rP/1R6 b - - 0 29",
    "R7/8/8/8/6K1/5p2/5Pk1/4r3 w - - 0 1",
    "8/5ppk/3N4/6n1/3RP3/1r6/5PPK/8 b - - 0 1",
    "6k1/2p3np/1p1p2p1/3P4/1PPK1R2/6PB/7P/4r3 w - - 0 1",
    "8/r7/5PK1/3k4/p7/8/1R6/8 w - - 0 0",
    "5k2/5p2/6p1/2P1Pn1p/3pBP2/1N1P3b/5K2/8 w - - 0 0",
    "3k4/1K5p/p2rpp2/4b1p1/P7/2P2B1P/1P3PP1/4R3 b - - 0 41",
    "8/8/1pB2k1p/2p1pPpP/2P1P1P1/bP6/P1K5/8 w - - 0 1",
    "8/pR4pk/1b2p3/2p3p1/N1p5/7P/PP1r2P1/6K1 b - - 0 1",
    "3R4/7k/8/2p5/1pPb4/1P5P/3n2KP/8 w - - 0 50",
    "R7/P4r2/5k2/3Kp3/8/8/8/8 w - - 0 0",
    "8/pp4pp/4k3/3rPp2/1Pr4P/2B1KPP1/1P6/4R3 b - - 0 30",
    "8/p3k1p1/5p1p/5P2/3PP3/8/P5K1/8 w - - 0 0",
    "8/8/1p1k4/5ppp/PPK1p3/6P1/5PP1/8 b - - 0 40",
    "8/5pp1/7p/5P1P/2p3P1/2k5/5P2/2K5 w - - 0 0",
    "8/8/5n2/1P1k4/3p1P2/3P1K2/8/8 w - - 0 0",
    "8/4kppp/R7/1r6/4PK1P/6P1/5P2/8 b - - 0 3",
    "8/6p1/5p2/4pk2/r6p/5P2/4RKPP/8 b - - 0 1",
    "8/5p2/r4kpp/P7/R6P/6P1/5PK1/8 w - - 0 1",
    "R7/5pk1/P5p1/7p/7P/r5P1/5P2/5K2 b - - 0 1",
    "8/r7/5ppk/p6p/8/R5P1/5P1P/6K1 w - - 0 1",
    "6k1/6pp/8/2r2p2/P4P1P/3R2P1/8/5K2 b - - 0 1",
    "8/5k2/4p3/2R2p2/6p1/4P1P1/1P2KP2/7r w - - 0 1",
    "R7/6k1/P5p1/5p1p/5P1P/r5P1/5K2/8 b - - 0 1",
    "R7/6k1/P7/5p1p/5PpP/6P1/r7/6K1 w - - 0 1",
    "8/5p2/4pk2/p6p/3P3P/2K1PP2/8/8 b - - 0 43",
    "8/PR4p1/5k2/7p/4p3/7P/r4PP1/5K2 w - - 0 42",
    "3r4/5p1p/pkn3p1/1p6/8/1P2R3/1PB2PPP/4K3 b - - 0 28",
    "b1k4r/p4ppp/4n3/1R6/8/8/PPP2P1P/2KR4 w - - 0 20",
    "3r4/4k1p1/3pp2p/2p2p2/r1P5/3KPP2/P2R2PP/3R4 w - - 0 30",
    "4k3/p1p2r1p/1p4p1/n2p4/P2P1P1P/2PB2P1/6K1/R7 w - - 0 27",
    "5rk1/p3r2p/1pp3p1/5p2/R1PPp2P/4P1P1/P4P2/1R3K2 w - - 0 27",
    "8/5pp1/p3p3/1p1kP2p/1b3P1P/1P1K2P1/P4B2/8 b - - 0 33",
    "4n3/1p1b1p2/p2k2p1/P2p3p/1P1N3P/2PB1PP1/5K2/8 w - - 0 32",
    "8/p2R1pkp/1p4p1/4P1r1/1P6/8/P3KPP1/8 w - - 0 37",
    "3b4/8/1p6/p2k4/PP4Kp/8/8/4B3 b - - 0 1",
    "8/8/8/1p2b1pp/p3Pp2/Pk3P1P/1P6/2KN4 b - - 0 1",
    "8/2R5/p3k1p1/nr4P1/3PKP2/2B5/8/8 w - - 0 1",
    "8/8/8/p1k2K1R/5P1P/8/4p1n1/8 w - - 0 1",
    "8/2k5/8/8/R4b2/4p1p1/5r2/4B1K1 b - - 0 1",
    "8/B2k4/1P3K2/3bP3/8/8/8/8 w - - 0 1",
    "8/5k1p/5PpB/3PR3/2r4P/1p3K2/2b5/8 b - - 0 1",
    "8/1p2p1kp/4p3/7p/8/5K2/1PP3P1/8 w - - 0 1",
    "6k1/1pp3pp/p4p2/8/8/Pb2B3/1P3PPP/6K1 w - - 0 1",
    "8/5k2/6R1/4r2p/8/6KP/6P1/8 w - - 5 49",
    "8/pp4pp/2pn1k2/3p1p2/3P1K2/6PP/PPP1B1P1/8 w - - 0 24",
    "4n3/p3k3/1p4P1/2pK4/P2p4/1P6/2P1B3/8 w - - 0 49",
    "r7/pp5k/7p/3P1Np1/8/PP5P/1B5K/8 w - - 0 36",
    "8/8/1Q5p/5p1k/P7/5PP1/b6K/q7 w - - 0 45",
    "8/p5k1/6p1/n1p5/4B3/8/P5PP/5K2 w - - 0 1",
    "8/1p4p1/5p1p/1k3P2/6PP/3KP3/8/8 w - - 0 50",
    "8/8/1p1k4/5ppp/PPK1p3/6P1/5PP1/8 b - - 0 0",
    "8/5k2/4p2p/4P3/B1np1KP1/3b4/8/2B5 b - - 0 1"};

struct BenchmarkResult {
    U64 nodes = 0;
    U64 time = 0;
};

BenchmarkResult Run(std::string title, const std::vector<std::string> &positions, U8 depth) {
    BenchmarkResult result;
    printf("-----%s-----\n", title.c_str());
    for (size_t i = 0; i < positions.size();) {
        Board board = Import::FEN(positions.at(i));
        Perft perft = Perft();
        std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
        result.nodes += perft.RunFromPosition(board, depth);
        std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
        result.time += std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        ++i;
        if (positions.size() < 10 || i % (positions.size() / 10) == 0 || i == positions.size())
            printf("Position: %zu of %zu\n", i, positions.size());
    }
    return result;
}

int main(int, char *argv[]) {
    BenchmarkResult opening = Run("Opening", openings, 4);
    BenchmarkResult midgame = Run("Mid Game", testPositions, 3);
    BenchmarkResult endgame = Run("End Game", endgamePositions, 4);
    printf("----- Report -----\n");
    printf("Opening: %llu ms (%llu nodes/ms)\n", opening.time, opening.nodes / opening.time);
    printf("Mid Game: %llu ms (%llu nodes/ms)\n", midgame.time, midgame.nodes / midgame.time);
    printf("End Game: %llu ms (%llu nodes/ms)\n", endgame.time, endgame.nodes / endgame.time);
}
