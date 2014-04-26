#include "stdafx.h"
#include "MovePosition.h"


MovePosition::MovePosition()
{
}

MovePosition::~MovePosition()
{
}

void MovePosition::OnBegin()
{
	printf_s( "MovePosition OnBegin \n" );
}

void MovePosition::OnTick()
{
	printf_s( "MovePosition OnTick \n" );
}

void MovePosition::OnEnd()
{
	printf_s( "MovePosition OnEnd \n" );

	Action::OnEnd();
}
