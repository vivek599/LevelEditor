#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <shobjidl.h> //open file dialog

#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11.h>
#include <dxgi.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <thread>
#include <future>
#include <queue>
#include <chrono>   
#include <thread>   
using namespace std::chrono;

#include <WICTextureLoader.h>

#include <SimpleMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib") 
#pragma comment(lib, "D3DCompiler.lib") 
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib, "dwrite.lib" )
#ifdef _DEBUG
#pragma comment(lib, "DirectXTKd.lib" )
#else
#pragma comment(lib, "DirectXTK.lib" )
#endif
//#pragma comment(lib, "assimp-vc142-mtd.lib" )
#pragma comment(lib, "dxguid.lib")

//#include <assimp\Importer.hpp>
//#include <assimp\scene.h>
//#include <assimp\postprocess.h>

#include <vector>
#include <string>
#include <fstream>
#include <assert.h>
#include <atlstr.h>

#include <wrl/client.h>
using Microsoft::WRL::ComPtr;


#include <assert.h>


#include "ALogger.h"
#include "ADXTime.h"


using namespace std;
using namespace DirectX;
using namespace DirectX::SimpleMath;

typedef Matrix Mat4;

#define SAFERELEASE(ppT) { if(ppT) { (ppT)->Release(); (ppT) = nullptr; } }
#define SAFEDELETE(ppT) { if(ppT) { delete [] (ppT); (ppT) = nullptr; } }

//#include <vld.h>
#include "PNG/lodepng.h"

#include "ACommonDefinitions.h"



struct TerrainInitializationParams
{
	const wchar_t* heightMap;
	const wchar_t* pixelSHader;
	const wchar_t* vertexSHader;
	vector<const wchar_t*>	textureLayers;
};








