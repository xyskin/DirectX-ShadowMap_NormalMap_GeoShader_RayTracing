#include "cameraclass.h"

#include <iostream>
using namespace std;
CameraClass::CameraClass()
{

}

CameraClass::CameraClass(const CameraClass& other)
{

}

CameraClass::~CameraClass()
{

}

void CameraClass::GetViewMatrix(D3DXMATRIX& matrix)
{
	matrix = m_viewMatrix;
}

D3DXVECTOR3 CameraClass::GetPosition()
{
	return m_position;
}

D3DXVECTOR3 CameraClass::GetRotation()
{
	return m_rotation;
}

void CameraClass::SetPosition(float x, float y, float z)
{
	m_position = D3DXVECTOR3(x, y, z);
}

void CameraClass::SetRotation(float x, float y, float z)
{
	//cout << "x = " << x << " y = " << y << " z = " << z << endl;
	m_rotation = D3DXVECTOR3(x, y, z);
}

void CameraClass::SetUp(float x, float y, float z)
{
	m_up = D3DXVECTOR3(x, y, z);
}

void CameraClass::SetLookAt(float x, float y, float z)
{
	m_lookAt = D3DXVECTOR3(x, y, z);
}

void CameraClass::Render()
{
	float pitch, yaw, roll;
	D3DXMATRIX rotationMatrix;

	pitch = RADIANS(m_rotation.x);
	yaw = RADIANS(m_rotation.y);
	roll = RADIANS(m_rotation.z);

	m_afterRotateLook = m_lookAt;
	m_afterRotateUp = m_up;

	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	D3DXVec3TransformCoord(&m_afterRotateLook, &m_afterRotateLook, &rotationMatrix);
	D3DXVec3TransformCoord(&m_afterRotateUp, &m_afterRotateUp, &rotationMatrix);
	
	D3DXVec3Normalize(&m_afterRotateLook, &m_afterRotateLook);

	D3DXVECTOR3 look = m_afterRotateLook + m_position;
	//D3DXVec3Normalize(&look, &look);
	D3DXVec3Normalize(&m_afterRotateUp, &m_afterRotateUp);
	D3DXMatrixLookAtLH(&m_viewMatrix, &m_position, &look, &m_afterRotateUp);
}

void CameraClass::MoveForward()
{
	m_position += m_afterRotateLook * 0.1f;
}

void CameraClass::MoveBackward()
{
	m_position -= m_afterRotateLook * 0.1f;
}

void CameraClass::MoveLeft()
{
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &m_afterRotateUp, &m_afterRotateLook);

	m_position -= right * 0.1f;
}

void CameraClass::MoveRight()
{
	D3DXVECTOR3 right;
	D3DXVec3Cross(&right, &m_afterRotateUp, &m_afterRotateLook);

	m_position += right * 0.1f;
}