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
	m_EyePoint = { 0, 0, 0 };
	m_LookAtPoint = { 0, 0, 1.0f };
	
	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::MoveForward( float speed, bool zoom )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	
	if ( !zoom )
	{
		// 크레인에 매달린 카메라 같은 효과를 주기 위해 y값 0으로 고정
		view.y = 0;
	}
	D3DXVec3Normalize( &view, &view );
	m_EyePoint += view * speed;
	m_LookAtPoint += view * speed;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::MoveSide( float speed )
{
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 cross;
	D3DXVec3Cross( &cross, &m_UpVector, &view );
	m_EyePoint += cross * speed;
	m_LookAtPoint += cross * speed;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::MoveElevate( float speed )
{
	m_LookAtPoint.y += speed;
	m_EyePoint.y += speed;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

//////////////////////////////////////////////////////////////////////////
// Rotate Up과 Rotate Side는 Picking Ray 쏴서
// 피킹 된 점을 기준으로 회전해야 함
//
// 수정해야 됨
//////////////////////////////////////////////////////////////////////////
void CameraController::RotateUp( float angle )
{
	// 회전각 제한 1
	if ( m_LookAtPoint.y >= m_EyePoint.y && angle < 0 )
	{
		return;
	}
	D3DXVECTOR3 preView = m_LookAtPoint - m_EyePoint;
	D3DXVECTOR3 view = { 0, 0, 0 };
	D3DXVECTOR3 axis = { 0, 0, 0 };

	D3DXVec3Cross( &axis, &m_UpVector, &preView );
	D3DXMATRIXA16 rotateMatrix;
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXVec3TransformCoord( &view, &preView, &rotateMatrix );

	// 회전각 제한 2
	if ( preView.x * view.x < 0 || preView.y * view.y < 0 || preView.z * view.z < 0 )
	{
		return;
	}
	m_LookAtPoint = m_EyePoint + view;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );

// 	printf_s( "\n%f %f %f %f \n", viewMatrix._11, viewMatrix._12, viewMatrix._13, viewMatrix._14 );
// 	printf_s( "%f %f %f %f \n", viewMatrix._21, viewMatrix._22, viewMatrix._23, viewMatrix._24 );
// 	printf_s( "%f %f %f %f \n", viewMatrix._31, viewMatrix._32, viewMatrix._33, viewMatrix._34 );
// 	printf_s( "%f %f %f %f \n", viewMatrix._41, viewMatrix._42, viewMatrix._43, viewMatrix._44 );
}

void CameraController::RotateSide( float angle )
{
	// 회전축
	D3DXVECTOR3 axis = { 0, 1, 0 };
	D3DXVECTOR3 pickedPointOfCenter = Renderer::GetInstance()->GetPickedPointOfCenter();
	D3DXVECTOR3 view = m_LookAtPoint - m_EyePoint;

	D3DXMATRIXA16 transMatrix;
	D3DXMATRIXA16 rotateMatrix;

	D3DXMatrixTranslation( &transMatrix, -pickedPointOfCenter.x, 0, -pickedPointOfCenter.z );
	D3DXMatrixRotationAxis( &rotateMatrix, &axis, angle );
	D3DXMATRIXA16 resultMatrix = transMatrix * rotateMatrix;

	D3DXMatrixTranslation( &transMatrix, pickedPointOfCenter.x, 0, pickedPointOfCenter.z );
	resultMatrix = resultMatrix * transMatrix;

	D3DXVec3TransformCoord( &m_LookAtPoint, &m_LookAtPoint, &resultMatrix );
	D3DXVec3TransformCoord( &m_EyePoint, &m_EyePoint, &resultMatrix );
	// m_LookAtPoint = m_EyePoint + view;

	D3DXMATRIXA16 viewMatrix;
	D3DXMatrixLookAtLH( &viewMatrix, &m_EyePoint, &m_LookAtPoint, &m_UpVector );
	Renderer::GetInstance()->SetViewMatrix( viewMatrix );
}

void CameraController::ChangeMouseZoomStatus(int zoom)
{
	if (m_MouseZoomStatus == MouseZoomStatus::ZOOM_STATUS_NONE)
	{
		return;
	}

	if (zoom < 0)
	{
		if (m_MouseZoomStatus == MouseZoomStatus::ZOOM_STATUS_NEAR)
		{
			return;
		}
		m_MouseZoomStatus = static_cast<MouseZoomStatus>(static_cast<int>(m_MouseZoomStatus)-1);
	}

	if (zoom > 0)
	{
		if (m_MouseZoomStatus == MouseZoomStatus::ZOOM_STATUS_FAR)
		{
			return;
		}
		m_MouseZoomStatus = static_cast<MouseZoomStatus>(static_cast<int>(m_MouseZoomStatus)+1);
	}

	printf_s("zoom enum : %d\n", m_MouseZoomStatus);
}
