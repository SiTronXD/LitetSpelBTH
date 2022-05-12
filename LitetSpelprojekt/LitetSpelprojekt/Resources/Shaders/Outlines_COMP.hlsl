RWTexture2D<float4> outputTexture : register(u0);
Texture2D depthTexture : register(t0);

cbuffer InfoBuffer : register(b0)
{
    matrix projectionInv;
    float width;
    float height;
    float thickness;
    float pad;
}

[numthreads(32, 32, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    float2 directions[4] = { float2(-1.0f, -1.0f), float2(1.0f, 1.0f), float2(1.0f, -1.0f), float2(-1.0f, 1.0f) };
    float2 thicknessInPixels = thickness * float2(width, height);

    float4 viewCoords = mul(float4(0.0f, 0.0f, depthTexture[DTid.xy].r, 1.0f), projectionInv);
    float curLinearDepth = viewCoords.z / viewCoords.w;
    float threshold = abs(curLinearDepth) * 0.1f + 1.0f;
    float average = 0.0f;

    for (uint i = 0; i < 4; i++)
    {
        viewCoords = mul(float4(0.0f, 0.0f, depthTexture[DTid.xy + directions[i] * thicknessInPixels].r, 1.0f), projectionInv);
        average += (viewCoords.z / viewCoords.w);
    }
    average /= 4.0f;

    if (abs(curLinearDepth - average) >= threshold)
        outputTexture[DTid.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
}