#ifndef _CAMERACLASS_H_
#define _CAMERACLASS_H_

#define RADIANS(x) ((x) * (0.0174532925f))

#include <d3dx10math.h>

class CameraClass
{
public:
	CameraClass();
	CameraClass(const CameraClass&);
	~CameraClass();

	void GetViewMatrix(D3DXMATRIX&);
	D3DXVECTOR3 GetPosition();
	D3DXVECTOR3 GetRotation();
	
	void SetPosition(float, float, float);
	void SetRotation(float, float, float);
	void SetUp(float, float, float);
	void SetLookAt(float, float, float);

	void Render();
	void MoveForward();
	void MoveBackward();
	void MoveLeft();
	void MoveRight();
private:
private:
	D3DXMATRIX m_viewMatrix;
	D3DXVECTOR3 m_lookAt, m_up, m_position, m_rotation;
	D3DXVECTOR3 m_afterRotateLook, m_afterRotateUp;
};

#endif // !_CAMERACLASS_H_

