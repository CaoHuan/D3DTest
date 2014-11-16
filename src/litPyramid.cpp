#include "D3DBase.h"
class LitPyramid : public D3DBase
{
	struct Vertex
	{
		Vertex(){}

		Vertex(float x, float y, float z, float nx, float ny, float nz)
		{
			_x = x;  _y = y;	_z = z;
			_nx = nx; _ny = ny; _nz = nz;
		}
		float  _x, _y, _z;
		float _nx, _ny, _nz;
	};

	enum
	{
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL,
	};

public:
	LitPyramid()
		:pyramid_(0)
	{}

	~LitPyramid()
	{
	}

public:
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		CallBaseSetup;

		// turn on lighting
		pDevice_->SetRenderState(D3DRS_LIGHTING, true);

		pDevice_->CreateVertexBuffer(
			12 * sizeof(Vertex), D3DUSAGE_WRITEONLY,
			FVF, D3DPOOL_MANAGED, &pyramid_, 0);

		// Fill the vertex buffer with pyramid data
		Vertex* v;
		pyramid_->Lock(0, 0, (void**)&v, 0);

		// front face
		v[0] = Vertex(-1.f, 0.f, -1.f, 0.f, 0.707f, -0.707f);
		v[1] = Vertex(0.f, 1.f, 0.f, 0.f, 0.707f, -0.707f);
		v[2] = Vertex(1.f, 0.f, -1.0f, 0.f, 0.707f, -0.707f);

		// left face
		v[3] = Vertex(-1.0f, 0.0f, 1.0f, -0.707f, 0.707f, 0.0f);
		v[4] = Vertex(0.0f, 1.0f, 0.0f, -0.707f, 0.707f, 0.0f);
		v[5] = Vertex(-1.0f, 0.0f, -1.0f, -0.707f, 0.707f, 0.0f);

		// right face
		v[6] = Vertex(1.0f, 0.0f, -1.0f, 0.707f, 0.707f, 0.0f);
		v[7] = Vertex(0.0f, 1.0f, 0.0f, 0.707f, 0.707f, 0.0f);
		v[8] = Vertex(1.0f, 0.0f, 1.0f, 0.707f, 0.707f, 0.0f);

		// back face
		v[9] = Vertex(1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);
		v[10] = Vertex(0.0f, 1.0f, 0.0f, 0.0f, 0.707f, 0.707f);
		v[11] = Vertex(-1.0f, 0.0f, 1.0f, 0.0f, 0.707f, 0.707f);

		pyramid_->Unlock();

		// create and set the material
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = static_cast<D3DXCOLOR>(WHITE);
		mtrl.Diffuse = static_cast<D3DXCOLOR>(WHITE);
		mtrl.Specular = static_cast<D3DXCOLOR>(WHITE);
		mtrl.Emissive = static_cast<D3DXCOLOR>(BLACK);
		mtrl.Power = 5.f;
		pDevice_->SetMaterial(&mtrl);

		// setup a directional light

		D3DLIGHT9 dir;
		ZeroMemory(&dir, sizeof(dir));
		dir.Type = D3DLIGHT_DIRECTIONAL;
		dir.Diffuse = static_cast<D3DXCOLOR>(WHITE);
		dir.Specular = static_cast<D3DXCOLOR>(WHITE)* 0.3f;
		dir.Ambient = static_cast<D3DXCOLOR>(WHITE)* 0.6f;
		dir.Direction = D3DXVECTOR3(1.f, 0.f, 0.f);

		// set and enable the light
		pDevice_->SetLight(0, &dir);
		pDevice_->LightEnable(0, true);

		// turn on specular lighting and instruct direct3d
		// to renormalize normals
		pDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		pDevice_->SetRenderState(D3DRS_SPECULARENABLE, true);

		// position and aim the camera
		D3DXVECTOR3 pos(0.f, 1.f, -3.f);
		D3DXVECTOR3 target(0.f, 0.f, 0.f);
		D3DXVECTOR3 up(0.f, 1.f, 0.f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &pos, &target, &up);
		pDevice_->SetTransform(D3DTS_VIEW, &V);

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

		D3DXMATRIX yRot;

		static float y = 0.f;
		D3DXMatrixRotationY(&yRot, y);

		y += timeDelta;
		if (y >= 2 * D3DX_PI)
		{
			y = 0.f;
		}

		pDevice_->SetTransform(D3DTS_WORLD, &yRot);

		// draw the scene
		pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.f, 0);
		pDevice_->BeginScene();
		pDevice_->SetStreamSource(0, pyramid_, 0, sizeof(Vertex));
		pDevice_->SetFVF(FVF);
		pDevice_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 4);

		pDevice_->EndScene();
		pDevice_->Present(0, 0, 0, 0);
	}

private:
	IDirect3DVertexBuffer9* pyramid_;
};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new LitPyramid();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}