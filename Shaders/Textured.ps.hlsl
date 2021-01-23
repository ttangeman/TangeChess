SamplerState Sampler;
Texture2D TextureIn;

struct PixelIn 
{
	float4 Position : SV_POSITION;
	float4 Color: COLOR;
	float2 TexCoord : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET 
{
	float4 textureSample = TextureIn.Sample(Sampler, input.TexCoord);

	textureSample.r += input.Color.r;
	textureSample.g += input.Color.g;
	textureSample.b += input.Color.b;
	textureSample.a *= input.Color.a;

	return textureSample;
}