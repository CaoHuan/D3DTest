#include "D3DBase.h"
#include "assert.h"

class TexAlpha : public D3DBase
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
	class Cube
	{
	public:
		Cube(IDirect3DDevice9* pDevice) : pDevice_(pDevice)
		{
			pDevice_->CreateVertexBuffer(24 * sizeof(Vertex),
										 D3DUSAGE_WRITEONLY,
										 FVF, D3DPOOL_MANAGED, &vb_, 0);
			Vertex* v;
			vb_->Lock(0, 0, (void**)&v, 0);

			// build box

			// fill in the front face vertex data
			v[0] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
			v[1] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
			v[2] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);
			v[3] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

			// fill in the back face vertex data
			v[4] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
			v[5] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f);
			v[6] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f);
			v[7] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f);

			// fill in the top face vertex data
			v[8] = Vertex(-1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f);
			v[9] = Vertex(-1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f);
			v[10] = Vertex(1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f);
			v[11] = Vertex(1.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f);

			// fill in the bottom face vertex data
			v[12] = Vertex(-1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f);
			v[13] = Vertex(1.0f, -1.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f);
			v[14] = Vertex(1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f);
			v[15] = Vertex(-1.0f, -1.0f, 1.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

			// fill in the left face vertex data
			v[16] = Vertex(-1.0f, -1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[17] = Vertex(-1.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[18] = Vertex(-1.0f, 1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[19] = Vertex(-1.0f, -1.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			// fill in the right face vertex data
			v[20] = Vertex(1.0f, -1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
			v[21] = Vertex(1.0f, 1.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
			v[22] = Vertex(1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f);
			v[23] = Vertex(1.0f, -1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f);

			vb_->Unlock();

			pDevice_->CreateIndexBuffer(36 * sizeof(WORD),
										D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,
										D3DPOOL_MANAGED, &ib_, 0);
			WORD* i = NULL;
			ib_->Lock(0, 0, (void**)&i, 0);

			// fill in the front face index data
			i[0] = 0; i[1] = 1; i[2] = 2;
			i[3] = 0; i[4] = 2; i[5] = 3;

			// fill in the back face index data
			i[6] = 4; i[7] = 5; i[8] = 6;
			i[9] = 4; i[10] = 6; i[11] = 7;

			// fill in the top face index data
			i[12] = 8; i[13] = 9; i[14] = 10;
			i[15] = 8; i[16] = 10; i[17] = 11;

			// fill in the bottom face index data
			i[18] = 12; i[19] = 13; i[20] = 14;
			i[21] = 12; i[22] = 14; i[23] = 15;

			// fill in the left face index data
			i[24] = 16; i[25] = 17; i[26] = 18;
			i[27] = 16; i[28] = 18; i[29] = 19;

			// fill in the right face index data
			i[30] = 20; i[31] = 21; i[32] = 22;
			i[33] = 20; i[34] = 22; i[35] = 23;

			ib_->Unlock();

		}

		~Cube()
		{
			ReleaseD3DObject(vb_);
			ReleaseD3DObject(ib_);
		}

		bool Draw(D3DXMATRIX* world, D3DMATERIAL9* mtrl, IDirect3DTexture9* tex)
		{
			if (world)
			{
				pDevice_->SetTransform(D3DTS_WORLD, world);
			}

			if (mtrl)
			{
				pDevice_->SetMaterial(mtrl);
			}

			if (tex)
			{
				pDevice_->SetTexture(0, tex);
			}

			pDevice_->SetStreamSource(0, vb_, 0, sizeof(Vertex));
			pDevice_->SetIndices(ib_);
			pDevice_->SetFVF(FVF);
			pDevice_->DrawIndexedPrimitive(
				D3DPT_TRIANGLELIST,
				0, 0, 24, 0, 12);
			return true;
		}

	private:
		IDirect3DDevice9* pDevice_;
		IDirect3DVertexBuffer9* vb_;
		IDirect3DIndexBuffer9* ib_;
	};

public:
	TexAlpha()
		: crateTex_(0)
		, box_(0)
		, cubeWorldMatrix_()
		, backDropVB_(0)
		, backDropTex_(0)
	{}

	~TexAlpha()
	{
		if (box_)
		{
			delete box_;
			box_ = NULL;
		}

		ReleaseD3DObject(crateTex_);
		ReleaseD3DObject(backDropTex_);
		ReleaseD3DObject(backDropVB_);

	}

public:
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		CallBaseSetup;

		pDevice_->CreateVertexBuffer(
			6 * sizeof(Vertex), D3DUSAGE_WRITEONLY,
			FVF, D3DPOOL_MANAGED, &backDropVB_, 0);
		
		Vertex* v;
		backDropVB_->Lock(0, 0, (void**)&v, 0);
		// quad built from two triangles, note texture coordinates:
		v[0] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[1] = Vertex(-10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f);
		v[2] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);

		v[3] = Vertex(-10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f);
		v[4] = Vertex(10.0f, 10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f);
		v[5] = Vertex(10.0f, -10.0f, 5.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f);

		backDropVB_->Unlock();

		// create the cube
		box_ = new Cube(pDevice_);

		D3DXCreateTextureFromFile(pDevice_,
							  L"cratewalpha.dds", &crateTex_);
		D3DXCreateTextureFromFile(pDevice_,
								  L"lobbyxpos.jpg", &backDropTex_);
		assert(crateTex_ && backDropTex_ && L"将源码目录下的相应图片资源复制到exe目录下， 如果从vs启动， 需要复制到工程目录下");

		pDevice_->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
		pDevice_->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
		pDevice_->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);

		// set alpha blending stuff

		// use alpha channel in texture for alpha
		pDevice_->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice_->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);

		// set blending factors so that alpha componenet determines transparency
		pDevice_->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice_->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// disable lighting
		pDevice_->SetRenderState(D3DRS_LIGHTING, false);
		
		// set camera
		D3DXVECTOR3 pos(0.f, 0.f, -2.5f);
		D3DXVECTOR3 target(0.f, 0.f, 0.f);
		D3DXVECTOR3 up(0.f, 1.f, 0.f);

		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &pos, &target, &up);
		pDevice_->SetTransform(D3DTS_VIEW, &V);

		// set projection matrix
		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj,
								   D3DX_PI * 0.5f, (float)width / (float)height,
								   1.f, 1000.f);
		pDevice_->SetTransform(D3DTS_PROJECTION, &proj);

		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!pDevice_)
		{
			return;
		}

		D3DXMATRIX xRot;
		D3DXMatrixRotationX(&xRot, D3DX_PI * 0.2f);

		static float y = 0.f;
		D3DXMATRIX yRot;
		D3DXMatrixRotationY(&yRot, y);
		y += timeDelta;

		if (y >= 6.28f)
		{
			y = 0.f;
		}

		cubeWorldMatrix_ = xRot * yRot;

		// render

		pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xff0000ff, 1.f, 0);
		pDevice_->BeginScene();

		// draw back drop
		D3DXMATRIX I;
		D3DXMatrixIdentity(&I);
		pDevice_->SetStreamSource(0, backDropVB_, 0, sizeof(Vertex));
		pDevice_->SetFVF(FVF);
		pDevice_->SetTexture(0, backDropTex_);
		pDevice_->SetTransform(D3DTS_WORLD, &I);
		pDevice_->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

		// draw cube
		pDevice_->SetRenderState(D3DRS_ALPHABLENDENABLE, true);
		if (box_)
		{
			box_->Draw(&cubeWorldMatrix_, 0, crateTex_);
		}

		pDevice_->SetRenderState(D3DRS_ALPHABLENDENABLE, false);

		pDevice_->EndScene();
		pDevice_->Present(0, 0, 0, 0);

	}

private:
	IDirect3DTexture9* crateTex_;
	Cube* box_;
	D3DXMATRIX cubeWorldMatrix_;
	IDirect3DVertexBuffer9* backDropVB_;
	IDirect3DTexture9*	backDropTex_;

};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new TexAlpha();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}