#include "Pieces.h"

Piece::Piece() {
	Coordinate::Coordinate();
	color = {};
	dead = false;
	chosen = false; 
	type = {};
	tableMove = {};
	id = -1;
}
Piece::Piece(const Piece& piece) {
	this->position = piece.position;
	this->color = piece.color;
	this->dead = piece.dead;
	this->chosen = piece.chosen;
	this->type = piece.type;
	this->id = piece.id;
	this->tableMove = piece.tableMove;
}
Piece::Piece(const Coordinate& position, Color color) {
	this->position = position;
	this->color = color;
}
Piece::~Piece() {
	tableMove.clear();
}

void Piece::setPosition(Coordinate position) {
	this->position = position;
}
void Piece::setColor(Color color) {
	this->color = color;
}
void Piece::setDead(bool dead) {
	this->dead = dead;
}
void Piece::setChosen(bool chosen) {
	this->chosen = chosen;
}
void Piece::setID(int id) {
	this->id = id;
}

Coordinate Piece::getPosition() const {
	return this->position;
}
Color Piece::getColor() const {
	return this->color;
}
bool Piece::getDead() const {
	return this->dead;
}
bool Piece::getChosen() const {
	return this->chosen;
}
PieceType Piece::getType() const {
	return this->type;
}
int Piece::getID() {
	return this->id;
}

void Piece::getMovesWhenCheckmated(std::vector<Coordinate>& moves, std::vector<std::vector<Piece*>> board) {
	if (this->getChosen()) {
		King* king = nullptr;
		for (auto& v : board) {
			bool flag = false;
			for (auto& u : v) {
				if (u && u->getColor() == this->color && u->getType() == PieceType::King) {
					king = (King*)u;
					flag = true;
				}
			}
			if (flag) break;
		}
		for (int i = 0; i < moves.size(); i++) {
			Piece* temp = nullptr;
			Coordinate move = moves[i];
			temp = board[moves[i].getX()][moves[i].getY()];
			board[this->position.getX()][this->position.getY()] = nullptr;
			board[moves[i].getX()][moves[i].getY()] = this;
			if (king && king->checkmate(king->getPosition(), board)) {
				moves.erase(moves.begin() + i);
				i--;
			}
			board[move.getX()][move.getY()] = temp;
			board[this->position.getX()][this->position.getY()] = this;
		}
	}
}

//--------------------------------------------------------------------------------------------------
King::King() {
	ableCastling = true;
}
King::King(const King& king) : Piece(king) {
	this->type = PieceType::King;
	this->ableCastling = king.ableCastling;
}
King::King(const Coordinate& position, Color color) : Piece(position, color) {
	this->type = PieceType::King;
	this->dead = false;
	this->chosen = false;
	this->ableCastling = true;
}
King::~King() {

}
Piece* King::move(const Coordinate& c, std::vector<std::vector<Piece*>> board) {
	if (ableCastling) {
		std::vector<Coordinate> moves = getCastlingMove(board);
		if (moves.size() != 0) {
			for (auto& pos : moves) {
				if (pos == c) {
					if (this->position.getX() > c.getX()) {
						board[0][c.getY()]->move(Coordinate(2, c.getY()), board);
					}
					else if (this->position.getX() < c.getX()) {
						board[7][c.getY()]->move(Coordinate(4, c.getY()), board);
					}
					this->setPosition(c);
					ableCastling = false;

					return nullptr;
				}
			}
		}
		ableCastling = false;
	}
	if (!board[c.getX()][c.getY()]) {
		this->setPosition(c);

		return nullptr;
	}

	if (this->getColor() != board[c.getX()][c.getY()]->getColor()) {
		this->setPosition(c);
		board[c.getX()][c.getY()]->setDead(true);

		return board[c.getX()][c.getY()];
	}
	return nullptr;
}
std::vector<std::vector<Coordinate>> King::getPossibleMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Coordinate> moves;
	std::vector<std::vector<Coordinate>> result;
	result.resize(2);
	Coordinate tmp(this->getPosition().getX(), this->getPosition().getY());
	if (ableCastling && this->getChosen()) {
		std::vector<Coordinate> tmpMoves = this->getCastlingMove(board);
		for (auto& i : tmpMoves) {
			moves.push_back(i);
		}
	}
	//x - 1, y - 1
	int X = tmp.getX() - 1;
	int Y = tmp.getY() - 1;
	if (X >= 0 && Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x + 1, y - 1
	X = tmp.getX() + 1;
	Y = tmp.getY() - 1;
	if (X <= _BOARD_HEIGHT && Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x - 1, y + 1
	X = tmp.getX() - 1;
	Y = tmp.getY() + 1;
	if (X >= 0 && Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x + 1, y + 1
	X = tmp.getX() + 1;
	Y = tmp.getY() + 1;
	if (X <= _BOARD_HEIGHT && Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x + 1
	X = tmp.getX() + 1;
	Y = tmp.getY();
	if (X <= _BOARD_HEIGHT) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x - 1
	X = tmp.getX() - 1;
	Y = tmp.getY();
	if (X >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//y + 1
	X = tmp.getX();
	Y = tmp.getY() + 1;
	if (Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//y - 1
	X = tmp.getX();
	Y = tmp.getY() - 1;
	if (Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	// case: checkmate
	if (this->getChosen()) {
		for (int i = 0; i < moves.size(); i++) {
			if (checkmate(moves[i], board)) {
				moves.erase(moves.begin() + i);
				i--;
			}
		}
	}

	for (auto& e : moves) {
		if (!board[e.getX()][e.getY()]) {
			result[0].push_back(e);
		}
		else result[1].push_back(e);
	}

	return result;
}
Piece* King::clone() const{
	return new King(*this);
}
std::vector<Coordinate> King::getCastlingMove(std::vector<std::vector<Piece*>> board) {
	/*
	Rule 1. You cannot castle if you have moved your king(or the rook)!
	Rule 2. You are not allowed to castle out of check!
	Rule 3. You are not allowed to castle through check!
	Rule 4. No pieces can be between the king and the rook
	*/
	std::vector<Coordinate> moves;
	std::vector<Rook*> rooks;
	for (int i = 0; i <= 7; i += 7) {
		Rook* temp = dynamic_cast<Rook*>(board[i][this->position.getY()]);
		if (temp) rooks.push_back(temp);
	}
	// Violate Rule 1
	if (!ableCastling || rooks.size() == 0) {
		ableCastling = false; 
		return moves;
	}
	// Violate Rule 2
	if (checkmate(this->position, board)) return moves;

	for (int i = 0; i < 2; i++) {
		// Violate rule 1
		if (!rooks[i]->getFirstMove()) continue;

		bool flag = false;
		int coeff = pow(-1, i + 1);
		int tempY = this->position.getY(), tempX = this->position.getX();
		while (true) {
			tempX = tempX + coeff;
			if (tempX <= 0 || tempX >= 7) break;

			if (board[tempX][tempY]) {
				flag = true;
				break;
			}
		}
		// Violate rule 4
		if (flag) continue;

		// Violate rule 3
		if (checkmate(Coordinate(this->position.getX() + coeff * 2, this->position.getY()), board)) continue;

		moves.push_back(Coordinate(this->position.getX() + coeff * 2, this->position.getY()));
	}

	return moves;
}
bool King::checkmate(const Coordinate& positionOfKing, std::vector<std::vector<Piece*>> board) {
	std::vector<Coordinate> possibleMoves;

	for (auto& row : board) {
		for (Piece* piece : row) {
			if (!piece) {
				continue;
			}
			
			if (this->getColor() != piece->getColor()) {
				std::vector<std::vector<Coordinate>> temp = piece->getPossibleMoves(board);
				possibleMoves.reserve(temp[0].size() + temp[1].size());
				std::copy(temp[0].begin(), temp[0].end(), std::back_inserter(possibleMoves));
				std::copy(temp[1].begin(), temp[1].end(), std::back_inserter(possibleMoves));

			}
		}
	}
	for (auto& c : possibleMoves) {
		if (c.getX() == positionOfKing.getX() && c.getY() == positionOfKing.getY()) {
			return true;
		}
	}
	return false;
}

//King& King::operator=(const Piece* piece) {
//	if (this == piece) return *this;
//	King* temp = dynamic_cast<King*>(piece);
//	this->position = temp->position;
//	this->color = temp->color;
//	this->dead = temp->dead;
//	this->chosen = temp->chosen;
//	this->imagePath = temp->imagePath;
//	this->texture = nullptr;
//	this->type = temp.type;
//	this->ableCastling = temp.ableCastling;
//	this->id = piece.id;
//	return *this;
//}

//--------------------------------------------------------------------------------------------------
Queen::Queen() {

}
Queen::Queen(const Queen& queen) : Piece(queen) {
	this->type = PieceType::Queen;
}
Queen::Queen(const Coordinate& position, Color color) : Piece(position, color) {
	this->dead = false;
	this->chosen = false;
	this->type = PieceType::Queen;
}
Queen::~Queen() {

}

Piece* Queen::move(const Coordinate& c, std::vector<std::vector<Piece*>> board) {
	if (!board[c.getX()][c.getY()]) {
		this->setPosition(c);

		return nullptr;
	}
	
	if (this->getColor() != board[c.getX()][c.getY()]->getColor()) {
		this->setPosition(c);
		board[c.getX()][c.getY()]->setDead(true);

		return board[c.getX()][c.getY()];
	}
	return nullptr;
}
std::vector<std::vector<Coordinate>> Queen::getPossibleMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Coordinate> moves;
	std::vector<std::vector<Coordinate>> result;
	result.resize(2);
	Coordinate tmp(this->getPosition().getX(), this->getPosition().getY());
	//x - 1
	int X = tmp.getX() - 1;
	int Y = tmp.getY();
	while (X >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X--;
		}
		else if (this->getColor() !=  board[X][Y]->getColor()) { 
			moves.push_back(Coordinate(X, Y));
			break; 
		}
		else { break; }
	}

	//x + 1
	X = tmp.getX() + 1;
	Y = tmp.getY();
	while (X <= _BOARD_HEIGHT) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}
	
	//y - 1
	X = tmp.getX();
	Y = tmp.getY() - 1;
	while (Y >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			Y--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//y + 1
	X = tmp.getX();
	Y = tmp.getY() + 1;
	while (Y <= _BOARD_WIDTH) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			Y++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x + 1, y + 1
	X = tmp.getX() + 1;
	Y = tmp.getY() + 1;
	while (X <= _BOARD_HEIGHT && Y <= _BOARD_WIDTH) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X++;
			Y++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x - 1, y - 1
	X = tmp.getX() - 1;
	Y = tmp.getY() - 1;
	while (X >= 0 && Y >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X--;
			Y--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x - 1, y + 1
	X = tmp.getX() - 1;
	Y = tmp.getY() + 1;	
	while (X >= 0 && Y <= _BOARD_WIDTH) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X--;
			Y++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x + 1, y - 1
	X = tmp.getX() + 1;
	Y = tmp.getY() - 1;	
	while (X <= _BOARD_HEIGHT && Y >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X++;
			Y--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	// case: checkmate
	this->getMovesWhenCheckmated(moves, board);

	for (auto& e : moves) {
		if (!board[e.getX()][e.getY()]) result[0].push_back(e);
		else result[1].push_back(e);
	}

	return result;
}
Piece* Queen::clone() const{
	return new Queen(*this);
}

//Queen& Queen::operator=(const Queen& piece) {
//	if (this == &piece) return *this;
//
//	this->position = piece.position;
//	this->color = piece.color;
//	this->dead = piece.dead;
//	this->chosen = piece.chosen;
//	this->imagePath = piece.imagePath;
//	this->texture = nullptr;
//	this->type = piece.type;
//	this->id = piece.id;
//
//	return *this;
//}

//--------------------------------------------------------------------------------
Bishop::Bishop() {

}
Bishop::Bishop(const Bishop& bishop) : Piece(bishop) {
	this->type = PieceType::Bishop;
}
Bishop::Bishop(const Coordinate& position, Color color) : Piece(position, color) {
	this->dead = false;
	this->chosen = false;
	this->type = PieceType::Bishop;
}
Bishop::~Bishop() {

}

Piece* Bishop::move(const Coordinate& c, std::vector<std::vector<Piece*>> board) {
	if (!board[c.getX()][c.getY()]) {
		this->setPosition(c);

		return nullptr;
	}
	if (this->getColor() != board[c.getX()][c.getY()]->getColor()) {
		this->setPosition(c);
		board[c.getX()][c.getY()]->setDead(true);

		return board[c.getX()][c.getY()];
	}

	return nullptr;
}
std::vector<std::vector<Coordinate>> Bishop::getPossibleMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Coordinate> moves;
	std::vector<std::vector<Coordinate>> result;
	result.resize(2);
	Coordinate tmp(this->getPosition().getX(), this->getPosition().getY());

	//x + 1, y + 1
	int X = tmp.getX() + 1;
	int Y = tmp.getY() + 1;
	while (X <= _BOARD_HEIGHT && Y <= _BOARD_WIDTH) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X++;
			Y++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x - 1, y - 1
	X = tmp.getX() - 1;
	Y = tmp.getY() - 1;
	while (X >= 0 && Y >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X--;
			Y--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x - 1, y + 1
	X = tmp.getX() - 1;
	Y = tmp.getY() + 1;
	while (X >= 0 && Y <= _BOARD_WIDTH) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X--;
			Y++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x + 1, y - 1
	X = tmp.getX() + 1;
	Y = tmp.getY() - 1;
	while (X <= _BOARD_HEIGHT && Y >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X++;
			Y--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	// case: checkmate
	this->getMovesWhenCheckmated(moves, board);

	for (auto& e : moves) {
		if (!board[e.getX()][e.getY()]) result[0].push_back(e);
		else result[1].push_back(e);
	}

	return result;
}
Piece* Bishop::clone() const{
	return new Bishop(*this);
}

//Bishop& Bishop::operator=(const Bishop& piece) {
//	if (this == &piece) return *this;
//
//	this->position = piece.position;
//	this->color = piece.color;
//	this->dead = piece.dead;
//	this->chosen = piece.chosen;
//	this->imagePath = piece.imagePath;
//	this->texture = nullptr;
//	this->type = piece.type;
//	this->id = piece.id;
//
//	return *this;
//}
//----------------------------------------------------------------------------------
Rook::Rook() {
	firstMove = true;
}
Rook::Rook(const Rook& rook) : Piece(rook) {
	this->type = PieceType::Rook;
	this->firstMove = rook.firstMove;
}
Rook::Rook(const Coordinate& position, Color color) : Piece(position, color) {
	this->dead = false;
	this->chosen = false;
	this->type = PieceType::Rook;
	this->firstMove = true;
}
Rook::~Rook() {

}
void Rook::setFirstMove(bool firstMove) {
	this->firstMove = firstMove;
}
bool Rook::getFirstMove() {
	return firstMove;
}
Piece* Rook::move(const Coordinate& c, std::vector<std::vector<Piece*>> board) {
	if (firstMove) {
		firstMove = false;
	}
	if (!board[c.getX()][c.getY()]) {
		this->setPosition(c);

		return nullptr;
	}
	if (this->getColor() != board[c.getX()][c.getY()]->getColor()) {
		this->setPosition(c);
		board[c.getX()][c.getY()]->setDead(true);

		return board[c.getX()][c.getY()];
	}
	return nullptr;
}
std::vector<std::vector<Coordinate>> Rook::getPossibleMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Coordinate> moves;
	std::vector<std::vector<Coordinate>> result;
	result.resize(2);
	Coordinate tmp(this->getPosition().getX(), this->getPosition().getY());

	//x - 1
	int X = tmp.getX() - 1;
	int Y = tmp.getY();
	while (X >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//x + 1
	X = tmp.getX() + 1;
	Y = tmp.getY();
	while (X <= _BOARD_HEIGHT) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			X++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//y - 1
	X = tmp.getX();
	Y = tmp.getY() - 1;	
	while (Y >= 0) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			Y--;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	//y + 1
	X = tmp.getX();
	Y = tmp.getY() + 1;
	while (Y <= _BOARD_WIDTH) {
		if (!board[X][Y]) {
			moves.push_back(Coordinate(X, Y));
			Y++;
		}
		else if (this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
			break;
		}
		else { break; }
	}

	// case: checkmate
	this->getMovesWhenCheckmated(moves, board);

	for (auto& e : moves) {
		if (!board[e.getX()][e.getY()]) result[0].push_back(e);
		else result[1].push_back(e);
	}

	return result;
}
Piece* Rook::clone() const{
	return new Rook(*this);
}

//Rook& Rook::operator=(const Rook& piece) {
//	if (this == &piece) return *this;
//
//	this->position = piece.position;
//	this->color = piece.color;
//	this->dead = piece.dead;
//	this->chosen = piece.chosen;
//	this->imagePath = piece.imagePath;
//	this->texture = nullptr;
//	this->type = piece.type;
//	this->firstMove = piece.firstMove;
//	this->id = piece.id;
//
//	return *this;
//}
//------------------------------------------------------------------------------
Knight::Knight() {

}
Knight::Knight(const Knight& knight) : Piece(knight) {
	this->type = PieceType::Knight;
}
Knight::Knight(const Coordinate& position, Color color) : Piece(position, color) {
	this->dead = false;
	this->chosen = false;
	this->type = PieceType::Knight;
}
Knight::~Knight() {

}

Piece* Knight::move(const Coordinate& c, std::vector<std::vector<Piece*>> board) {
	if (!board[c.getX()][c.getY()]) {
		this->setPosition(c);

		return nullptr;
	}
	if (this->getColor() != board[c.getX()][c.getY()]->getColor()) {
		this->setPosition(c);
		board[c.getX()][c.getY()]->setDead(true);

		return board[c.getX()][c.getY()];
	}
	return nullptr;
}
std::vector<std::vector<Coordinate>> Knight::getPossibleMoves(std::vector<std::vector<Piece*>> board) {
	std::vector<Coordinate> moves;
	std::vector<std::vector<Coordinate>> result;
	result.resize(2);
	Coordinate tmp(this->getPosition().getX(), this->getPosition().getY());

	//x + 1, y - 2
	int X = tmp.getX() + 1;
	int Y = tmp.getY() - 2;
	if (X <= _BOARD_HEIGHT && Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	//x + 1, y + 2
	X = tmp.getX() + 1;
	Y = tmp.getY() + 2;
	if (X <= _BOARD_HEIGHT && Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	//x - 1, y + 2
	X = tmp.getX() - 1;
	Y = tmp.getY() + 2;

	if (X >= 0 && Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	//x - 1, y - 2
	X = tmp.getX() - 1;
	Y = tmp.getY() - 2;
	if (X >= 0 && Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	//x + 2, y + 1
	X = tmp.getX() + 2;
	Y = tmp.getY() + 1;

	if (X <= _BOARD_HEIGHT && Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	//x - 2, y + 1
	X = tmp.getX() - 2;
	Y = tmp.getY() + 1;
	if (X >= 0 && Y <= _BOARD_WIDTH) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x + 2, y - 1
	X = tmp.getX() + 2;
	Y = tmp.getY() - 1;
	if (X <= _BOARD_HEIGHT && Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}
	//x - 2, y - 1
	X = tmp.getX() - 2;
	Y = tmp.getY() - 1;

	if (X >= 0 && Y >= 0) {
		if (!board[X][Y] || this->getColor() != board[X][Y]->getColor()) {
			moves.push_back(Coordinate(X, Y));
		}
	}

	// case: checkmate
	this->getMovesWhenCheckmated(moves, board);

	for (auto& e : moves) {
		if (!board[e.getX()][e.getY()]) result[0].push_back(e);
		else result[1].push_back(e);
	}

	return result;
}
Piece* Knight::clone() const{
	return new Knight(*this);
}

//Knight& Knight::operator=(const Knight& piece) {
//	if (this == &piece) return *this;
//
//	this->position = piece.position;
//	this->color = piece.color;
//	this->dead = piece.dead;
//	this->chosen = piece.chosen;
//	this->imagePath = piece.imagePath;
//	this->texture = nullptr;
//	this->type = piece.type;
//	this->id = piece.id;
//
//	return *this;
//}

//---------------------------------------------------------------------------
Pawn::Pawn() {
	promotion = nullptr;
	firstMove = true;
}
Pawn::Pawn(const Pawn& pawn) : Piece(pawn) {
	this->type = PieceType::Pawn;
	// TODO - Check again
	if (pawn.promotion) this->promotion = pawn.promotion->clone();
	else this->promotion = nullptr;

	this->firstMove = pawn.firstMove;
}
Pawn::Pawn(const Coordinate& position, Color color) : Piece(position, color) {
	this->type = PieceType::Pawn;
	this->dead = false;
	this->chosen = false;
	promotion = nullptr;
	firstMove = true;
}
Pawn::~Pawn() {
	delete promotion;
}

Piece* Pawn::getPromotion() const {
	return this->promotion;
}
void Pawn::setFirstMove(bool firstMove) {
	this->firstMove = firstMove;
}
bool Pawn::getFirstMove() {
	return this->firstMove;
}
bool Piece::canEnPassant(std::vector<std::vector<Piece*>> board) {
	if (!this) {
		return false;
	}
	if (this->getType() != PieceType::Pawn) {
		return false;
	}
	Coordinate tmp = this->getPosition();
	int X = tmp.getX();
	int Y = tmp.getY();

	if (this->getColor() == Color::White && Y == 3 && this->tableMove[this->tableMove.size() - 1] == 1) {
		if (X > 0) {
			if (board[X - 1][Y] != nullptr && board[X - 1][Y]->getColor() == Color::Black && !board[X][Y - 1]) {
				return true;
			}
		}
		if (X < _BOARD_HEIGHT) {
			if (board[X + 1][Y] != nullptr && board[X + 1][Y]->getColor() == Color::Black && !board[X][Y - 1]) {
				return true;
			}
		}
	}
	if (this->getColor() == Color::Black && Y == 4 && this->tableMove[this->tableMove.size() - 1] == 1) {
		if (X > 0) {
			if (board[X - 1][Y] != nullptr && board[X - 1][Y]->getColor() == Color::White && !board[X][Y - 1]) {
				return true;
			}
		}
		if (X < _BOARD_HEIGHT) {
			if (board[X + 1][Y] != nullptr && board[X + 1][Y]->getColor() == Color::White && !board[X][Y - 1]) {
				return true;
			}
		}
	}
	return false;
}

Piece* Pawn::move(const Coordinate& c, std::vector<std::vector<Piece*>> board) {
	if (this->promotion) {
		Piece* piece = this->promotion->move(c, board);
		this->position = this->promotion->getPosition();

		return piece;
	}

	if (this->getFirstMove()) {
		if (!board[c.getX()][c.getY()]) {
			this->setPosition(c);
			tableMove.push_back(1);
			this->setFirstMove(false);

			return nullptr;
		}
		if (board[c.getX()][c.getY()] != nullptr && this->getColor() != board[c.getX()][c.getY()]->getColor()) {
			board[c.getX()][c.getY()]->setDead(true);
			this->setPosition(c);
			tableMove.push_back(1);
			this->setFirstMove(false);

			return board[c.getX()][c.getY()];
		}
	}
	Coordinate tmp = this->getPosition();
	if (this->getColor() == Color::White) {
		if (tmp.getX() > 0 && tmp.getY() == 4 && tmp.getX() - 1 == c.getX() && c.getY() == 5) {
			if (board[tmp.getX() - 1][tmp.getY()] != nullptr && board[tmp.getX() - 1][tmp.getY()]->getColor() == Color::Black
				&& board[tmp.getX() - 1][tmp.getY()]->canEnPassant(board)) {
				this->tableMove.push_back(this->tableMove[this->tableMove.size() - 1] + 1);
				board[tmp.getX() - 1][tmp.getY()]->setDead(true);
				this->setPosition(c);

				return board[tmp.getX() - 1][tmp.getY()];
			}
		}
		if (tmp.getX() < _BOARD_HEIGHT && tmp.getY() == 4 && tmp.getX() + 1 == c.getX() && c.getY() == 5) {
			if (board[tmp.getX() + 1][tmp.getY()] != nullptr && board[tmp.getX() + 1][tmp.getY()]->getColor() == Color::Black 
				&& board[tmp.getX() + 1][tmp.getY()]->canEnPassant(board)) {
				this->tableMove.push_back(this->tableMove[this->tableMove.size() - 1] + 1);
				board[tmp.getX() + 1][tmp.getY()]->setDead(true);
				this->setPosition(c);

				return board[tmp.getX() + 1][tmp.getY()];
			}
		}
	}
	if (this->getColor() == Color::Black) {
		if (tmp.getX() > 0 && tmp.getY() == 3 && tmp.getX() - 1 == c.getX() && c.getY() == 2) {
			if (board[tmp.getX() - 1][tmp.getY()] != nullptr && board[tmp.getX() - 1][tmp.getY()]->getColor() == Color::White 
				&& board[tmp.getX() - 1][tmp.getY()]->canEnPassant(board)) {
				this->tableMove.push_back(this->tableMove[this->tableMove.size() - 1] + 1);
				board[tmp.getX() - 1][tmp.getY()]->setDead(true);
				this->setPosition(c);

				return board[tmp.getX() - 1][tmp.getY()];
			}
		}
		if (tmp.getX() < _BOARD_HEIGHT && tmp.getY() == 3 && tmp.getX() + 1 == c.getX() && c.getY() == 2) {
			if (board[tmp.getX() + 1][tmp.getY()] != nullptr && board[tmp.getX() + 1][tmp.getY()]->getColor() == Color::White 
				&& board[tmp.getX() + 1][tmp.getY()]->canEnPassant(board)) {
				this->tableMove.push_back(this->tableMove[this->tableMove.size() - 1] + 1);
				board[tmp.getX() + 1][tmp.getY()]->setDead(true);
				this->setPosition(c);

				return board[tmp.getX() + 1][tmp.getY()];
			}
		}
	}
	if (!board[c.getX()][c.getY()]) {
		this->setPosition(c);
		tableMove.push_back(tableMove[tableMove.size() - 1] + 1);

		return nullptr;
	}
	if (this->getColor() != board[c.getX()][c.getY()]->getColor()) {
		board[c.getX()][c.getY()]->setDead(true);
		this->tableMove.push_back(this->tableMove[this->tableMove.size() - 1] + 1);
		this->setPosition(c);

		return board[c.getX()][c.getY()];
	}
	return nullptr;
}
std::vector<std::vector<Coordinate>> Pawn::getPossibleMoves(std::vector<std::vector<Piece*>> board) {
	if (this->promotion) {
		return this->promotion->getPossibleMoves(board);
	}

	std::vector<Coordinate> moves;
	std::vector<std::vector<Coordinate>> result;
	result.resize(2);
	Coordinate tmp(this->getPosition().getX(), this->getPosition().getY());
	int X = tmp.getX();
	int Y = tmp.getY();

	if (this->getColor() == Color::White) {
		if (Y == _BOARD_WIDTH) {
			return result;
		}
		if (this->firstMove) {
			if (!board[X][Y + 1]) {
				moves.push_back(Coordinate(X, Y + 1));
			}
			if (!board[X][Y + 2] && !board[X][Y + 1]) {
				moves.push_back(Coordinate(X, Y + 2));
			}

			if (Y < _BOARD_WIDTH) {
				if (X < _BOARD_HEIGHT) {
					if (board[X + 1][Y + 1] != nullptr && board[X + 1][Y + 1]->getColor() == Color::Black) {
						moves.push_back(Coordinate(X + 1, Y + 1));
					}
				}
				if (X > 0) {
					if (board[X - 1][Y + 1] != nullptr && board[X - 1][Y + 1]->getColor() == Color::Black) {
						moves.push_back(Coordinate(X - 1, Y + 1));
					}
				}			
			}
		}
		else {
			if (Y < _BOARD_WIDTH) {
				if (!board[X][Y + 1]) {
					moves.push_back(Coordinate(X, Y + 1));
				}
			}
			if (Y < _BOARD_WIDTH) {
				if (X < _BOARD_HEIGHT) {
					if (board[X + 1][Y + 1] != nullptr && board[X + 1][Y + 1]->getColor() == Color::Black) {
						moves.push_back(Coordinate(X + 1, Y + 1));
					}
				}
				if (X > 0) {
					if (board[X - 1][Y + 1] != nullptr && board[X - 1][Y + 1]->getColor() == Color::Black) {
						moves.push_back(Coordinate(X - 1, Y + 1));
					}
				}
			}
			//en-passant
			if (Y == 4) {
				if (X > 0) {
					if (board[X - 1][Y] != nullptr && board[X - 1][Y]->getColor() == Color::Black && board[X - 1][Y]->canEnPassant(board)) {
						moves.push_back(Coordinate(X - 1, Y + 1));
					}
				}
				if (X < _BOARD_HEIGHT) {
					if (board[X + 1][Y] != nullptr && board[X + 1][Y]->getColor() == Color::Black && board[X + 1][Y]->canEnPassant(board)) {
						moves.push_back(Coordinate(X + 1, Y + 1));
					}
				}
			}
		}
	}
	else if (this->getColor() == Color::Black) {
		if (Y == 0) {
			return result;
		}
		if (this->firstMove) {
			if (!board[X][Y - 1]) {
				moves.push_back(Coordinate(X, Y - 1));
			}
			if (!board[X][Y - 2] && !board[X][Y - 1]) {
				moves.push_back(Coordinate(X, Y - 2));
			}

			if (Y > 0) {
				if (X < _BOARD_HEIGHT) {
					if (board[X + 1][Y - 1] != nullptr && board[X + 1][Y - 1]->getColor() == Color::White) {
						moves.push_back(Coordinate(X + 1, Y - 1));
					}
				}
				if (X > 0) {
					if (board[X - 1][Y - 1] != nullptr && board[X - 1][Y - 1]->getColor() == Color::White) {
						moves.push_back(Coordinate(X - 1, Y - 1));
					}
				}
			}
		}
		else {
			if (Y > 0) {
				if (!board[X][Y - 1]) {
					moves.push_back(Coordinate(X, Y - 1));
				}
			}

			if (Y > 0) {
				if (X < _BOARD_HEIGHT) {
					if (board[X + 1][Y - 1] != nullptr && board[X + 1][Y - 1]->getColor() == Color::White) {
						moves.push_back(Coordinate(X + 1, Y - 1));
					}
				}
				if (X > 0) {
					if (board[X - 1][Y - 1] != nullptr && board[X - 1][Y - 1]->getColor() == Color::White) {
						moves.push_back(Coordinate(X - 1, Y - 1));
					}
				}
			}
			//en-passant
			if (Y == 3) {
				if (X > 0) {
					if (board[X - 1][Y] != nullptr && board[X - 1][Y]->getColor() == Color::White && board[X - 1][Y]->canEnPassant(board)) {
						moves.push_back(Coordinate(X - 1, Y - 1));
					}
				}
				if (X < _BOARD_HEIGHT) {
					if (board[X + 1][Y] != nullptr && board[X + 1][Y]->getColor() == Color::White && board[X + 1][Y]->canEnPassant(board)) {
						moves.push_back(Coordinate(X + 1, Y - 1));
					}
				}
			}
		}
	}

	// case: checkmate
	this->getMovesWhenCheckmated(moves, board);

	for (auto& e : moves) {
		if (!board[e.getX()][e.getY()]) result[0].push_back(e);
		else result[1].push_back(e);
	}

	return result;
}

Piece* Pawn::clone() const{
	return new Pawn(*this);
}
Piece* Pawn::getPromotion() {
	return this->promotion;
}
void promote(Piece* newPiece, PieceType& type) {
	if (newPiece->getType() == PieceType::Pawn) {
		if (newPiece->getColor() == Color::White) {
			if (newPiece->getPosition().getY() == 7) {
				if (type == PieceType::Queen) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Queen(positionTmp, colorTmp);
				}
				else if (type == PieceType::Rook) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Rook(positionTmp, colorTmp);
				}
				else if (type == PieceType::Knight) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Knight(positionTmp, colorTmp);
				}
				else if (type == PieceType::Bishop) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Bishop(positionTmp, colorTmp);
				}
			}
		}
		else {
			if (newPiece->getPosition().getY() == 0) {
				if (type == PieceType::Queen) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Queen(positionTmp, colorTmp);
				}
				else if (type == PieceType::Rook) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Rook(positionTmp, colorTmp);
				}
				else if (type == PieceType::Knight) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Knight(positionTmp, colorTmp);
				}
				else if (type == PieceType::Bishop) {
					Coordinate positionTmp = newPiece->getPosition();
					Color colorTmp = newPiece->getColor();
					delete newPiece;
					newPiece = new Bishop(positionTmp, colorTmp);
				}
			}
		}
	}
	
}
