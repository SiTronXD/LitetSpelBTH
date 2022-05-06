uint wang_hash(uint seed)
{
    seed = uint(seed ^ uint(61)) ^ uint(seed >> uint(16));
    seed *= uint(9);
    seed = seed ^ (seed >> 4);
    seed *= uint(0x27d4eb2d);
    seed = seed ^ (seed >> 15);

    return seed;
}

float randomFloat(float state)
{
    return float(wang_hash(uint(state))) / 4294967296.0;
}

float2 randomFloat(float2 state)
{
    return float2(
        randomFloat(state.x + state.y * 1024.0),
        randomFloat(state.x * 512.0 + state.y * 2048.0)
    );
}

struct particle
{
    matrix worldMatrix;
    float3 velocity;
    float lifetime;
    float scaleFactor;
    float3 finalColor;
};

cbuffer ParticleSystemBuffer : register(b0)
{
    float3 cameraPosition;
    float deltaTime;
    float3 startPosition;
    int start;
    float3 color1;
    float speed;
    float3 color2;
    float lifeTime;
    float randomTimer;
    float3 padding;
};

RWStructuredBuffer<particle> buffer : register(u0);

[numthreads(32, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID )
{
  
    //Reset all if recreated the particle system
    if (start == 1)
    {
        //Set explosion to a circel
        buffer[DTid.x].worldMatrix[3].xyz = startPosition;
        buffer[DTid.x].lifetime = randomFloat(DTid.x * randomTimer) * lifeTime;;
        buffer[DTid.x].scaleFactor = 0.2 + randomFloat(DTid.x * randomTimer) * 0.4;
        
        float angle1 = randomFloat(DTid.x * 2 * randomTimer) * 360.0 / 180.0 * 3.14;
        float angle2 = randomFloat((DTid.x + 1 * 2) * randomTimer) * 360.0 / 180.0 * 3.14;
        float radius = 1 + randomFloat(DTid.x) * speed;
        
        buffer[DTid.x].velocity.x = radius * cos(angle1) * sin(angle2);
        buffer[DTid.x].velocity.y = radius * sin(angle1) * sin(angle2);
        buffer[DTid.x].velocity.z = radius * cos(angle2);
        
        //float angle1 = randomFloat(DTid.x) * 360.0 / 180.0 * 3.14;
        //float angle2 = randomFloat(DTid.x + 1) * 360.0 / 180.0 * 3.14;
        //float radius = 1 + randomFloat(DTid.x) * speed;
        
        //buffer[DTid.x].velocity.x = cos(angle1) * sin(angle2);
        //buffer[DTid.x].velocity.y = sin(angle1) * sin(angle2);
        //buffer[DTid.x].velocity.z = cos(angle2);
        
        //buffer[DTid.x].velocity += float3(0.0f, 1.0f, 0.0f);
        //buffer[DTid.x].velocity = normalize(buffer[DTid.x].velocity) * radius;
        
        //Update color
        float randomColorValue = randomFloat(DTid.x * randomTimer);
        buffer[DTid.x].finalColor = (float4(lerp(color1, color2, randomColorValue), 1.0f));
 
        //buffer[DTid.x].color.x = randomFloat(DTid.x * 3);
        //buffer[DTid.x].color.y = randomFloat(DTid.x * 3 + 1);
        //buffer[DTid.x].color.z = randomFloat(DTid.x * 3 + 2);
           
    }
    
    //No need to update paricle when scale is below 0
    if (buffer[DTid.x].scaleFactor > 0.0f)
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
        if(start == 0)
        {
            buffer[DTid.x].worldMatrix[3].xyz += buffer[DTid.x].velocity * deltaTime;
        }
            
        //Update lifetime
        buffer[DTid.x].lifetime -= 2 * deltaTime;
    }
    //If lifetime is 0 we remove them by setting scale to 0.
    if (buffer[DTid.x].lifetime < 0.0f)
    {
        if (buffer[DTid.x].scaleFactor > 0.0f)
            buffer[DTid.x].scaleFactor -= 0.8f * deltaTime;

    }
    //Update color
    float fadingValue = buffer[DTid.x].lifetime / lifeTime;
    buffer[DTid.x].finalColor = (float4(lerp(color1, color2, fadingValue), 1.0f));
   
}