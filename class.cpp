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
