#pragma once

#include <d3dx9math.h>

extern const float NOT_A_NUMBER_FLOAT;

class Interface3D
{
public:
	Interface3D();
	virtual ~Interface3D();

	virtual void	Update() = 0;
	virtual void	Render() = 0;

	virtual void	SetVisible( bool visible ) { m_Visible = visible; }
	D3DXMATRIXA16	GetMatrix( bool leftHand = true );

protected:
	D3DXVECTOR3		m_EyePoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_LookAtPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_UpVector = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_Scale = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };

	int		m_Speed = 0;
	bool	m_Visible = false;
};

