#ifndef _POSITIONCLASS_H_
#define _POSITIONCLASS_H_

#include <d3dx10math.h>

class PositionClass
{
public:
	PositionClass();
	PositionClass(const PositionClass&);
	~PositionClass();

	void SetPosition(float, float, float);
	void SetRotation(float, float, float);

	void GetPosition(float&, float&, float&);
	D3DXVECTOR3 GetPosition();
	void GetRotation(float&, float&, float&);
	D3DXVECTOR3 GetRotation();

	void Move(float, float, float);
	void Rotate(float, float, float); 
private:
	D3DXVECTOR3 m_position, m_rotation;
};


#endif // !_POSITIONCLASS_H_

