cbuffer Transforms {
	matrix world;
	matrix view;
	matrix projection;
};

struct VertexIn {
	float4 pos : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD0;
};

struct VertexOut {
	float4 pos : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD0;
};

VertexOut main(VertexIn input) {
	VertexOut output;

	input.pos = float4(input.pos.xyz, 1);
	output.pos = mul(input.pos, world);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, projection);
	output.color = input.color;
	output.texcoord = input.texcoord;

	return output;
}