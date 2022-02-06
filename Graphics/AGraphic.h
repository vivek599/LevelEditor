#pragma once

#include "../Graphics/ACommonIncludes.h"

class TerrainInitializationParams;

enum ESculptMode
{
	RAISE,
	LOWER,
	FLATTEN,
	SMOOTH
};

class AGraphic
{
public:
	AGraphic(HWND hwnd, int screenWidth, int screenHeight, bool vsyncEnabled, bool full_screen);
	~AGraphic();

	bool InitializeTerrain(TerrainInitializationParams* params);
	void BeginScene(Color color);
	bool Update(float deltaTime);

	void SculptTerrain(float deltaTime);

	bool Render();
	void EndScene();
	void Resize(int width, int height);

	Matrix GetProjectionMatrix();
	Matrix GetWorldMatrix();
	Matrix GetOrthoMatrix();

	void SetTextureUVScale(float val);
	bool UnprojectMouseCoord();
	void SetMouseState(int mouseX, int mouseY, bool mouseDown, bool mouseMoving);
	void SetSculptRadius(int radius);
	void SetSculptStrenght(int strength);
private:

	Matrix m_ProjectionMatrix;
	Matrix m_WorldMatrix;
	Matrix m_OrthoMatrix;

	class ARenderDevice*	m_RenderDevice;

	unique_ptr<class ACemara>	m_Camera;

	ComPtr<ID3D11Device>				m_Device;
	ComPtr<ID3D11DeviceContext>			m_DeviceContext;
	Viewport				m_Viewport;
	float					m_FieldOfView;
	float					m_AspectRatio;
	float					m_Near;
	float					m_Far;
	float					m_OrthoNear;
	float					m_OrthoFar;
	Vector3					m_RayDirection;
	Vector3					m_RayOrigin;

	bool					m_ResizeSuccess;
	bool					m_TerrainInitilized;

	unique_ptr<class ATerrain>	m_Terrain;
	float						m_TextureUVScale;

	int						m_ScreenWidth;
	int						m_ScreenHeight;
	int						m_MouseX;
	int						m_MouseY;
	bool					m_LeftMouseDown;
	ESculptMode				m_TerrainSculptmode;

	bool m_MouseMoving;
public:
	void SetTerrainSculptMode(ESculptMode mode);
};

