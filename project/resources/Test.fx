cbuffer constants : register(b0)
{
    row_major float4x4 WorldViewProjection;
    row_major float4x4 World;
    row_major float4x4 ViewInverse;
    float4 CameraPosition;
    float4 LightDirInv;
    bool SampleNormals;
}

Texture2D Texture_Diffuse : register(t0);
Texture2D Texture_Specular : register(t1);
Texture2D Texture_Normal : register(t2);
Texture2D Texture_Gloss : register(t3);
SamplerState Sampler : register(s0);

static const float g_PI = 3.141592654f;
static const float g_SpecularReflectance = 1.f;
static const float g_Shininess = 25.f;
static const float3 g_AmbientLight = float3(0.03f, 0.03f, 0.03f);


//---------------------
// Input/Output Structs
//---------------------

struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float4 WorldPosition : WORLDPOSITION;
    float2 TexCoord : TEXCOORD;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
};

//--------------
// Vertex Shader
//--------------

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Position = mul(float4(input.Position, 1.f), WorldViewProjection);
    output.WorldPosition = mul(float4(input.Position, 1.f), World);
    output.TexCoord = input.TexCoord;
    output.Normal = mul(input.Normal, (float3x3)World);
    output.Tangent = mul(input.Tangent, (float3x3)World);

    return output;
}

//-------------
// Pixel Shader
//-------------

float3 GetLambert(float kd, float3 diffuseSample)
{
    return diffuseSample * kd / g_PI;
}

float4 ps_main(VS_OUTPUT input) : SV_TARGET
{
    const float3 diffuseSample = Texture_Diffuse.Sample(Sampler, input.TexCoord).xyz;
    const float3 normalSample = Texture_Normal.Sample(Sampler, input.TexCoord).xyz;
    const float3 specularSample = Texture_Specular.Sample(Sampler, input.TexCoord).xyz;
    const float3 glossSample = Texture_Gloss.Sample(Sampler, input.TexCoord).xyz;

    const float3 viewDir = normalize(input.WorldPosition.xyz - ViewInverse[3].xyz);
    float3 normal = input.Normal;

    if(SampleNormals)
    {
        const float3 binormal = normalize(cross(input.Normal, input.Tangent));
        const float3x3 tangentSpaceAxis =
            float3x3(
                normalize(input.Tangent),
                binormal,
                normalize(input.Normal)
            );

        normal = 2.f * normalSample - 1.f;
        normal = normalize(mul(normal, tangentSpaceAxis));
    }

    const float cosAngle = dot(normal, LightDirInv.xyz);

    const float3 lambert = GetLambert(7.f, diffuseSample);

    // specular (not a function because we'd have to pass pretty much everything)
    float3 specular;
    {
        const float phongExponent = glossSample.r;

        const float3 reflectVec = LightDirInv.xyz - 2 * cosAngle * normal;
        const float cosAlpha = max(dot(reflectVec, viewDir), 0.f);
        const float multiplier = pow(cosAlpha, phongExponent * g_Shininess);
        specular = specularSample * g_SpecularReflectance * multiplier;
    }

    float3 color = lambert + specular + g_AmbientLight;

    return float4(color * cosAngle, 1.f);
}

float4 ps_main_transparent(VS_OUTPUT input) : SV_TARGET
{
    const float4 diffuseSample = Texture_Diffuse.Sample(Sampler, input.TexCoord);

    return diffuseSample;
}

// Technique
technique11 DefaultTechnique
{
	pass P0
	{
		SetVertexShader( CompileShader( vs_5_0, vs_main() ) );
		SetGeometryShader( NULL );
		SetPixelShader( CompileShader( ps_5_0, ps_main() ) );
	}
}
// vim: set ft=hlsl:
