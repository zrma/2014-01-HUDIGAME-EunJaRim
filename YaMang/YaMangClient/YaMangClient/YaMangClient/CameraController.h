#pragma once

#include "Logger.h"
#include <d3dx9math.h>

#define ALIGNMENT_SIZE	16

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

class CameraController
{
public:
	CameraController();
	~CameraController();

	static CameraController* GetInstance( void )
	{
		if ( !m_Instance )
		{
			m_Instance = (CameraController*) _aligned_malloc( sizeof( CameraController ), ALIGNMENT_SIZE );
			new (m_Instance) CameraController();
			// m_Instance = new CameraController();
		}
		return m_Instance;
	}

	static void Release( void )
	{
		if ( m_Instance )
		{
 			m_Instance->~CameraController();
 			_aligned_free( m_Instance );
			//SafeDelete( m_Instance );
		}
	}

	void	Init();
	void	TransCamera( TransType trans, float range );
	
	void	AddAxis( FLOAT axis ) { m_Axis += axis; }
	void	SetAxis( FLOAT axis ) { m_Axis = axis; }
	void	AddHeight( FLOAT height ) { m_Height += height; }
	void	SetHeight( FLOAT height ) { m_Height = height; }

	D3DXMATRIXA16	GetMatrix();
	D3DXMATRIXA16	GetInvMatrix();

private:
	FLOAT			m_Axis = 0;
	FLOAT			m_Height = 0;

	__declspec( align( ALIGNMENT_SIZE ) ) 
					D3DXMATRIXA16	m_Matrix;
	
	static CameraController*		m_Instance;
};