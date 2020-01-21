#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_

#include <fstream>
#include <d3dx11.h>
#include <d3dx10math.h>
using namespace std;

#include "textureclass.h"

class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 vertex;
		D3DXVECTOR2 uv;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

public:
	struct ModelType
	{
		float x, y, z;
		float u, v;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	//struct TempVertexType
	//{
	//	float x, y, z;
	//	float u, v;
	//	float nx, ny, nz;
	//};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();
	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, float);
	void Shutdown();

	void Render(ID3D11DeviceContext*);

	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView* GetNormalMap();
	int GetIndexCount();
	void GetModel(ModelType*&);
private:
	bool InitializeBuffers(ID3D11Device*);
	bool LoadTexture(ID3D11Device*, WCHAR*);
	bool LoadModel(char*, float);
	bool LoadNormalMap(ID3D11Device*, WCHAR*);

	void ReleaseBuffer();
	void ReleaseTexture();
	void ReleaseNormalMap();
	void ReleaseModel();

	void RenderBuffer(ID3D11DeviceContext*);

	void CalcModelVector();
	void CalcTangentAndBinormal(ModelType&, ModelType&, ModelType&, D3DXVECTOR3*, D3DXVECTOR3*);
	void CalcNormal(D3DXVECTOR3&, D3DXVECTOR3&, D3DXVECTOR3*);
private:
	TextureClass* m_Texture;
	TextureClass* m_NormalMap;

	ID3D11Buffer* m_VertexBuffer, *m_IndexBuffer;
	int m_VertexCount, m_IndexCount;
	ModelType* m_model;
};

#endif // !_MODELCLASS_H_

