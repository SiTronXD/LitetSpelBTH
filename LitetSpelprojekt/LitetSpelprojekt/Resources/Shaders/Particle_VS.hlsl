cbuffer CameraBuffer : register(b0)
{
    matrix modelMatrix;
    matrix vpMatrix;
};

struct particle
{
    matrix worldMatrix;
};

StructuredBuffer<particle> particle_OUT : register(t0);

float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 out_pos = mul(float4(pos, 1.0f), particle_OUT[0].worldMatrix);
    out_pos = mul(out_pos, vpMatrix);
    return out_pos;
}