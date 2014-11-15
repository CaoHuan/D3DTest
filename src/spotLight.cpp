#include "D3DBase.h"

class SpotLight : public D3DBase
{
public:
	SpotLight()
		: objects_()
		, worlds_()
		, matrls_()
		, spot_()
		, WHITE(D3DCOLOR_XRGB(255,255,255) )
		, BLACK(D3DCOLOR_XRGB(0, 0, 0) )
		, RED(D3DCOLOR_XRGB(255, 0, 0) )
		, GREEN(D3DCOLOR_XRGB(0, 255, 0) )
		, BLUE(D3DCOLOR_XRGB(0, 0, 255) )
		, YELLOW(D3DCOLOR_XRGB(255, 255, 0) )
		, CYAN(D3DCOLOR_XRGB(0, 255, 255) )
		, MAGENTA(D3DCOLOR_XRGB(255, 0, 255) )
		, WHITE_MTRL(InitMtrl(WHITE, WHITE, WHITE, BLACK, 2.0f) )
		, RED_MTRL(InitMtrl(RED, RED, RED, BLACK, 2.0f) )
		, GREEN_MTRL(InitMtrl(GREEN, GREEN, GREEN, BLACK, 2.0f) )
		, BLUE_MTRL(InitMtrl(BLUE, BLUE, BLUE, BLACK, 2.0f) )
		, YELLOW_MTRL(InitMtrl(YELLOW, YELLOW, YELLOW, BLACK, 2.0f) )
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

		D3DXCreateTeapot(_pDevice, &objects_[0], 0);
		D3DXCreateSphere(_pDevice, 1.f, 20, 20, &objects_[1], 0);
		D3DXCreateTorus(_pDevice, .5f, 1.f, 20, 20, &objects_[2], 0);
		D3DXCreateCylinder(_pDevice, .5f, .5f, 2.f, 20, 20, &objects_[3], 0);

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
		_pDevice->SetLight(0, &spot_);
		_pDevice->LightEnable(0, true);

		// set light related render states

		_pDevice->SetRenderState(D3DRS_NORMALIZENORMALS, true);
		_pDevice->SetRenderState(D3DRS_SPECULARENABLE, true);

		// position and aim the camera

		D3DXVECTOR3 position(0.f, 0.f, -5.f);
		D3DXVECTOR3 target(0.f, 0.f, 0.f);
		D3DXVECTOR3 up(0.f, 1.f, 0.f);
		D3DXMATRIX V;
		D3DXMatrixLookAtLH(&V, &position, &target, &up);
		_pDevice->SetTransform(D3DTS_VIEW, &V);


		// set the projection matrix

		D3DXMATRIX proj;
		D3DXMatrixPerspectiveFovLH(
			&proj, D3DX_PI / 2, (float)width / (float)height, 1.0f, 1000.f);
		_pDevice->SetTransform(D3DTS_PROJECTION, &proj);
		return true;
	}

	virtual void Display(float timeDelta)
	{
		if (!_pDevice)
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
		_pDevice->SetLight(0, &spot_);
		_pDevice->LightEnable(0, true);

		//
		// Draw the scene:
		//
		_pDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0);
		_pDevice->BeginScene();

		for (int i = 0; i < 4; i++)
		{
			// set material and world matrix for ith object, then render
			// the ith object.
			_pDevice->SetMaterial(&matrls_[i]);
			_pDevice->SetTransform(D3DTS_WORLD, &worlds_[i]);
			objects_[i]->DrawSubset(0);
		}

		_pDevice->EndScene();
		_pDevice->Present(0, 0, 0, 0);
	}

private:
	D3DMATERIAL9 InitMtrl(D3DXCOLOR a, D3DXCOLOR d, D3DXCOLOR s,
		D3DXCOLOR e, float p)
	{
		D3DMATERIAL9 mtrl;
		mtrl.Ambient = a;
		mtrl.Diffuse = d;
		mtrl.Specular = s;
		mtrl.Emissive = e;
		mtrl.Power = p;
		return mtrl;
	}

	D3DLIGHT9 InitSpotLight(D3DXVECTOR3* position, D3DXVECTOR3* direction, D3DXCOLOR* color)
	{
		D3DLIGHT9 light;
		ZeroMemory(&light, sizeof(light));

		light.Type = D3DLIGHT_SPOT;
		light.Ambient = *color * 0.f;
		light.Diffuse = *color;
		light.Specular = *color * 0.6f;
		light.Position = *position;
		light.Direction = *direction;
		light.Range = 1000.0f;
		light.Falloff = 1.0f;
		light.Attenuation0 = 1.0f;
		light.Attenuation1 = 0.f;
		light.Attenuation2 = 0.f;
		light.Theta = 0.4f;
		light.Phi = 0.9f;
		
		return light;
	}
private:
	const D3DCOLOR WHITE;
	const D3DCOLOR BLACK;
	const D3DCOLOR RED;
	const D3DCOLOR GREEN;
	const D3DCOLOR BLUE;
	const D3DCOLOR YELLOW;
	const D3DCOLOR CYAN;
	const D3DCOLOR MAGENTA;

	const D3DMATERIAL9 WHITE_MTRL;
	const D3DMATERIAL9 RED_MTRL;
	const D3DMATERIAL9 GREEN_MTRL;
	const D3DMATERIAL9 BLUE_MTRL;
	const D3DMATERIAL9 YELLOW_MTRL;

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