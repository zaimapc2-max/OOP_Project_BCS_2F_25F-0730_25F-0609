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
class Bishop : public Piece {
public:
	Bishop(Color c) : Piece(c, BISHOP) {}

	char getSymbol() const override {
		return (color == WHITE) ? 'B' : 'b';
	}

	bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
		// Same color capture check
		if (board[endX][endY]->getColor() == color) {

			return false;
		}

		if (abs(endX - startX) != abs(endY - startY))
			return false;

		int dx = (endX > startX) ? 1 : -1;
		int dy = (endY > startY) ? 1 : -1;

		int x = startX + dx;
		int y = startY + dy;

		while (x != endX && y != endY) {
			if (board[x][y]->getType() != EMPTY)
				return false;
			x += dx;
			y += dy;
		}

		return true;
	}
};

//Knight Piece (Derived from Piece)
class Knight : public Piece {
public:
	Knight(Color c) : Piece(c, KNIGHT) {}

	char getSymbol() const override {
		return (color == WHITE) ? 'N' : 'n';
	}

	bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
		// Same color capture check
		if (board[endX][endY]->getColor() == color) {

			return false;
		}

		int dx = abs(endX - startX);
		int dy = abs(endY - startY);

		return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
	}
};

//Queen Piece (Derived from Piece)
class Queen : public Piece {
public:
	Queen(Color c) : Piece(c, QUEEN) {}

	char getSymbol() const override {
		return (color == WHITE) ? 'Q' : 'q';
	}

	bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
		// Same color capture check
		if (board[endX][endY]->getColor() == color) {

			return false;
		}

		int dx = abs(endX - startX);
		int dy = abs(endY - startY);

		// Diagonal
		if (dx == dy) {
			int stepX = (endX > startX) ? 1 : -1;
			int stepY = (endY > startY) ? 1 : -1;

			int x = startX + stepX;
			int y = startY + stepY;

			while (x != endX && y != endY) {
				if (board[x][y]->getType() != EMPTY)
					return false;
				x += stepX;
				y += stepY;
			}
			return true;
		}

		// Straight
		if (startX == endX || startY == endY) {
			int stepX = (endX > startX) ? 1 : (endX < startX ? -1 : 0);
			int stepY = (endY > startY) ? 1 : (endY < startY ? -1 : 0);

			int x = startX + stepX;
			int y = startY + stepY;

			while (x != endX || y != endY) {
				if (board[x][y]->getType() != EMPTY)
					return false;
				x += stepX;
				y += stepY;
			}
			return true;
		}

		return false;
	}
};

//King Piece (Derived from Piece)
class King : public Piece {
public:
	King(Color c) : Piece(c, KING) {}

	char getSymbol() const override {
		return (color == WHITE) ? 'K' : 'k';
	}

	bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
		// Same color capture check
		if (board[endX][endY]->getColor() == color) {

			return false;
		}

		int dx = abs(endX - startX);
		int dy = abs(endY - startY);

		return dx <= 1 && dy <= 1;
	}
};
//Board Class
class Board {
private:
    Piece* squares[8][8];

public:
    Board() {       //constructor
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                squares[i][j] = new EmptySquare();
    }

    ~Board() {          //destructor
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                delete squares[i][j];
    }

    Piece* getPiece(int x, int y) {
        if (x >= 0 && x < 8 && y >= 0 && y < 8)
            return squares[x][y];
        return nullptr;
    }

    void setPiece(int x, int y, Piece* piece) {
        delete squares[x][y];
        squares[x][y] = piece;
    }

    void setPieceNoDelete(int x, int y, Piece* piece) {
        squares[x][y] = piece;
    }

    void display() {            //display board
        cout << "\n    a   b   c   d   e   f   g   h\n";
        cout << "  +---+---+---+---+---+---+---+---+\n";

        for (int i = 0; i < 8; i++) {
            cout << (8 - i) << " | ";

            for (int j = 0; j < 8; j++) {
                if (squares[i][j] != nullptr)
                    cout << squares[i][j]->getSymbol() << " | ";
                else
                    cout << "? | ";
            }
            cout << (8 - i) << "\n";
            cout << "  +---+---+---+---+---+---+---+---+\n";
        }

        cout << "    a   b   c   d   e   f   g   h\n\n";
    }

    Piece* (*getSquares())[8] {
        return squares;
    }

    void initializeBoard() {
        // Black pieces (rows 0-1)
        setPiece(0, 0, new Rook(BLACK));
        setPiece(0, 1, new Knight(BLACK));
        setPiece(0, 2, new Bishop(BLACK));
        setPiece(0, 3, new Queen(BLACK));
        setPiece(0, 4, new King(BLACK));
        setPiece(0, 5, new Bishop(BLACK));
        setPiece(0, 6, new Knight(BLACK));
        setPiece(0, 7, new Rook(BLACK));

        for (int i = 0;i < 8;i++) setPiece(1, i, new Pawn(BLACK));

        // White pieces (rows 6-7)
        for (int i = 0;i < 8;i++) setPiece(6, i, new Pawn(WHITE));

        setPiece(7, 0, new Rook(WHITE));
        setPiece(7, 1, new Knight(WHITE));
        setPiece(7, 2, new Bishop(WHITE));
        setPiece(7, 3, new Queen(WHITE));
        setPiece(7, 4, new King(WHITE));
        setPiece(7, 5, new Bishop(WHITE));
        setPiece(7, 6, new Knight(WHITE));
        setPiece(7, 7, new Rook(WHITE));
    }
};
