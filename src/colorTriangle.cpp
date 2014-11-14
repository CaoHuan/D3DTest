#include "D3DBase.h"

class ColorTriangle : public D3DBase
{
public:
	ColorTriangle()
		: WorldMatrix()
		, pTriangle(nullptr)
	{
	}
	~ColorTriangle()
	{
		if (pTriangle)
		{
			pTriangle->Release();
			pTriangle = nullptr;
		}
	}
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		if (!D3DBase::Setup(hInstance, width, height, windowed, deviceType))
		{
			return false;
		}

		_pDevice->CreateVertexBuffer(3 * sizeof(ColorVertex),
			D3DUSAGE_WRITEONLY, FVF, D3DPOOL_MANAGED,
			&pTriangle, 0);
		// FIll the buffer with the triangle data

		ColorVertex *v;
		pTriangle->Lock(0, 0, (void **)&v, 0);
		v[0] = ColorVertex(-1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(255, 0, 0));
		v[1] = ColorVertex(0.0f, 1.0f, 2.0f, D3DCOLOR_XRGB(0, 255, 0));
		v[2] = ColorVertex(1.0f, 0.0f, 2.0f, D3DCOLOR_XRGB(0, 0, 255));

		pTriangle->Unlock();

		// set the projection matrix
		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f,
			Width / Height, 1.0f, 1000.0f);
		_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
		// turn off lighting

		_pDevice->SetRenderState(D3DRS_LIGHTING, false);
		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!_pDevice)
		{
			return;
		}

		_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0);
		_pDevice->BeginScene();

		_pDevice->SetFVF(FVF);

		_pDevice->SetStreamSource(0, pTriangle, 0, sizeof(ColorVertex));

		// draw the triangle to the left with flat shading
		D3DXMatrixTranslation(&WorldMatrix, -1.0f, 0.0f, 0.0f);
		_pDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

		_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_FLAT);
		_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		// draw the triangle to the right with gouraud shading
		D3DXMatrixTranslation(&WorldMatrix, 1.0f, 0.0f, 0.0f);
		_pDevice->SetTransform(D3DTS_WORLD, &WorldMatrix);

		_pDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
		_pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		_pDevice->EndScene();
		_pDevice->Present(0, 0, 0, 0);

		return;
	}
private:
	struct ColorVertex
	{
		ColorVertex(){}

		ColorVertex(float x, float y, float z, D3DCOLOR c)
			: _x(x)
			, _y(y)
			, _z(z)
			, _color(c)
		{
		}

		float _x, _y, _z;
		D3DCOLOR _color;
	};

	D3DXMATRIX WorldMatrix;
	IDirect3DVertexBuffer9* pTriangle;
	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_DIFFUSE,
	};
};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *pBase = new ColorTriangle();
	InitData(pBase);

	pBase->Run();
	delete pBase;
	pBase = NULL;
	return 0;
}