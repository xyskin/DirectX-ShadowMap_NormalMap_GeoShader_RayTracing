#include "lightclass.h"

LightClass::LightClass()
{
	
}

LightClass::LightClass(const LightClass&)
{

}

LightClass::~LightClass()
{

}

void LightClass::SetAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor.x = r;
	m_ambientColor.y = g;
	m_ambientColor.z = b;
	m_ambientColor.w = a;
}

void LightClass::SetDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor.x = r;
	m_diffuseColor.y = g;
	m_diffuseColor.z = b;
	m_diffuseColor.w = a;
}

D3DXVECTOR4 LightClass::GetAmbientColor()
{
	return m_ambientColor;
}

D3DXVECTOR4 LightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}