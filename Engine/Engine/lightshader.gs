cbuffer Time : register(c0)
{
	float time;
	float3 padding;
}

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

struct Pixel
{
	float4 position : SV_POSITION;
	float2 uv : TEXCOORD0;
	float3 normal : NORMAL;
	float4 worldPos : POSITION1;
	float4 sPosition : TEXCOORD1;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

[maxvertexcount(3)]
void LightGeoShader(triangle Geo input[3], inout TriangleStream<Pixel> output)
{
	float3 normal = normalize(cross(input[1].position.xyz - input[0].position.xyz, input[2].position.xyz - input[1].position.xyz));
	for(int i = 0; i < 3; i++)
	{
		//input[i].position.x += sin(time) * normal.x;
		//input[i].position.y += sin(time) * normal.y;
		//input[i].position.z += sin(time) * normal.z;
		//input[i].position.xyz += sin(time) * normal;
		output.Append(input[i]);
	}

	output.RestartStrip();
}

//technique11 Tech
//{
	//pass P0
	//{
		//SetVertexShader(CompileShader(vs_5_0, LightVertexShader()));
		//SetGeometryShader(CompileShader(gs_5_0, LightGeoShader()));
		//SetPixelShader(CompileShader(ps_5_0, LightPixelShader()));
	//}
//}