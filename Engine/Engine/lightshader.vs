cbuffer Matrix : register(c0)
{
	matrix world;
	matrix view;
	matrix proj;
	matrix sview;
	matrix sproj;
};

cbuffer Variable : register(c1)
{
	float4 padding;
};

struct Vertex
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

struct Geo
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : POSITION1;
	float4 sPosition : TEXCOORD1;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

Geo LightVertexShader(Vertex input)
{
	Geo output;
	input.position.w = 1;
	output.position = mul(input.position, world);
	output.worldPos = output.position;
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    
	output.sPosition = mul(input.position, world);
	output.sPosition = mul(output.sPosition, sview);
	output.sPosition = mul(output.sPosition, sproj);

	output.uv = input.uv;
	output.normal = mul(input.normal, (float3x3)world);
	output.normal = normalize(output.normal);

	output.tangent = mul(input.tangent, (float3x3)world);
	output.tangent = normalize(output.tangent);

	output.binormal = mul(input.binormal, (float3x3)world);
	output.binormal = normalize(output.binormal);

	return output;
}