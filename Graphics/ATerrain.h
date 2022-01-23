#pragma once

#include "ACommonIncludes.h"

class ARenderDevice;

struct TerrainInitializationParams
{
	const wchar_t* heightMap;
	const wchar_t* pixelSHader;
	const wchar_t* vertexSHader;
	vector<const wchar_t*>	textureLayers;
};

class ATerrain
{
public:
	ATerrain();
	~ATerrain();

	bool Initialize(ARenderDevice* renderDevice, TerrainInitializationParams& params);


	
	void Update(ARenderDevice* renderDevice, float deltaTime, Matrix worlMatrix, Matrix viewMatrix, Matrix projMatrix);
	void Render(ARenderDevice* renderDevice);

	int GetIndexCount();

	void SetAmbientColor( Vector4 val );
	void SetDiffuseColor( Vector4 val );
	void SetLightDirection( Vector3 val );

	void SetTextureUVScale(float val);

	uint32_t GetWidth() const;
	uint32_t GetHeight() const;
	bool RayTerrainIntersect(Vector3 rayOrigin, Vector3 rayDirection);
	void Raise(float deltaTime);
	void Lower(float deltaTime);
	void Flatten(float deltaTime);
	void Smooth(float deltaTime);
	void SetBrushRadius(int val);
	void SetBrushStrength(float val);
	void ResetClosestPoint();
private:

	struct VertexType
	{
		Vector4 position;
		Vector2 texture;
		Vector3 normal;
	};

	struct Particle 
	{
		//Construct Particle at Position
		Particle(Vector2 _pos) 
		{ 
			Pos = _pos; 
		}

		Vector2 Pos;
		Vector2 Speed = Vector2(0.0);

		float Volume	= 1.0;   //This will vary in time
		float Sediment	= 0.0; //Fraction of Volume that is Sediment!
	};

	//Particle Properties
	//float dt = 1.2;
	float m_Density				= 1.0;  //This gives varying amounts of inertia and stuff...
	float m_EvapRate			= 0.001;
	float m_DepositionRate		= 0.1;
	float m_MinVol				= 0.01;
	float m_Friction			= 0.05;

	bool InitGeometry(VertexType*& Vertices, uint32_t*& Indices);
	bool InitConstantBuffers(ID3D11Device* device);
	bool LoadHeightMapFromBMP(const wchar_t* heightMapFilePath);
	bool LoadHeightMapFromPNG(const wchar_t* heightMapFilePath);
	void NormalizeHeightMap();
	bool CalculateNormals();
	void CalculateTextureCoordinates();
	bool LoadTexture(ID3D11Device* device, vector<const wchar_t*>& textureFilenames );
	void ShutdownHeightMap();
	void Erode(int cycles, float dt);

	uint32_t m_TerrainWidth;
	uint32_t m_TerrainHeight;
	uint32_t m_VertexCount;
	uint32_t m_IndexCount;

	ComPtr<ID3D11Buffer>	m_VertexBuffer;
	ComPtr<ID3D11Buffer>	m_IndexBuffer;

	VertexType*				m_HeightMap;
	VertexType*				m_Vertices;
	uint32_t*				m_Indices;

	unique_ptr<class AShaderCache>	m_PixelShader;
	unique_ptr<class AShaderCache>	m_VertexShader;

	struct MatrixBufferType
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	struct LightBufferType
	{
		Vector4 ambientColor;
		Vector4 diffuseColor;
		Vector3 lightDirection;
		float padding;
	};

	struct ShaderParametersBuffer
	{
		Vector4 TextureUVScale;
		Vector3 HitPosition;
		float	Radius;
	};

	vector<ID3D11ShaderResourceView*>	m_TerrainTextureSrvLayers;
	vector<ID3D11Resource*>				m_TerrainTextureLayers;

	ComPtr<ID3D11Buffer>	m_MatrixBuffer;
	ComPtr<ID3D11Buffer>	m_LightBuffer;
	ComPtr<ID3D11Buffer>	m_ShaderParametersBuffer;

	Vector4 m_AmbientColor;
	Vector4 m_DiffuseColor;
	Vector3 m_LightDirection;

	Vector4 m_TerrainTextureUVScale;
	Vector3 m_PickedPoint;
	int m_radiusMax;
	float m_strength;
	Vector3 GetBestIntersectionPoint(Ray ray, BoundingBox& outBox);
	Vector3 m_ClosestPoint = Vector3(-1.0f);
};

