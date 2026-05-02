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
