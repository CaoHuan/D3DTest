#include "D3DBase.h"
#include "stdio.h"


class Cube : public D3DBase
{
public:
	Cube()
	{
	}

	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		if (!D3DBase::Setup(hInstance, width, height, windowed, deviceType))
		{
			return false;
		}

		// Create vertex and index buffers

		_pDevice->CreateVertexBuffer(8 * sizeof(Vertex),
			D3DUSAGE_WRITEONLY, FVF,
			D3DPOOL_MANAGED, &vb, 0);

		_pDevice->CreateIndexBuffer(36 * sizeof(WORD),
			D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED,
			&ib, 0);

		// Fill the buffers with cube


		// define unique vertices
		Vertex* vertices;
		vb->Lock(0, 0, (void **)&vertices, 0);

		// vertices of a unit cube
		vertices[0] = Vertex(-1.0f, -1.0f, -1.0f);
		vertices[1] = Vertex(-1.0f, 1.0f, -1.0f);
		vertices[2] = Vertex(1.0f, 1.0f, -1.0f);
		vertices[3] = Vertex(1.0f, -1.0f, -1.0f);
		vertices[4] = Vertex(-1.0f, -1.0f, 1.0f);
		vertices[5] = Vertex(-1.0f, 1.0f, 1.0f);
		vertices[6] = Vertex(1.0f, 1.0f, 1.0f);
		vertices[7] = Vertex(1.0f, -1.0f, 1.0f);

		vb->Unlock();

		// define the triangles of the cube
		WORD* indices = 0;

		ib->Lock(0, 0, (void **)&indices, 0);

		// front side
		indices[0] = 0; indices[1] = 1; indices[2] = 2;
		indices[3] = 0; indices[4] = 2; indices[5] = 3;

		//back side
		indices[6] = 4; indices[7] = 6; indices[8] = 5;
		indices[9] = 4; indices[10] = 7; indices[11] = 6;

		// left side
		indices[12] = 4; indices[13] = 5; indices[14] = 1;
		indices[15] = 4; indices[16] = 1; indices[17] = 0;

		// right side
		indices[18] = 3; indices[19] = 2; indices[20] = 6;
		indices[21] = 3; indices[22] = 6; indices[23] = 7;

		// top
		indices[24] = 1; indices[25] = 5; indices[26] = 6;
		indices[27] = 1; indices[28] = 6; indices[29] = 2;

		// bottom
		indices[30] = 4; indices[31] = 0; indices[32] = 3;
		indices[33] = 4; indices[34] = 3; indices[35] = 7;

		ib->Unlock();

		// position and aim the camera
		D3DXVECTOR3 position(0.0f, 0.0f, -5.0f);
		D3DXVECTOR3 target(0.0f, 0.0f, 0.0f);
		D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
		D3DXMATRIX v;

		D3DXMatrixLookAtLH(&v, &position, &target, &up);
		_pDevice->SetTransform(D3DTS_VIEW, &v);

		// set the projection matrix

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(&proj, D3DX_PI * 0.5f,
			Width / Height, 1.0f, 1000.0f);
		_pDevice->SetTransform(D3DTS_PROJECTION, &proj);

		// switch to wireframe mode.
		_pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);

		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!_pDevice)
		{
			return;
		}

		// spin the cube
		D3DXMATRIX Rx, Ry;

		// rotate 45 degrees on x-axis
		D3DXMatrixRotationX(&Rx, 3.14f / 4.0f);

		// incremement y-rotation angle each frame
		static float y = 0.0f;
		D3DXMatrixRotationY(&Ry, y);
		y += timeDelta;

		// reset angle to zero when angle reaches 2* PI
		if (y >= 6.28f)
		{
			y = 0.0f;
		}

		// cobine x and y-axis rotation transformations
		D3DXMATRIX p = Rx*Ry;
		_pDevice->SetTransform(D3DTS_WORLD, &p);

		// draw the scene:
		_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(255, 255, 255), 1.0f, 0);

		_pDevice->BeginScene();

		_pDevice->SetStreamSource(0, vb, 0, sizeof(Vertex));
		_pDevice->SetIndices(ib);
		_pDevice->SetFVF(FVF);

		// draw cube
		_pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 8, 0, 12);

		_pDevice->EndScene();

		_pDevice->Present(0, 0, 0, 0);
	}
	~Cube(){
	}
private:
	struct Vertex
	{
		Vertex(){}

		Vertex(float x, float y, float z)
			: _x(x)
			, _y(y)
			, _z(z)
		{
		}

		float _x, _y, _z;
	};

	IDirect3DVertexBuffer9* vb;
	IDirect3DIndexBuffer9* ib = 0;

	enum
	{
		FVF = D3DFVF_XYZ,
	};
};


void GetMemory(char *p, int num)
{
	p = (char*)malloc(sizeof(char)* num);
}

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *pBase = new Cube();
	InitData(pBase);

	pBase->Run();
	delete pBase;
	pBase = NULL;
	return 0;
}