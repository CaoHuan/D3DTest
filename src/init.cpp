#include "D3DBase.h"

class Init : public D3DBase
{
public:
	Init()
		: D3DBase()
	{}

	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{

		return D3DBase::Setup(hInstance, width, height, windowed, deviceType);
	}

	virtual void Display(float timeDelta)
	{
		if (pDevice_)
		{
			pDevice_->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
				D3DCOLOR_XRGB(0, 0, 255), 1.0f, 0);

			pDevice_->Present(0, 0, 0, 0);
		}
	}
	~Init()
	{}

};

INT WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Init *base = new Init();
	InitData(base);
	base->Run();

	delete base;
	base = nullptr;

	return 0;
}