struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD;
};

PixelInputType main(VertexInputType input, uint id : SV_VertexID )
{
    PixelInputType output;

    input.position.w = 1.0f;

    output.position = input.position;
    output.tex = input.tex;
    
    return output;
}






