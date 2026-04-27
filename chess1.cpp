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


//Bishop Piece (Derived from Piece)
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

//Game Class (Main Class)
class Game {
private:
    Board board;
    Color currentPlayer;
    bool gameOver;
    EmptySquare emptyPlaceholder;   // reusable placeholder for simulations

    //Find King  (step 1)
    bool findKing(Color color, int& kingX, int& kingY) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece* piece = board.getPiece(i, j);
                if (piece->getType() == KING && piece->getColor() == color) {
                    kingX = i;
                    kingY = j;
                    return true;
                }
            }
        }
        return false;
    }

    //Check if square is under attack
    bool isSquareUnderAttack(int x, int y, Color byColor) {
        for (int i = 0; i < 8; i++) {
            for (int j = 0; j < 8; j++) {
                Piece* piece = board.getPiece(i, j);

                if (piece == nullptr || piece->getType() == EMPTY || piece->getColor() != byColor)
                    continue;

                if (piece->isValidMove(i, j, x, y, board.getSquares()))
                    return true;
            }
        }
        return false;
    }

    // Check if king is in check
    bool isInCheck(Color color) {
        int kingX, kingY;
        if (!findKing(color, kingX, kingY)) return false;

        Color opponent = (color == WHITE) ? BLACK : WHITE;
        return isSquareUnderAttack(kingX, kingY, opponent);
    }

    // Simulate a move, test if the given color's king is safe, then undo — zero heap allocation
    bool testMoveDoesNotLeaveInCheck(int startX, int startY, int endX, int endY, Color color) {
        Piece* piece = board.getPiece(startX, startY);
        Piece* captured = board.getPiece(endX, endY);

        // simulate: swap pointers only, use stack-allocated placeholder for vacated square
        board.setPieceNoDelete(endX, endY, piece);
        board.setPieceNoDelete(startX, startY, &emptyPlaceholder);

        bool safe = !isInCheck(color);

        // undo: restore original pointers exactly
        board.setPieceNoDelete(startX, startY, piece);
        board.setPieceNoDelete(endX, endY, captured);

        return safe;
    }

    // Check if player has legal moves
    bool hasLegalMoves(Color color) {
        for (int startX = 0; startX < 8; startX++) {
            for (int startY = 0; startY < 8; startY++) {

                Piece* piece = board.getPiece(startX, startY);

                if (piece == nullptr || piece->getType() == EMPTY || piece->getColor() != color)
                    continue;

                for (int endX = 0; endX < 8; endX++) {
                    for (int endY = 0; endY < 8; endY++) {

                        if (!piece->isValidMove(startX, startY, endX, endY, board.getSquares()))
                            continue;

                        // simulate move and check if king is still safe
                        if (testMoveDoesNotLeaveInCheck(startX, startY, endX, endY, color))
                            return true;
                    }
                }
            }
        }
        return false;
    }

public:
    Game() : currentPlayer(WHITE), gameOver(false) {
        board.initializeBoard();
    }

    // Convert chess notation
    bool parsePosition(string pos, int& row, int& col) {
        if (pos.length() != 2) return false;

        col = pos[0] - 'a';
        row = 8 - (pos[1] - '0');

        if (row < 0 || row >= 8 || col < 0 || col >= 8)
            return false;

        return true;
    }

    // Make move
    bool makeMove(int startX, int startY, int endX, int endY) {
        Piece* piece = board.getPiece(startX, startY);

        if (piece->getType() == EMPTY) {
            cout << "No piece at starting position!\n";
            return false;
        }

        if (piece->getColor() != currentPlayer) {
            cout << "That's not your piece!\n";
            return false;
        }

        if (!piece->isValidMove(startX, startY, endX, endY, board.getSquares())) {
            cout << "Invalid move for this piece!\n";
            return false;
        }

        //  simulate move first
        if (!testMoveDoesNotLeaveInCheck(startX, startY, endX, endY, currentPlayer)) {
            //checks KING
            cout << "Move leaves your king in check!\n";
            return false;
        }

        if (board.getPiece(endX, endY)->getColor() == currentPlayer) {
            cout << "You cannot capture your own color\n";
            return false;
        }

        // REAL MOVE — get captured piece, delete it, move piece, place new EmptySquare at source
        Piece* captured = board.getPiece(endX, endY);
        delete captured;
        board.setPieceNoDelete(endX, endY, piece);
        board.setPieceNoDelete(startX, startY, new EmptySquare());

        piece->setHasMoved(true);

        //check opponent state
        Color opponent = (currentPlayer == WHITE) ? BLACK : WHITE;

        if (isInCheck(opponent)) {
            if (!hasLegalMoves(opponent)) {
                cout << "\n*** CHECKMATE! ***\n";
                cout << (currentPlayer == WHITE ? "White" : "Black") << " wins!\n";
                gameOver = true;
            }
            else {
                cout << "CHECK!\n";
            }
        }

        return true;
    }

    // Switch turns
    void switchPlayer() {
        currentPlayer = (currentPlayer == WHITE) ? BLACK : WHITE;
    }

    // Main game loop
    void play() {
        string from, to;

        cout << "\n===========================================\n";
        cout << "       CHESS GAME - LET'S PLAY!\n";
        cout << "===========================================\n";
        cout << "Enter moves in format: e2 e4\n";
        cout << "Type 'quit' to exit\n";
        cout << "===========================================\n\n";

        while (!gameOver) {
            board.display();

            cout << (currentPlayer == WHITE ? "White" : "Black") << "'s turn\n";

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

            if (makeMove(startX, startY, endX, endY)) {
                switchPlayer();
            }
        }

        cout << "\nThanks for playing!\n";
    }
};

int main() {
    Game g;
    g.play();
    return 0;
}
