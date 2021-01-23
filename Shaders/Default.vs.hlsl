cbuffer Transforms: register(b0)
{
	matrix World;
	matrix View;
	matrix Projection;
};

struct VertexIn 
{
	float4 Position : POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD0;
};

struct VertexOut 
{
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
	float2 TexCoord : TEXCOORD0;
};

VertexOut main(VertexIn input) 
{
	VertexOut output;
	
	float4 position = float4(input.Position.xyz, 1);

	position = mul(position, World);
	position = mul(position, View);
	position = mul(position, Projection);

	output.Position = position;
	output.Color = input.Color;
	output.TexCoord = input.TexCoord;
	
	return output;
}