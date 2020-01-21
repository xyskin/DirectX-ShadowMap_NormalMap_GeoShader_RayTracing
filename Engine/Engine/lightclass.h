#ifndef _LIGHTCLASS_H_
#define _LIGHTCLASS_H_

#include <d3dx10math.h>

class LightClass
{
public:
	LightClass();
	LightClass(const LightClass&);
	~LightClass();

	void SetDiffuseColor(float, float, float, float);
	void SetAmbientColor(float, float, float, float);

	D3DXVECTOR4 GetDiffuseColor();
	D3DXVECTOR4 GetAmbientColor();

private:
	D3DXVECTOR4 m_diffuseColor;
	D3DXVECTOR4 m_ambientColor;

};

#endif // !_LIGHTCLASS_H_

