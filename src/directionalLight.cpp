#include "D3DBase.h"

class DirectionLight : public D3DBase
{
public:
	DirectionLight(){}
	~DirectionLight(){}

public:
	virtual bool Setup(HINSTANCE hInstance, int width, int height, bool windowed, D3DDEVTYPE deviceType)
	{
		CallBaseSetup;

		return true;
	}

	virtual void Display(float timeDelta)
	{
	}
private:

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