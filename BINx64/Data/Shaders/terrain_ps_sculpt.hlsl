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
        const float offset5[5] = { 0.0f, 1.0f, 2.0f, 3.0f, 4.0f };
        const float weight5[5] = {0.2270270270f, 0.1945945946f, 0.1216216216f, 0.0540540541f, 0.0162162162f};
        
        if (dist <= BrushParams.x)
        {
            float avgY = HeightMapTexture.Sample(SampleType, input.tex) * weight5[0];
            for (int i = 1; i < 5; i++)
            {
                float offsetX = offset5[i] / TerrainSize.x;
                float offsetY = offset5[i] / TerrainSize.x;
                
                avgY += HeightMapTexture.Sample(SampleType, input.tex + float2(offsetX, 0.0f )) * weight5[i] * 0.5f;
                avgY += HeightMapTexture.Sample(SampleType, input.tex - float2(offsetX, 0.0f)) * weight5[i] * 0.5f;
                
                avgY += HeightMapTexture.Sample(SampleType, input.tex + float2(0.0f, offsetY)) * weight5[i] * 0.5f;
                avgY += HeightMapTexture.Sample(SampleType, input.tex - float2(0.0f, offsetY)) * weight5[i] * 0.5f;
            }
            
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
    else if (SculptMode.x == 7)//Holes
    {
        if (dist <= BrushParams.x)
        {
            SculptOffset = float(0xDEADBEEF);
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
    if (SculptMode.x == 3 || SculptMode.x == 4 || SculptMode.x == 6 || SculptMode.x == 7)//Flatten or Smooth or Noise or Holes
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

































