#include "GameState.h"

namespace Game 
{
    // Finds the "opposite" color of the specified color.
    // In other words, PieceColor::White returns PieceColor::Black.
    // Only works if the parameter is a valid color!
    static PieceColor FindOppositeColor(PieceColor color)
    {
        return (PieceColor) (1 - (int32)color);
    }

    // Converts a 2D square index into a 1D index for the board state.
    static int32 IndexBoardState(Vec2i square)
    {
        return square.Y * ColCount + square.X; 
    }

    void GameState::StartGame(PieceColor desiredColor)
    {
        m_boardState.fill(0);
        m_pieceData.fill({});
        
        auto squareIndex = 0;
        auto pieceIndex = 0;
        
        // Standard setup for the non-pawn rows.
        PieceType standardPieceRow[ColCount] =
        {
            PieceType::Rook, PieceType::Knight, PieceType::Bishop, PieceType::Queen,
            PieceType::King, PieceType::Bishop, PieceType::Knight, PieceType::Rook,
        };
        
        // Fill the bottom two rows with the desired color.
        while (squareIndex < ColCount)
        {
            auto id = pieceIndex + 1;
            auto piecePosition = Vec2i(squareIndex % ColCount, squareIndex / RowCount);
            
            m_boardState[squareIndex] = id;
            m_pieceData[pieceIndex] =
            {
                id,
                desiredColor,
                standardPieceRow[squareIndex % ColCount],
                piecePosition,
                false,
            };
            
            squareIndex++;
            pieceIndex++;
        }
        
        while (squareIndex < ColCount * 2)
        {
            auto id = pieceIndex + 1;
            auto piecePosition = Vec2i(squareIndex % ColCount, squareIndex / RowCount);
            
            m_boardState[squareIndex] = id;
            m_pieceData[pieceIndex] =
            {
                id,
                desiredColor,
                PieceType::Pawn,
                piecePosition,
                false,
            };
            
            squareIndex++;
            pieceIndex++;
        }
        
        // Fill the top two rows with the same pieces, but the opposite color.
        // Also, the rows are filled in the reverse order as the bottom two rows.
        PieceColor oppositeColor = FindOppositeColor(desiredColor);
        squareIndex = ColCount * 6;
        
        while (squareIndex < ColCount * 7)
        {
            auto id = pieceIndex + 1;
            auto piecePosition = Vec2i(squareIndex % ColCount, squareIndex / RowCount);
            
            m_boardState[squareIndex] = id;
            m_pieceData[pieceIndex] =
            {
                id,
                oppositeColor,
                PieceType::Pawn,
                piecePosition,
                false,
            };
            
            squareIndex++;
            pieceIndex++;
        }
        
        while (squareIndex < ColCount * 8)
        {
            auto id = pieceIndex + 1;
            auto piecePosition = Vec2i(squareIndex % ColCount, squareIndex / RowCount);
            
            m_boardState[squareIndex] = id;
            m_pieceData[pieceIndex] =
            {
                id,
                oppositeColor,
                standardPieceRow[squareIndex % ColCount],
                piecePosition,
                false,
            };
            
            squareIndex++;
            pieceIndex++;
        }
    }
    
    void GameState::EndGame()
    {
    }

    bool GameState::IsValidSquare(Vec2i square) const
    {
        return ((square.X <= 7 && square.X >= 0) &&
                (square.Y <= 7 && square.Y >= 0)) ?
                true : false;
    }
    
    bool GameState::IsOccupiedSquare(Vec2i square) const
    {
        // A piece exists at the specified square if the id is > 0.
        return m_boardState.at(IndexBoardState(square)) > 0;
    }

    bool GameState::IsOccupiedSquare(Vec2i square, PieceColor color) const
    {
        auto pieceId = LookupPieceId(square);
        
        if (pieceId == 0)
        {
            return false;
        }
        
        auto piece = LookupPiece(pieceId);

        return piece.Color == color;
    }

    PieceId GameState::LookupPieceId(Vec2i square) const
    {
        if (IsValidSquare(square))
        {
            return m_boardState.at(IndexBoardState(square));
        }
        return 0;
    }
    
    Piece GameState::LookupPiece(PieceId id) const 
    {
        ASSERT(id != 0);
        return m_pieceData.at(id - 1);
    }

    Piece& GameState::GetPiece(PieceId id) 
    {
        ASSERT(id != 0);
        return m_pieceData.at(id - 1);
    }
    
    std::vector<PieceMove> GameState::FindValidMoves(PieceId id) const
    {
        std::vector<PieceMove> validMoves;
        validMoves.reserve(16);
        
        auto piece = LookupPiece(id);
        auto oppositeColor = FindOppositeColor(piece.Color);

        switch (piece.Type)
        {
            // Either can go forward one square -- or two if its the pawn's first move --
            // or attempt to capture pieces on the left and right diagonal.
            case PieceType::Pawn:
            {
                // TODO: Implement en passant and promotion.
                Vec2i forward = piece.Position + Vec2i(0, 1);
                Vec2i twoForward = piece.Position + Vec2i(0, 2);
                Vec2i leftDiagonal = piece.Position + Vec2i(-1, 1);
                Vec2i rightDiagonal = piece.Position + Vec2i(1, 1);

                if (!IsOccupiedSquare(forward) && IsValidSquare(forward))
                {
                    validMoves.push_back({forward, 0});
                    
                    // Special case: being able to move two squares on the first move.
                    if (!IsOccupiedSquare(twoForward) && !piece.HasMoved)
                    {
                        validMoves.push_back({twoForward, 0});
                    }
                }
                
                if (IsValidSquare(leftDiagonal) && IsOccupiedSquare(leftDiagonal, oppositeColor))
                {
                    auto captureId = LookupPieceId(leftDiagonal);
                    validMoves.push_back({leftDiagonal, captureId});
                }

                if (IsValidSquare(rightDiagonal) && IsOccupiedSquare(rightDiagonal, oppositeColor))
                {
                    auto captureId = LookupPieceId(rightDiagonal);
                    validMoves.push_back({rightDiagonal, captureId});
                }
            } break;
            
            case PieceType::King:
            {
                // TODO: Castle and handle checks.
                Vec2i possibleMoveSet[] =
                {
                    Vec2i(piece.Position.X, piece.Position.Y + 1),  
                    Vec2i(piece.Position.X, piece.Position.Y - 1),  
                    Vec2i(piece.Position.X + 1, piece.Position.Y),  
                    Vec2i(piece.Position.X - 1, piece.Position.Y),  

                    Vec2i(piece.Position.X + 1, piece.Position.Y + 1),  
                    Vec2i(piece.Position.X - 1, piece.Position.Y + 1),  
                    Vec2i(piece.Position.X + 1, piece.Position.Y - 1),  
                    Vec2i(piece.Position.X - 1, piece.Position.Y - 1),  
                };             

                for (auto it : possibleMoveSet)
                {
                    if (IsValidSquare(it))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(it, oppositeColor))
                        {
                            captureId = LookupPieceId(it);
                        }
                        
                        if (!IsOccupiedSquare(it, piece.Color))
                        {
                            validMoves.push_back({it, captureId});
                        }
                    }
                }
            } break;
            
            // HACK: Bad hack to make the queen use the same valid moves as both the bishop and rook!
            case PieceType::Queen:
            {
                case PieceType::Bishop:
                {
                    Vec2i topLeftDiagonal = piece.Position + Vec2i(-1, 1);
                    Vec2i topRightDiagonal = piece.Position + Vec2i(1, 1);
                    Vec2i bottomLeftDiagonal = piece.Position + Vec2i(-1, -1);
                    Vec2i bottomRightDiagonal = piece.Position + Vec2i(1, -1);

                    while (IsValidSquare(topLeftDiagonal))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(topLeftDiagonal, oppositeColor))
                        {
                            captureId = LookupPieceId(topLeftDiagonal);
                        }

                        if (!IsOccupiedSquare(topLeftDiagonal, piece.Color))
                        {
                            validMoves.push_back({topLeftDiagonal, captureId});
                        }
                        
                        topLeftDiagonal += Vec2i(-1, 1);
                    }

                    while (IsValidSquare(topRightDiagonal))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(topRightDiagonal, oppositeColor))
                        {
                            captureId = LookupPieceId(topRightDiagonal);
                        }

                        if (!IsOccupiedSquare(topRightDiagonal, piece.Color))
                        {
                            validMoves.push_back({topRightDiagonal, captureId});
                        }

                        topRightDiagonal += Vec2i(1, 1);
                    }

                    while (IsValidSquare(bottomLeftDiagonal))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(bottomLeftDiagonal, oppositeColor))
                        {
                            captureId = LookupPieceId(bottomLeftDiagonal);
                        }

                        if (!IsOccupiedSquare(bottomLeftDiagonal, piece.Color))
                        {
                            validMoves.push_back({bottomLeftDiagonal, captureId});
                        }

                        bottomLeftDiagonal += Vec2i(-1, -1);
                    }

                    while (IsValidSquare(bottomRightDiagonal))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(bottomRightDiagonal, oppositeColor))
                        {
                            captureId = LookupPieceId(bottomRightDiagonal);
                        }

                        if (!IsOccupiedSquare(bottomRightDiagonal, piece.Color))
                        {
                            validMoves.push_back({bottomRightDiagonal, captureId});
                        }

                        bottomRightDiagonal += Vec2i(1, -1);
                    }

                    // HACK: Queens use the above code too!
                    if (piece.Type == PieceType::Bishop) break;
                }
                
                case PieceType::Rook:
                {
                    Vec2i up = piece.Position + Vec2i(0, 1);
                    Vec2i down = piece.Position + Vec2i(0, -1);
                    Vec2i right = piece.Position + Vec2i(1, 0);
                    Vec2i left = piece.Position + Vec2i(-1, 0);
                    
                    while (IsValidSquare(up))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(up, oppositeColor))
                        {
                            captureId = LookupPieceId(up);
                        }

                        if (!IsOccupiedSquare(up, piece.Color))
                        {
                            validMoves.push_back({up, captureId});
                        }
                        
                        up += Vec2i(0, 1);
                    }

                    while (IsValidSquare(down))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(down, oppositeColor))
                        {
                            captureId = LookupPieceId(down);
                        }

                        if (!IsOccupiedSquare(down, piece.Color))
                        {
                            validMoves.push_back({down, captureId});
                        }

                        down += Vec2i(0, -1);
                    }

                    while (IsValidSquare(right))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(right, oppositeColor))
                        {
                            captureId = LookupPieceId(right);
                        }

                        if (!IsOccupiedSquare(right, piece.Color))
                        {
                            validMoves.push_back({right, captureId});
                        }

                        right += Vec2i(1, 0);
                    }

                    while (IsValidSquare(left))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(left, oppositeColor))
                        {
                            captureId = LookupPieceId(left);
                        }

                        if (!IsOccupiedSquare(left, piece.Color))
                        {
                            validMoves.push_back({left, captureId});
                        }

                        left += Vec2i(0, -1);
                    }
                    
                } break;
            }
            
            case PieceType::Knight:
            {
                Vec2i possibleMoveSet[] = 
                {
                    Vec2i(piece.Position.X - 1, piece.Position.Y + 2),
                    Vec2i(piece.Position.X - 2, piece.Position.Y + 1),
                    Vec2i(piece.Position.X + 1, piece.Position.Y + 2),
                    Vec2i(piece.Position.X + 2, piece.Position.Y + 1),
                    
                    Vec2i(piece.Position.X - 1, piece.Position.Y - 2),
                    Vec2i(piece.Position.X - 2, piece.Position.Y - 1),
                    Vec2i(piece.Position.X + 1, piece.Position.Y - 2),
                    Vec2i(piece.Position.X + 2, piece.Position.Y - 1),
                };
                
                for (auto it : possibleMoveSet)
                {
                    if (IsValidSquare(it))
                    {
                        auto captureId = 0;

                        if (IsOccupiedSquare(it, oppositeColor))
                        {
                            captureId = LookupPieceId(it);
                        }
                        
                        if (!IsOccupiedSquare(it, piece.Color))
                        {
                            validMoves.push_back({it, captureId});
                        }
                    }
                }
            } break;
        }

        return validMoves;
    }
    
    bool GameState::MovePiece(PieceId id, Vec2i desiredSquare)
    {
        auto validMoveSet = FindValidMoves(id);
        return MovePiece(id, desiredSquare, validMoveSet);
    }

    // TODO: Currently the captureId is not used from PieceMove, but
    // it is very likely that it will be needed for notifiying that a capture has
    // occured on the network side of things (I would assume). So it will not be
    // refactored out for now.
    bool GameState::MovePiece(PieceId id, Vec2i desiredSquare, 
                              const std::vector<PieceMove>& validMoveSet)
    {
        for (const auto& it : validMoveSet)
        {
            // The desired square is valid if it is in the valid set!
            if (it.destinationSquare == desiredSquare)
            {
                Piece& piece = GetPiece(id);
                piece.Position = desiredSquare;

                m_boardState[IndexBoardState(piece.Position)] = 0;
                m_boardState[IndexBoardState(desiredSquare)] = id;
                
                return true;
            }
        }
        
        return false;
    }
}