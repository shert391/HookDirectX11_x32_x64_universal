#include <d3d11.h>
#include <d3dx11.h>
#include <xnamath.h>
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dx11.lib")
#pragma comment(lib, "d3dcompiler.lib")

#include "Hooks.h"

void Render();

struct VERTEX {
	XMFLOAT3 Position;
};

class Directx {
private:
	Hook hook;

	ID3D11DeviceContext* _pOriginDeviseContext = nullptr;
	ID3D11Device* _pOriginDevise = nullptr;
	IDXGISwapChain* _pOriginSwapChain = nullptr;

	ID3D11VertexShader* _pVertexShader = nullptr;
	ID3D11PixelShader* _pPixelShader = nullptr;
	ID3D11Buffer* _pVertexBuffer = nullptr;
	ID3D11InputLayout* _pInputLayout = nullptr;

	void GetPresentPointer(OUT void** ppPresent);
	void GetOriginSwapChain(void* pPresent, IDXGISwapChain** pOriginSwapChain);
	bool GetData();

	bool CompileShader(const char* szShader, const char* szEntrypoint, const char* szTarget, ID3D10Blob** pCompiledShaderBlob);
	bool CreateShader();
public:
	void Load();
	void DrawRectangle();
};