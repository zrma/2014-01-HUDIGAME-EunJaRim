#pragma once

#include "MacroSet.h"
#include <d3dx9math.h>

enum MouseZoomStatus
{
	ZOOM_STATUS_NONE,
	ZOOM_STATUS_NEAR,
	ZOOM_STATUS_MID,
	ZOOM_STATUS_FAR,
	ZOOM_STATUS_MAX
};

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

	void	ChangeMouseZoomStatus(int zoom);

private:
	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	D3DXVECTOR3		m_EyePoint = { 0, 0, 0 };
	D3DXVECTOR3		m_LookAtPoint = { 0, -0.01f, 1.0f };
	D3DXVECTOR3		m_UpVector = { 0, 1, 0 };

	static CameraController*		m_Instance;

	MouseZoomStatus	m_MouseZoomStatus = MouseZoomStatus::ZOOM_STATUS_NEAR;
};