
cbuffer LightBuffer : register(b0)
{
    matrix vpLightMatrix;
};

cbuffer DirLightBuffer : register(b1)
{
    float3 lightDir;
    float padding0;
}

cbuffer PixelShaderBuffer : register(b2)
{
    float3 multiplyColor;
    int shade;
}

struct Input
{
    float4 position : SV_POSITION;
    float4 clipPos : POSITION2;
    float3 worldPos : POSITION1;
    float3 worldNormal : NORMAL;
    float2 uv : UV;
};

SamplerState textureSampler : register(s0);
SamplerState depthSampler : register(s1);
Texture2D diffuseTexture : register(t0);
Texture2D sunShadowMap : register(t1);

float4 main(Input input) : SV_TARGET
{
    // Ordered dithering
    const float bayerMatrix8x8[8 * 8] = {
        0.0, 48.0, 12.0, 60.0, 3.0, 51.0, 15.0, 63.0,
        32.0, 16.0, 44.0, 28.0, 35.0, 19.0, 47.0, 31.0,
        8.0, 56.0, 4.0, 52.0, 11.0, 59.0, 7.0, 55.0,
        40.0, 24.0, 36.0, 20.0, 43.0, 27.0, 39.0, 23.0,
        2.0, 50.0, 14.0, 62.0, 1.0, 49.0, 13.0, 61.0,
        34.0, 18.0, 46.0, 30.0, 33.0, 17.0, 45.0, 29.0,
        10.0, 58.0, 6.0, 54.0, 9.0, 57.0, 5.0, 53.0,
        42.0, 26.0, 38.0, 22.0, 41.0, 25.0, 37.0, 21.0
    };

    float2 ndcPos = input.clipPos.xy / input.clipPos.w;

    // Clip against bayer matrix
    int2 pos = int2(((ndcPos + float2(1.0f, 1.0f)) * 0.5f) * 500.0f);
    pos.x = pos.x % 8;
    pos.y = pos.y % 8;

    // Regular color or "background"?
    float3 finalCol = bayerMatrix8x8[pos.x + 8 * pos.y] - 0.2f * 64.0f > 0.0f ? 
        multiplyColor : float3(0.722, 0.871, 0.992);

    return float4(finalCol, 1.0f);
}