#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

class CameraController : public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();

	void	Init();
	
	D3DXMATRIXA16	GetViewMatrix();
	D3DXMATRIXA16	GetProjMatrix();
	D3DXMATRIXA16	GetInvViewMatrix();
	D3DXMATRIXA16	GetInvProjMatrix();

	void	MoveForward( float speed ); //< 앞뒤이동
	void	MoveSide( float speed ); //< 좌우이동
	void	RotateUp( float angle ); //< 상하회전
	void	RotateSide( float angle ); //< 좌우회전

private:
	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	D3DXVECTOR3		m_EyePoint = { 0, 0, 0 };
	D3DXVECTOR3		m_LookAtPoint = { 0, 0, -1 };
	D3DXVECTOR3		m_UpVector = { 0, 1, 0 };

	static CameraController*		m_Instance;
};