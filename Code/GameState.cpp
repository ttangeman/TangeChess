#include "GameState.h"

namespace Tange
{
    GameState::GameState()
    {
        EntityManager::RegisterComponent<PieceComponent>();
    }

    // Finds the "opposite" color of the specified color.
    // In other words, PieceColor::White returns PieceColor::Black.
    // Only works if the parameter is a valid color!
    static PieceColor FindOppositeColor(PieceColor color)
    {
        return (PieceColor)(1 - (int32)color);
    }

    void GameState::StartGame(PieceColor desiredColor)
    {
        BoardState.fill({});
        
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
            Entity entity = EntityManager::RegisterEntity();
            auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
            auto& piece = EntityManager::AttachComponent<PieceComponent>(entity);
            //auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);

            worldP.Position = Vec3(squareIndex % ColCount + 1, squareIndex / RowCount + 1, 1);

            piece.Color = desiredColor;
            piece.Type = standardPieceRow[squareIndex % ColCount];

            entity.hRender.AttachMesh(GetPieceName(entity));
            entity.hRender.AttachTexture("Texture/Pieces");
            entity.Transform.Orthographic(Vec2(0.5, 0.5), Vec2(8.5, 8.5), 0.1, 100.0);

            /*
            dragable.SetMouseMapping(Vec2(0.5, 0.5), Vec2(8.5, 8.5));
            dragable.Initialize(worldP.Position.XY, worldP.Scale.XY);
            dragable.BindInputHandlers();
            */

            BoardState[squareIndex] = entity;
            
            squareIndex++;
            pieceIndex++;
        }
        
        while (squareIndex < ColCount * 2)
        {
            Entity entity = EntityManager::RegisterEntity();
            auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
            auto& piece = EntityManager::AttachComponent<PieceComponent>(entity);
            //auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);

            piece.Color = desiredColor;
            piece.Type = PieceType::Pawn;

            entity.hRender.AttachMesh(GetPieceName(entity));
            entity.hRender.AttachTexture("Texture/Pieces");
            entity.Transform.Orthographic(Vec2(0.5, 0.5), Vec2(8.5, 8.5), 0.1, 100.0);

            worldP.Position = Vec3(squareIndex % ColCount + 1, squareIndex / RowCount + 1, 1);

            /*
            dragable.SetMouseMapping(Vec2(0.5, 0.5), Vec2(8.5, 8.5));
            dragable.Initialize(worldP.Position.XY, worldP.Scale.XY);
            dragable.BindInputHandlers();
            */

            BoardState[squareIndex] = entity;
            
            squareIndex++;
            pieceIndex++;
        }
        
        // Fill the top two rows with the same pieces, but the opposite color.
        // Also, the rows are filled in the reverse order as the bottom two rows.
        PieceColor oppositeColor = FindOppositeColor(desiredColor);
        squareIndex = ColCount * 6;
        
        while (squareIndex < ColCount * 7)
        {
            Entity entity = EntityManager::RegisterEntity();

            auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
            auto& piece = EntityManager::AttachComponent<PieceComponent>(entity);
            //auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);

            piece.Color = FindOppositeColor(desiredColor);
            piece.Type = PieceType::Pawn;

            entity.hRender.AttachMesh(GetPieceName(entity));
            entity.hRender.AttachTexture("Texture/Pieces");
            entity.Transform.Orthographic(Vec2(0.5, 0.5), Vec2(8.5, 8.5), 0.1, 100.0);

            worldP.Position = Vec3(squareIndex % ColCount + 1, squareIndex / RowCount + 1, 1);

            /*
            dragable.SetMouseMapping(Vec2(0.5, 0.5), Vec2(8.5, 8.5));
            dragable.Initialize(worldP.Position.XY, worldP.Scale.XY);
            dragable.BindInputHandlers();
            */

            BoardState[squareIndex] = entity;
            
            squareIndex++;
            pieceIndex++;
        }
        
        while (squareIndex < ColCount * 8)
        {
            Entity entity = EntityManager::RegisterEntity();

            auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
            auto& piece = EntityManager::AttachComponent<PieceComponent>(entity);
            //auto& dragable = EntityManager::AttachComponent<Dragable2D>(entity);

            piece.Color = FindOppositeColor(desiredColor);
            piece.Type = standardPieceRow[squareIndex % ColCount];

            entity.hRender.AttachMesh(GetPieceName(entity));
            entity.hRender.AttachTexture("Texture/Pieces");
            entity.Transform.Orthographic(Vec2(0.5, 0.5), Vec2(8.5, 8.5), 0.1, 100.0);

            worldP.Position = Vec3(squareIndex % ColCount + 1, squareIndex / RowCount + 1, 1);

            /*
            dragable.SetMouseMapping(Vec2(0.5, 0.5), Vec2(8.5, 8.5));
            dragable.Initialize(worldP.Position.XY, worldP.Scale.XY);
            dragable.BindInputHandlers();
            */

            BoardState[squareIndex] = entity;
            
            squareIndex++;
            pieceIndex++;
        }
    }
    
    void GameState::EndGame()
    {
    }

    // Converts a 2D square index into a 1D index for the board state.
    static int32 IndexBoardState(Vec2 square)
    {
        return (square.Y - 1) * GameState::ColCount + (square.X - 1); 
    }

    Entity GameState::GetEntity(Vec2 square) const
    {
        return BoardState.at(IndexBoardState(square));
    }

    const std::string& GameState::GetPieceName(Entity entity) const
    {
        auto& piece = EntityManager::GetComponent<PieceComponent>(entity);
        return g_PieceNames[(int32)piece.Type + ((int32)piece.Color * 6)];
    }

    bool GameState::IsValidSquare(Vec2 square) const
    {
        return ((square.X <= 8 && square.X >= 1) &&
                (square.Y <= 8 && square.Y >= 1)) ?
                true : false;
    }
    
    bool GameState::IsOccupiedSquare(Vec2 square) const
    {
        return GetEntity(square).IsValid();
    }

    bool GameState::IsOccupiedSquare(Vec2 square, PieceColor color) const
    {
        auto entity = GetEntity(square);
        auto& piece = EntityManager::GetComponent<PieceComponent>(entity);
        
        if (!entity.IsValid())
        {
            return false;
        }
        return piece.Color == color;
    }

    std::vector<PieceMove> GameState::FindValidMoves(Entity entity) const
    {
        std::vector<PieceMove> validMoves;
        validMoves.reserve(16);
        
        auto& piece = EntityManager::GetComponent<PieceComponent>(entity);
        auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
        auto oppositeColor = FindOppositeColor(piece.Color);

        switch (piece.Type)
        {
            // Either can go forward one square -- or two if its the pawn's first move --
            // or attempt to capture pieces on the left and right diagonal.
            case PieceType::Pawn:
            {
                // TODO: Implement en passant and promotion.
                Vec2 forward = worldP.Position.XY + Vec2(0, 1);
                Vec2 twoForward = worldP.Position.XY + Vec2(0, 2);
                Vec2 leftDiagonal = worldP.Position.XY + Vec2(-1, 1);
                Vec2 rightDiagonal = worldP.Position.XY + Vec2(1, 1);

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
                    auto capture = GetEntity(leftDiagonal);
                    validMoves.push_back({leftDiagonal, capture});
                }

                if (IsValidSquare(rightDiagonal) && IsOccupiedSquare(rightDiagonal, oppositeColor))
                {
                    auto capture = GetEntity(rightDiagonal);
                    validMoves.push_back({rightDiagonal, capture});
                }
            } break;
            
            case PieceType::King:
            {
                // TODO: Castle and handle checks.
                Vec2 possibleMoveSet[] =
                {
                    Vec2(worldP.Position.X, worldP.Position.Y + 1),  
                    Vec2(worldP.Position.X, worldP.Position.Y - 1),  
                    Vec2(worldP.Position.X + 1, worldP.Position.Y),  
                    Vec2(worldP.Position.X - 1, worldP.Position.Y),  

                    Vec2(worldP.Position.X + 1, worldP.Position.Y + 1),  
                    Vec2(worldP.Position.X - 1, worldP.Position.Y + 1),  
                    Vec2(worldP.Position.X + 1, worldP.Position.Y - 1),  
                    Vec2(worldP.Position.X - 1, worldP.Position.Y - 1),  
                };             

                for (auto it : possibleMoveSet)
                {
                    if (IsValidSquare(it))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(it, oppositeColor))
                        {
                            capture = GetEntity(it);
                        }
                        
                        if (!IsOccupiedSquare(it, piece.Color))
                        {
                            validMoves.push_back({it, capture});
                        }
                    }
                }
            } break;
            
            // HACK: Bad hack to make the queen use the same valid moves as both the bishop and rook!
            case PieceType::Queen:
            {
                case PieceType::Bishop:
                {
                    Vec2 topLeftDiagonal = worldP.Position.XY + Vec2(-1, 1);
                    Vec2 topRightDiagonal = worldP.Position.XY + Vec2(1, 1);
                    Vec2 bottomLeftDiagonal = worldP.Position.XY + Vec2(-1, -1);
                    Vec2 bottomRightDiagonal = worldP.Position.XY + Vec2(1, -1);

                    while (IsValidSquare(topLeftDiagonal))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(topLeftDiagonal, oppositeColor))
                        {
                            capture = GetEntity(topLeftDiagonal);
                        }

                        if (!IsOccupiedSquare(topLeftDiagonal, piece.Color))
                        {
                            validMoves.push_back({topLeftDiagonal, capture});
                        }
                        
                        topLeftDiagonal += Vec2(-1, 1);
                    }

                    while (IsValidSquare(topRightDiagonal))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(topRightDiagonal, oppositeColor))
                        {
                            capture = GetEntity(topRightDiagonal);
                        }

                        if (!IsOccupiedSquare(topRightDiagonal, piece.Color))
                        {
                            validMoves.push_back({topRightDiagonal, capture});
                        }

                        topRightDiagonal += Vec2(1, 1);
                    }

                    while (IsValidSquare(bottomLeftDiagonal))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(bottomLeftDiagonal, oppositeColor))
                        {
                            capture = GetEntity(bottomLeftDiagonal);
                        }

                        if (!IsOccupiedSquare(bottomLeftDiagonal, piece.Color))
                        {
                            validMoves.push_back({bottomLeftDiagonal, capture});
                        }

                        bottomLeftDiagonal += Vec2(-1, -1);
                    }

                    while (IsValidSquare(bottomRightDiagonal))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(bottomRightDiagonal, oppositeColor))
                        {
                            capture = GetEntity(bottomRightDiagonal);
                        }

                        if (!IsOccupiedSquare(bottomRightDiagonal, piece.Color))
                        {
                            validMoves.push_back({bottomRightDiagonal, capture});
                        }

                        bottomRightDiagonal += Vec2(1, -1);
                    }

                    // HACK: Queens use the above code too!
                    if (piece.Type == PieceType::Bishop) break;
                }
                
                case PieceType::Rook:
                {
                    Vec2 up = worldP.Position.XY + Vec2(0, 1);
                    Vec2 down = worldP.Position.XY + Vec2(0, -1);
                    Vec2 right = worldP.Position.XY + Vec2(1, 0);
                    Vec2 left = worldP.Position.XY + Vec2(-1, 0);
                    
                    while (IsValidSquare(up))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(up, oppositeColor))
                        {
                            capture = GetEntity(up);
                        }

                        if (!IsOccupiedSquare(up, piece.Color))
                        {
                            validMoves.push_back({up, capture});
                        }
                        
                        up += Vec2(0, 1);
                    }

                    while (IsValidSquare(down))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(down, oppositeColor))
                        {
                            capture = GetEntity(down);
                        }

                        if (!IsOccupiedSquare(down, piece.Color))
                        {
                            validMoves.push_back({down, capture});
                        }

                        down += Vec2(0, -1);
                    }

                    while (IsValidSquare(right))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(right, oppositeColor))
                        {
                            capture = GetEntity(right);
                        }

                        if (!IsOccupiedSquare(right, piece.Color))
                        {
                            validMoves.push_back({right, capture});
                        }

                        right += Vec2(1, 0);
                    }

                    while (IsValidSquare(left))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(left, oppositeColor))
                        {
                            capture = GetEntity(left);
                        }

                        if (!IsOccupiedSquare(left, piece.Color))
                        {
                            validMoves.push_back({left, capture});
                        }

                        left += Vec2(0, -1);
                    }
                    
                } break;
            }
            
            case PieceType::Knight:
            {
                Vec2 possibleMoveSet[] = 
                {
                    Vec2(worldP.Position.X - 1, worldP.Position.Y + 2),
                    Vec2(worldP.Position.X - 2, worldP.Position.Y + 1),
                    Vec2(worldP.Position.X + 1, worldP.Position.Y + 2),
                    Vec2(worldP.Position.X + 2, worldP.Position.Y + 1),
                    
                    Vec2(worldP.Position.X - 1, worldP.Position.Y - 2),
                    Vec2(worldP.Position.X - 2, worldP.Position.Y - 1),
                    Vec2(worldP.Position.X + 1, worldP.Position.Y - 2),
                    Vec2(worldP.Position.X + 2, worldP.Position.Y - 1),
                };
                
                for (auto it : possibleMoveSet)
                {
                    if (IsValidSquare(it))
                    {
                        Entity capture = {};

                        if (IsOccupiedSquare(it, oppositeColor))
                        {
                            capture = GetEntity(it);
                        }
                        
                        if (!IsOccupiedSquare(it, piece.Color))
                        {
                            validMoves.push_back({it, capture});
                        }
                    }
                }
            } break;
        }

        return validMoves;
    }
    
    bool GameState::MovePiece(Entity entity, Vec2 desiredSquare)
    {
        auto validMoveSet = FindValidMoves(entity);
        return MovePiece(entity, desiredSquare, validMoveSet);
    }

    bool GameState::MovePiece(Entity entity, Vec2 desiredSquare, 
                              const std::vector<PieceMove>& validMoveSet)
    {
        for (auto& it : validMoveSet)
        {
            // The desired square is valid if it is in the valid set!
            if (it.DestinationSquare == desiredSquare)
            {
                auto& worldP = EntityManager::AttachComponent<WorldTransform>(entity);
                
                // NOTE: Need to synchronize the two positions!
                worldP.Position.XY = desiredSquare;
                
                if (it.CaptureEntity.IsValid())
                {
                    Entity captured = BoardState[IndexBoardState(desiredSquare)];
                    EntityManager::DestroyEntity(captured);
                }

                BoardState[IndexBoardState(worldP.Position.XY)] = {};
                BoardState[IndexBoardState(desiredSquare)] = entity;

                return true;
            }
        }
        
        return false;
    }
}