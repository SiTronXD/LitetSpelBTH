
cbuffer CameraBuffer : register(b0)
{
    matrix modelMatrix;
    matrix vpMatrix;
};

struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float4 boneWeights : TEXCOORD1;
    int4 boneIndices : TEXCOORD2;
};

struct Output
{
    float4 position : SV_POSITION;
    float4 clipPos : POSITION2;
    float3 worldPos : POSITION1;
    float3 worldNormal : NORMAL;
    float2 uv : UV;
};

StructuredBuffer<float4x4> boneTransformations : register(t0);

Output main(Vertex input)
{
    Output output;

    // Bone transformations
    float4x4 skeletonTransformation = float4x4(
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    );
    if (input.boneIndices.x >= 0)
        skeletonTransformation += boneTransformations[input.boneIndices.x] * input.boneWeights.x;
    if (input.boneIndices.y >= 0)
        skeletonTransformation += boneTransformations[input.boneIndices.y] * input.boneWeights.y;
    if (input.boneIndices.z >= 0)
        skeletonTransformation += boneTransformations[input.boneIndices.z] * input.boneWeights.z;
    if (input.boneIndices.w >= 0)
        skeletonTransformation += boneTransformations[input.boneIndices.w] * input.boneWeights.w;

    // Regular setup
    output.position = mul(float4(input.position, 1.0f), skeletonTransformation);
    output.position = mul(output.position, modelMatrix);
    output.worldPos = output.position;
    output.position = mul(output.position, vpMatrix);
    output.worldNormal = mul(float4(input.normal, 0.0f), modelMatrix).xyz;
    output.uv = input.uv;
    output.clipPos = output.position;

    return output;
}