#include "D3DBase.h"
#include "assert.h"
class TexQuad : public D3DBase
{
	struct Vertex
	{
		Vertex(){}
		Vertex(
			float x, float y, float z,
			float nx, float ny, float nz,
			float u, float v)
		{
			_x = x;  _y = y;  _z = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u = u;  _v = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v; // texture coordinates
	};

	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
	};

public:
	TexQuad() : Quad_(0)
		, Tex_(0)
	{}

	~TexQuad()
	{
		ReleaseD3DObject(Quad_);
		ReleaseD3DObject(Tex_);
	}
public:

	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		CallBaseSetup;
		pDevice_->CreateVertexBuffer(6 * sizeof(Vertex), D3DUSAGE_WRITEONLY, FVF,
									 D3DPOOL_MANAGED, &Quad_, 0);
		Vertex* v;
		Quad_->Lock(0, 0, (void**)&v, 0);

		v[0] = Vertex(-1.f, -1.f, 2.f, 0.f, 0.f, -1.f, 0.f, 1.f);
		v[1] = Vertex(-1.f, 1.f, 2.f, 0.f, 0.f, -1.f, 0.f, 0.f);
		v[2] = Vertex(1.f, 1.f, 2.f, 0.f, 0.f, -1.f, 1.f, 0.f);

		v[3] = Vertex(-1.f, -1.f, 1.25f, 0.f, 0.f, -1.f, 0.f, 1.f);
		v[4] = Vertex(1.f, 1.f, 1.25f, 0.f, 0.f, -1.f, 1.f, 0.f);
		v[5] = Vertex(1.f, -1.f, 1.25f, 0.f, 0.f, -1.f, 1.f, 1.f);

		Quad_->Unlock();

		// create the texture and set filters.
		D3DXCreateTextureFromFile(pDevice_,
								  L"dx.bmp", &Tex_);
		assert(Tex_ && L"将源码目录下的相应图片资源复制到exe目录下， 如果从vs启动， 需要复制到工程目录下");
		pDevice_->SetTexture(0, Tex_);

		pDevice_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pDevice_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pDevice_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		// dont use lighting for this sample.
		pDevice_->SetRenderState(D3DRS_LIGHTING, false);

		// set the projection matrix
		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,
								   D3DX_PI * 0.5f, (float)width / (float)height, 1.f, 1000.f);

		pDevice_->SetTransform(D3DTS_PROJECTION, &proj);

		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!pDevice_)
		{
			return;
		}

		pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
						0xffffffff, 1.f, 0);
		pDevice_->BeginScene();

		pDevice_->SetStreamSource(0, Quad_, 0, sizeof(Vertex));
		pDevice_->SetFVF(FVF);
		pDevice_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		pDevice_->EndScene();
		pDevice_->Present(0, 0, 0, 0);
	}
private:
	IDirect3DVertexBuffer9* Quad_;
	IDirect3DTexture9*		Tex_;

};


INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new TexQuad();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}