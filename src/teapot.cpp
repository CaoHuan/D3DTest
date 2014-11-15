#include "D3DBase.h"

class Teapot : public D3DBase
{
	struct Vertex
	{
		Vertex(){}
		Vertex(float x, float y, float z)
			: x_(x)
			, y_(y)
			, z_(z)
		{
		}

		float x_, y_, z_;
	};

public:
	Teapot()
		: pTeapot_(0)
	{}

	~Teapot()
	{
		ReleaseD3DObject(pTeapot_);
	}

	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		if (!D3DBase::Setup(hInstance, width, height, windowed, deviceType))
		{
			return false;
		}
		D3DXCreateTeapot(_pDevice, &pTeapot_, 0);

		D3DXVECTOR3 position(0.0f, 0.0f, -3.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX v;

		D3DXMatrixLookAtLH(&v, &position, &target, &up);
		_pDevice->SetTransform(D3DTS_VIEW, &v);

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,
			D3DX_PI / 2, (float)width / (float)height, 1.f, 1000.f);

		_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

		_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		//_pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!_pDevice)
		{
			return;
		}

		_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xffffffff, 1.0f, 0);

		_pDevice->BeginScene();
		static float x = 0.f;
		x += timeDelta;
		if (x >= 2 * D3DX_PI)
		{
			x = .0f;
		}
		D3DXMATRIX matrix;
		D3DXMatrixRotationY(&matrix, x);

		_pDevice->SetTransform(D3DTS_WORLD, &matrix);
		pTeapot_->DrawSubset(0);
		

		_pDevice->EndScene();
		_pDevice->Present(0, 0, 0, 0);

	}
private:
	enum
	{
		FVF = D3DFVF_XYZ,
	};
	ID3DXMesh* pTeapot_;
};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new Teapot();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}