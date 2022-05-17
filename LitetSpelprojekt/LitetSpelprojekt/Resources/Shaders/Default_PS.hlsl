
cbuffer LightBuffer : register(b0)
{
    matrix vpLightMatrix;
};

cbuffer DirLightBuffer : register(b1)
{
    float3 lightDir;
    float padding0;
    float3 globalColor;
    float padding1;
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

#define SHADOW_BIAS 0.012f
#define SHADOW_MAP_SIZE 512.0f

float4 main(Input input) : SV_TARGET
{
    // Ordered dithering fog
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

    float3 ndcPos = input.clipPos.xyz / input.clipPos.w;

    // Fog value
    float fog = (ndcPos.z - 0.9998f) / (1.0f - 0.9998f);
    fog = clamp(fog, 0.0f, 1.0f);

    // Clip against bayer matrix
    int2 pos = int2(((ndcPos.xy + float2(1.0f, 1.0f)) * 0.5f) * 500.0f);
    pos.x = pos.x % 8;
    pos.y = pos.y % 8;
    clip(bayerMatrix8x8[pos.x + 8 * pos.y] - fog * 64.0f);

    // Clip transparent pixels
    float4 texCol = diffuseTexture.Sample(textureSampler, input.uv);
    clip(texCol.a - 0.5f);

    float shadowFactor = 1.0f;

    // Shadow mapping + diffuse light
    if (shade)
    {
        // Light space
        float4 lightPos = mul(float4(input.worldPos, 1.0f), vpLightMatrix);
        lightPos /= lightPos.w;
        lightPos.xy = (lightPos.xy + float2(1.0f, 1.0f)) * 0.5f;
        lightPos.y *= -1.0f;

        float2 invSize = 1.0f / SHADOW_MAP_SIZE;
        float2 lightPosCorner = lightPos.xy * SHADOW_MAP_SIZE;
        float2 floorLightPosCorner = floor(lightPosCorner);
        float2 fractLightPosCorner = frac(lightPosCorner);

        // Compare
        float lightDepth0 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner)*invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;
        float lightDepth1 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner + float2(1.0f, 0.0f)) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;
        float lightDepth2 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner + float2(0.0f, 1.0f)) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;
        float lightDepth3 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner + float2(1.0f, 1.0f)) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;

        // Interpolate
        float edge0 = lerp(lightDepth0, lightDepth1, fractLightPosCorner.x);
        float edge1 = lerp(lightDepth2, lightDepth3, fractLightPosCorner.x);
        float shadowMapFactor = lerp(edge0, edge1, fractLightPosCorner.y);

        // Shadow map + diffuse light
        shadowFactor = lerp(
            0.4f,
            1.0f,
            shadowMapFactor *
            saturate(dot(input.worldNormal, -lightDir))
        );
    }

    float4 finalCol = texCol * shadowFactor * float4(multiplyColor, 1.0f) * 
        float4(lerp(float3(1.0f, 1.0f, 1.0f), globalColor, 0.6f), 1.0f);

    return finalCol;
}