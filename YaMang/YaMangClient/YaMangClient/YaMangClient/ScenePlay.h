#pragma once
#include "Scene.h"
#include <d3dx9math.h>

class Corps;
class MouseRender;

class ScenePlay:
	public Scene
{
public:
	ScenePlay();
	~ScenePlay();

	void		Create();
	void		Destroy();
	void		Update();
	void		Render() const;

	void		AddCorps( int corpsID, Corps* corps );
	bool		CheckCorps( int corpsID );
	void		ChangeCorpsFormation( int corpsID, FormationType formationType );
	void		MoveCorpsStart( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector, float speed );
	void		MoveCorpsStop( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector );
	void		SetCorpsHP( int corpsID, int unitNum );

	Corps*		SearchCorpsByPosition( float x, float z, bool selectOption = true );

	UnitType	GetUnitTypeByID( int corpsID );

private:
	std::hash_map<int, Corps*>	m_CorpsList;

	MouseRender*	m_MouseCursor;
};

