#pragma once

// 노드의 상태를 표시
enum NodeStatus
{
	NodeStatusFree,
	NodeStatusWall,
	NodeStatusPath,
	NodeStatusStart,
	NodeStatusEnd
};

// xy좌표와 연산자 오버라이드를 통해 비교를 쉽게 함
struct NodePoint
{
	int x, y;
	bool operator == ( NodePoint p ) ///# const NodePoint& p
	{
		return x == p.x && y == p.y;
	}
	bool operator != ( NodePoint p )
	{
		return !( *this == p );
	}
};

// 노드의 정보 상태값, 비용 계산에 필요한f,g,h값과 노드 위치 좌표와 부모의 위치좌표를 가진다. 
struct Node
{
	NodeStatus type;
	int f, g, h;
	NodePoint point;
	NodePoint parent;
};


class GameMapPathFinder
{
public:
	GameMapPathFinder();
	~GameMapPathFinder();



	void InitializeMap( int weight, int height );
	void MakeWall( int startX, int startY, int endX, int endY );
	void SetStartAndEndNode( int startX, int startY, int endX, int endY );
	void FindPath();
	void PrintMap();

private:
	void CalculateG( NodePoint nowNodeP, NodePoint nearNodeP, int cost );
	void CalculateF( NodePoint nodeP );
	void CalculateH( NodePoint nodeP );
	bool ValidPoint( NodePoint nodeP );
	void WritePathOnTheMap();


private:
	std::vector<std::vector<Node>> m_Map; ///# 마찬가지로 백터는 이런 용도가 아임 ㅜㅜ
	///# 지형의 기반 자료구조가 동적 자료구조인 벡터면.. 길찾기 느릴텐데..
	NodePoint m_StartNode, m_EndNode;
	bool m_IsFindPath;


};

