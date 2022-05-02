struct particle
{
    matrix worldMatrix;
    float3 velocity;
    float lifetime;
    float scaleFactor;
    float3 padding;
};

cbuffer CameraBuffer : register(b0)
{
    float3 cameraPosition;
    float deltaTime;
    float3 startPosition; //FIX Startposition
    int start;
};

RWStructuredBuffer<particle> buffer : register(u0);

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
    //Caculate the direction
    float3 forwardVector = cameraPosition - buffer[DTid.x].worldMatrix[3].xyz;
    forwardVector = normalize(forwardVector);
    
    //Rotate thoughts the camera
    float3 rightVector = normalize(cross(float3(0.0f, 1.0f, 0.0f), forwardVector));
    float3 upVector = cross(forwardVector, rightVector);
    
    buffer[DTid.x].worldMatrix[0] = float4(rightVector, 0.0f) * buffer[DTid.x].scaleFactor;
    buffer[DTid.x].worldMatrix[1] = float4(upVector, 0.0f) * buffer[DTid.x].scaleFactor;
    buffer[DTid.x].worldMatrix[2] = float4(forwardVector, 0.0f) * buffer[DTid.x].scaleFactor;
    
    //Update position
    buffer[DTid.x].worldMatrix[3].xyz += buffer[DTid.x].velocity * deltaTime;
      
    //Update lifetime
    buffer[DTid.x].lifetime--;
  
}