#include "computeshaderclass.h"
#include <random>
#include <functional>
using namespace std;


inline float randomf()
{
	static uniform_real_distribution<float> distribution(0.0f, 1.0f);
	static mt19937 generator;
	static function<float()> rand_generator = bind(distribution, generator);
	return rand_generator();
}

ComputeShaderClass::ComputeShaderClass()
{
	m_comShader = 0;
	m_inputSRV = 0;
	m_outputUAV = 0;
	m_texture = 0;
	m_constBuffer = 0;
	m_randomBuffer = 0;
}

ComputeShaderClass::~ComputeShaderClass()
{

}

ComputeShaderClass::ComputeShaderClass(const ComputeShaderClass& other)
{

}

bool ComputeShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;
	result = InitializeShader(device, hwnd, L"../Engine/computeshader.coms");
	if (!result)
	{
		return false;
	}

	return true;
}

bool ComputeShaderClass::InitializeShader(ID3D11Device* device, HWND hwnd, WCHAR* csFileName)
{
	HRESULT result;
	ID3D10Blob* errorMessage;
	ID3D10Blob* computeShaderBuffer;
	D3D11_BUFFER_DESC inputBufferDesc;
	D3D11_BUFFER_DESC outputBufferDesc;
	D3D11_BUFFER_DESC constBufferDesc;
	D3D11_BUFFER_DESC randomBufferDesc;
	D3D11_TEXTURE2D_DESC textureDesc;
	D3D11_SHADER_RESOURCE_VIEW_DESC inputSRVDesc;
	D3D11_UNORDERED_ACCESS_VIEW_DESC outputUAVDesc;
	D3D11_SUBRESOURCE_DATA initData;
	ID3D11Buffer* inputBuffer;
	ID3D11Buffer* outputBuffer;

	Sphere* input;

	errorMessage = 0;
	computeShaderBuffer = 0;

	result = D3DX11CompileFromFile(csFileName, NULL, NULL, "ComShader", "cs_5_0", D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_DEBUG, NULL, NULL, &computeShaderBuffer, &errorMessage, NULL);
	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderError(errorMessage, hwnd, csFileName);
		}

		else
		{
			MessageBox(hwnd, csFileName, L"Missing Shader File", MB_OK);
		}

		return false;
	}

	result = device->CreateComputeShader(computeShaderBuffer->GetBufferPointer(), computeShaderBuffer->GetBufferSize(), NULL, &m_comShader);
	if (FAILED(result))
	{
		return false;
	}

	computeShaderBuffer->Release();
	computeShaderBuffer = 0;

	inputBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	inputBufferDesc.ByteWidth = sizeof(Sphere) * SPHERENUM;
	inputBufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	inputBufferDesc.CPUAccessFlags = 0;
	inputBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	inputBufferDesc.StructureByteStride = sizeof(Sphere);

	ModelClass::ModelType* modData = 0;
	input = new Sphere[SPHERENUM];
	//for (int i = 0; i < SPHERENUM; i++)
	//{
	//	
	//}
	input[0].center = D3DXVECTOR3(0, 0, 1);
	input[0].radius = 0.5f;
	input[0].color = D3DXVECTOR3(0.2, 0.2, 0.9);
	input[0].material = 0;

	input[1].center = D3DXVECTOR3(0, -100.5, 1);
	input[1].radius = 100;
	input[1].color = D3DXVECTOR3(0.2, 0.2, 0.2);
	input[1].material = 0;

	input[2].center = D3DXVECTOR3(1, 0, 1);
	input[2].radius = 0.5f;
	input[2].color = D3DXVECTOR3(0.6, 0.6, 0.2);
	input[2].material = 0.6;

	input[3].center = D3DXVECTOR3(0, 1, 1);
	input[3].radius = 0.5f;
	input[3].color = D3DXVECTOR3(0.6, 0.2, 0.2);
	input[3].material = 1;

	input[4].center = D3DXVECTOR3(-1, 0, 1);
	input[4].radius = 0.5f;
	input[4].color = D3DXVECTOR3(1, 1, 1);
	input[4].material = -1.5;

	input[5].center = D3DXVECTOR3(-1, 0, 2);
	input[5].radius = 0.5f;
	input[5].color = D3DXVECTOR3(1, 0, 1);
	input[5].material = 0.0f;

	initData.pSysMem = input;

	result = device->CreateBuffer(&inputBufferDesc, &initData, &inputBuffer);
	if (FAILED(result))
	{
		return false;
	}

	//outputBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	//outputBufferDesc.ByteWidth = sizeof(D3DXVECTOR4) * model->GetIndexCount();
	//outputBufferDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS;
	//outputBufferDesc.CPUAccessFlags = 0;
	//outputBufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
	//outputBufferDesc.StructureByteStride = sizeof(D3DXVECTOR4);
	//
	//result = device->CreateBuffer(&outputBufferDesc, NULL, &outputBuffer);
	//if (FAILED(result))
	//{
	//	return false;
	//}

	constBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBufferDesc.ByteWidth = sizeof(ConstBuffer);
	constBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBufferDesc.MiscFlags = 0;
	constBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&constBufferDesc, NULL, &m_constBuffer);
	if (FAILED(result))
	{
		return false;
	}

	randomBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	randomBufferDesc.ByteWidth = sizeof(Rand);
	randomBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	randomBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	randomBufferDesc.MiscFlags = 0;
	randomBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&randomBufferDesc, NULL, &m_randomBuffer);
	if (FAILED(result))
	{
		return false;
	}

	textureDesc.Width = WIDTH;
	textureDesc.Height = HEIGHT;
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.BindFlags = D3D11_BIND_UNORDERED_ACCESS | D3D11_BIND_SHADER_RESOURCE;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;

	result = device->CreateTexture2D(&textureDesc, NULL, &m_texture);
	if (FAILED(result))
	{
		return false;
	}

	inputSRVDesc.Format = DXGI_FORMAT_UNKNOWN;
	inputSRVDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFEREX;
	inputSRVDesc.BufferEx.FirstElement = 0;
	inputSRVDesc.BufferEx.Flags = 0;
	inputSRVDesc.BufferEx.NumElements = SPHERENUM;

	result = device->CreateShaderResourceView(inputBuffer, &inputSRVDesc, &m_inputSRV);
	if (FAILED(result))
	{
		return false;
	}

	outputUAVDesc.Format = DXGI_FORMAT_UNKNOWN;
	outputUAVDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	outputUAVDesc.Buffer.FirstElement = 0;
	outputUAVDesc.Buffer.Flags = 0;
	outputUAVDesc.Buffer.NumElements = WIDTH * HEIGHT;

	result = device->CreateUnorderedAccessView(m_texture, &outputUAVDesc, &m_outputUAV);
	if (FAILED(result))
	{
		return false;
	}

	inputBuffer->Release();
	inputBuffer = 0;

	delete[] input;
	input = 0;

	random = Rand();
	for (int i = 0; i < 99; i++)
	{
		random.r[i] = RandRef();
		float square_dis;
		do
		{
			random.r[i].pos = 2 * D3DXVECTOR3(randomf(), randomf(), randomf()) - D3DXVECTOR3(1, 1, 1);
			random.r[i].padding = 0;
			square_dis = pow(random.r[i].pos.x, 2) + pow(random.r[i].pos.y, 2) + pow(random.r[i].pos.x, 2);
		} while (square_dis > 1);
	}

	return true;
}

void ComputeShaderClass::OutputShaderError(ID3D10Blob* errorMessage, HWND hwnd, WCHAR* shaderFilename)
{
	char* compileErrors;
	unsigned long bufferSize, i;
	ofstream fout;


	// Get a pointer to the error message text buffer.
	compileErrors = (char*)(errorMessage->GetBufferPointer());

	// Get the length of the message.
	bufferSize = errorMessage->GetBufferSize();

	// Open a file to write the error message to.
	fout.open("shader-error.txt");

	// Write out the error message.
	for (i = 0; i<bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	// Close the file.
	fout.close();

	// Release the error message.
	errorMessage->Release();
	errorMessage = 0;

	// Pop a message up on the screen to notify the user to check the text file for compile errors.
	MessageBox(hwnd, L"Error compiling shader.  Check shader-error.txt for message.", shaderFilename, MB_OK);

	return;
}

void ComputeShaderClass::ShutDown()
{
	if (m_comShader)
	{
		m_comShader->Release();
		m_comShader = 0;
	}

	if (m_inputSRV)
	{
		m_inputSRV->Release();
		m_inputSRV = 0;
	}

	if (m_outputUAV)
	{
		m_outputUAV->Release();
		m_outputUAV = 0;
	}

	if (m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}
}

bool ComputeShaderClass::SetShaderParameters(ID3D11DeviceContext* deviceContext)
{
	HRESULT result;
	ConstBuffer* dataPtr;
	Rand* dataPtr2;
	D3D11_MAPPED_SUBRESOURCE mappedResource;

	result = deviceContext->Map(m_constBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr = (ConstBuffer*)mappedResource.pData;

	dataPtr->origin = D3DXVECTOR3(-2, 2, -2);
	dataPtr->to = D3DXVECTOR3(0, 0, 1);
	dataPtr->padding1 = SPHERENUM;
	dataPtr->padding2 = 0;

	deviceContext->Unmap(m_constBuffer, NULL);

	result = deviceContext->Map(m_randomBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &mappedResource);
	if (FAILED(result))
	{
		return false;
	}

	dataPtr2 = (Rand*)mappedResource.pData;
	
	for (int i = 0; i < 100; i++)
	{
		dataPtr2->r[i] = random.r[i];
	}

	deviceContext->Unmap(m_randomBuffer, NULL);

	deviceContext->CSSetConstantBuffers(0, 1, &m_constBuffer);
	deviceContext->CSSetConstantBuffers(1, 1, &m_randomBuffer);

	deviceContext->CSSetUnorderedAccessViews(0, 1, &m_outputUAV, NULL);
	deviceContext->CSSetShaderResources(0, 1, &m_inputSRV);
	deviceContext->CSSetShader(m_comShader, NULL, 0);
	deviceContext->Dispatch(WIDTH, HEIGHT, 1);

	return true;
}

bool ComputeShaderClass::Render(ID3D11DeviceContext* deviceContext)
{
	bool result;
	result = SetShaderParameters(deviceContext);
	if (!result)
	{
		return false;
	}
	return true;
}

ID3D11Texture2D* ComputeShaderClass::GetTexture()
{
	return m_texture;
}