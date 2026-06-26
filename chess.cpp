#include <iostream>
#include <cmath>
using namespace std;

//enum for color
enum Color {
    WHITE, BLACK, NONE
};

//enum for chess piece type
enum PieceType {
    KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY
};

//Base Abstract Class (Piece)
class Piece {
protected:
    Color color;
    PieceType type;
    bool hasMoved;

public:
    Piece(Color c = NONE, PieceType t = EMPTY) : color(c), type(t), hasMoved(false) {}

    Color getColor() const { return color; }
    PieceType getType() const { return type; }
    bool getHasMoved() const { return hasMoved; }
    void setHasMoved(bool moved) { hasMoved = moved; }

    // PURE VIRTUAL FUNCTIONS 
    virtual bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) = 0;
    virtual char getSymbol() const = 0;

    virtual ~Piece() {}
};

//derived class (Empty Square)
class EmptySquare : public Piece {
public:
    EmptySquare() : Piece(NONE, EMPTY) {}

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        return false;       // Empty squares cannot move
    }

    char getSymbol() const override {
        return ' ';
    }
};

//Pawn Class (Derived from Piece Class)
class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PAWN) {}

    char getSymbol() const override {
        return (color == WHITE) ? 'P' : 'p';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        int direction = (color == WHITE) ? -1 : 1;

        // Same color capture check

        if (board[endX][endY]->getColor() == color) {
            return false;
        }

        // Move forward
        if (startY == endY) {
            // 1 step
            if (endX == startX + direction && board[endX][endY]->getType() == EMPTY)
                return true;

            // 2 steps (first move)
            if (!hasMoved && endX == startX + 2 * direction &&
                board[startX + direction][startY]->getType() == EMPTY &&
                board[endX][endY]->getType() == EMPTY)
                return true;
        }

        // Diagonal capture
        if (abs(endY - startY) == 1 && endX == startX + direction) {
            if (board[endX][endY]->getType() != EMPTY)
                return true;
        }

        return false;
    }
};


//Rook Piece (Derived from Piece)
class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, ROOK) {}    //constructor

    char getSymbol() const override {
        return (color == WHITE) ? 'R' : 'r';
    }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        // Same color capture check

        if (board[endX][endY]->getColor() == color) {

            return false;
        }

        //straight logic
        if (startX != endX && startY != endY) return false;

        //path logic
        int dx = (endX > startX) ? 1 : (endX < startX ? -1 : 0);
        int dy = (endY > startY) ? 1 : (endY < startY ? -1 : 0);

        int x = startX + dx;
        int y = startY + dy;

        while (x != endX || y != endY) {
            if (board[x][y]->getType() != EMPTY)
                return false;
            x += dx;
            y += dy;
        }

        return true;
    }
};