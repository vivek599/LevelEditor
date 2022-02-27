struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 diffuseColor;
    float3 lightDirection;
    float padding;
};

cbuffer ShaderParameters : register(b1)
{
    float4 TextureUVScale;
    float4 SculptMode;
    float4 TerrainPosition;
    float4 PickedPoint;
    float4 BrushParams;
    float4 TerrainSize;
    float4 DeltaTime;
};

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PixelInputType input) : SV_TARGET 
{
    float2 uv = input.tex * TextureUVScale.x;

    // Sample the pixel color from the texture using the sampler at this texture coordinate location.
    float4 textureColor = shaderTexture.Sample(SampleType, uv);

    // Set the default output color to the ambient light value for all pixels.
    float4 color = ambientColor;

    // Invert the light direction for calculations.
    float3 lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    float lightIntensity = saturate(dot(input.normal, lightDir));

    float diff = max(lightIntensity, 0.0001f);
        
    // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
    color += (diffuseColor * diff);

    // Saturate the final light color.
    color = saturate(color);

    // Multiply the texture pixel and the final light color to get the result.
    color *= textureColor;

    float2 CurrentPixelPosition = 0;
    CurrentPixelPosition.x = input.tex.x * TerrainSize.x;
    CurrentPixelPosition.y = input.tex.y * TerrainSize.z;
    float distanceToPixel = length(CurrentPixelPosition - PickedPoint.xz);
    if (distanceToPixel <= BrushParams.x)
    {
        color *= float4(0.0f, 1.0f, 1.0f, 1.0f);
        color = saturate(color);
    }
    
    return color;
}















