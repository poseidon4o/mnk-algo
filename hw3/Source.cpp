#include "Game.h"
#include "GameTree.h"
#include <iostream>
using namespace std;

int main() {

    GameTree tree(RED, 4);
    const GameField & field = tree.GetField();

    while (!field.HasWinner()) {
        if (tree.NextToMove() == RED) {
            while (tree.NextToMove() == RED) {
                int inp;
                cout << field << endl << "player move: ";
                cin >> inp;
                tree.MakeMove(inp);
            }
        } else {
            tree.MakeBestMove();
        }
    }

    cout << "YOU " << (field.GetWinner() == RED ? "WON" : "LOST");
    cout << endl << field;
    cin.ignore();
    cin.get();

    return 0;
}