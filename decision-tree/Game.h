#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <cassert>
#include <stdint.h>
#include <iostream>
#include <functional>

#define GAME_WIDTH 7
#define GAME_HEIGHT 6

enum GameColor: uint8_t { RED = 0, BLACK = 1, NONE };

class GameCol {
    friend class GameField;
#define PADDING 8u
#define FIELD_SIZE 4u
#define FIELD_MASK ((1 << FIELD_SIZE) - 1) // for FIELD_SIZE = 4 FIELD_MASK = 0xf

    uint32_t data;
    GameCol(): data(0) {
        for (int c = 0; c < GAME_HEIGHT; ++c) {
            this->operator[](c) = NONE;
        }
    }

    class ColorSetter {
        friend class GameCol;
        int pos;
        GameCol * self;
        ColorSetter(int pos, GameCol * owner): pos(pos), self(owner) {}
    public:
        ColorSetter & operator=(GameColor color) {
            self->data = (color << (PADDING + pos * FIELD_SIZE)) | self->data & ~(FIELD_MASK << (PADDING + pos * FIELD_SIZE));
            return *this;
        }

        operator GameColor() {
            return static_cast<GameColor>((self->data >> (PADDING + pos * FIELD_SIZE)) & FIELD_MASK);
        }
    };
public:
    ColorSetter operator[](int pos) {
        assert(pos >= 0 && pos < 6);
        return ColorSetter(pos, this);
    }

    GameColor operator[](int pos) const {
        assert(pos >= 0 && pos < 6);
        return static_cast<GameColor>((data >> (PADDING + pos * FIELD_SIZE)) & FIELD_MASK);
    }
};

class GameField {
    GameCol cols[GAME_WIDTH];
    GameColor winner;
public:
    GameField(): winner(NONE) {}

    const GameCol & operator[](int pos) const {
        assert(pos >= 0 && pos < GAME_WIDTH);
        return cols[pos];
    }

    bool CanDropAt(int pos) const {
        return this->operator[](pos)[0] == NONE;
    }

    bool HasWinner() const {
        return winner != NONE;
    }

    GameColor GetWinner() const {
        return winner;
    }

    void DropAt(int pos, GameColor color) {
        for (int c = 5; c >= 0; --c) {
            if (this->operator[](pos)[c] == NONE) {
                this->operator[](pos)[c] = color;
                this->checkWinner(pos, c);
                return;
            }
        }
    }

    GameField DroppedAt(int pos, GameColor color) {
        GameField newBoard(*this);
        newBoard.DropAt(pos, color);
        return newBoard;
    }

    void GetImmediateSums(int sums[4], int x, int y, GameColor target) const {
        const GameField & self = *this;
        auto checker = [&sums](int idx, std::function<bool(int)> cont) {
            for (int c = 1; c < 4; ++c) {
                if (!cont(c)) {
                    return;
                }
                ++sums[idx];
            }
        };
        // horizontal
        checker(0, [&](int c) { return x + c < GAME_WIDTH && self[x + c][y] == target; });
        checker(0, [&](int c) { return x - c >= 0 && self[x - c][y] == target; });

        // vertical
        checker(1, [&](int c) { return y + c < GAME_HEIGHT && self[x][y + c] == target; });
        checker(1, [&](int c) { return y - c >= 0 && self[x][y - c] == target; });

        // main diag
        checker(2, [&](int c) { return x + c < GAME_WIDTH && y + c < GAME_HEIGHT && self[x + c][y + c] == target; });
        checker(2, [&](int c) { return x - c >= 0 && y - c >= 0 && self[x - c][y - c] == target; });

        // second diag
        checker(3, [&](int c) { return x + c < GAME_WIDTH && y - c >= 0 && self[x + c][y - c] == target; });
        checker(3, [&](int c) { return x - c >= 0 && y + c < GAME_HEIGHT && self[x - c][y + c] == target; });
    }
private:
    void checkWinner(int x, int y) {
        GameColor target = this->operator[](x)[y];
        
        int sums[4] = { 1, 1, 1, 1 };
        GetImmediateSums(sums, x, y, target);
        
        bool win = false;
        for (int c = 0; c < 4; ++c) {
            win = win || sums[c] == 4;
        }

        winner = win ? target : NONE;
    }


    GameCol & operator[](int pos) {
        assert(pos >= 0 && pos < 7);
        return cols[pos];
    }
};


std::ostream & operator<<(std::ostream & out, const GameField & field) {
    char symbols[3];
    symbols[RED] = 'R';
    symbols[BLACK] = 'B';
    symbols[NONE] = ' ';
    for (int r = 0; r < GAME_HEIGHT; ++r) {
        out << "|";
        for (int c = 0; c < GAME_WIDTH; ++c) {
            out << symbols[field[c][r]] << "|";
        }
        out << std::endl;
    }
    return out;
}

#endif // GAME_H_INCLUDED