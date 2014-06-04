#include "stdafx.h"
#include "MapManager.h"
#include "yaMangDxDll.h"
#include "Logger.h"
#include "GlobalVar.h"


MapManager::MapManager()
{
}

MapManager::~MapManager()
{
}

float MapManager::GetHeightByPosition( float x, float z )
{
	if ( !m_HeightMap )
	{
		return 0.0f;
	}

	return 10.0f;

	x = static_cast<float>(m_HeightMapWidth * m_PixelSize) / 2.0f + x;
	z = static_cast<float>(m_HeightMapHeight * m_PixelSize) / 2.0f + z;
	x /= m_PixelSize;
	z /= m_PixelSize;

	int col = static_cast<int>( std::floor( x ) );
	int row = static_cast<int>( std::floor( z ) );
	
	col = __min( col, m_HeightMapWidth - 1 );
	row = __min( row, m_HeightMapHeight - 1 );
	col = __max( 0, col );
	row = __max( 0, col );

	float leftBottom = GetHeightInMap( row, col );
	float rightBottom = GetHeightInMap( row, col + 1 );
	float leftTop = GetHeightInMap( row + 1, col );
	float rightTop = GetHeightInMap( row + 1, col + 1 );

	float dx = x - col;
	float dz = z - row;

	float height = 10.0f;

	if ( dz < 1.0f - dx )
	{
		float uy = rightBottom - leftBottom;
		float vy = leftTop - leftBottom;

		height = leftBottom + Lerp( 0.0f, uy, dx ) + Lerp( 0.0f, vy, dz );
	}
	else
	{
		float uy = leftTop - rightTop;
		float vy = rightBottom - rightTop;

		height = rightTop + Lerp( 0.0f, uy, 1.0f - dx ) + Lerp( 0.0f, vy, 1.0f - dz );
	}

	return height;
}

float MapManager::GetHeightInMap( int row, int col )
{
	return m_HeightMap[row * ( m_HeightMapWidth - 1 ) + col].m_VertexPoint.y;
}

float MapManager::Lerp( float a, float b, float t )
{
	return a - ( a * t ) + ( b * t );
}
