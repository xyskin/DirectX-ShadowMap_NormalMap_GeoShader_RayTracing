#include "textureclass.h"

TextureClass::TextureClass()
{
	m_texture = 0;
}

TextureClass::TextureClass(const TextureClass& other)
{

}

TextureClass::~TextureClass()
{

}

bool TextureClass::Initialize(ID3D11Device* device, WCHAR* textureName)
{
	HRESULT result;

	result = D3DX11CreateShaderResourceViewFromFile(device, textureName, NULL, NULL, &m_texture, NULL);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}

void TextureClass::Shutdown()
{
	if (m_texture)
	{
		m_texture->Release();
		//delete m_texture;
		m_texture = 0;
	}
}