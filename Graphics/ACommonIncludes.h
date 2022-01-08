#pragma once

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include <shobjidl.h> //open file dialog

#if defined(DEBUG) || defined(_DEBUG)
	#define _CRTDBG_MAP_ALLOC
	#include <crtdbg.h>
#endif

#include <d2d1_3.h>
#include <dwrite.h>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_6.h>
#include <d3dcommon.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <thread>
#include <future>
#include <queue>
#include <chrono>   
#include <thread>   

#include <WICTextureLoader.h>

#include <SimpleMath.h>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib") 
#pragma comment(lib, "D3DCompiler.lib") 
#pragma comment(lib, "d2d1.lib" )
#pragma comment(lib, "dwrite.lib" )

#pragma comment(lib, "DirectXTK.lib" )

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
using namespace std::chrono;
 

#define SAFERELEASE(ppT) { if(ppT) { (ppT)->Release(); (ppT) = nullptr; } }
#define SAFEDELETE(ppT) { if(ppT) { delete [] (ppT); (ppT) = nullptr; } }

//#include <vld.h>
#include "PNG/lodepng.h"

#include "ACommonDefinitions.h"
#include "AMathHeader.h"








