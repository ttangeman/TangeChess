cbuffer ColorBuffer : register(b0)
{
	float4 AdditiveColor;
};

struct PixelIn 
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(PixelIn input) : SV_TARGET 
{
	input.Color.r += AdditiveColor.r;
	input.Color.g += AdditiveColor.g;
	input.Color.b += AdditiveColor.b;
	input.Color.a *= AdditiveColor.a;
	return input.Color;
}