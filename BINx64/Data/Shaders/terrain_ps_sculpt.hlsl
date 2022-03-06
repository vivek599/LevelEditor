#include "FastNoiseLite.hlsl"

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

Texture2D<float> HeightMapTexture   : register(t0);
Texture2D<float> AlphaMapTexture    : register(t1);
SamplerState SampleType : register(s0);
 
cbuffer ShaderParameters : register(b0)
{
    float4 TextureUVScale;
    uint4  SculptMode;
    float4 TerrainPosition;
    float4 PickedPoint;
    float4 BrushParams;
    float4 TerrainSize;
    float4 DeltaTime;
    float  NoiseSeed; 
    float  NoiseScale; 
    float  NoiseFrequency;
    float  NoiseOctaves; 
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
    else if (SculptMode.x == 2)//Lower
    {
        if (dist <= BrushParams.x)
        {
            SculptOffset -= BrushParams.y * smoothstep(0, BrushParams.x, BrushParams.x - dist) * DeltaTime.x * 10.0f;
        }
    }
    else if (SculptMode.x == 3)//Flatten
    {
        if (dist <= BrushParams.x)
        {
            SculptOffset = PickedPoint.y;
        }
    }
    else if (SculptMode.x == 4)//Smooth
    {
        if (dist <= BrushParams.x)
        {
            int smoothRadius = 1;
            int samplesTaken = 0;
            float avgY = 0.0f;
            for (int k = -smoothRadius; k < smoothRadius; k++)
            {
                for (int l = -smoothRadius; l < smoothRadius; l++)
                {
                    float2 smoothingPos = input.tex + float2(k, l) / TerrainSize.x;
                    float radiusLocal = length(int2(k, l));
                    avgY += radiusLocal > smoothRadius ? 0.0f : HeightMapTexture.Sample(SampleType, smoothingPos);
                    samplesTaken += (radiusLocal > smoothRadius ? 0 : 1);
                }
            }
            avgY /= samplesTaken;
            
            SculptOffset = avgY;
        }
    }
    else if (SculptMode.x == 5)//AlphaMap
    {
        float2 AlphaMapTextureSize = 0;
        AlphaMapTexture.GetDimensions(AlphaMapTextureSize.x, AlphaMapTextureSize.y);
        
        float2 AlphaTexCoordMin = (PickedPoint.xz / TerrainSize.x) - (AlphaMapTextureSize * 0.5f / TerrainSize.x);
        float2 AlphaTexCoordMax = (PickedPoint.xz / TerrainSize.x) + (AlphaMapTextureSize * 0.5f / TerrainSize.x);
        
        
        if (AlphaTexCoordMin.x < input.tex.x &&
            AlphaTexCoordMin.y < input.tex.y &&
            input.tex.x < AlphaTexCoordMax.x &&
            input.tex.y < AlphaTexCoordMax.y)
        {
            float2 alphatex = smoothstep(AlphaTexCoordMin, AlphaTexCoordMax, input.tex);
            SculptOffset += AlphaMapTexture.Sample(SampleType, alphatex);
        }
    }
    else if (SculptMode.x == 6)//Noise
    {
        if (dist <= BrushParams.x)
        {
            fnl_state noise     = fnlCreateState(NoiseSeed);
            noise.noise_type    = FNL_NOISE_OPENSIMPLEX2;
            noise.frequency     = NoiseFrequency;
            noise.octaves       = NoiseOctaves;
            noise.fractal_type  = FNL_FRACTAL_FBM;
            
            SculptOffset = NoiseScale * fnlGetNoise2D(noise, input.tex.x * TerrainSize.x, input.tex.y * TerrainSize.x);
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
    if (SculptMode.x == 3 || SculptMode.x == 4 || SculptMode.x == 6)//Flatten or Smooth or Noise
    {
        if (dist <= BrushParams.x)
            return Sculpt(input);
        else
            return HeightMapTexture.Sample(SampleType, input.tex);
    }
    else if ( SculptMode.x == 5)
    {
        float2 AlphaMapTextureSize = 0;
        AlphaMapTexture.GetDimensions(AlphaMapTextureSize.x, AlphaMapTextureSize.y);
        
        float2 AlphaTexCoordMin = (PickedPoint.xz / TerrainSize.x) - (AlphaMapTextureSize * 0.5f / TerrainSize.x);
        float2 AlphaTexCoordMax = (PickedPoint.xz / TerrainSize.x) + (AlphaMapTextureSize * 0.5f / TerrainSize.x);
        
        if (AlphaTexCoordMin.x < input.tex.x &&
            AlphaTexCoordMin.y < input.tex.y &&
            input.tex.x < AlphaTexCoordMax.x &&
            input.tex.y < AlphaTexCoordMax.y)
        {
            float sculptval = Sculpt(input);
            return sculptval < 5.0f ? HeightMapTexture.Sample(SampleType, input.tex) : sculptval;
        }
        else
            return HeightMapTexture.Sample(SampleType, input.tex);
    }
    else
    {
        return HeightMapTexture.Sample(SampleType, input.tex) + Sculpt(input);
    }
}

































