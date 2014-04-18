#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

enum class TransType
{
	TRANS_TYPE_NONE,

	TRANS_TYPE_TRANSLATE_X,
	TRANS_TYPE_TRANSLATE_Y,
	TRANS_TYPE_TRANSLATE_Z,

	TRANS_TYPE_ROTATE_X,
	TRANS_TYPE_ROTATE_Y,
	TRANS_TYPE_ROTATE_Z,

	TRANS_TYPE_SCALE_X,
	TRANS_TYPE_SCALE_Y,
	TRANS_TYPE_SCALE_Z,

	TRANS_TYPE_MAX
};

class CameraController : public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();

	void	Init();
	void	TransCamera( TransType trans, float range );
	
	void	AddAxis( FLOAT axis ) { m_Axis += axis; }
	void	SetAxis( FLOAT axis ) { m_Axis = axis; }
	void	AddHeight( FLOAT height ) { m_Height += height; }
	void	SetHeight( FLOAT height ) { m_Height = height; }

	D3DXMATRIXA16	GetViewMatrix() { m_ViewMatrix; }
	D3DXMATRIXA16	GetProjMatrix() { m_ProjMatrix; }
	D3DXMATRIXA16	GetInvViewMatrix();
	D3DXMATRIXA16	GetInvProjMatrix();

	void	MoveForward( float speed ); //< 앞뒤이동
	void	MoveSide( float speed ); //< 좌우이동
	void	RotateUp( float angle ); //< 상하회전
	void	RotateSide( float angle ); //< 좌우회전

private:
	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	__declspec( align( ALIGNMENT_SIZE ) ) D3DXMATRIXA16	m_ViewMatrix;
	__declspec( align( ALIGNMENT_SIZE ) ) D3DXMATRIXA16	m_ProjMatrix;

	D3DXVECTOR3		m_EyePoint = { 0, 0, 0 };
	D3DXVECTOR3		m_LookAtPoint = { 0, 0, -1 };
	D3DXVECTOR3		m_UpVector = { 0, 1, 0 };

	static CameraController*		m_Instance;
};