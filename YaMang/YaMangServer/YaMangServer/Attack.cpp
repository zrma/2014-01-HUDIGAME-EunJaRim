#include "stdafx.h"
#include "Attack.h"
#include "PacketType.h"
#include "Corps.h"
#include "ClientManager.h"

Attack::Attack()
{
}


Attack::~Attack()
{
}

void Attack::OnBegin()
{
	printf_s( "Attack OnBegin \n" );
	m_ActionStatus = ACTION_TICK;
	m_OwnerCrops->DoNextAction( this, 100 );
}

void Attack::OnTick()
{

	// 둘중 하나라도 죽으면 어텍 취소
	if ( m_OwnerCrops->IsDead( ) || m_TargerCrops->IsDead( ) )
	{
		printf_s( "Attack Failed \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 100 );
		return;
	}

	PositionInfo myCorpsPositionInfo = m_OwnerCrops->GetPositionInfo();
	PositionInfo targetPositionInfo = m_TargerCrops->GetPositionInfo();

	float nowX = myCorpsPositionInfo.m_EyePoint.x;
	float nowZ = myCorpsPositionInfo.m_EyePoint.z;
	float targetX = targetPositionInfo.m_EyePoint.x;
	float targetZ = targetPositionInfo.m_EyePoint.z;

	D3DXVECTOR2 vector;
	vector.x = targetX - nowX;
	vector.y = targetZ - nowZ;

	float length = D3DXVec2Length( &vector );

	if ( length < m_OwnerCrops->GetAttackRange() )
	{
		

		// 공격 하세요
		// 그냥 stop이 아니라 따로 포메이션 같은걸로 공격 모션, 맞는 모션 있어야 할듯
		// AttackCorpsResult에 적용 예정
		m_TargerCrops->AddDamage( m_OwnerCrops->GetAttackPower( ) );
		StopCorpsResult outPacket;
		outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID( );
		m_ClientManager->BroadcastPacket( &outPacket );

		outPacket.m_CorpsID = m_TargerCrops->GetCorpsID();
		m_ClientManager->BroadcastPacket( &outPacket );

		printf_s( "length:%f  range:%f damage:%f \n", length, m_OwnerCrops->GetAttackRange( ), m_OwnerCrops->GetAttackPower( ) );

		printf_s( "Attack OnTick Attack Success \n" );
		m_ActionStatus = ACTION_END;
		m_OwnerCrops->DoNextAction( this, 100 );
	}
	else
	{
		// 마저 쫓아 가세요
		MoveCorpsResult outPacket;
		outPacket.m_CorpsID = m_OwnerCrops->GetCorpsID();

		// 임시로 계산 해둔 식
		D3DXVECTOR3	view = myCorpsPositionInfo.m_EyePoint - targetPositionInfo.m_EyePoint;
		float length = D3DXVec3Length( &view );
		float speed = m_OwnerCrops->GetSpeed();
		float time = ( length / speed ) * 1000;

		// 걸어갈 방향을 지정
		D3DXVec3Normalize( &view, &view );

		// target위치를 서버가 계산해줘야함
		outPacket.m_TargetX = targetPositionInfo.m_EyePoint.x;
		outPacket.m_TargetZ = targetPositionInfo.m_EyePoint.z;
		outPacket.m_LookX = view.x;
		outPacket.m_LookZ = view.z;
		
		PositionInfo position;
		position.m_EyePoint = { targetPositionInfo.m_EyePoint.x, 0.0f, targetPositionInfo.m_EyePoint.z };
		position.m_LookAtPoint = { view.x, 0.0f, view.z };

		m_OwnerCrops->SetPositionInfo( position );

		m_ClientManager->BroadcastPacket( &outPacket );

		printf_s( "Attack OnTick Chase \n" );
		m_ActionStatus = ACTION_TICK;
		m_OwnerCrops->DoNextAction( this, static_cast<ULONGLONG>( time ) );
	}

}

void Attack::OnEnd()
{
	printf_s( "Attack OnEnd \n" );

	Action::OnEnd();
}
