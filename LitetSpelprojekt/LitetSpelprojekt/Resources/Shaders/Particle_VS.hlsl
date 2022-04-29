cbuffer matrixBuffer : register(b0)
{
    matrix worldMatrix;
};

float4 main( float3 pos : POSITION ) : SV_POSITION
{
    float4 out_pos = mul(float4(pos, 1.0f), worldMatrix);      
    return out_pos;
}