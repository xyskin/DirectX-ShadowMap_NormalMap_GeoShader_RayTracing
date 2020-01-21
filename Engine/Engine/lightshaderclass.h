#ifndef _LIGHTSHADERCLASS_H_
#define _LIGHTSHADERCLASS_H_

#include <d3dx11.h>
#include <d3dx10math.h>
#include <d3dx11async.h>
#include <fstream>
#include <d3dcompiler.h>
using namespace std;

class LightShaderClass
{
private:
	struct MatrixBuffer
	{
		D3DXMATRIX world;
		D3DXMATRIX view;
		D3DXMATRIX projection;
		D3DXMATRIX shadowCamView;
		D3DXMATRIX shadowProj;
	};

	struct LightBuffer
	{
		D3DXVECTOR4 ambientColor;
		D3DXVECTOR4 diffuseColor;
		D3DXVECTOR3 lightPos;
		float padding1;
		D3DXVECTOR3 viewPos;
		float padding2;
	};

	struct VariableBuffer
	{
		D3DXVECTOR3 lightPos;
		float padding;
	};

	struct TimeBuffer
	{
		float time;
		D3DXVECTOR3 padding;
	};

public:
	LightShaderClass();
	LightShaderClass(const LightShaderClass&);
	~LightShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX, 
		D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, ID3D11ShaderResourceView*, 
		D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float, D3DXVECTOR3, ID3D11ShaderResourceView*);

private:
	bool InitializeShader(ID3D11Device*, HWND, WCHAR*, WCHAR*, WCHAR*);
	void ShutdownShader();
	void OutputShaderError(ID3D10Blob*, HWND, WCHAR*);

	bool SetShaderParameters(ID3D11DeviceContext*, D3DXMATRIX, D3DXMATRIX, D3DXMATRIX,
		D3DXVECTOR3, D3DXVECTOR4, D3DXVECTOR4, ID3D11ShaderResourceView*, 
		D3DXMATRIX, D3DXMATRIX, ID3D11ShaderResourceView*, float, D3DXVECTOR3, ID3D11ShaderResourceView*);
	void RenderShader(ID3D11DeviceContext*, int);
private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11GeometryShader* m_geoShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState, *m_sampleClamp;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_variableBuffer;
	ID3D11Buffer* m_timeBuffer;
};

#endif
