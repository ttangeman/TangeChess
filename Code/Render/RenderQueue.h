#pragma once

class RenderQueue
{
    void Batch();
    void Push();
    void Dispatch();  
};

/*
 
 experiment with std::function for ImGui stuff?
 could be a special render job type
struct RenderJob
{
 
};

*/