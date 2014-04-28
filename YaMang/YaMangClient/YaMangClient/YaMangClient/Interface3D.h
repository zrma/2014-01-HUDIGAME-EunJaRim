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

	D3DXVECTOR3		GetEyePoint() { return m_EyePoint; }
	void			SetEyePoint( D3DXVECTOR3 eyePoint ) { m_EyePoint = eyePoint; }
	D3DXVECTOR3		GetLookAtPoint() { return m_LookAtPoint; }
	void			SetLookAtPoint( D3DXVECTOR3 lookAtPoint ) { m_LookAtPoint = lookAtPoint; }
	
	D3DXVECTOR3		GetTargetPosition() { return m_TargetPoint; }
	void			SetTargetPosition( D3DXVECTOR3 targetPosition ) { m_TargetPoint = targetPosition; }
	D3DXVECTOR3		GetLookAtVector() { return ( m_LookAtPoint - m_EyePoint ); }
	void			SetLookAtPosition( D3DXVECTOR3 lookAtVector ) { m_LookAtPoint = m_EyePoint + lookAtVector; }
	float			GetSpeed() { return m_Speed; }
	void			SetSpeed( float speed ) { m_Speed = speed; }

protected:
	D3DXVECTOR3		m_EyePoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_LookAtPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_UpVector = { 0.0f, 1.0f, 0.0f };
	D3DXVECTOR3		m_TargetPoint = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };
	D3DXVECTOR3		m_Scale = { NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT, NOT_A_NUMBER_FLOAT };

	float	m_Speed = 0.0f;
	bool	m_Visible = false;
};

