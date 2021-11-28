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

    if (lightIntensity > 0.0f)
    {
        // Determine the final diffuse color based on the diffuse color and the amount of light intensity.
        color += (diffuseColor * lightIntensity);
    }

    // Saturate the final light color.
    color = saturate(color);

    // Multiply the texture pixel and the final light color to get the result.
    color = color * textureColor;

    return color;
}















