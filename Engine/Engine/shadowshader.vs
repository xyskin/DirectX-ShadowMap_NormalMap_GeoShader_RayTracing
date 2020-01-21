cbuffer Matrix : register(b0)
{
	matrix world;
	matrix view;
	matrix proj;
};

cbuffer Variable : register(b1)
{
	
};

struct Vertex
{
	float4 position : POSITION0;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
};

struct Pixel
{
	float4 position : SV_POSITION;
	//float2 uv : TEXCOORD0;
	//float3 normal : NORMAL;
	//float4 worldPos : POSITION1;
	//float4 depthPos : POSITION0;
};

Pixel ShadowVertexShader(Vertex input)
{
	Pixel output;
	input.position.w = 1;
	output.position = mul(input.position, world);
	//output.worldPos = output.position;
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    
	//output.uv = input.uv;
	//output.normal = mul(input.normal, (float3x3)world);
	//output.normal = normalize(output.normal);

	//output.depthPos = output.position;

	return output;
}