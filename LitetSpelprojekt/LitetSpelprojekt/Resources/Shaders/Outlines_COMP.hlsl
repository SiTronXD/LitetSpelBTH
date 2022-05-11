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
    float4 viewPositions[4];
    float2 thicknessInPixels = thickness * float2(width, height);
    float threshold = 1.0f;
    
    float4 viewCoords = mul(float4(0.0f, 0.0f, depthTexture[DTid.xy].r, 1.0f), projectionInv);
    float curLinearDepth = viewCoords.z / viewCoords.w;
    float average = 0.0f;
    
    // Calc depth values
    for (uint i = 0; i < 4; i++)
    {
        float2 pixelpos = ((DTid.xy + directions[i] * thicknessInPixels) / float2(width, height) * 2 - float2(1.0f, 1.0f));
        pixelpos.y *= -1.0f;
        viewPositions[i] = mul(float4(pixelpos, depthTexture[DTid.xy + directions[i] * thicknessInPixels].r, 1.0f), projectionInv);
        viewPositions[i].xyz /= viewPositions[i].w;
        average += viewPositions[i].z;
    }
    average /= 4.0f;
    
    // Calc normals
    float3 n1 = -normalize(cross((viewPositions[0] - viewPositions[2]).xyz, (viewPositions[0] - viewPositions[3]).xyz));
    float3 n2 = normalize(cross((viewPositions[1] - viewPositions[2]).xyz, (viewPositions[1] - viewPositions[3]).xyz));
    
    if (abs(curLinearDepth - average) >= threshold/* && dot(n1, n2) <= 0.9f*/)
        outputTexture[DTid.xy] = float4(0.0f, 0.0f, 0.0f, 1.0f);
    outputTexture[DTid.xy] = float4(dot(n1, n2), dot(n1, n2), dot(n1, n2), 1.0f);
}