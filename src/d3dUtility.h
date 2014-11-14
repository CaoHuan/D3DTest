#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment( lib,"winmm.lib" )
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

bool InitD3D(
	HINSTANCE hInstance,
	int width, int height,
	bool windowed,
	D3DDEVTYPE deviceType,
	IDirect3DDevice9** device);

int EnterMsgLoop(
	bool(*ptr_display)(float timeDelta));

LRESULT CALLBACK WndProc(
	HWND hWnd,
	UINT msg,
	WPARAM wParam,
	LPARAM lParam);

template<class T> void Release(T t)
{
	if (t)
	{
		t->Release();
		t = NULL;
	}
}

template<class T> void DeleteT(T t)
{
	if (t)
	{
		delete t;
		t = NULL;
	}
}

bool Setup();

void Cleanup();

bool Display(float timeDelta);
