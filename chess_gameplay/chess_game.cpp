
#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <optional>
#include <string>
using namespace std;

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
    string      whiteName;
    string      blackName;
    string      statusMessage;   // last message, shown in the GUI status bar instead of cout

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
        whiteName = "Pink";
        blackName = "Matcha";
        statusMessage = "Pink's turn";
    }

    string nameFor(Color color) { return (color == WHITE) ? whiteName : blackName; }
    Color  getCurrentPlayer() const { return currentPlayer; }
    bool   isGameOver() const { return gameOver; }
    Board& getBoard() { return board; }
    string getStatusMessage() const { return statusMessage; }

    void setNames(const string& white, const string& black) {
        whiteName = white.empty() ? "Pink" : white;
        blackName = black.empty() ? "Matcha" : black;
        statusMessage = whiteName + "'s turn";
    }

    bool isKingInCheck(Color color) { return isInCheck(color); }

    // Returns the list of legal destination squares for the piece at (x,y).
    // Used by the GUI to draw move-dots after a piece is clicked.
    void getLegalMoves(int x, int y, bool legal[8][8]) {
        for (int i = 0; i < 8; i++)
            for (int j = 0; j < 8; j++)
                legal[i][j] = false;

        Piece* p = board.getPiece(x, y);
        if (!p || p->getType() == EMPTY || p->getColor() != currentPlayer) return;

        for (int endX = 0; endX < 8; endX++)
            for (int endY = 0; endY < 8; endY++) {
                if (!p->isValidMove(x, y, endX, endY, board.getSquares())) continue;
                if (board.getPiece(endX, endY)->getColor() == currentPlayer) continue;
                if (!testMoveDoesNotLeaveInCheck(x, y, endX, endY, currentPlayer)) continue;
                legal[endX][endY] = true;
            }
    }

    bool makeMove(int startX, int startY, int endX, int endY) {
        Piece* piece = board.getPiece(startX, startY);

        if (piece->getType() == EMPTY) {
            statusMessage = "No piece there!";
            return false;
        }
        if (piece->getColor() != currentPlayer) {
            statusMessage = "That's not your piece!";
            return false;
        }
        if (!piece->isValidMove(startX, startY, endX, endY, board.getSquares())) {
            statusMessage = "Invalid move for this piece!";
            return false;
        }
        if (!testMoveDoesNotLeaveInCheck(startX, startY, endX, endY, currentPlayer)) {
            statusMessage = "That move leaves your king in check!";
            return false;
        }
        if (board.getPiece(endX, endY)->getColor() == currentPlayer) {
            statusMessage = "You can't capture your own piece!";
            return false;
        }

        Piece* captured = board.getPiece(endX, endY);
        delete captured;
        board.setPieceNoDelete(endX, endY, piece);
        board.setPieceNoDelete(startX, startY, new EmptySquare());
        piece->setHasMoved(true);

        Color opponent = (currentPlayer == WHITE) ? BLACK : WHITE;
        bool opponentInCheck = isInCheck(opponent);

        if (opponentInCheck && !hasLegalMoves(opponent)) {
            statusMessage = "CHECKMATE! " + nameFor(currentPlayer) + " wins!";
            gameOver = true;
            return true;
        }

        currentPlayer = opponent;
        statusMessage = opponentInCheck
            ? (nameFor(currentPlayer) + " is in CHECK!")
            : (nameFor(currentPlayer) + "'s turn");

        return true;
    }
};

// ---------------------------------------------------------------------------
// SFML rendering layer
// ---------------------------------------------------------------------------

// Strawberry-matcha palette
namespace Theme {
    const sf::Color cream(0xFF, 0xF6, 0xF3);          // window background
    const sf::Color lightSquare(0xFC, 0xE4, 0xE8);    // strawberry milk
    const sf::Color darkSquare(0xA9, 0xC1, 0xA0);      // matcha leaf
    const sf::Color boardBorder(0xE8, 0xB4, 0xBC);     // dusty strawberry frame
    const sf::Color whitePiece(0xD6, 0x33, 0x6C);      // strawberry jam
    const sf::Color blackPiece(0x3F, 0x5D, 0x38);      // dark matcha
    const sf::Color selectHighlight(0xFF, 0xD9, 0xE0, 230); // selected square glow
    const sf::Color legalDot(0x7A, 0x9A, 0x6E, 200);   // muted matcha dot
    const sf::Color captureRing(0xE8, 0x55, 0x7A, 220); // warm pink ring on capturable squares
    const sf::Color checkGlow(0xE8, 0x55, 0x7A, 160);   // king-in-check highlight
    const sf::Color statusText(0x6B, 0x4F, 0x4F);       // cocoa
    const sf::Color panelBg(0xFF, 0xEC, 0xF0);          // soft panel behind status text
}

const int   SQUARE_SIZE = 80;
const int   BOARD_MARGIN = 40;
const int   BOARD_PIXELS = SQUARE_SIZE * 8;
const int   WINDOW_W = BOARD_PIXELS + BOARD_MARGIN * 2;
const int   WINDOW_H = BOARD_PIXELS + BOARD_MARGIN * 2 + 90; // extra strip for status bar

// Returns the Unicode codepoint for a piece's chess symbol (white-style glyph; we recolor via fill).
char32_t codepointFor(PieceType type) {
    switch (type) {
    case KING:   return 0x2654;
    case QUEEN:  return 0x2655;
    case ROOK:   return 0x2656;
    case BISHOP: return 0x2657;
    case KNIGHT: return 0x2658;
    case PAWN:   return 0x2659;
    default:     return U' ';
    }
}

int main() {
    Game game;

    sf::Font font;
    bool fontLoaded = font.openFromFile("C:/Windows/Fonts/seguisym.ttf")
        || font.openFromFile("C:/Windows/Fonts/segoeui.ttf")
        || font.openFromFile("segoeui.ttf")
        || font.openFromFile("/usr/share/fonts/truetype/segoeui/segoeui.ttf");

    if (!fontLoaded) {
        cerr << "Could not load a font with chess glyphs. Place DejaVuSans.ttf next to the .exe.\n";
        return 1;
    }

    sf::RenderWindow window(sf::VideoMode({ static_cast<unsigned int>(WINDOW_W), static_cast<unsigned int>(WINDOW_H) }),
        "Chess - Strawberry Matcha");
    window.setFramerateLimit(60);

    // ---------------------------------------------------------------
    // Name entry screen
    // ---------------------------------------------------------------
    {
        string whiteInput, blackInput;
        int focusedBox = 0; // 0 = white box, 1 = black box, -1 = none

        const float boxW = 360.0f, boxH = 50.0f;
        sf::Vector2f whiteBoxPos{ (WINDOW_W - boxW) / 2.0f, WINDOW_H / 2.0f - 70.0f };
        sf::Vector2f blackBoxPos{ (WINDOW_W - boxW) / 2.0f, WINDOW_H / 2.0f + 10.0f };
        sf::FloatRect whiteBoxRect({ whiteBoxPos.x, whiteBoxPos.y }, { boxW, boxH });
        sf::FloatRect blackBoxRect({ blackBoxPos.x, blackBoxPos.y }, { boxW, boxH });

        const float btnW = 200.0f, btnH = 54.0f;
        sf::Vector2f startBtnPos{ (WINDOW_W - btnW) / 2.0f, WINDOW_H / 2.0f + 100.0f };
        sf::FloatRect startBtnRect({ startBtnPos.x, startBtnPos.y }, { btnW, btnH });

        sf::Text title(font, "Strawberry-Match Chess!!", 30);
        title.setStyle(sf::Text::Bold);
        title.setFillColor(Theme::statusText);
        {
            sf::FloatRect tb = title.getLocalBounds();
            title.setOrigin({ tb.position.x + tb.size.x / 2.0f, tb.position.y });
            title.setPosition({ WINDOW_W / 2.0f, WINDOW_H / 2.0f - 170.0f });
        }

        sf::Text whiteLabel(font, "Pink player's name:", 18);
        whiteLabel.setFillColor(Theme::whitePiece);
        whiteLabel.setStyle(sf::Text::Bold);
        whiteLabel.setPosition({ whiteBoxPos.x, whiteBoxPos.y - 26.0f });

        sf::Text blackLabel(font, "Matcha player's name:", 18);
        blackLabel.setFillColor(Theme::blackPiece);
        blackLabel.setStyle(sf::Text::Bold);
        blackLabel.setPosition({ blackBoxPos.x, blackBoxPos.y - 26.0f });

        sf::Text startLabel(font, "Start Game", 22);
        startLabel.setStyle(sf::Text::Bold);
        startLabel.setFillColor(Theme::cream);

        bool namesConfirmed = false;

        while (window.isOpen() && !namesConfirmed) {
            while (const std::optional<sf::Event> event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }

                if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouse->button == sf::Mouse::Button::Left) {
                        sf::Vector2f click(static_cast<float>(mouse->position.x),
                            static_cast<float>(mouse->position.y));

                        if (whiteBoxRect.contains(click)) focusedBox = 0;
                        else if (blackBoxRect.contains(click)) focusedBox = 1;
                        else if (startBtnRect.contains(click)) namesConfirmed = true;
                        else focusedBox = -1;
                    }
                }

                if (const auto* text = event->getIf<sf::Event::TextEntered>()) {
                    if (focusedBox != -1) {
                        string& target = (focusedBox == 0) ? whiteInput : blackInput;
                        char32_t unicode = text->unicode;

                        if (unicode == 8) { // backspace
                            if (!target.empty()) target.pop_back();
                        }
                        else if (unicode == 13) { // enter -- move to next field, or confirm on last
                            if (focusedBox == 0) focusedBox = 1;
                            else namesConfirmed = true;
                        }
                        else if (unicode >= 32 && unicode < 127 && target.size() < 16) {
                            target += static_cast<char>(unicode);
                        }
                    }
                }
            }

            window.clear(Theme::cream);
            window.draw(title);
            window.draw(whiteLabel);
            window.draw(blackLabel);

            // Text boxes
            for (int i = 0; i < 2; i++) {
                sf::Vector2f pos = (i == 0) ? whiteBoxPos : blackBoxPos;
                sf::RectangleShape box({ boxW, boxH });
                box.setPosition(pos);
                box.setFillColor(Theme::panelBg);
                box.setOutlineThickness(3.0f);
                box.setOutlineColor(focusedBox == i ? Theme::captureRing : Theme::boardBorder);
                window.draw(box);

                string& value = (i == 0) ? whiteInput : blackInput;
                string display = value + (focusedBox == i ? "|" : "");
                sf::Text fieldText(font, display, 22);
                fieldText.setFillColor(Theme::statusText);
                fieldText.setPosition({ pos.x + 12.0f, pos.y + 10.0f });
                window.draw(fieldText);
            }

            // Start button
            sf::RectangleShape startBtn({ btnW, btnH });
            startBtn.setPosition(startBtnPos);
            startBtn.setFillColor(Theme::whitePiece);
            window.draw(startBtn);

            sf::FloatRect lb = startLabel.getLocalBounds();
            startLabel.setOrigin({ lb.position.x + lb.size.x / 2.0f, lb.position.y + lb.size.y / 2.0f });
            startLabel.setPosition({ startBtnPos.x + btnW / 2.0f, startBtnPos.y + btnH / 2.0f });
            window.draw(startLabel);

            window.display();
        }

        if (!window.isOpen()) return 0; // closed during name entry

        game.setNames(whiteInput, blackInput);
    }

    // ---------------------------------------------------------------
    // Main game
    // ---------------------------------------------------------------
    int selectedX = -1, selectedY = -1; // -1 = nothing selected
    bool legalMoves[8][8] = {};

    sf::Text statusText(font, "", 24);
    statusText.setFillColor(Theme::statusText);
    statusText.setStyle(sf::Text::Bold);

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }

            if (const auto* mouse = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouse->button == sf::Mouse::Button::Left && !game.isGameOver()) {
                    int mx = mouse->position.x;
                    int my = mouse->position.y;

                    int col = (mx - BOARD_MARGIN) / SQUARE_SIZE;
                    int row = (my - BOARD_MARGIN) / SQUARE_SIZE;

                    bool insideBoard = (mx >= BOARD_MARGIN && mx < BOARD_MARGIN + BOARD_PIXELS &&
                        my >= BOARD_MARGIN && my < BOARD_MARGIN + BOARD_PIXELS);

                    if (insideBoard) {
                        if (selectedX == -1) {
                            // First click: try to select a piece belonging to the current player.
                            Piece* p = game.getBoard().getPiece(row, col);
                            if (p && p->getType() != EMPTY && p->getColor() == game.getCurrentPlayer()) {
                                selectedX = row;
                                selectedY = col;
                                game.getLegalMoves(row, col, legalMoves);
                            }
                        }
                        else {
                            // Second click: attempt the move (clicking the same square deselects).
                            if (row == selectedX && col == selectedY) {
                                selectedX = -1;
                                selectedY = -1;
                            }
                            else {
                                bool moved = game.makeMove(selectedX, selectedY, row, col);
                                selectedX = -1;
                                selectedY = -1;
                                if (!moved) {
                                    // If the click happens to be a different one of the player's
                                    // own pieces, select that one instead of just failing silently.
                                    Piece* p = game.getBoard().getPiece(row, col);
                                    if (p && p->getType() != EMPTY && p->getColor() == game.getCurrentPlayer()) {
                                        selectedX = row;
                                        selectedY = col;
                                        game.getLegalMoves(row, col, legalMoves);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        // --- Draw ---
        window.clear(Theme::cream);

        // Board frame
        sf::RectangleShape frame({ static_cast<float>(BOARD_PIXELS + 12), static_cast<float>(BOARD_PIXELS + 12) });
        frame.setPosition({ static_cast<float>(BOARD_MARGIN - 6), static_cast<float>(BOARD_MARGIN - 6) });
        frame.setFillColor(Theme::boardBorder);
        window.draw(frame);

        // Squares
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                sf::RectangleShape square({ static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE) });
                square.setPosition({ static_cast<float>(BOARD_MARGIN + c * SQUARE_SIZE),
                                     static_cast<float>(BOARD_MARGIN + r * SQUARE_SIZE) });
                bool isLight = ((r + c) % 2 == 0);
                square.setFillColor(isLight ? Theme::lightSquare : Theme::darkSquare);
                window.draw(square);
            }
        }

        // Check highlight (glow behind the king that's in check)
        for (Color side : {WHITE, BLACK}) {
            if (game.isKingInCheck(side)) {
                for (int r = 0; r < 8; r++)
                    for (int c = 0; c < 8; c++) {
                        Piece* p = game.getBoard().getPiece(r, c);
                        if (p->getType() == KING && p->getColor() == side) {
                            sf::RectangleShape glow({ static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE) });
                            glow.setPosition({ static_cast<float>(BOARD_MARGIN + c * SQUARE_SIZE),
                                               static_cast<float>(BOARD_MARGIN + r * SQUARE_SIZE) });
                            glow.setFillColor(Theme::checkGlow);
                            window.draw(glow);
                        }
                    }
            }
        }

        // Selected square highlight
        if (selectedX != -1) {
            sf::RectangleShape sel({ static_cast<float>(SQUARE_SIZE), static_cast<float>(SQUARE_SIZE) });
            sel.setPosition({ static_cast<float>(BOARD_MARGIN + selectedY * SQUARE_SIZE),
                              static_cast<float>(BOARD_MARGIN + selectedX * SQUARE_SIZE) });
            sel.setFillColor(Theme::selectHighlight);
            window.draw(sel);

            // Legal-move indicators: dot for empty squares, ring for capturable squares
            for (int r = 0; r < 8; r++) {
                for (int c = 0; c < 8; c++) {
                    if (!legalMoves[r][c]) continue;
                    float cx = BOARD_MARGIN + c * SQUARE_SIZE + SQUARE_SIZE / 2.0f;
                    float cy = BOARD_MARGIN + r * SQUARE_SIZE + SQUARE_SIZE / 2.0f;

                    bool isCapture = game.getBoard().getPiece(r, c)->getType() != EMPTY;

                    if (isCapture) {
                        sf::CircleShape ring(SQUARE_SIZE / 2.0f - 4.0f);
                        ring.setOrigin({ ring.getRadius(), ring.getRadius() });
                        ring.setPosition({ cx, cy });
                        ring.setFillColor(sf::Color::Transparent);
                        ring.setOutlineColor(Theme::captureRing);
                        ring.setOutlineThickness(4.0f);
                        window.draw(ring);
                    }
                    else {
                        sf::CircleShape dot(10.0f);
                        dot.setOrigin({ 10.0f, 10.0f });
                        dot.setPosition({ cx, cy });
                        dot.setFillColor(Theme::legalDot);
                        window.draw(dot);
                    }
                }
            }
        }

        // Pieces (Unicode glyphs colored per side)
        for (int r = 0; r < 8; r++) {
            for (int c = 0; c < 8; c++) {
                Piece* p = game.getBoard().getPiece(r, c);
                if (!p || p->getType() == EMPTY) continue;

                char32_t cp = codepointFor(p->getType());
                sf::String glyphStr(cp);

                sf::Text glyph(font, glyphStr, 56);
                glyph.setFillColor(p->getColor() == WHITE ? Theme::whitePiece : Theme::blackPiece);

                sf::FloatRect bounds = glyph.getLocalBounds();
                glyph.setOrigin({ bounds.position.x + bounds.size.x / 2.0f,
                                  bounds.position.y + bounds.size.y / 2.0f });

                float cx = BOARD_MARGIN + c * SQUARE_SIZE + SQUARE_SIZE / 2.0f;
                float cy = BOARD_MARGIN + r * SQUARE_SIZE + SQUARE_SIZE / 2.0f;
                glyph.setPosition({ cx, cy });

                window.draw(glyph);
            }
        }

        // Status bar panel
        sf::RectangleShape statusPanel({ static_cast<float>(WINDOW_W - BOARD_MARGIN * 2 + 12), 70.0f });
        statusPanel.setPosition({ static_cast<float>(BOARD_MARGIN - 6),
                                  static_cast<float>(BOARD_MARGIN * 2 + BOARD_PIXELS + 4) });
        statusPanel.setFillColor(Theme::panelBg);
        window.draw(statusPanel);

        statusText.setString(game.getStatusMessage());
        sf::FloatRect tb = statusText.getLocalBounds();
        statusText.setOrigin({ tb.position.x, tb.position.y });
        statusText.setPosition({ static_cast<float>(BOARD_MARGIN + 10),
                                 static_cast<float>(BOARD_MARGIN * 2 + BOARD_PIXELS + 24) });
        window.draw(statusText);

        window.display();
    }

    return 0;
}