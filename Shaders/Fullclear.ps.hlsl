struct PixelIn 
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

float4 main(PixelIn input) : SV_TARGET 
{
	return input.Color;
}