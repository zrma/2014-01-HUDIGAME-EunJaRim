#pragma once
#include "Logger.h"
#include "EnumSet.h"

class Unit;

struct Act
{
public:
	virtual void operator()( Unit&, Corps& ) = 0;
};

struct ActDefault: public Act
{
	virtual void operator() ( Unit&, Corps& )
	{
		// Log( "디폴트입니다. \n" );
	}
};

//////////////////////////////////////////////////////////////////////////
// 공격 형태의 FSM을 구현합니다.
//
// 공격 명령, 이동공격(적을 직접 어택해서 거리 좁혀가면서 이동이나 추적 할 때) 등등
//////////////////////////////////////////////////////////////////////////
struct ActAttackSeries: public Act
{
	virtual void operator() ( Unit& unit, Corps& owner )
	{
		// Log( "유닛이 공격! \n" );
	}
};

//////////////////////////////////////////////////////////////////////////
// 이동 형태의 FSM을 구현합니다.
//
// 진형 변경, 전진, 회전 등등 모두 이곳에서 처리
//////////////////////////////////////////////////////////////////////////
struct ActMoveSeries: public Act
{
	virtual void operator() ( Unit& unit, Corps& owner )
	{
		// Log( "유닛이 이동! \n" );

		/*if ( !m_Corps )
		{
		assert( false );
		}
		D3DXMATRIXA16 parentMatrix = m_Corps->GetMatrix();

		D3DXMATRIXA16 formMatrix;
		D3DXMatrixIdentity( &formMatrix );

		D3DXVECTOR3 formationVector = ( m_Corps->GetFormation( m_UnitId ) ) * 3;
		D3DXMatrixTranslation( &formMatrix, formationVector.x, formationVector.y, formationVector.z );

		formMatrix = formMatrix * parentMatrix;

		D3DXVECTOR3	targetEyePoint = { 0.0f, m_EyePoint.y, 0.0f };
		D3DXVECTOR3 targetLookAtPoint = { 0.0f, m_LookAtPoint.y, 1.0f };
		D3DXVec3TransformCoord( &targetEyePoint, &targetEyePoint, &formMatrix );
		D3DXVec3TransformCoord( &targetLookAtPoint, &targetLookAtPoint, &formMatrix );

		float time = static_cast<float>(Timer::GetInstance()->GetElapsedTime());

		if ( targetEyePoint.x - m_EyePoint.x > 0.5f || targetEyePoint.x - m_EyePoint.x < -0.5f )
		{
		m_EyePoint.x += ( targetEyePoint.x - m_EyePoint.x ) * time / 1000;
		}
		if ( targetEyePoint.z - m_EyePoint.z > 0.5f || targetEyePoint.z - m_EyePoint.z < -0.5f )
		{
		m_EyePoint.z += ( targetEyePoint.z - m_EyePoint.z ) * time / 1000;
		}

		if ( targetLookAtPoint.x - m_LookAtPoint.x > 0.5f || targetLookAtPoint.x - m_LookAtPoint.x < -0.5f )
		{
		m_LookAtPoint.x += ( targetLookAtPoint.x - m_LookAtPoint.x ) * time / 1000;
		}
		if ( targetLookAtPoint.z - m_LookAtPoint.z > 0.5f || targetLookAtPoint.z - m_LookAtPoint.z < -0.5f )
		{
		m_LookAtPoint.z += ( targetLookAtPoint.z - m_LookAtPoint.z ) * time / 1000;
		}*/
	}
};