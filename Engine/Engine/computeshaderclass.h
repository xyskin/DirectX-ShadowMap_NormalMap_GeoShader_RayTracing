#ifndef _COMPUTESHADERCLASS_
#define _COMPUTESHADERCLASS_

#define SPHERENUM 6
#define WIDTH 800
#define HEIGHT 600

#include "modelclass.h"

#include <d3dx11.h>
#include <d3dx10math.h>
#include <d3dcompiler.h>
#include <fstream>
using namespace std;

class ComputeShaderClass
{
private:
	struct Sphere
	{
		D3DXVECTOR3 center;
		float radius;
		D3DXVECTOR3 color;
		float material; //0 ->漫反射非金属， 0~1 -> 金属度，漫反射~镜面反射， -∞~0 -> 半透明物体折射率
	};

	struct ConstBuffer
	{
		D3DXVECTOR3 origin;
		float padding1;
		D3DXVECTOR3 to;
		float padding2;
	};

	struct RandRef
	{
		D3DXVECTOR3 pos;
		float padding;
	};

	struct Rand
	{
		RandRef r[100];
	};

public:
	ComputeShaderClass();
	~ComputeShaderClass();
	ComputeShaderClass(const ComputeShaderClass&);

	bool Initialize(ID3D11Device*, HWND);
	void ShutDown();
	bool Render(ID3D11DeviceContext*);
	ID3D11Texture2D* GetTexture();
private:
	void OutputShaderError(ID3D10Blob*, HWND, WCHAR*);
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*);
	bool SetShaderParameters(ID3D11DeviceContext*);
private:
	ID3D11ComputeShader* m_comShader;
	ID3D11ShaderResourceView* m_inputSRV;
	ID3D11UnorderedAccessView* m_outputUAV;

	ID3D11Texture2D* m_texture;
	ID3D11Buffer* m_constBuffer;
	ID3D11Buffer* m_randomBuffer;
	Rand random;
};

#endif
