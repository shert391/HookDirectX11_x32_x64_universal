#include <iostream>

#include "DirectX.h"

#include "Shader.h"

void Directx::GetPresentPointer(OUT void** ppPresent)
{
    IDXGISwapChain* pFakeSwapChain{};

    DXGI_SWAP_CHAIN_DESC spawChainParam{};

    ZeroMemory(&spawChainParam, sizeof(DXGI_SWAP_CHAIN_DESC));

    spawChainParam.BufferCount = 1;
    spawChainParam.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    spawChainParam.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    spawChainParam.OutputWindow = GetDesktopWindow();
    spawChainParam.Windowed = TRUE;
    spawChainParam.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
    spawChainParam.SampleDesc.Count = 1;

    HRESULT status = D3D11CreateDeviceAndSwapChain(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, NULL, NULL, NULL, D3D11_SDK_VERSION, &spawChainParam, &pFakeSwapChain, NULL, NULL, NULL);

    if (FAILED(status)) {
        printf("[-] D3D11CreateDeviceAndSwapChain failed!\n");
        return;
    }

    *ppPresent = *(*(void***)pFakeSwapChain + 0x8);

    pFakeSwapChain->Release();
}

void Directx::GetOriginSwapChain(void* pPresent, IDXGISwapChain** pOriginSwapChain)
{
    void* location = hook.hook1(pPresent, pOriginSwapChain);

    if(location != nullptr)
        std::cout << "[+] Hook Present successfully installed! Patch location: 0x" << location << "\n";
    else
        std::cout << "[-] Failed to hook1 Present :(" << "\n";
}

bool Directx::GetData()
{
    void* pPresent = nullptr;

    GetPresentPointer(&pPresent);

    if (pPresent == nullptr) {
        std::cout << "[-] Failed to get pointer to Present :(" << "\n";
        return false;
    }

    GetOriginSwapChain(pPresent, &_pOriginSwapChain);

    while (_pOriginSwapChain == nullptr) {
        Sleep(50);
    }

    std::cout << "[+] OriginSwapChain* successfully received! Result: 0x" << _pOriginSwapChain << "\n";
    
    HRESULT hr = _pOriginSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&_pOriginDevise);

    if (FAILED(hr)) {
        std::cout << "[-] Error -> GetDevise failed!" << "\n";
        return false;
    }

    std::cout << "[+] OriginDevise* successfully received! Result: 0x" << _pOriginDevise << "\n";

    _pOriginDevise->GetImmediateContext(&_pOriginDeviseContext);

    if (_pOriginDeviseContext == nullptr) {
        std::cout << "[-] Failed to get pointer to DeviseContext :(" << "\n";
        return false;
    }

    std::cout << "[+] OriginDeviseContext* successfully received! Result: 0x" << _pOriginDeviseContext << "\n";

    void* location = hook.hook2(pPresent, Render);

    if(location != nullptr)
        std::cout << "[+] Hook Present successfully installed! Patch location: 0x" << location << "\n";
    else
        std::cout << "[-] Failed to hook2 Present :(" << "\n";

    return true;
}

void Directx::Load()
{
    if (!GetData())
        return;

    if (!CreateShader())
        return;
}

bool Directx::CompileShader(const char* data, const char* shaderEntrypoint, const char* shaderTarget, ID3D10Blob** pCompiledShaderBlob)
{
    ID3D10Blob* pErrorBlob = nullptr;

    HRESULT hr = D3DCompile(data, strlen(data), 0, nullptr, nullptr, shaderEntrypoint, shaderTarget, D3DCOMPILE_ENABLE_STRICTNESS, 0, pCompiledShaderBlob, &pErrorBlob);

    if (FAILED(hr))
        return false;

    return true;
}

bool Directx::CreateShader()
{
    ID3D10Blob* pCompiledShaderBlob = nullptr;

    if (!CompileShader(shader, "VS", "vs_5_0", &pCompiledShaderBlob))
    {
        std::cout << "[-] Failed to compile Vertex shader :(" << "\n";
        return false;
    }

    HRESULT hr = _pOriginDevise->CreateVertexShader(pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), nullptr, &_pVertexShader);

    if (FAILED(hr)) 
    {
        std::cout << "[-] Failed CreateVertexShader :(" << "\n";
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[] =
    {
        { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
    };

    UINT numElements = ARRAYSIZE(layout);

    _pOriginDevise->CreateInputLayout(layout, numElements, pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), &_pInputLayout);

    if (!CompileShader(shader, "PS", "ps_5_0", &pCompiledShaderBlob))
    {
        std::cout << "[-] Failed to compile Pixel shader" << "\n";
        return false;
    }

    hr = _pOriginDevise->CreatePixelShader(pCompiledShaderBlob->GetBufferPointer(), pCompiledShaderBlob->GetBufferSize(), nullptr, &_pPixelShader);

    if (FAILED(hr))
    {
        std::cout << "[-] Failed CreatePixelShader :(" << "\n";
        return false;
    }

    pCompiledShaderBlob->Release();

    return true;
}

void Directx::DrawRectangle()
{
    UINT stride = sizeof(VERTEX);
    UINT offset = 0;

    if (_pVertexBuffer == nullptr) {
        VERTEX arrayVertex[] = {
            { XMFLOAT3(-0.8f,  0.8,  0) },
            { XMFLOAT3(-0.4f,  0.8,  0) },
            { XMFLOAT3(-0.8f,  0.6,  0) },
            { XMFLOAT3(-0.4,  0.6,  0) },
        };

        D3D11_BUFFER_DESC bufferParam;

        ZeroMemory(&bufferParam, sizeof(bufferParam));

        bufferParam.Usage = D3D11_USAGE_DEFAULT;
        bufferParam.ByteWidth = sizeof(VERTEX) * 4;
        bufferParam.BindFlags = D3D11_BIND_VERTEX_BUFFER;
        bufferParam.CPUAccessFlags = 0;

        D3D11_SUBRESOURCE_DATA initData;
        ZeroMemory(&initData, sizeof(initData));
        initData.pSysMem = arrayVertex;

        HRESULT result{};

        result = _pOriginDevise->CreateBuffer(&bufferParam, &initData, &_pVertexBuffer);

        if (FAILED(result)) {
            printf("[-] DirectX::DrawRectangle -> CreateBuffer exeption! HRESULT = %i\n", result);
            return;
        }
        else {
            printf("[+] DirectX::DrawRectangle -> CreateBuffer success!\n");
        }
    }

    _pOriginDeviseContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    _pOriginDeviseContext->IASetInputLayout(_pInputLayout);
    _pOriginDeviseContext->VSSetShader(_pVertexShader, nullptr, 0);
    _pOriginDeviseContext->PSSetShader(_pPixelShader, nullptr, 0);
    _pOriginDeviseContext->IASetVertexBuffers(0, 1, &_pVertexBuffer, &stride, &offset);
    _pOriginDeviseContext->Draw(4, 0);
}
