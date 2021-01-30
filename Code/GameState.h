#pragma once

#include "Core/Common.h"
#include "Entity/Entity.h"
#include "Entity/EntityManager.h"
#include "Entity/Component.h"
#include "Render/Transformable.h"
#include "Render/Drawable.h"

namespace Tange
{   
    static const int32 UniquePieceCount = 12;
    static const int32 TotalPieceCount = 32;
    static const int32 RowCount = 8;
    static const int32 ColCount = 8;
    static const int32 SquareCount = RowCount * ColCount;
    
    static const std::string PieceNames[] =
    {
        "BlackKing",
        "BlackQueen",
        "BlackBishop",
        "BlackKnight",
        "BlackRook",
        "BlackPawn",
        "WhiteKing",
        "WhiteQueen",
        "WhiteBishop",
        "WhiteKnight",
        "WhiteRook",
        "WhiteBishop",
    };
    
    // NOTE: This order must match the PieceNames array!
    enum class PieceType
    {
        King,
        Queen,
        Bishop,
        Knight,
        Rook,
        Pawn,
    };
    
    // Values manually set to 1 and 2 for indexing PieceNames.
    enum class PieceColor
    {
        White = 1,
        Black = 2,
    };
    
    struct PieceComponent : public Component<PieceComponent>
    {
        PieceColor Color;
        PieceType Type;
        bool HasMoved;
    };

    // Stores a possible move for a piece.
    // If the move should cause a capture, then captureEntity
    // should be set to the possibly destroyed entity.
    struct PieceMove
    {
        Vec2 destinationSquare;
        Entity captureEntity;
    };
    
    class GameState
    {
    public: 
        // Keeps track of where each piece is on the board. Note that a entity id 
        // of 0 means that the square is empty.
        // NOTE: This has to be synchronized alongside the piece position.
        std::array<Entity, SquareCount> BoardState;
        
        GameState();
        
        void StartGame(PieceColor desiredColor);
        void EndGame();
        
        Entity GetEntity(Vec2 square) const;

        const std::string& GetPieceName(Entity entity) const;

        // Checks if the coordinates are within the interval [0, 7] in x and y.  
        bool IsValidSquare(Vec2 square) const;

        // Checks if a square is occupied.
        bool IsOccupiedSquare(Vec2 square) const;
        bool IsOccupiedSquare(Vec2 square, PieceColor color) const;

        // Finds the set of all valid squares for a particular piece to move to.
        std::vector<PieceMove> FindValidMoves(Entity entity) const;

        // Tries to move an active piece to the desired square x and y.
        // Returns true on a successful move.
        bool MovePiece(Entity entity, Vec2 desiredSquare);
        
        // This overload uses a precomputed valid move set for the piece.
        // Tries to move an active piece to the desired square x and y. 
        // Returns true on a successful move.
        bool MovePiece(Entity entity, Vec2 desiredSquare, 
                       const std::vector<PieceMove>& validMoveSet);
    };
}