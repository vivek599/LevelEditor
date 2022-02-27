struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D<float> heightMapTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer ShaderParameters : register(b0)
{
    float4 TextureUVScale;
    float4 SculptMode;
    float4 TerrainPosition;
    float4 PickedPoint;
    float4 BrushParams;
    float4 TerrainSize;
    float4 DeltaTime;
};

float Sculpt(PixelInputType input)
{
    float SculptOffset = 0.0f;
     
    float dist = length(int2(PickedPoint.xz) - int2(input.tex * TerrainSize.x));
    
    if (SculptMode.x == 1)//Raise
    {
        if (dist <= BrushParams.x)
        {
            SculptOffset += BrushParams.y * smoothstep(0, BrushParams.x, BrushParams.x - dist) * DeltaTime.x * 10.0f;
        }
    }
    else if (SculptMode.y == 1)//Lower
    {
        if (dist <= BrushParams.x)
        {
            SculptOffset -= BrushParams.y * smoothstep(0, BrushParams.x, BrushParams.x - dist) * DeltaTime.x * 10.0f;
        }
    }
    else if (SculptMode.z == 1)//Flatten
    {
        if (dist <= BrushParams.x)
        {
            SculptOffset = PickedPoint.y;
        }
    }
    else if (SculptMode.w == 1)//Smooth
    {
        if (dist <= BrushParams.x)
        {
            float2 VertexTexCoord = input.tex;
            VertexTexCoord.y = 1.0f - VertexTexCoord.y;
            
            int smoothRadius = 5;
            int samplesTaken = 0;
            float avgY = 0.0f;
            for (int k = -smoothRadius; k < smoothRadius; k++)
            {
                for (int l = -smoothRadius; l < smoothRadius; l++)
                {
                    float2 smoothingPos = VertexTexCoord + float2(k, l)/TerrainSize.x;
                    float radiusLocal = length(int2(k, l));
                    avgY += radiusLocal > smoothRadius ? 0.0f : heightMapTexture.Sample(SampleType, smoothingPos);
                    radiusLocal > smoothRadius ? samplesTaken : samplesTaken++;
                }
            }
            avgY /= samplesTaken;

            float localHeight = heightMapTexture.Sample(SampleType, VertexTexCoord);
            //float adjustment = BrushParams.y * smoothstep(0, BrushParams.x, BrushParams.x - dist) * DeltaTime.x * 10.0f;
            float adjustment = smoothstep(0, BrushParams.x, BrushParams.x - dist) * DeltaTime.x * 10.0f;
            if (localHeight > avgY)
            {
                SculptOffset -= adjustment;
            }
            else
            {
                SculptOffset += adjustment;
            } 
        }
    }
    else
    {
        
    }
    
    return SculptOffset;
}

float main(PixelInputType input) : SV_TARGET 
{  
    float dist = length(int2(PickedPoint.xz) - int2(input.tex * TerrainSize.x));
    if (SculptMode.z == 1)//Flatten
    {
        if (dist <= BrushParams.x)
            return Sculpt(input);
        else
            return heightMapTexture.Sample(SampleType, input.tex);
    }
    else
    {
        return heightMapTexture.Sample(SampleType, input.tex) + Sculpt(input);
    }
}

































