#pragma once

#include "ACommonIncludes.h"

class ARenderDevice;

struct TerrainInitializationParams
{
	const wchar_t* heightMap;
	const wchar_t* pixelSHader;
	const wchar_t* vertexSHader;
	const wchar_t* sculptPixelSHader;
	const wchar_t* sculptVertexSHader;
	vector<const wchar_t*>	textureLayers;
};

class ATerrain
{
public:
	ATerrain();
	~ATerrain();

	bool										Initialize(ARenderDevice* renderDevice, TerrainInitializationParams& params);

	void										Update(ARenderDevice* renderDevice, float deltaTime, Matrix worlMatrix, Matrix viewMatrix, Matrix projMatrix);
	void										Render(ARenderDevice* renderDevice);

	void										RenderTerrain(ARenderDevice* renderDevice);

	int  										GetIndexCount();

	void										SetAmbientColor( Vector4 val );
	void										SetDiffuseColor( Vector4 val );
	void										SetLightDirection( Vector3 val );

	void										SetTextureUVScale(float val);

	uint32_t									GetWidth() const;
	uint32_t									GetHeight() const;
	bool										RayTerrainIntersect(Vector3 rayOrigin, Vector3 rayDirection);
	void										Raise(ARenderDevice* renderDevice, float deltaTime);
	void										Lower(ARenderDevice* renderDevice, float deltaTime);
	void										Flatten(ARenderDevice* renderDevice, float deltaTime);
	void										Smooth(ARenderDevice* renderDevice, float deltaTime);
	void										AlphaMap(ARenderDevice* renderDevice, float deltaTime);
	void										Noise(ARenderDevice* renderDevice, float deltaTime);
	void										Holes(ARenderDevice* renderDevice, float deltaTime);
	void										UpdateHeightMapTexture(ARenderDevice* renderDevice);
	void										SetBrushRadius(int val);
	void										SetBrushStrength(float val);
	bool										SculptingInProgress();
	void										ResetSculptingProgress(ARenderDevice* renderDevice);
	bool										SetCurrentAlphaMap(ARenderDevice* renderDevice, wchar_t* filePath); 
	void SetNoiseScale(int Scale);
	void SetNoiseFreq(int Freq);
	void SetNoiseSeed(int Seed);
	void SetNoiseOctaves(int Octaves);
private:
	struct QuadVertex
	{
		Vector4 Pos;
		Vector2 Tex;
	};

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

	bool										InitGeometry(VertexType*& Vertices, uint32_t*& Indices);
	bool										InitConstantBuffers(ID3D11Device* device);
	bool										LoadHeightMapFromBMP(const wchar_t* heightMapFilePath);
	bool										LoadHeightMapFromPNG(const wchar_t* heightMapFilePath);
	bool										CalculateNormals();
	void										CalculateTextureCoordinates();
	bool										LoadTexture(ID3D11Device* device, vector<const wchar_t*>& textureFilenames);
	void										ShutdownHeightMap();
	void										Erode(int cycles, float dt);
	float										GetHeight(UINT x, UINT z);
	Vector3										GetBestIntersectionPointLineDrawing(Ray ray);
	void										SendSculptingParams(ARenderDevice* renderDevice, float deltaTime, UINT SculptMode);
	void										RenderSculptingQuad(ARenderDevice* renderDevice);
	void										CreateHeightMapStaging(ARenderDevice* renderDevice);
	void										UpdateHeightmapPixelData(ARenderDevice* renderDevice);

	uint32_t m_TerrainWidth;
	uint32_t m_TerrainHeight;
	uint32_t m_VertexCount;
	uint32_t m_IndexCount;

	ComPtr<ID3D11Buffer>	m_VertexBuffer;
	ComPtr<ID3D11Buffer>	m_IndexBuffer;
	ComPtr<ID3D11Buffer>	m_SculptVertexBuffer;

	VertexType*				m_HeightMap;
	VertexType*				m_Vertices;
	uint32_t*				m_Indices;

	unique_ptr<class AShaderCache>	m_PixelShader;
	unique_ptr<class AShaderCache>	m_VertexShader;

	unique_ptr<class AShaderCache>	m_SculptPixelShader;
	unique_ptr<class AShaderCache>	m_SculptVertexShader;

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
		XMUINT4 SculptMode;
		Vector4 TerrainPosition;
		Vector4 PickedPoint;
		Vector4 BrushParams;
		Vector4 TerrainSize;
		Vector4 DeltaTime;
		float  NoiseSeed;
		float  NoiseScale;
		float  NoiseFrequency;
		float  NoiseOctaves;
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
	UINT m_AlphaMapWidth = 0.0f;
	UINT m_AlphaMapHeight = 0.0f;
	int		m_NoiseScale;
	float	m_NoiseFreq;
	int		m_NoiseSeed;
	int		m_NoiseOctaves;

	bool m_bSculptingInProgress = false;
	bool m_bWireFrame;	
	enum ESculptMode : UINT
	{
		NONE		= 0,
		RAISE		= 1,
		LOWER		= 2,
		FLATTEN		= 3,
		SMOOTH		= 4,
		ALPHAMAP	= 5,
		NOISE		= 6,
		HOLES		= 7
	};/*, float raise, float lower, float flatten, float smooth*/
	unique_ptr<class ATexture> m_HeightMapFinal;
	unique_ptr<class ATexture> m_HeightMapRenderTarget;
	unique_ptr<class ATexture> m_AlphaMapTexture;
	ComPtr<ID3D11Texture2D> m_HeightMapStagingTexture;



};

