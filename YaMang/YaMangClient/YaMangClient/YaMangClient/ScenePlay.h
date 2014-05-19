#pragma once
#include "Scene.h"
#include "EnumSet.h"

class Corps;
class MouseRender;
class UIObject;
class MiniMap;

class ScenePlay:
	public Scene
{
public:
	ScenePlay();
	virtual ~ScenePlay();

	virtual void	Create();
	virtual void	Destroy();
	virtual void	Update();
	virtual void	Render() const;

	void		AddCorps( int corpsID, Corps* corps );
	bool		CheckCorps( int corpsID );
	void		ChangeCorpsFormation( int corpsID, FormationType formationType );
	void		MoveCorpsStart( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtVector, float speed );
	void		MoveCorpsStop( int corpsID, D3DXVECTOR3 targetPosition, D3DXVECTOR3 lookAtPoint );
	void		SetCorpsHP( int corpsID, int unitNum );
	void		SyncOneCorp( int corpsID, D3DXVECTOR3 corpsNow, D3DXVECTOR3 corpsLook, int unitNum, FormationType formationType );

	Corps*		SearchCorpsByPosition( float x, float z, bool selectOption = true );
	
	const Corps* GetCorpsByID( int corpsID );
	UnitType	GetUnitTypeByID( int corpsID ); // 필요 안하게 할 수 있는데...

	void		UpdateBase( int BaseIndex, int playerID );


	void		SetKingIndex( int index ) { m_KingIndex = index; }

	// 맵 출력을 위한 함수
	void		InitUIObjects();
	void		SetMapVisible(bool visible){ m_IsMapVisible = visible; }

private:
	std::hash_map<int, Corps*>	m_CorpsList;
	std::vector<int>			m_BasePlayerIDList;

	MouseRender*				m_MouseCursor;

	int							m_KingIndex = -1;

	SceneType					m_SceneType = SCENE_PLAY;

	// 맵 출력을 위한 멤버
	MiniMap*					m_Minimap;
	bool						m_IsMapVisible = false;
	UIObject*					m_RegenBar;
	UIObject*					m_RegenFlag;

};

