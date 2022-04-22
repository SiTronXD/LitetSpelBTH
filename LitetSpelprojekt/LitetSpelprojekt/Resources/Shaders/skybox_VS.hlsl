cbuffer matrixbuffer : register(b0)
{
    matrix mvpMatrix;
    float3 cameraPosition;
    float padding;
};

struct SKYMAP_VS_OUTPUT
{
    float4 outPos : SV_POSITION;
    float3 outTexCoord : UV;
};

struct SKYMAP_VS_INPUT
{
    float3 inPos : POSITION;
    float3 inNormal : NORMAL;
    float3 inTexCoord : UV;
};


SKYMAP_VS_OUTPUT main(SKYMAP_VS_INPUT input)
{
    SKYMAP_VS_OUTPUT output;
    
    output.outPos = mul(float4(input.inPos + cameraPosition, 1.0f), mvpMatrix).xyww; //Make sure that our Z value is 1.0,
    output.outTexCoord = input.inPos; //Our textcoord will be defined by our vertex poistion.

    return output;
}