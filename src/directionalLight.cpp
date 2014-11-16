#include "D3DBase.h"

class DirectionLight : public D3DBase
{
public:
	DirectionLight()
		: objects_()
		, worlds_()
		, mtrls_()
	{}

	~DirectionLight()
	{}

public:
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		CallBaseSetup;
		// create objects
		D3DXCreateTeapot(pDevice_, &objects_[0], 0);
		D3DXCreateSphere(pDevice_, 1.f, 20, 20, &objects_[1], 0);
		D3DXCreateTorus(pDevice_, .5f, 1.f, 20, 20, &objects_[2], 0);
		D3DXCreateCylinder(pDevice_, .5f, .5f, 2.f, 20, 20, &objects_[3], 0);

		// build world matrices --position the objects in the world space
		D3DXMatrixTranslation(&worlds_[0], 0.f, 2.f, 0.f);
		D3DXMatrixTranslation(&worlds_[1], 0.f, -2.f, -0.f);
		D3DXMatrixTranslation(&worlds_[2], -3.f, 0.f, 0.f);
		D3DXMatrixTranslation(&worlds_[3], 3.f, 0.f, 0.f);

		// setup the object's materials
		mtrls_[0] = RED_MTRL;
		mtrls_[1] = BLUE_MTRL;
		mtrls_[2] = GREEN_MTRL;
		mtrls_[3] = YELLOW_MTRL;

		// setup a directional light
		D3DXVECTOR3 dir(1.f, -0.f, 0.25f);
		D3DXCOLOR c = WHITE;
		D3DLIGHT9 dirLight = InitDirectionalLight(&dir, &c);
		
		// set and enable the light
		pDevice_->SetLight(0, &dirLight);
		pDevice_->LightEnable(0, true);

		// set lighting related render states
		pDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		pDevice_->SetRenderState(D3DRS_SPECULARENABLE, false);

		// set the projection matrix

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(
			&proj, D3DX_PI * 0.25f, (float)width / (float)height,
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

		static float angle = (3.f * D3DX_PI) / 2.f;
		static float height = 5.f;
		if (GetAsyncKeyState('A') && 0x8000)
		{
			angle -= 0.5f * timeDelta;
		}

		if (GetAsyncKeyState('D') && 0x8000)
		{
			angle += 0.5f * timeDelta;
		}

		if (GetAsyncKeyState('W') && 0x8000)
		{
			height += 0.5f * timeDelta;
		}

		if (GetAsyncKeyState('S') && 0x8000)
		{
			height -= 0.5f * timeDelta;
		}

		D3DXVECTOR3 pos(cosf(angle) * 7.f, height, sinf(angle) * 0.7f);
		D3DXVECTOR3 target(0.f, 0.f, 0.f);
		D3DXVECTOR3 up(0.f, 1.f, 0.f);
		D3DXMATRIX v;
		D3DXMatrixLookAtLH(&v, &pos, &target, &up);

		pDevice_->SetTransform(D3DTS_VIEW, &v);

		// draw the scene
		pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.f, 0);
		pDevice_->BeginScene();
		for (int i = 0; i < 4; i++)
		{
			// set material and world matrix for ith object, then
			// render the ith object.
			pDevice_->SetMaterial(&mtrls_[i]);
			pDevice_->SetTransform(D3DTS_WORLD, &worlds_[i]);
			objects_[i]->DrawSubset(0);
		}

		pDevice_->EndScene();
		pDevice_->Present(0, 0, 0, 0);

	}

private:
	ID3DXMesh* objects_[4];
	D3DXMATRIX worlds_[4];
	D3DMATERIAL9 mtrls_[4];
};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new DirectionLight();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}