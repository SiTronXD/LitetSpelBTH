
cbuffer CameraBuffer : register(b0)
{
    matrix mvpMatrix;
};

struct Vertex
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

struct Output
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

Output main(Vertex input)
{
    Output output;
    
    output.position = mul(float4(input.position, 1.0f), mvpMatrix);
    output.normal = input.normal;
    output.uv = input.uv;
    
    return output;
}