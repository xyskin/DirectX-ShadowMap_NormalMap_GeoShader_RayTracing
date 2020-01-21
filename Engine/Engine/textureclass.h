#ifndef _TEXTURECLASS_H_
#define _TEXTURECLASS_H_

#include <d3dx11.h>

class TextureClass
{
public:
	TextureClass();
	TextureClass(const TextureClass&);
	~TextureClass();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();
private:

private:
	ID3D11ShaderResourceView* m_texture;
};

#endif
