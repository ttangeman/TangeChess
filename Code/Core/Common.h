#pragma once

#include <stdint.h>
#include <vector>
#include <array>
#include <memory>
#include <string>
#include <atomic>
#include <unordered_map>

#include "Debug.h"

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef size_t usize;

#define ABS_VALUE(x) ((x)<0 ? -(x) : (x))
#define ARRAY_LENGTH(array) (sizeof(array) / sizeof(array[0]))

union Vec2 
{
    struct 
    {
        float X, Y;
    };
    
    struct 
    {
        float U, V;
    };
    
    struct 
    {
        float Width, Height;
    };

    float Data[2];
    
    Vec2() = default;
    Vec2(float x, float y) : X(x), Y(y)
    {
    }

    Vec2 operator+(Vec2 operand) const
    {
        Vec2 result;
        result.X = this->X + operand.X;
        result.Y = this->Y + operand.Y;
        return result;
    }
    
    void operator+=(Vec2 operand)
    {
        this->X += operand.X;
        this->Y += operand.Y;
    }
    
    Vec2 operator-(Vec2 operand) const
    {
        Vec2 result;
        result.X = this->X - operand.X;
        result.Y = this->Y - operand.Y;
        return result;
    }

    void operator-=(Vec2 operand)
    {
        this->X -= operand.X;
        this->Y -= operand.Y;
    }

    bool operator==(Vec2 operand) const
    {
        return (this->X == operand.X && this->Y == operand.Y);
    }
};

union Vec2i 
{
    struct 
    {
        int32 X, Y;
    };
    
    struct 
    {
        int32 Width, Height;
    };

    int32 Data[2];
    
    Vec2i() = default; 
    Vec2i(int32 x, int32 y) : X(x), Y(y)
    {
    }
    
    Vec2i operator+(Vec2i operand) const 
    {
        Vec2i result;
        result.X = this->X + operand.X;
        result.Y = this->Y + operand.Y;
        return result;
    }
    
    void operator+=(Vec2i operand)
    {
        this->X += operand.X;
        this->Y += operand.Y;
    }
    
    Vec2i operator-(Vec2i operand) const
    {
        Vec2i result;
        result.X = this->X - operand.X;
        result.Y = this->Y - operand.Y;
        return result;
    }
    
    void operator-=(Vec2i operand)
    {
        this->X -= operand.X;
        this->Y -= operand.Y;
    }

    bool operator==(Vec2i operand) const
    {
        return (this->X == operand.X && this->Y == operand.Y);
    }
};

union Vec3 
{
    struct 
    {
        float X, Y, Z;
    };
    
    struct
    {
        float R, G, B;
    };
    
    float Data[3];
    
    Vec3() = default;    
    Vec3(float x, float y, float z) : X(x), Y(y), Z(z)
    {
    }
    
    Vec3 operator+(Vec3 operand) const
    {
        Vec3 result;
        result.X = this->X + operand.X;
        result.Y = this->Y + operand.Y;
        result.Z = this->Z + operand.Z;
        return result;
    }
    
    void operator+=(Vec3 operand)
    {
        this->X += operand.X;
        this->Y += operand.Y;
        this->Z += operand.Z;
    }
    
    Vec3 operator-(Vec3 operand) const
    {
        Vec3 result;
        result.X = this->X - operand.X;
        result.Y = this->Y - operand.Y;
        result.Z = this->Z - operand.Z;
        return result;
    }

    void operator-=(Vec3 operand)
    {
        this->X -= operand.X;
        this->Y -= operand.Y;
        this->Z -= operand.Z;
    }
    
    bool operator==(Vec3 operand) const
    {
        return (this->X == operand.X && 
                this->Y == operand.Y &&
                this->Z == operand.Z);
    }
};

union Vec4 
{
    struct 
    {
        float X, Y, Z, W;
    };
    
    struct
    {
        float R, G, B, A;
    };
    
    float Data[4];
    
    Vec4() = default;
    Vec4(float x, float y, float z, float w) 
        : X(x), Y(y), Z(z), W(w)
    { 
    }
    
    Vec4 operator+(Vec4 operand) const
    {
        Vec4 result;
        result.X = this->X + operand.X;
        result.Y = this->Y + operand.Y;
        result.Z = this->Z + operand.Z;
        result.W = this->W + operand.W;
        return result;
    }
    
    void operator+=(Vec4 operand)
    {
        this->X += operand.X;
        this->Y += operand.Y;
        this->Z += operand.Z;
        this->W += operand.W;
    }
    
    Vec4 operator-(Vec4 operand) const
    {
        Vec4 result;
        result.X = this->X - operand.X;
        result.Y = this->Y - operand.Y;
        result.Z = this->Z - operand.Z;
        result.W = this->W - operand.W;
        return result;
    }

    void operator-=(Vec4 operand)
    {
        this->X -= operand.X;
        this->Y -= operand.Y;
        this->Z -= operand.Z;
        this->W -= operand.W;
    }
    
    bool operator==(Vec4 operand) const
    {
        return (this->X == operand.X && 
                this->Y == operand.Y &&
                this->Z == operand.Z &&
                this->W == operand.W);
    }
};