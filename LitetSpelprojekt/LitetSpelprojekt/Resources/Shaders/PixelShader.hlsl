struct Input
{ 
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};

float4 main(Input input) : SV_TARGET
{
    return float4(input.uv, 1.0f, 1.0f);
}