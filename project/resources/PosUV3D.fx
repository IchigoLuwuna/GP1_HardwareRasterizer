// -------------
// | Constants |
// -------------
static const float pi = 3.14159265f;
static const float lightIntensity = 7.f;
static const float shininess = 25.f;
static const float3 lightDirection = {0.577f, -0.577f, 0.577f};

// -----------------
// | Scene Globals |
// -----------------
// Camera & Worldspace
float4x4 gWorldViewProj : WorldViewProjection;
float4x4 gWorld : World;
float4 gCameraOrigin;

// Textures
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

// ------------
// | Lighting |
// ------------
float CalculateOA(float3 normal)
{
	return dot(normal, -lightDirection);
}

float3 CalculateLambert(float2 uv)
{
	float3 sampledColor = gDiffuseMap.Sample(gSampler, uv).xyz;
	return sampledColor * lightIntensity / pi;
}

// ---------
// | Extra |
// ---------
float3 GetToCamera(float3 origin)
{
	return (gCameraOrigin.xyz - origin);
}

float3 GetSampledNormal()
{
	// TODO
}

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
	output.Normal = normalize( mul( input.Normal, (float3x3)gWorld ).xyz );
	output.Tangent = normalize( mul( input.Tangent, (float3x3)gWorld ).xyz );
	return output;
}

// Pixel Shader
float4 PxlShader(VS_OUTPUT input) : SV_TARGET
{
	// Calculate view direction
	float3 originToCamera = GetToCamera(input.WorldPosition.xyz);

	// Calculate diffuse
	float3 lambertDiffuse = CalculateLambert(input.UV);

	// Calculate color
	float3 finalColor = lambertDiffuse * CalculateOA(input.Normal);
	finalColor = saturate(finalColor);

	return float4(finalColor, 1.f);
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
