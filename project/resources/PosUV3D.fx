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
float4 gCameraOrigin : CameraOrigin;

// Textures
Texture2D gDiffuseMap : DiffuseMap;
Texture2D gNormalMap : NormalMap;
Texture2D gSpecularMap : SpecularMap;
Texture2D gGlossMap : GlossMap;
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

// ---------
// | Extra |
// ---------
float3 GetToCamera(float3 origin)
{
	return normalize(gCameraOrigin.xyz - origin);
}

float3 GetSampledNormal()
{
	// TODO
}

float3 MultColor(float3 a, float3 b)
{
	return float3(a.r * b.r, a.g * b.g, a.b * b.b);
}

// ------------
// | Lighting |
// ------------
float CalculateOA(float3 normal)
{
	return dot(normal, -lightDirection);
}

float3 CalculateLambert(float3 diffuseColor, float diffuseReflectance)
{
	return diffuseColor * diffuseReflectance / pi;
}

float3 CalculatePhong(float3 specularReflect, float phongExponent, float3 lightInDir, float3 camToDir, float3 normal)
{
	float3 reflectLight = reflect(lightInDir, normal);
	float closingFactor = max(dot(reflectLight, camToDir), 0.f);
	return specularReflect * pow(closingFactor, phongExponent);
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
	float3 sampledColor = gDiffuseMap.Sample(gSampler, input.UV).rgb;
	float3 lambertDiffuse = CalculateLambert(sampledColor, lightIntensity);

	// Calculate phong
	float3 sampledSpecular = gSpecularMap.Sample(gSampler, input.UV).rgb;
	float sampledGloss = gGlossMap.Sample(gSampler, input.UV).r;
	float phongExponent = sampledGloss * shininess;
	float3 phongSpecular = CalculatePhong(sampledSpecular, phongExponent, lightDirection, originToCamera, input.Normal);

	// Calculate final color
	float3 brdf = lambertDiffuse + phongSpecular;
	float3 finalColor = brdf * CalculateOA(input.Normal);
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
