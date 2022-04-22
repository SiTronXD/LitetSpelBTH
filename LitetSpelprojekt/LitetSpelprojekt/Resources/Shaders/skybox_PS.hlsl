struct SKYMAP_PS_INPUT
{
    float3 inPos : POSITION;
    float3 inNormal : NORMAL;
    float3 inTexCoord : UV;
};

TextureCube tex : register(t0);
SamplerState sam : register(s0);

float4 main(SKYMAP_PS_INPUT input) : SV_TARGET
{
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
    return tex.Sample(sam, input.inTexCoord);
}