cbuffer MatrixBuffer : register(b0)
{
	matrix worldMatrix;
	matrix viewMatrix;
	matrix projectionMatrix;
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

Texture2D<float> heightMapTextureFinal : register(t0);

float3 GetVertexNormal(VertexInputType input)
{
	int2 loc0 = input.position.xz; 
	int2 loc1 = loc0 + int2(1, 0);
	int2 loc2 = loc0 + int2(0, 1);
	
	float h0 = heightMapTextureFinal.Load(int3(loc0, 0), 0);
	float h1 = heightMapTextureFinal.Load(int3(loc1, 0), 0);
	float h2 = heightMapTextureFinal.Load(int3(loc2, 0), 0);
	
	float3 v0 = float3(loc0.x, h0, loc0.z)
	float3 v1 = float3(loc1.x, h1, loc1.z)
	float3 v2 = float3(loc2.x, h2, loc2.z)
	
	float3 v10 = v1 - v0;
	float3 v20 = v2 - v0;
	
	return normalize(cross(v10, v20));
}

PixelInputType main(VertexInputType input, uint id : SV_VertexID )
{
    PixelInputType output;

    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;
    
    input.position.y = heightMapTextureFinal.Load(int3(input.position.xz, 0), 0);
     
    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;

    // Calculate the normal vector against the world matrix only.
    //output.normal = mul(input.normal, (float3x3)worldMatrix);
    output.normal = mul(GetVertexNormal(input), (float3x3)worldMatrix);

    // Normalize the normal vector.
    output.normal = normalize(output.normal);

    return output;
}






