
cbuffer UIOrientationBuffer : register(b0)
{
    int2 position;
    int2 textureSize;
    int2 uiSize;
    float2 padding2;
}

Texture2D<float4> imageTexture : register(t0);

RWTexture2D<float4> outputTexture : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint2 imagePos = uint2((float2(dispatchThreadID.xy) / uiSize) * textureSize);
    float4 col = imageTexture[imagePos];

    if(col.a > 0.5f)
        outputTexture[position + dispatchThreadID.xy] = float4(col.rgb, 1.0f);
}