
#ifndef GAME_TREE_INCLUDED
#define GAME_TREE_INCLUDED

#include "Game.h"

#include <array>
#include <limits>

#include <iostream>
using namespace std;

const int GAME_PINF = std::numeric_limits<int>::max();
const int GAME_NINF = std::numeric_limits<int>::min();

#define SWITCH_COLOR(col) (static_cast<GameColor>(!col))

class GameTree {

    struct node {
        std::array<node *, GAME_WIDTH> turns;
        node() {
            fill(turns.begin(), turns.end(), nullptr);
        }
    };

    GameColor nextMove, maximizingPlayer;
    const int maxDepth;
    node * root;
    GameField field;

public:
    GameTree(GameColor firstMover, int maxDepth): nextMove(firstMover), maxDepth(maxDepth), root(new node()) {}

    const GameField & GetField() const {
        return field;
    }

    bool MakeMove(int pos) {
        if (!field.CanDropAt(pos)) {
            return false;
        }
        field.DropAt(pos, nextMove);
        nextMove = SWITCH_COLOR(nextMove);
        return true;
    }

    void MakeBestMove() {
        int score;
        int col = min_max(nextMove, field, maxDepth, nextMove, score);

        if (col == -1) {
            throw 42;
        }
        MakeMove(col);
    }

    GameColor NextToMove() const {
        return nextMove;
    }
private:
    GameTree(const GameTree &) = delete;
    GameTree & operator=(const GameTree &) = delete;

    int min_max(const GameColor ai, GameField board, int depth, GameColor onMove, int & bestScore) {
		bestScore = (ai == onMove) ? GAME_NINF : GAME_PINF;
        int col = -1, row = -1;
        int score = 0;

        if (!depth || board.HasWinner()) {
            bestScore = evalFor(board, onMove);
            return col;
        }

        for (int c = 0; c < GAME_WIDTH; ++c) {
            if (!board.CanDropAt(c)) {
                continue;
            }
            min_max(ai, board.DroppedAt(c, onMove), depth - 1, SWITCH_COLOR(onMove), score);
            if (depth == maxDepth) {
                cout << score << ' ';
            }
            if (ai == onMove) {
                if (score > bestScore) {
                    bestScore = score;
                    col = c;
                }
            } else {
                if (score < bestScore) {
                    bestScore = score;
                    col = c;
                }
            }
        }

        if (depth == maxDepth) {
            cout << endl;
        }

        return col;
    }


    int alpha_beta(GameField board, int alpha, int beta, GameColor color, int remainingSteps) {
        if (!remainingSteps || board.HasWinner()) {
            return evalFor(board, color);
        }

        if (color == nextMove) {
            for (int c = 0; c < GAME_WIDTH; ++c) {
                if (!board.CanDropAt(c)) continue;
                alpha = std::max(alpha, alpha_beta(board.DroppedAt(c, color), alpha, beta, SWITCH_COLOR(color), remainingSteps - 1));
                if (alpha >= beta) {
                    break;
                }
            }
            return alpha;
        } else {
            for (int c = 0; c < GAME_WIDTH; ++c) {
                if (!board.CanDropAt(c)) continue;
                beta = std::min(beta, alpha_beta(board.DroppedAt(c, color), alpha, beta, SWITCH_COLOR(color), remainingSteps - 1));
                if (beta <= alpha) {
                    break;
                }
            }
            return beta;
        }
    }

    int evalFor(const GameField & board, GameColor color) {
        static int evalTable[GAME_HEIGHT][GAME_WIDTH] = {
            { 3, 4, 5, 7, 5, 4, 3 },
            { 4, 6, 8, 10, 8, 6, 4 },
            { 5, 8, 11, 13, 11, 8, 5 },
            { 5, 8, 11, 13, 11, 8, 5 },
            { 4, 6, 8, 10, 8, 6, 4 },
            { 3, 4, 5, 7, 5, 4, 3 }
        };

        int score = 0;
        for (int c = 0; c < GAME_HEIGHT; ++c) {
            for (int r = 0; r < GAME_WIDTH; ++r) {
                if (board[r][c] == color) {
                    score += evalTable[c][r];
                } else if (board[r][c] == SWITCH_COLOR(color)) {
                    score -= evalTable[c][r];
                }
            }
        }

        return score;
    }
};

#endif //GAME_TREE_INCLUDED