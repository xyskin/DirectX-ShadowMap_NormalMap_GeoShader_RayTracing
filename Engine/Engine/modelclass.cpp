#include "modelclass.h"

ModelClass::ModelClass()
{
	m_Texture = 0;
	m_model = 0;
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;
	m_NormalMap = 0;
}

ModelClass::ModelClass(const ModelClass& other)
{

}

ModelClass::~ModelClass()
{

}

bool ModelClass::Initialize(ID3D11Device* device, char* fileName, WCHAR* textureName, WCHAR* normalName, float scale)
{
	bool result;

	result = LoadModel(fileName, scale);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureName);
	if (!result)
	{
		return false;
	}

	result = LoadNormalMap(device, normalName);
	if (!result)
	{
		return false;
	}

	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned int* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;

	vertices = new VertexType[m_VertexCount];
	if (!vertices)
	{
		return false;
	}

	indices = new unsigned int[m_IndexCount];
	if (!indices)
	{
		return false;
	}

	for (int i = 0; i < m_VertexCount; i++)
	{
		vertices[i].vertex = D3DXVECTOR3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].uv = D3DXVECTOR2(m_model[i].u, m_model[i].v);
		vertices[i].normal = D3DXVECTOR3(m_model[i].nx, m_model[i].ny, m_model[i].nz);
		vertices[i].tangent = D3DXVECTOR3(m_model[i].tx, m_model[i].ty, m_model[i].tz);
		vertices[i].binormal = D3DXVECTOR3(m_model[i].bx, m_model[i].by, m_model[i].bz);

		indices[i] = i;
	}

	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_VertexCount;
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_VertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	indexBufferDesc.ByteWidth = sizeof(unsigned int) * m_IndexCount;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_IndexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

bool ModelClass::LoadTexture(ID3D11Device* device, WCHAR* textureName)
{
	bool result;

	m_Texture = new TextureClass;
	if (!m_Texture)
	{
		return false;
	}

	result = m_Texture->Initialize(device, textureName);
	if (!result)
	{
		return false;
	}

	return true;
}

bool ModelClass::LoadModel(char* modelName, float scale)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(modelName);

	// If it could not open the file then exit.
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_VertexCount;

	// Set the number of indices to be the same as the vertex count.
	m_IndexCount = m_VertexCount;

	// Create the model using the vertex count that was read in.
	m_model = new ModelType[m_VertexCount];
	if (!m_model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < m_VertexCount; i++)
	{
		fin >> m_model[i].x >> m_model[i].y >> m_model[i].z;
		fin >> m_model[i].u >> m_model[i].v;
		fin >> m_model[i].nx >> m_model[i].ny >> m_model[i].nz;

		m_model[i].x *= scale;
		m_model[i].y *= scale;
		m_model[i].z *= scale;
	}

	// Close the model file.
	fin.close();

	CalcModelVector();

	return true;
}

void ModelClass::Shutdown()
{
	ReleaseBuffer();
	ReleaseTexture();
	ReleaseModel();
	ReleaseNormalMap();
}

void ModelClass::ReleaseTexture()
{
	if (m_Texture)
	{
		m_Texture->Shutdown();
		delete m_Texture;
		m_Texture = 0;
	}
}

void ModelClass::ReleaseModel()
{
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
}

void ModelClass::ReleaseBuffer()
{
	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}

	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}
}

void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	RenderBuffer(deviceContext);
}

void ModelClass::RenderBuffer(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_VertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_IndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D11ShaderResourceView* ModelClass::GetTexture()
{
	return m_Texture->GetTexture();
}

int ModelClass::GetIndexCount()
{
	return m_IndexCount;
}

bool ModelClass::LoadNormalMap(ID3D11Device* device, WCHAR* normalName)
{
	bool result;

	m_NormalMap = new TextureClass;
	if (!m_NormalMap)
	{
		return false;
	}

	result = m_NormalMap->Initialize(device, normalName);
	if (!result)
	{
		return false;
	}

	return true;
}

void ModelClass::ReleaseNormalMap()
{
	if (m_NormalMap)
	{
		m_NormalMap->Shutdown();
		delete m_NormalMap;
		m_NormalMap = 0;
	}
}

ID3D11ShaderResourceView* ModelClass::GetNormalMap()
{
	return m_NormalMap->GetTexture();
}

void ModelClass::CalcModelVector()
{
	int triangleCount = m_VertexCount / 3;
	D3DXVECTOR3 tangent, binormal, normal;

	for (int i = 0; i < triangleCount; i += 3)
	{
		CalcTangentAndBinormal(m_model[i], m_model[i + 1], m_model[i + 2], &tangent, &binormal);
		CalcNormal(tangent, binormal, &normal);

		m_model[i].tx = tangent.x;
		m_model[i].ty = tangent.y;
		m_model[i].tz = tangent.z;
		m_model[i].bx = binormal.x;
		m_model[i].by = binormal.y;
		m_model[i].bz = binormal.z;
		m_model[i].nx = normal.x;
		m_model[i].ny = normal.y;
		m_model[i].nz = normal.z;

		m_model[i + 1].tx = tangent.x;
		m_model[i + 1].ty = tangent.y;
		m_model[i + 1].tz = tangent.z;
		m_model[i + 1].bx = binormal.x;
		m_model[i + 1].by = binormal.y;
		m_model[i + 1].bz = binormal.z;
		m_model[i + 1].nx = normal.x;
		m_model[i + 1].ny = normal.y;
		m_model[i + 1].nz = normal.z;

		m_model[i + 2].tx = tangent.x;
		m_model[i + 2].ty = tangent.y;
		m_model[i + 2].tz = tangent.z;
		m_model[i + 2].bx = binormal.x;
		m_model[i + 2].by = binormal.y;
		m_model[i + 2].bz = binormal.z;
		m_model[i + 2].nx = normal.x;
		m_model[i + 2].ny = normal.y;
		m_model[i + 2].nz = normal.z;
	}
}

void ModelClass::CalcTangentAndBinormal(ModelType& v1, ModelType& v2, ModelType& v3,
	D3DXVECTOR3* tang, D3DXVECTOR3* bino)
{
	D3DXVECTOR3 p1, p2;
	D3DXVECTOR2 uv1, uv2;
	float den;

	p1.x = v2.x - v1.x;
	p1.y = v2.y - v1.y;
	p1.z = v2.z - v1.z;

	p2.x = v3.x - v1.x;
	p2.y = v3.y - v1.y;
	p2.z = v3.z - v1.z;

	uv1.x = v2.u - v1.u;
	uv1.y = v2.v - v1.v;

	uv2.x = v3.u - v1.u;
	uv2.y = v3.v - v1.v;

	den = 1.0f / (uv1.x * uv2.y - uv2.x * uv1.y);

	tang->x = (uv2.y * p1.x - uv1.y * p2.x) * den;
	tang->y = (uv2.y * p1.y - uv1.y * p2.y) * den;
	tang->z = (uv2.y * p1.z - uv1.y * p2.z) * den;

	bino->x = (uv1.x * p2.x - uv2.x * p1.x) * den;
	bino->y = (uv1.x * p2.y - uv2.x * p1.y) * den;
	bino->z = (uv1.x * p2.z - uv2.x * p1.z) * den;

	D3DXVec3Normalize(tang, tang);
	D3DXVec3Normalize(bino, bino);

}

void ModelClass::CalcNormal(D3DXVECTOR3& tang, D3DXVECTOR3& bino, D3DXVECTOR3* normal)
{
	D3DXVec3Cross(normal, &tang, &bino);
	//normal->x = tang.y * bino.z - bino.y * tang.z;
	//normal->y = tang.z * bino.x - bino.z * tang.x;
	//normal->z = tang.x * bino.y - bino.x * tang.y;

	D3DXVec3Normalize(normal, normal);
}

void ModelClass::GetModel(ModelType*& model)
{
	model = m_model;
}
