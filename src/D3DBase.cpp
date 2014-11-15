#include "D3DBase.h"

D3DBase::D3DBase() 
	: _pDevice(NULL)
{
	
}

D3DBase::~D3DBase()
{
	ReleaseD3DObject(_pDevice);
}

LRESULT CALLBACK D3DBase::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		if (wParam == VK_ESCAPE)
		{
			::DestroyWindow(hWnd);
			break;
		}
	}

	return ::DefWindowProc(hWnd, msg, wParam, lParam);
}

int D3DBase::Run()
{
	MSG msg = { 0 };

	static float lastTime = (float)timeGetTime();

	while (msg.message != WM_QUIT)
	{
		if (::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}
		else
		{
			float currTime = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;

			Display(timeDelta);

			lastTime = currTime;
		}
	}
	return msg.wParam;
}

void D3DBase::Display(float timeDelta)
{
}

bool D3DBase::Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
{
	// Create the main application window
	WNDCLASSEX wc = { sizeof(wc), CS_CLASSDC,
		D3DBase::WndProc, 0L, 0L, hInstance,
		NULL, NULL, NULL, 0, L"d3d", 0 };

	if (!RegisterClassEx(&wc))
	{
		::MessageBox(0, L"RegisterClass() - FAILED", 0, 0);
		return false;
	}

	HWND hwnd = 0;
	hwnd = ::CreateWindow(L"d3d", L"d3d3", WS_OVERLAPPEDWINDOW,
		0, 0, width, height, 0, 0, hInstance, 0);
	if (!hwnd)
	{
		::MessageBox(0, L"CreateWindow() -- FAILED", 0, 0);
		return false;
	}

	::ShowWindow(hwnd, SW_SHOW);
	::UpdateWindow(hwnd);

	// Init D3D
	HRESULT hr = 0;

	IDirect3D9* d3d9 = 0;
	d3d9 = Direct3DCreate9(D3D_SDK_VERSION);

	if (!d3d9)
	{
		::MessageBox(0, L"DirectDCreate9() - FAILED", 0, 0);
		return false;
	}

	// Step 2: check for hardware vp

	D3DCAPS9 caps;

	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, deviceType, &caps);
	int vp = 0;

	if (caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	{
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	}
	else
	{
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	}

	// step 3: fill out the D3DPRESENT_PARAMETERS structure.
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth = width;
	d3dpp.BackBufferHeight = height;
	d3dpp.BackBufferFormat = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount = 1;
	d3dpp.MultiSampleType = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality = 0;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.hDeviceWindow = hwnd;
	d3dpp.Windowed = windowed;
	d3dpp.EnableAutoDepthStencil = true;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Flags = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;

	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT,
		deviceType, hwnd, vp, &d3dpp, &_pDevice);

	if (FAILED(hr))
	{
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT,
			deviceType, hwnd, vp, &d3dpp, &_pDevice);
		if (FAILED(hr))
		{
			d3d9->Release();
			::MessageBox(0, L"CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}
	ReleaseD3DObject(d3d9);
	return true;
}
