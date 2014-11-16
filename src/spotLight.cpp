#include "D3DBase.h"

class SpotLight : public D3DBase
{
public:
	SpotLight()
		: objects_()
		, worlds_()
		, matrls_()
		, spot_()
	{
		ZeroMemory(objects_, 4 * sizeof(ID3DXMesh));
	}

	~SpotLight()
	{

		for (int i = 0; i < 4; ++i)
		{
			ReleaseD3DObject(objects_[i]);
		}
	}
public:
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		if (!D3DBase::Setup(hInstance, width, height, windowed, deviceType) )
		{
			return false;
		}

		D3DXCreateTeapot(pDevice_, &objects_[0], 0);
		D3DXCreateSphere(pDevice_, 1.f, 20, 20, &objects_[1], 0);
		D3DXCreateTorus(pDevice_, .5f, 1.f, 20, 20, &objects_[2], 0);
		D3DXCreateCylinder(pDevice_, .5f, .5f, 2.f, 20, 20, &objects_[3], 0);

		D3DXMatrixTranslation(&worlds_[0], .0f, 2.f, 0.f);
		D3DXMatrixTranslation(&worlds_[1], .0f, -2.f, 0.f);
		D3DXMatrixTranslation(&worlds_[2], -3.f, 0.f, 0.f);
		D3DXMatrixTranslation(&worlds_[3], 3.f, 0.f, 0.f);
		
		D3DXMATRIX Rx;
		D3DXMatrixRotationX(&Rx, D3DX_PI / 2);
		worlds_[3] = Rx * worlds_[3];

		
		matrls_[0] = RED_MTRL;
		matrls_[1] = BLUE_MTRL;
		matrls_[2] = GREEN_MTRL;
		matrls_[3] = YELLOW_MTRL;

		for (int i = 0; i < 4; ++i)
		{
			matrls_[i].Power = 20.0f;
		}

		// setup a spot light
		D3DXVECTOR3 pos(.0f, .0f, -5.f);
		D3DXVECTOR3 dir(.0f, .0f, 1.f);
		D3DXCOLOR c = WHITE;
		spot_ = InitSpotLight(&pos, &dir, &c);

		// set and enable spotlight
		pDevice_->SetLight(0, &spot_);
		pDevice_->LightEnable(0, true);

		// set light related render states

		pDevice_->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		pDevice_->SetRenderState(D3DRS_SPECULARENABLE, true);

		// position and aim the camera

		D3DXVECTOR3 position(0.f, 0.f, -5.f);
		D3DXVECTOR3 target(0.f, 0.f, 0.f);
		D3DXVECTOR3 up(0.f, 1.f, 0.f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		pDevice_->SetTransform(D3DTS_VIEW, &V);


		// set the projection matrix

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(
			&proj, D3DX_PI / 2, (float)width / (float)height, 1.0f, 1000.f);
		pDevice_->SetTransform(D3DTS_PROJECTION, &proj);
		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!pDevice_)
		{
			return;
		}

		static float angle = (3.f * D3DX_PI) / 2;
		if (::GetAsyncKeyState(VK_LEFT) & 0x8000f)
			spot_.Direction.x -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_RIGHT) & 0x8000f)
			spot_.Direction.x += 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_DOWN) & 0x8000f)
			spot_.Direction.y -= 0.5f * timeDelta;

		if (::GetAsyncKeyState(VK_UP) & 0x8000f)
			spot_.Direction.y += 0.5f * timeDelta;

		// update the light 
		pDevice_->SetLight(0, &spot_);
		pDevice_->LightEnable(0, true);

		//
		// Draw the scene:
		//
		pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		pDevice_->BeginScene();

		for (int i = 0; i < 4; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			pDevice_->SetMaterial(&matrls_[i]);
			pDevice_->SetTransform(D3DTS_WORLD, &worlds_[i]);
			objects_[i]->DrawSubset(0);
		}

		pDevice_->EndScene();
		pDevice_->Present(0, 0, 0, 0);
	}

private:
	ID3DXMesh* objects_[4];
	D3DXMATRIX worlds_[4];
	D3DMATERIAL9 matrls_[4];
	D3DLIGHT9 spot_;



};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	D3DBase *base = new SpotLight();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;
	return 0;
}