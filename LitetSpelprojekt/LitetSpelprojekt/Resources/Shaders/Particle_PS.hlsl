struct Input
{
    float4 position : SV_Position;
    float2 uv : UV;
    float4 color : COLOR;
};

SamplerState textureSampler : register(s0);
Texture2D textureTest : register(t0);

float4 main(Input input) : SV_TARGET
{
    float4 texCol = textureTest.Sample(textureSampler, input.uv);

    clip(texCol.a - 0.5);
    
    return texCol = input.color;
}