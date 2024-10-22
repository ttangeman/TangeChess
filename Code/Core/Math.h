#pragma once

#define PI 3.14159
#define ABS_VALUE(x) ((x)<0 ? -(x) : (x))
#define DEGREES_TO_RADIANS(degrees) ((degrees) * (PI / 180))

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

    Vec2(float value) : X(value), Y(value)
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

    Vec2 operator+(Vec2i operand) const
    {
        Vec2 result;
        result.X = this->X + (float)operand.X;
        result.Y = this->Y + (float)operand.Y;
        return result;
    }
    
    void operator+=(Vec2i operand)
    {
        this->X += (float)operand.X;
        this->Y += (float)operand.Y;
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

    Vec2 operator-(Vec2i operand) const
    {
        Vec2 result;
        result.X = this->X - (float)operand.X;
        result.Y = this->Y - (float)operand.Y;
        return result;
    }

    void operator-=(Vec2i operand)
    {
        this->X -= (float)operand.X;
        this->Y -= (float)operand.Y;
    }

    bool operator==(Vec2 operand) const
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

    struct
    {
        Vec2 XY;
        float Z;
    };
    
    float Data[3];
    
    Vec3() = default;
    
    Vec3(Vec2 xy, float z) : X(xy.X), Y(xy.Y), Z(z)
    {

    } 

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

struct Rect
{
    Vec2 MinCorner;
    Vec2 MaxCorner;

    Rect() = default;
    Rect(Vec2 minCorner, Vec2 maxCorner)
        : MinCorner(minCorner), MaxCorner(maxCorner)
    {
    }

    Vec2 operator+(Vec2 offset)
    {
        return MinCorner + offset;
    }

    Vec2 operator-(Vec2 offset)
    {
        return MinCorner - offset;
    }

    void operator+=(Vec2 offset)
    {
        MinCorner += offset;
    }

    void operator-=(Vec2 offset)
    {
        MinCorner -= offset;
    }

    bool operator==(Rect operand) const
    {
        return (this->MinCorner == operand.MinCorner &&
                this->MaxCorner == operand.MaxCorner);
    }

    static Rect ComputeBoundingBox(Vec2 centerP, Vec2 scale)
    {
        // The scale _is_ the diameter.
        Vec2 radius = Vec2(scale.X / 2.0, scale.Y / 2.0);
        return Rect(centerP - radius, centerP + radius);
    }
};

inline float SafeRatioN(float numerator, float divisor, float value) 
{
    float result = value;
    
    if (divisor != 0.0) 
    {
        result = numerator / divisor;
    }

    return result;
}

inline float SafeRatio0(float numerator, float divisor) 
{
    return SafeRatioN(numerator, divisor, 0);
}

inline float SafeRatio1(float numerator, float divisor) 
{
    return SafeRatioN(numerator, divisor, 1);
}

inline float SafeInverse(float value) 
{
    return SafeRatio0(1.0, value);
}

inline float Lerp(float a, float t, float b) {
    return (1.0f - t)*a + t*b;
}

inline Vec2 Lerp(Vec2 a, float t, Vec2 b) {
    return Vec2((1.0f - t)*a.X + t*b.Y, (1.0f - t)*a.Y + t*b.Y);
}