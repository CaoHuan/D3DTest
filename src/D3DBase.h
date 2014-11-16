#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#include <d3dx9math.h>
#pragma comment( lib,"winmm.lib" )
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")

#define InitData(p) if(!p->Setup(hInstance, Height, Width, true, D3DDEVTYPE_HAL) )  \
				{																    \
					return 0;														\
				}																	\

#define CallBaseSetup if (!D3DBase::Setup(hInstance, width, height, windowed, deviceType) ) \
						{																	\
							return false;													\
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

protected:
	virtual void Display(float timeDelta) = 0;
protected:
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s,
		D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = a;
		mtrl.Diffuse = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power = p;
		return mtrl;
	}

	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));

		light.Type = D3DLIGHT_SPOT;
		light.Ambient = *color * 0.f;
		light.Diffuse = *color;
		light.Specular = *color * 0.6f;
		light.Position = *position;
		light.Direction = *direction;
		light.Range = 1000.0f;
		light.Falloff = 1.0f;
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.f;
		light.Attenuation2 = 0.f;
		light.Theta = 0.4f;
		light.Phi = 0.9f;

		return light;
	}

	D3DLIGHT9 InitPointLight(D3DXVECTOR3* position, D3DXCOLOR* color)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));

		light.Type = D3DLIGHT_POINT;
		light.Ambient = *color * 0.6f;
		light.Diffuse = *color;
		light.Specular = *color * 0.6f;
		light.Position = *position;
		light.Range = 1000.0f;
		light.Falloff = 1.0f;
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.f;
		light.Attenuation2 = 0.f;

		return light;
	}

	D3DLIGHT9 InitDirectionalLight(D3DXVECTOR3* direction, D3DXCOLOR* color)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));

		light.Type = D3DLIGHT_DIRECTIONAL;
		light.Ambient = *color * 0.6f;
		light.Diffuse = *color;
		light.Diffuse = *color * 0.6f;
		light.Direction = *direction;

		return light;
	}
protected:
	IDirect3DDevice9* pDevice_;
	const D3DCOLOR WHITE;
	const D3DCOLOR BLACK;
	const D3DCOLOR RED;
	const D3DCOLOR GREEN;
	const D3DCOLOR BLUE;
	const D3DCOLOR YELLOW;
	const D3DCOLOR CYAN;
	const D3DCOLOR MAGENTA;

	const D3DMATERIAL9 WHITE_MTRL;
	const D3DMATERIAL9 RED_MTRL;
	const D3DMATERIAL9 GREEN_MTRL;
	const D3DMATERIAL9 BLUE_MTRL;
	const D3DMATERIAL9 YELLOW_MTRL;
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