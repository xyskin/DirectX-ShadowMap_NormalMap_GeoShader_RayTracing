#include "positionclass.h"

PositionClass::PositionClass()
{

}

PositionClass::PositionClass(const PositionClass& other)
{

}

PositionClass::~PositionClass()
{

}

void PositionClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}

void PositionClass::GetPosition(float& x, float& y, float& z)
{
	x = m_position.x;
	y = m_position.y;
	z = m_position.z;
}

D3DXVECTOR3 PositionClass::GetPosition()
{
	return m_position;
}

void PositionClass::SetRotation(float ex, float ey, float ez)
{
	m_rotation.x = ex;
	m_rotation.y = ey;
	m_rotation.z = ez;
}

void PositionClass::GetRotation(float& ex, float& ey, float& ez)
{
	ex = m_rotation.x;
	ey = m_rotation.y;
	ez = m_rotation.z;
}

D3DXVECTOR3 PositionClass::GetRotation()
{
	return m_rotation;
}

void PositionClass::Move(float x, float y, float z)
{
	m_position.x += x;
	m_position.y += y;
	m_position.z += z;
}

void PositionClass::Rotate(float ex, float ey, float ez)
{
	m_rotation.x += ex;
	m_rotation.y += ey;
	m_rotation.z += ez;
}