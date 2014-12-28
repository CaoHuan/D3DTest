#include "D3DBase.h"

class StencilMirror : public D3DBase
{
	struct Vertex
	{
		Vertex(){}
		Vertex(float x, float y, float z,
			   float nx, float ny, float nz,
			   float u, float v)
		{
			_x = x;  _y = y;  _z = z;
			_nx = nx; _ny = ny; _nz = nz;
			_u = u;  _v = v;
		}
		float _x, _y, _z;
		float _nx, _ny, _nz;
		float _u, _v;
	};

	enum 
	{
		FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1,
	};
public:
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		CallBaseSetup;
	}

	virtual void Display(float timeDelta)
	{
	}

private:
	IDirect3DVertexBuffer9* vb_;
	IDirect3DTexture9* floorTex_;
	IDirect3DTexture9* wallTex_;
	IDirect3DTexture9* mirrorTex_;
	D3DMATERIAL9 floorMtrl_;
	D3DMATERIAL9 wallMtrl_;
	D3DMATERIAL9 mirrorMtrl_;
	ID3DXMesh* teapot_;
	D3DXVECTOR3 teapotPosition_;
	D3DMATERIAL9 teapotMtrl_;



};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new StencilMirror();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}
