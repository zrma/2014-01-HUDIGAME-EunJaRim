#include "stdafx.h"
#include "CameraController.h"
#include "InputDispatcher.h"
#include "Renderer.h"

CameraController* CameraController::m_Instance = nullptr;

CameraController::CameraController()
{
}

CameraController::~CameraController()
{
}

void CameraController::Init()
{
	D3DXMatrixIdentity( &m_ViewMatrix );
	D3DXMatrixIdentity( &m_ProjMatrix );

	D3DXMATRIXA16 matrix;
	
	D3DXMatrixTranslation( &matrix, 0, -40.0f, 5.0f );
	D3DXMatrixMultiply( &m_ViewMatrix, &m_ViewMatrix, &matrix );
}

D3DXMATRIXA16 CameraController::GetInvViewMatrix()
{
	D3DXMATRIXA16 outMatrix;
	D3DXMatrixInverse( &outMatrix, NULL, &m_ViewMatrix );

	return outMatrix;
}

D3DXMATRIXA16 CameraController::GetInvProjMatrix()
{
	D3DXMATRIXA16 outMatrix;
	D3DXMatrixInverse( &outMatrix, NULL, &m_ProjMatrix );

	return outMatrix;
}

void CameraController::MoveForward( float speed )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVec3Normalize( &view, &view );
	m_EyePoint += view * speed;
	m_LookAtPoint += view * speed;

	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetCamera( m_ViewMatrix );
}

void CameraController::MoveSide( float speed )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross, &m_UpVector, &view );
	m_EyePoint += cross * speed;
	m_LookAtPoint += cross * speed;

	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetCamera( m_ViewMatrix );
}

void CameraController::RotateUp( float angle )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 axis = { 0, 0, 0 };

	D3DXVec3Cross( &axis, &m_UpVector, &view );
	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXVec3TransformCoord( &view, &view, &rotateMatrix );
	m_LookAtPoint = m_EyePoint + view;

	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetCamera( m_ViewMatrix );
}

void CameraController::RotateSide( float angle )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 axis = { 0, 1, 0 };
	// È¸ÀüÃà

	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXVec3TransformCoord( &view, &view, &rotateMatrix );
	m_LookAtPoint = m_EyePoint + view;

	D3DXMatrixLookAtLH( &m_ViewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetCamera( m_ViewMatrix );
}
