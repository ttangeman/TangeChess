SamplerState Sampler;
Texture2D input_texture;

struct PixelIn {
	float4 position : SV_POSITION;
	float4 color: COLOR;
	float2 texcoord : TEXCOORD0;
};

float4 main(PixelIn input) : SV_TARGET {
	float4 texture_sample = input_texture.Sample(Sampler, input.texcoord);
	// Is this proper color modulation?
	texture_sample.r += input.color.r;
	texture_sample.g += input.color.g;
	texture_sample.b += input.color.b;
	// This may or may not be janky, but this is the easiest
	// way I could figure out myself for "adding" transparency to
	// opaque textures.
	texture_sample.a *= input.color.a;
	return texture_sample;
}