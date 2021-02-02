SamplerState Sampler;
Texture2D TextureIn;

cbuffer ColorBuffer : register(b0)
{
	float4 AdditiveColor;
};

struct PixelIn 
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET 
{
	float4 textureSample = TextureIn.Sample(Sampler, input.TexCoord);

	textureSample.r += input.Color.r + AdditiveColor.r;
	textureSample.g += input.Color.g + AdditiveColor.g;
	textureSample.b += input.Color.b + AdditiveColor.b;
	textureSample.a *= input.Color.a * AdditiveColor.a;

	return textureSample;
}