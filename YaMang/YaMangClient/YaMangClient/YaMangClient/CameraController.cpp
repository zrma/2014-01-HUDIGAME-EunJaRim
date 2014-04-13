#include "stdafx.h"
#include "CameraController.h"
#include "InputDispatcher.h"

CameraController::CameraController()
{
	ZeroMemory( &m_Matrix, sizeof( m_Matrix ) );
}


CameraController::~CameraController()
{
}

void CameraController::Init()
{
	D3DXMatrixIdentity( &m_Matrix );
}

void CameraController::TransCamera( TransType trans, float range )
{
	D3DXMATRIXA16 matrix;
	switch ( trans )
	{
		case TransType::TRANS_TYPE_TRANSLATE_X:
			D3DXMatrixTranslation( &matrix, range, 0, 0 );
			break;
		case TransType::TRANS_TYPE_TRANSLATE_Y:
			D3DXMatrixTranslation( &matrix, 0, range, 0 );
			break;
		case TransType::TRANS_TYPE_TRANSLATE_Z:
			D3DXMatrixTranslation( &matrix, 0, 0, range );
			break;
		case TransType::TRANS_TYPE_ROTATE_X:
			D3DXMatrixRotationX( &matrix, range );
			break;
		case TransType::TRANS_TYPE_ROTATE_Y:
			D3DXMatrixRotationY( &matrix, range );
			break;
		case TransType::TRANS_TYPE_ROTATE_Z:
			D3DXMatrixRotationZ( &matrix, range );
			break;
		case TransType::TRANS_TYPE_SCALE_X:
			D3DXMatrixScaling( &matrix, range, 1.0f, 1.0f );
			break;
		case TransType::TRANS_TYPE_SCALE_Y:
			D3DXMatrixScaling( &matrix, 1.0f, range, 1.0f );
			break;
		case TransType::TRANS_TYPE_SCALE_Z:
			D3DXMatrixScaling( &matrix, 1.0f, 1.0f, range );
			break;
		default:
			break;
	}

	D3DXMatrixMultiply( &m_Matrix, &m_Matrix, &matrix );

	Log( " %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n %f %f %f %f \n",
		 m_Matrix._11, m_Matrix._21, m_Matrix._31, m_Matrix._41,
		 m_Matrix._12, m_Matrix._22, m_Matrix._32, m_Matrix._42,
		 m_Matrix._13, m_Matrix._23, m_Matrix._33, m_Matrix._43,
		 m_Matrix._14, m_Matrix._24, m_Matrix._34, m_Matrix._44 );
}
