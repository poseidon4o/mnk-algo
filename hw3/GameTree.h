#ifndef GAME_TREE_INCLUDED
#define GAME_TREE_INCLUDED

#include "Game.h"

#include <array>
#include <limits>

const int GAME_PINF = std::numeric_limits<int>::max();
const int GAME_NINF = std::numeric_limits<int>::min();

#define SWITCH_COLOR(col) (static_cast<GameColor>(!col))

class GameTree {

    struct node {
        std::array<node *, GAME_WIDTH> turns;
        node(): turns{ { 0, 0, 0, 0, 0, 0, 0 } } {}
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
        int best = GAME_NINF, idx = 0;
        GameField board(field);
        
        for (int c = 0; c < GAME_WIDTH; ++c) {
            if (board.CanDropAt(c)) {
                idx = c;
                break;
            }
        }
        
        for (int c = 0; c < GAME_WIDTH; ++c) {
            if (!board.CanDropAt(c)) continue;
            int thisNode = alpha_beta(board.DroppedAt(c, nextMove), GAME_NINF, GAME_PINF, SWITCH_COLOR(nextMove), maxDepth - 1);
            if (best < thisNode) {
                best = thisNode;
                idx = c;
            }
        }
        MakeMove(idx);
    }

    GameColor NextToMove() const {
        return nextMove;
    }
private:
    GameTree(const GameTree &) = delete;
    GameTree & operator=(const GameTree &) = delete;

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
        int value = 0;

        if (board.HasWinner()) {
            return GAME_NINF;
        }

        for (int c = 0; c < GAME_WIDTH; ++c) {
            if (!board.CanDropAt(c)) {
                continue;
            }
            int top = GAME_HEIGHT - 1;
            for (int r = top; r >= 0; --r) {
                top = r;
                if (board[c][r] == NONE) {
                    break;
                }
            }

            int me[4] = { 0, 0, 0, 0 }, opp[4] = { 0, 0, 0, 0 };
            board.GetImmediateSums(me, c, top, color);
            board.GetImmediateSums(me, c, top, SWITCH_COLOR(color));
            for (int c = 0; c < 4; ++c) {
                value += (me[c] >= 3 ? 200 : me[c]) - (opp[c] >= 3 ? 100 : opp[c]);
            }
        }
        return value;
    }
};

#endif //GAME_TREE_INCLUDED