
cbuffer DirLightBuffer : register(b1)
{
    float3 lightDir;
    float shadowMapSize;
    float3 globalColor;
    float biasScale;
}

struct SKYMAP_PS_INPUT
{
    float4 inPos : SV_POSITION;
    float3 inTexCoord : UV;
};

TextureCube tex : register(t0);
SamplerState sam : register(s0);

float4 main(SKYMAP_PS_INPUT input) : SV_TARGET
{
    return tex.Sample(sam, input.inTexCoord) * float4(globalColor, 1.0f);
}