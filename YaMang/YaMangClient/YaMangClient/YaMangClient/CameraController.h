#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

class CameraController : public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();

	void	Init();

	void	MoveForward( float speed, bool zoom = false );
	void	MoveSide( float speed );
	void	MoveElevate( float speed );
	void	RotateUp( float angle );
	void	RotateSide( float angle );

private:
	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	D3DXVECTOR3		m_EyePoint = { 0, 0, 0 };
	D3DXVECTOR3		m_LookAtPoint = { 0, -0.01f, 1.0f };
	D3DXVECTOR3		m_UpVector = { 0, 1, 0 };

	static CameraController*		m_Instance;
};