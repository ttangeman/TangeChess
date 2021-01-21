#pragma once

#include "Core/Common.h"

namespace Game 
{
    // NOTE: A piece id of 0 is invalid, as 0 is used for marking a square as unoccupied.
    typedef int32 PieceId;
    
    static const int32 TotalPieceCount = 32;
    static const int32 RowCount = 8;
    static const int32 ColCount = 8;
    static const int32 SquareCount = RowCount * ColCount;
    
    // NOTE: The order of the pieces in this enum corresponds to the order in which
    // the pieces in the piece sprite sheet appear!
    enum class PieceType
    {
        King,
        Queen,
        Bishop,
        Knight,
        Rook,
        Pawn,
    };
    
    enum class PieceColor
    {
        White,
        Black,
    };
    
    struct Piece
    {
        PieceId Id;
        PieceColor Color;
        PieceType Type;
        Vec2i Position;
        bool HasMoved;
    };

    // Stores a possible move for a piece.
    // If the move should cause a capture, then captureId should match the id of the
    // piece at the destination square. 
    struct PieceMove
    {
        Vec2i destinationSquare;
        PieceId captureId;
    };
    
    class GameState
    {
        public:
        
        GameState() = default;
        
        // Initializes the GameState and starts a new game.
        void StartGame(PieceColor desiredColor);
        
        // Shutdowns the GameState and ends the current game.
        void EndGame();
        
        // Checks if the coordinates are within the interval [0, 7] in x and y.  
        bool IsValidSquare(Vec2i square) const;

        // Checks if a square is occupied.
        bool IsOccupiedSquare(Vec2i square) const;

        // Checks if a square is occupied by the specified color.
        bool IsOccupiedSquare(Vec2i square, PieceColor color) const;
        
        // Gets the piece data from the board at the specified square coordinates.
        PieceId LookupPieceId(Vec2i square) const;
        
        // Looks up a piece by id.
        Piece LookupPiece(PieceId id) const;

        // Grabs a reference to a piece by id.
        Piece& GetPiece(PieceId id);

        // Finds the set of all valid squares for a particular piece to move to.
        std::vector<PieceMove> FindValidMoves(PieceId id) const;

        // Tries to move an active piece to the desired square x and y.
        // Returns true on a successful move.
        bool MovePiece(PieceId id, Vec2i desiredSquare);
        
        // This overload uses a precomputed valid move set for the piece.
        // Tries to move an active piece to the desired square x and y. 
        // Returns true on a successful move.
        bool MovePiece(PieceId id, Vec2i desiredSquare, const std::vector<PieceMove>& validMoveSet);
        
        private:

        // Stores the data for all possible pieces, whether they are active or not.
        std::array<Piece, TotalPieceCount> m_pieceData;
        
        // Keeps track of where each piece is on the board. Note that a piece id of
        // 0 means that the square is empty.
        // NOTE: This has to be synchronized alongside Piece::Position.
        std::array<PieceId, SquareCount> m_boardState;
    };
}