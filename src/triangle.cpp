#include "D3DBase.h"

class Triangle : public D3DBase
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
	Triangle()
		: triangle_(0)
	{}

	~Triangle()
	{
		ReleaseD3DObject(triangle_); 
	}

	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		if (!D3DBase::Setup(hInstance, width, height, windowed, deviceType) )
		{
			return false;
		}

		pDevice_->CreateVertexBuffer(3 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY, FVF, D3DPOOL_DEFAULT, &triangle_, 0);
		
		Vertex* vertices;
		triangle_->Lock(0, 0, (void **)&vertices, 0);

		vertices[0] = Vertex(-1.f, .0f, 2.f);
		vertices[1] = Vertex(0.f, 1.f, 2.f);
		vertices[2] = Vertex(1.f, 0.f, 2.f);

		triangle_->Unlock();

		D3DXVECTOR3 position(0.0f, 0.0f, -10.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX v;

		D3DXMatrixLookAtLH(&v, &position, &target, &up);
		pDevice_->SetTransform(D3DTS_VIEW, &v);

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,
			D3DX_PI / 2, (float)width / (float)height, 1.f, 1000.f);

		pDevice_->SetTransform(D3DTS_PROJECTION, &proj);

		pDevice_->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
		pDevice_->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!pDevice_)
		{
			return;
		}

		pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0xffffffff, 1.0f, 0);

		pDevice_->BeginScene();
		static float x = 0.f;
		x += timeDelta;
		if (x >= 2* D3DX_PI)
		{
			x = .0f;
		}
		D3DXMATRIX matrix;
		D3DXMatrixRotationX(&matrix, x);
		
		pDevice_->SetTransform(D3DTS_WORLD, &matrix);

		pDevice_->SetStreamSource(0, triangle_, 0, sizeof(Vertex));

		pDevice_->SetFVF(FVF);

		pDevice_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

		pDevice_->EndScene();
		pDevice_->Present(0, 0, 0, 0);

	}
private:
	enum 
	{
		FVF = D3DFVF_XYZ,
	};
	IDirect3DVertexBuffer9 *triangle_;
};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new Triangle();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}