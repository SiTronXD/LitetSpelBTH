cbuffer ParticleSystemBuffer : register(b0)
{
    matrix modelMatrix;
    matrix vpMatrix;
};

struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    uint instanceID : SV_InstanceID;
};

struct particle
{
    matrix worldMatrix;
    float3 velocity;
    float lifetime;
    float scaleFactor;
    float3 finalColor;
};

struct Output
{
    float4 position : SV_Position;
    float2 uv : UV;
    float4 color : COLOR;
};

StructuredBuffer<particle> particle_OUT : register(t0);

Output main(Vertex input)
{
    Output out_put;
    out_put.position = mul(float4(input.position, 1.0f), particle_OUT[input.instanceID].worldMatrix);
    out_put.position = mul(out_put.position, vpMatrix);
    out_put.uv = input.uv;
    out_put.color = float4(particle_OUT[input.instanceID].finalColor, 1.0f);
    return out_put;
}