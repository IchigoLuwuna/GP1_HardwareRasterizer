// Input/Output
struct VS_INPUT
{
	float3 position : POSITION;
	float3 color : COLOR;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 color : COLOR;
};

// Vertex Shader
VS_OUTPUT VtxShader(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.position = float4(input.position, 1.f);
	output.color = input.color;
	return output;
}

// Pixel Shader
float4 PxlShader(VS_OUTPUT input) : SV_TARGET
{
	return float4(input.color,1.f);
}

// Technique
technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, VtxShader() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, PxlShader() ) );
	}
}
