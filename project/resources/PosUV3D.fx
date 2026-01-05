// -------------
// | Constants |
// -------------
static const float pi = 3.14159265f;

// -----------
// | Globals |
// -----------
float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorld : World;
Texture2D gDiffuseMap : DiffuseMap;
SamplerState gSampler : Sampler;

// -----------
// | Structs |
// -----------
struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Color : COLOR;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
	float4 Position : SV_POSITION;
	float4 WorldPosition : WORLD;
	float3 Color : COLOR;
	float2 UV : TEXCOORD;
	float3 Normal : NORMAL;
	float3 Tangent : TANGENT;
};


// -----------
// | Shaders |
// -----------
// Vertex Shader
VS_OUTPUT VtxShader(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Position = mul( float4( input.Position, 1.f ), gWorldViewProj );
	output.WorldPosition = mul( float4( input.Position, 1.f ), gWorld );
	output.Color = input.Color;
	output.UV = input.UV;
	output.Normal = normalize( mul( float4( input.Normal, 0.f ), gWorld ).xyz );
	output.Tangent = normalize( mul( float4( input.Tangent, 0.f ), gWorld ).xyz );
	return output;
}

// Pixel Shader
float4 PxlShader(VS_OUTPUT input) : SV_TARGET
{
	float4 sampledColor = gDiffuseMap.Sample(gSampler, input.UV);
	return sampledColor;
}

// --------------
// | Techniques |
// --------------
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
