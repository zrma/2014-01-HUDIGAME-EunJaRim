#pragma once
#include "Corps.h"

class Arrow:
	public Corps
{
public:
	Arrow( int playerID, int corpsID, PositionInfo position ):Corps( playerID, corpsID, position ) {};
	virtual ~Arrow( );

private:
	Arrow(); ///# 부모 생성자가 private이면 여기에 굳이 이렇게 계속 해놓을 필요 없다.
	Arrow( const Arrow& );
	Arrow& operator=( const Arrow& );

};

