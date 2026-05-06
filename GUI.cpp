#include <iostream>
#include <cmath>
using namespace std;

// ANSI color codes for piece letters only
#define WHITE_PIECE "\033[38;5;205m"  // pink
#define BLACK_PIECE "\033[38;5;157m"   // green
#define RESET       "\033[0m"
#define LINE_COLOR "\033[38;5;151m"
#define LOWER_LINE "\033[38;5;225m"
#define HEADING_COLOR "\033[38;5;231m"
#define APLHABETS "\033[38;5;218m"
#define APLHABETSU "\033[38;5;194m"


enum Color { WHITE, BLACK, NONE };
enum PieceType { KING, QUEEN, ROOK, BISHOP, KNIGHT, PAWN, EMPTY };

class Piece {
protected:
    Color     color;
    PieceType type;
    bool      hasMoved;

public:
    Piece(Color c = NONE, PieceType t = EMPTY) : color(c), type(t), hasMoved(false) {}

    Color     getColor()    const { return color; }
    PieceType getType()     const { return type; }
    bool      getHasMoved() const { return hasMoved; }
    void      setHasMoved(bool moved) { hasMoved = moved; }

    virtual bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) = 0;
    virtual char getSymbol() const = 0;

    virtual ~Piece() {}
};

class EmptySquare : public Piece {
public:
    EmptySquare() : Piece(NONE, EMPTY) {}
    bool isValidMove(int, int, int, int, Piece* (*)[8]) override { return false; }
    char getSymbol() const override { return ' '; }
};

class Pawn : public Piece {
public:
    Pawn(Color c) : Piece(c, PAWN) {}
    char getSymbol() const override { return (color == WHITE) ? 'P' : 'p'; }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        int direction = (color == WHITE) ? -1 : 1;
        if (board[endX][endY]->getColor() == color) return false;

        if (startY == endY) {
            if (endX == startX + direction && board[endX][endY]->getType() == EMPTY)
                return true;
            if (!hasMoved && endX == startX + 2 * direction &&
                board[startX + direction][startY]->getType() == EMPTY &&
                board[endX][endY]->getType() == EMPTY)
                return true;
        }
        if (abs(endY - startY) == 1 && endX == startX + direction &&
            board[endX][endY]->getType() != EMPTY)
            return true;

        return false;
    }
};

class Rook : public Piece {
public:
    Rook(Color c) : Piece(c, ROOK) {}
    char getSymbol() const override { return (color == WHITE) ? 'R' : 'r'; }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        if (board[endX][endY]->getColor() == color) return false;
        if (startX != endX && startY != endY) return false;

        int dx = (endX > startX) ? 1 : (endX < startX ? -1 : 0);
        int dy = (endY > startY) ? 1 : (endY < startY ? -1 : 0);
        int x = startX + dx, y = startY + dy;

        while (x != endX || y != endY) {
            if (board[x][y]->getType() != EMPTY) return false;
            x += dx; y += dy;
        }
        return true;
    }
};

class Bishop : public Piece {
public:
    Bishop(Color c) : Piece(c, BISHOP) {}
    char getSymbol() const override { return (color == WHITE) ? 'B' : 'b'; }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        if (board[endX][endY]->getColor() == color) return false;
        if (abs(endX - startX) != abs(endY - startY)) return false;

        int dx = (endX > startX) ? 1 : -1;
        int dy = (endY > startY) ? 1 : -1;
        int x = startX + dx, y = startY + dy;

        while (x != endX && y != endY) {
            if (board[x][y]->getType() != EMPTY) return false;
            x += dx; y += dy;
        }
        return true;
    }
};

class Knight : public Piece {
public:
    Knight(Color c) : Piece(c, KNIGHT) {}
    char getSymbol() const override { return (color == WHITE) ? 'N' : 'n'; }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        if (board[endX][endY]->getColor() == color) return false;
        int dx = abs(endX - startX), dy = abs(endY - startY);
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }
};

class Queen : public Piece {
public:
    Queen(Color c) : Piece(c, QUEEN) {}
    char getSymbol() const override { return (color == WHITE) ? 'Q' : 'q'; }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        if (board[endX][endY]->getColor() == color) return false;
        int dx = abs(endX - startX), dy = abs(endY - startY);

        if (dx == dy) {
            int stepX = (endX > startX) ? 1 : -1;
            int stepY = (endY > startY) ? 1 : -1;
            int x = startX + stepX, y = startY + stepY;
            while (x != endX && y != endY) {
                if (board[x][y]->getType() != EMPTY) return false;
                x += stepX; y += stepY;
            }
            return true;
        }

        if (startX == endX || startY == endY) {
            int stepX = (endX > startX) ? 1 : (endX < startX ? -1 : 0);
            int stepY = (endY > startY) ? 1 : (endY < startY ? -1 : 0);
            int x = startX + stepX, y = startY + stepY;
            while (x != endX || y != endY) {
                if (board[x][y]->getType() != EMPTY) return false;
                x += stepX; y += stepY;
            }
            return true;
        }

        return false;
    }
};

class King : public Piece {
public:
    King(Color c) : Piece(c, KING) {}
    char getSymbol() const override { return (color == WHITE) ? 'K' : 'k'; }

    bool isValidMove(int startX, int startY, int endX, int endY, Piece* (*board)[8]) override {
        if (board[endX][endY]->getColor() == color) return false;
        int dx = abs(endX - startX), dy = abs(endY - startY);
        return dx <= 1 && dy <= 1;
    }
};

class Board {
private:
    Piece* squares[8][8];

public:
    Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                squares[i][j] = new EmptySquare();
    }

    ~Board() {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                delete squares[i][j];
    }

    Piece* getPiece(int x, int y) {
        if (x >= 0 && x < 8 && y >= 0 && y < 8) return squares[x][y];
        return nullptr;
    }

    void setPiece(int x, int y, Piece* piece) {
        delete squares[x][y];
        squares[x][y] = piece;
    }

    void setPieceNoDelete(int x, int y, Piece* piece) {
        squares[x][y] = piece;
    }

    Piece* (*getSquares())[8] { return squares; }

    void display() {
        cout << APLHABETS << "\n    a   b   c   d   e   f   g   h\n" << RESET;
        cout << HEADING_COLOR << "  +---+---+---+---+---+---+---+---+\n" << RESET;

        for (int i = 0; i < 8; i++) {
            cout << HEADING_COLOR << (8 - i) << " | " << RESET;

            for (int j = 0; j < 8; j++) {
                Piece* p = squares[i][j];
                if (p != nullptr && p->getType() != EMPTY) {
                    if (p->getColor() == WHITE)
                        cout << WHITE_PIECE << p->getSymbol() << RESET;
                    else
                        cout << BLACK_PIECE << p->getSymbol() << RESET;
                }
                else {
                    cout << ' ';
                }
                cout << HEADING_COLOR << " | " << RESET;
            }

            cout << HEADING_COLOR << (8 - i) << "\n" << RESET;
            cout << HEADING_COLOR << "  +---+---+---+---+---+---+---+---+\n" << RESET;
        }

        cout << APLHABETSU << "    a   b   c   d   e   f   g   h\n\n" << RESET;
    }

    void initializeBoard() {
        setPiece(0, 0, new Rook(BLACK));
        setPiece(0, 1, new Knight(BLACK));
        setPiece(0, 2, new Bishop(BLACK));
        setPiece(0, 3, new Queen(BLACK));
        setPiece(0, 4, new King(BLACK));
        setPiece(0, 5, new Bishop(BLACK));
        setPiece(0, 6, new Knight(BLACK));
        setPiece(0, 7, new Rook(BLACK));
        for (int i = 0; i < 8; i++) setPiece(1, i, new Pawn(BLACK));

        for (int i = 0; i < 8; i++) setPiece(6, i, new Pawn(WHITE));
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

class Game {
private:
    Board       board;
    Color       currentPlayer;
    bool        gameOver;
    EmptySquare emptyPlaceholder;

    bool findKing(Color color, int& kingX, int& kingY) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                Piece* p = board.getPiece(i, j);
                if (p->getType() == KING && p->getColor() == color) {
                    kingX = i; kingY = j; return true;
                }
            }
        return false;
    }

    bool isSquareUnderAttack(int x, int y, Color byColor) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++) {
                Piece* p = board.getPiece(i, j);
                if (!p || p->getType() == EMPTY || p->getColor() != byColor) continue;
                if (p->isValidMove(i, j, x, y, board.getSquares())) return true;
            }
        return false;
    }

    bool isInCheck(Color color) {
        int kingX, kingY;
        if (!findKing(color, kingX, kingY)) return false;
        Color opponent = (color == WHITE) ? BLACK : WHITE;
        return isSquareUnderAttack(kingX, kingY, opponent);
    }

    bool testMoveDoesNotLeaveInCheck(int startX, int startY, int endX, int endY, Color color) {
        Piece* piece = board.getPiece(startX, startY);
        Piece* captured = board.getPiece(endX, endY);

        board.setPieceNoDelete(endX, endY, piece);
        board.setPieceNoDelete(startX, startY, &emptyPlaceholder);

        bool safe = !isInCheck(color);

        board.setPieceNoDelete(startX, startY, piece);
        board.setPieceNoDelete(endX, endY, captured);

        return safe;
    }

    bool hasLegalMoves(Color color) {
        for (int startX = 0; startX < 8; startX++)
            for (int startY = 0; startY < 8; startY++) {
                Piece* p = board.getPiece(startX, startY);
                if (!p || p->getType() == EMPTY || p->getColor() != color) continue;
                for (int endX = 0; endX < 8; endX++)
                    for (int endY = 0; endY < 8; endY++) {
                        if (!p->isValidMove(startX, startY, endX, endY, board.getSquares())) continue;
                        if (testMoveDoesNotLeaveInCheck(startX, startY, endX, endY, color)) return true;
                    }
            }
        return false;
    }

public:
    Game() : currentPlayer(WHITE), gameOver(false) {
        board.initializeBoard();
    }

    bool parsePosition(string pos, int& row, int& col) {
        if (pos.length() != 2) return false;
        col = pos[0] - 'a';
        row = 8 - (pos[1] - '0');
        if (row < 0 || row >= 8 || col < 0 || col >= 8) return false;
        return true;
    }

    bool makeMove(int startX, int startY, int endX, int endY) {
        Piece* piece = board.getPiece(startX, startY);

        if (piece->getType() == EMPTY) {
            cout << HEADING_COLOR << "No piece at starting position!\n" << RESET;
            return false;
        }
        if (piece->getColor() != currentPlayer) {
            cout << HEADING_COLOR << "That's not your piece!\n" << RESET;
            return false;
        }
        if (!piece->isValidMove(startX, startY, endX, endY, board.getSquares())) {
            cout << HEADING_COLOR << "Invalid move for this piece!\n" << RESET;
            return false;
        }
        if (!testMoveDoesNotLeaveInCheck(startX, startY, endX, endY, currentPlayer)) {
            cout << HEADING_COLOR << "Move leaves your king in check!\n" << RESET;
            return false;
        }
        if (board.getPiece(endX, endY)->getColor() == currentPlayer) {
            cout << HEADING_COLOR << "You cannot capture your own piece!\n" << RESET;
            return false;
        }

        Piece* captured = board.getPiece(endX, endY);
        delete captured;
        board.setPieceNoDelete(endX, endY, piece);
        board.setPieceNoDelete(startX, startY, new EmptySquare());
        piece->setHasMoved(true);

        Color opponent = (currentPlayer == WHITE) ? BLACK : WHITE;
        if (isInCheck(opponent)) {
            if (!hasLegalMoves(opponent)) {
                cout << "\n*** CHECKMATE! ***\n";
                cout << (currentPlayer == WHITE ? "PINK" : "GREEN") << " wins!\n";
                gameOver = true;
            }
            else {
                cout << HEADING_COLOR << "CHECK!\n" << RESET;
            }
        }

        return true;
    }

    void switchPlayer() {
        currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
    }

    void play() {
        string from, to;

        cout << LINE_COLOR << "\n===========================================\n" << RESET;
        cout << HEADING_COLOR "       CHESS GAME - LET'S PLAY!\n" << RESET;
        cout << LINE_COLOR << "===========================================\n" << RESET;
        cout << "  " << WHITE_PIECE << "PINK letters" << RESET << " = PINK player (K Q R B N P)\n";
        cout << "  " << BLACK_PIECE << "GREEN  letters" << RESET << " = GREEN player  (k q r b n p)\n";
        cout << HEADING_COLOR << "Enter moves in format: e2 e4\n" << RESET;
        cout << HEADING_COLOR << "Type 'quit' to exit\n" << RESET;
        cout << LOWER_LINE "===========================================\n\n" << RESET;

        while (!gameOver) {
            board.display();

            if (currentPlayer == WHITE)
                cout << "Pink's turn\n";
            else
                cout << "Green's turn\n";

            cout << "Enter move (from): ";
            cin >> from;

            if (from == "quit") {
                cout << "Game ended by player.\n";
                break;
            }

            cout << "Enter move (to): ";
            cin >> to;

            int startX, startY, endX, endY;

            if (!parsePosition(from, startX, startY)) {
                cout << "Invalid starting position!\n";
                continue;
            }
            if (!parsePosition(to, endX, endY)) {
                cout << "Invalid ending position!\n";
                continue;
            }

            if (makeMove(startX, startY, endX, endY))
                switchPlayer();
        }

        cout << "\nThanks for playing!\n";
    }
};

