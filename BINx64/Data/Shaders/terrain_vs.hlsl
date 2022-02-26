cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
};

cbuffer SculptingBuffer : register(b1)
{
    float4 SculptMode;
    float4 TerrainPosition;
    float4 PickedPoint;
    float4 BrushRadius;
    float4 TerrainSize;
    float4 DeltaTime;
};

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

RWTexture2D<float> PositionOffset : register(u1);

static const float PI = 3.14159265f;

void Sculpt(int2 VertexPosition)
{
    if (SculptMode.x == 1)//Raise
    {
        float dist = length(int2(PickedPoint.xz) - VertexPosition);
        if (dist <= BrushRadius.x)
        {
            //PositionOffset[VertexPosition] += (BrushRadius.y * (cos(PI * dist / float(BrushRadius.x)) + 1.0f) * 0.5f) * DeltaTime.x * 10.0f;
            PositionOffset[VertexPosition] += BrushRadius.y * smoothstep(0, BrushRadius.x, BrushRadius.x - dist) * DeltaTime.x * 10.0f;
        }
    }
    else if (SculptMode.y == 1)//Lower
    {
        float dist = length(uint2(PickedPoint.xz) - VertexPosition);
        if (dist <= BrushRadius.x)
        {
            PositionOffset[VertexPosition] -= BrushRadius.y * smoothstep(0, BrushRadius.x, BrushRadius.x - dist) * DeltaTime.x * 10.0f;
        }
    }
    else if (SculptMode.z == 1)//Flatten
    {
        float dist = length(int2(PickedPoint.xz) - VertexPosition);
        if (dist <= BrushRadius.x)
        {
            PositionOffset[VertexPosition] = PickedPoint.y;
        }
    }
    else if (SculptMode.w == 1)//Smooth
    {
        float dist = length(int2(PickedPoint.xz) - VertexPosition);
        if (dist <= BrushRadius.x)
        {
            int smoothRadius = 5;
            int samplesTaken = 0;
            float avgY = 0.0f;
            for (int k = -smoothRadius; k < smoothRadius; k++)
            {
                for (int l = -smoothRadius; l < smoothRadius; l++)
                {
                    int2 smoothingPos = int2(VertexPosition + int2(k, l));
                    float radiusLocal = length(int2(k, l));
                    avgY += radiusLocal > smoothRadius ? 0.0f : PositionOffset[smoothingPos];
                    radiusLocal > smoothRadius ? samplesTaken : samplesTaken++;
                }
            }
            avgY /= samplesTaken;

            float adjustment = BrushRadius.y * smoothstep(0, BrushRadius.x, BrushRadius.x - dist) * DeltaTime.x * avgY * 0.1f;
            if (PositionOffset[VertexPosition] > avgY)
            {
                PositionOffset[VertexPosition] -= adjustment;
            }
            else
            {
                PositionOffset[VertexPosition] += adjustment;
            }
             
        }
    }
    else
    {
        
    }
}

PixelInputType main(VertexInputType input, uint id : SV_VertexID )
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    int2 VertexPosition = int2(input.position.xz);
    
    Sculpt(VertexPosition);
    
    input.position.y = PositionOffset[VertexPosition];
     
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}






