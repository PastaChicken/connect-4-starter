#include "Connect4.h"

// -----------------------------------------------------------------------------
// Connect4.cpp
// -----------------------------------------------------------------------------
// This file contains my implementation of Connect4 With AI and choose between the two
// through wiring up a complete Connect4 implementation using the game engine’s
// Bit / BitHolder grid system.
//
// Rules recap:
//  - Two players place X / O on a 6x7 grid.
//  - Players take turns; Place and piece will drop to lowest square
//  - First player to get to 4 in a row in a line wins
//  - Dunno how you get a tie
//
// Notes about the provided engine types you'll use here:
//  - Bit              : a visual piece (sprite) that belongs to a Player
//  - BitHolder        : a square on the board that can hold at most one Bit
//  - Player           : the engine’s player object (you can ask who owns a Bit)
//  - Game options     : let the mouse know the grid is 6x7 (rowX, rowY)
//  - Helpers you’ll see used: setNumberOfPlayers, getPlayerAt, startGame, etc.
//
// I’ve already fully implemented PieceForPlayer() for you. Please leave that as‑is.
// The rest of the routines are written as “comment-first” TODOs for you to complete.
// -----------------------------------------------------------------------------

ConnectFour::ConnectFour() : Game() {
    _grid = new Grid(7, 6);
    _mustContinueJumping = false;
    _jumpingPiece = nullptr;
    _redPieces = 21;
    _yellowPieces = 21;
}

ConnectFour::~ConnectFour() {
    delete _grid;
}


Bit* ConnectFour::PieceForPlayer(const int playerNumber)
{
    // depending on playerNumber load the "red.png" or the "red.png" graphic
    Bit *bit = new Bit();
    bit->LoadTextureFromFile(playerNumber == 1 ? "red.png" : "yellow.png");
    bit->setOwner(getPlayerAt(playerNumber));
    return bit;
}


void ConnectFour::setUpBoard() {
    setNumberOfPlayers(2);
    _gameOptions.rowX = 7;
    _gameOptions.rowY = 6;

    // Initialize all squares
    _grid->initializeSquares(65, "square.png");
/*
    // Enable only dark squares and place pieces
    _grid->forEachSquare([&](ChessSquare* square, int x, int y) {
        bool isDark = (x + y) % 2 == 1;
        _grid->setEnabled(x, y, isDark);

        if (isDark) {
            if (y < 3) {
                Bit* piece = createPiece(RED_PIECE);
                piece->setPosition(square->getPosition());
                square->setBit(piece);
            } else if (y > 4) {
                Bit* piece = createPiece(YELLOW_PIECE);
                piece->setPosition(square->getPosition());
                square->setBit(piece);
            }
        }
    });
 */
    startGame();
}

Bit* ConnectFour::createPiece(int pieceType) {
    Bit* bit = new Bit();
    bool isRed = (pieceType == RED_PIECE || pieceType == RED_KING);
    bit->LoadTextureFromFile(isRed ? "red.png" : "yellow.png");
    bit->setOwner(getPlayerAt(isRed ? RED_PLAYER : YELLOW_PLAYER));
    bit->setGameTag(pieceType);
    return bit;
}

bool ConnectFour::actionForEmptyHolder(BitHolder &holder) {
    
     if (holder.bit()) {
        return false;
    }
    Bit *bit = createPiece(getCurrentPlayer()->playerNumber() == 0 ? HUMAN_PLAYER : AI_PLAYER);
    if (bit) {
        ImVec2 placement = bit->getPosition();
        
        bit->setPosition(holder.getPosition());
        holder.setBit(bit);
        endTurn();
        return true;
    }   
    return false;
}

bool ConnectFour::canBitMoveFrom(Bit &bit, BitHolder &src) {
    return false;
}

bool ConnectFour::canBitMoveFromTo(Bit& bit, BitHolder& src, BitHolder& dst) {
    return false;
}

Player* ConnectFour::checkForWinner() { //implement check for winner
   return nullptr;     
}

bool ConnectFour::checkForDraw() {
    return false;
}

void ConnectFour::stopGame() {
    _grid->forEachSquare([](ChessSquare* square, int x, int y) {
        square->destroyBit();
    });
    _redPieces = 0;
    _yellowPieces = 0;
}

//6x7 = 42 if I forget
std::string ConnectFour::initialStateString() {
    return "000000000000000000000000000000000000000000";
}

std::string ConnectFour::stateString() {
    return _grid->getStateString();
}

void ConnectFour::setStateString(const std::string &s) {
    if (s.length() != 42) return;

    _redPieces = 0;
    _yellowPieces = 0;

    _grid->setStateString(s);

    // Recreate pieces from state
    size_t index = 0;
    _grid->forEachEnabledSquare([&](ChessSquare* square, int x, int y) {
        if (index < s.length()) {
            int pieceType = s[index++] - '0';
            if (pieceType != 0) {
                Bit* piece = createPiece(pieceType);
                piece->setPosition(square->getPosition());
                square->setBit(piece);
                (pieceType == RED_PIECE || pieceType == RED_KING) ? _redPieces++ : _yellowPieces++;
            }
        }
    });
}

void ConnectFour::updateAI() {}

