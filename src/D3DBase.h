#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment( lib,"winmm.lib" )
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define InitData(p) if(!p->Setup(hInstance, Height, Width, true, D3DDEVTYPE_HAL) )  \
				{																    \
					return 0;														\
				}																	\

const int Width = 640;
const int Height = 480;

class D3DBase
{
public:
	D3DBase();
	virtual ~D3DBase();

	int Run();
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType) = 0;
protected:
	D3DBase(const D3DBase&);
	virtual void Display(float timeDelta) = 0;
private:
	// 去掉拷贝构造函数
protected:
	IDirect3DDevice9* _pDevice;
};

template<typename T>
void ReleaseD3DObject(T* p)
{
	if (p)
	{
		p->Release();
		p = NULL;
	}
}