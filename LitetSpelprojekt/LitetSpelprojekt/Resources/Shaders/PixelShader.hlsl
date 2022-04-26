struct Input
{ 
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

SamplerState textureSampler : register(s0);
Texture2D diffuseTexture : register(t0);
Texture2D sunShadowMap : register(t1);

float4 main(Input input) : SV_TARGET
{
    //return float4(input.uv, 1.0f, 1.0f);

    return sunShadowMap.Sample(textureSampler, input.uv);

    /*return diffuseTexture.Sample(textureSampler, input.uv); /*
        dot(input.normal, normalize(float3(1,1,1)));*/
}