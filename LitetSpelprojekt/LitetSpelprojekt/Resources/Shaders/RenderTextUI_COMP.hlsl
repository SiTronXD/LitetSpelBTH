
cbuffer TextOrientationBuffer : register(b0)
{
    int2 position;
    int2 textureSize;
    int2 uiSize;
    int2 cutoutRectPos;
    int2 cutoutRectSize;

    float2 padding0;

    float4 color;
}

Texture2D<float4> imageTexture : register(t0);

RWTexture2D<float4> outputTexture : register(u0);

[numthreads(1, 1, 1)]
void main(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    uint2 imagePos = uint2((float2(dispatchThreadID.xy) / uiSize) * cutoutRectSize.xy);
    float4 col = imageTexture[cutoutRectPos.xy + imagePos];

    if (col.a > 0.5f)
        outputTexture[position + dispatchThreadID.xy] = float4(col.rgb, 1.0f) * color;
    /*else
        outputTexture[position + dispatchThreadID.xy] = float4(float2(dispatchThreadID.xy) / uiSize, 0.0f, 1.0f);*/
}