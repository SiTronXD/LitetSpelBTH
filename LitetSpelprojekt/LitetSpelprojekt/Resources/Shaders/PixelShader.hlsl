
cbuffer LightBuffer : register(b0)
{
    matrix vpLightMatrix;
};

struct Input
{ 
    float4 position : SV_POSITION;
    float3 worldPos : POSITION1;
    float3 normal : NORMAL;
    float2 uv : UV;
};

SamplerState textureSampler : register(s0);
SamplerState depthSampler : register(s1);
Texture2D diffuseTexture : register(t0);
Texture2D sunShadowMap : register(t1);

#define SHADOW_BIAS 0.01f
#define SHADOW_MAP_SIZE 512.0f

float4 main(Input input) : SV_TARGET
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
    float lightDepth0 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner                     ) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;
    float lightDepth1 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner + float2(1.0f, 0.0f)) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;
    float lightDepth2 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner + float2(0.0f, 1.0f)) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;
    float lightDepth3 = sunShadowMap.Sample(depthSampler, (floorLightPosCorner + float2(1.0f, 1.0f)) * invSize) + SHADOW_BIAS >= lightPos.z ? 1.0f : 0.0f;

    // Interpolate
    float edge0 = lerp(lightDepth0, lightDepth1, fractLightPosCorner.x);
    float edge1 = lerp(lightDepth2, lightDepth3, fractLightPosCorner.x);
    float shadowFactor = lerp(edge0, edge1, fractLightPosCorner.y);

    float4 texCol = diffuseTexture.Sample(textureSampler, input.uv);

    return texCol * lerp(0.4f, 1.0f, shadowFactor); /*
        dot(input.normal, normalize(float3(1,1,1)));*/
}