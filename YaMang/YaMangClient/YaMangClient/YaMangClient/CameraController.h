#pragma once
#pragma 
#include "Logger.h"
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

class CameraController: public Singleton<CameraController>
{
public:
	CameraController();
	~CameraController();

	void	Init();
	void	TransCamera( TransType trans, float range );
	
	D3DXMATRIXA16	GetMatrix()
	{
		D3DXMATRIXA16 matrix;
		D3DXMatrixRotationX( &matrix, -1.5f );
		D3DXMatrixMultiply( &matrix, &m_Matrix, &matrix );
		return matrix;
	}
	D3DXMATRIXA16	GetInvMatrix();

private:
	D3DXMATRIXA16	m_Matrix;
};
