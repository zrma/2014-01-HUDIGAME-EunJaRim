#include "stdafx.h"
#include "ChangeFormation.h"
#include "Corps.h"
#include "MacroSet.h"

const ULONGLONG CHANGEING_TIME = 1000;
ChangeFormation::ChangeFormation()
{
}


ChangeFormation::~ChangeFormation()
{
}

void ChangeFormation::OnBegin()
{


	m_OwnerCorps->ReCalculatePosition( );

	const PositionInfo& myCorpsPositionInfo = m_OwnerCorps->GetPositionInfo( );

// 	PositionInfo destination;
// 	destination.m_EyePoint = { myCorpsPositionInfo.m_EyePoint.x, 0.0f, myCorpsPositionInfo.m_EyePoint.z };
// 	destination.m_LookAtPoint = { myCorpsPositionInfo.m_LookAtPoint.x, 0.0f, myCorpsPositionInfo.m_LookAtPoint.z };
// 	destination.m_EyePoint += ( myCorpsPositionInfo.m_LookAtPoint ) * 5;

	D3DXVECTOR2 destination;
	destination.x = myCorpsPositionInfo.m_EyePoint.x + myCorpsPositionInfo.m_LookAtPoint.x;
	destination.y = myCorpsPositionInfo.m_EyePoint.z + myCorpsPositionInfo.m_LookAtPoint.z;

	ULONGLONG movingTime = m_OwnerCorps->MoveStart( destination );

// 	D3DXVECTOR2 destination;
// 	destination.x = m_Destination.m_EyePoint.x;
// 	destination.y = m_Destination.m_EyePoint.z;
// 
// 	ULONGLONG movingTime = m_OwnerCrops->MoveStart( destination );

	m_ActionStatus = ACTION_TICK;
	m_OwnerCorps->DoNextAction( this, movingTime );
	m_OwnerCorps->ChangeFormation( FormationType::FORMATION_DEFENSE ); // 망포메이션
}

void ChangeFormation::OnTick()
{
	if ( m_OwnerCorps->IsDead() )
	{
		LogD( "ChangeFormation Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCorps->DoNextAction( this, 0 );
		return;
	}

	m_OwnerCorps->ReCalculatePosition();

	const PositionInfo& myCorpsPositionInfo = m_OwnerCorps->GetPositionInfo();
	D3DXVECTOR2 destination;
	destination.x = myCorpsPositionInfo.m_EyePoint.x;
	destination.y = myCorpsPositionInfo.m_EyePoint.z;

	ULONGLONG movingTime = m_OwnerCorps->MoveStart( destination );

	m_ActionStatus = ACTION_END;
	m_OwnerCorps->DoNextAction( this, movingTime );
}

void ChangeFormation::OnEnd()
{
	if ( m_OwnerCorps->IsDead() )
	{
		LogD( "ChangeFormation Failed \n" );
		return;
	}
	m_OwnerCorps->ChangeFormation( FormationType::FORMATION_DESTROY ); // 세팅한 포메이션
}
