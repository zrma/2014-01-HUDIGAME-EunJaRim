#include "stdafx.h"
#include "DbHelper.h"
#include "sqlite3.h"
#include "Exception.h"

sqlite3* DbHelper::m_Sqlite = NULL;


DbHelper::DbHelper( const char* sql )
: m_Result( NULL ), m_ResultColCount( 0 ), m_BindColCount( 0 ), m_ResultCurrentCol( 0 )
{
	char* errMsg = NULL;

	if ( sqlite3_prepare_v2( m_Sqlite, sql, -1, &m_Result, NULL ) != SQLITE_OK )
	{
		printf( "DbHelper Query [%s] Prepare failed: %s\n", sql, sqlite3_errmsg( m_Sqlite ) );
		CRASH_ASSERT( false );
	}
}

DbHelper::~DbHelper()
{
	// 전달 인자로 쿼리문이 prepared statment 객체로 전환 된 포인터를 넣음 = mResult
	sqlite3_finalize( m_Result );
}

// 따라서 EasyServer.cpp 에서 _tmain()에서 초기 기동 시에
// DbHelper::Initialize(DB_CONN_STR) 해줌. 실패 할 시에 서버 종료
bool DbHelper::Initialize( const char* connInfoStr )
{
	int result = sqlite3_open( connInfoStr, &m_Sqlite );

	if ( m_Sqlite == NULL || result != SQLITE_OK )
	{
		printf( "DbHelper::Initialize Error\n" );
		return false;
	}

	return true;
}

// 이 루틴은 이전에 sqlite3_open()을 호출함으로써 만들어진 DB연결을 닫는다.
// 모든 해당 연결에 관련된 prepared statement들은 해당 연결이 닫히기 전에 종결 되어야 한다.
// 정리 -> EasyServer.cpp 에서 서버 종료 전에 한 번 실행 함.
// DB 파일 열려 있으므로 닫는다.
void DbHelper::Finalize()
{
	if ( m_Sqlite )
	{
		sqlite3_close( m_Sqlite );
	}
}

// 이 함수는 실제로 사용되지 않았음
bool DbHelper::Execute( const char* format, ... )
{
	if ( !format )
	{
		return false;
	}

	if ( !m_Sqlite )
	{
		return false;
	}

	va_list ap;
	char sqlQuery[1024] = { 0, };
	va_start( ap, format );
	int res = vsnprintf_s( sqlQuery, 1024, format, ap );
	va_end( ap );

	if ( -1 == res )
	{
		return false;
	}

	char* errMsg = NULL;

	///FYI: 사실 실무에서는 (SQL Injection 때문에) 이렇게 쿼리를 직접 넣지 않고
	///파라미터별로 일일이 BIND한다. (BindParamXXX 멤버함수 참고)
	if ( sqlite3_exec( m_Sqlite, sqlQuery, NULL, NULL, &errMsg ) != SQLITE_OK )
	{
		printf( "SQL [%s] ERROR [%s] \n", sqlQuery, errMsg );
		return false;
	}


	return true;
}

// mBindColCount는 생성자에서 0으로 초기화 되고, Bind 할 때마다 1씩 증가
// DB 쿼리문 ~~~ values(?,?,?,?) 에서 n번째에 데이터를 넣으라고 컨트롤 하는 것을
// mBindColCount 로 컨트롤 하기 위해 ++ 함
bool DbHelper::BindParamInt( int param )
{
	// 1. 쿼리문이 prepared statment 객체로 전환 된 포인터 = mResult
	// 2. mBindColCount 번째 칸에
	// 3. param 값을 넣어라
	if ( sqlite3_bind_int( m_Result, ++m_BindColCount, param ) != SQLITE_OK )
	{
		printf( "DbHelper Bind Int failed: %s\n", sqlite3_errmsg( m_Sqlite ) );
		return false;
	}

	return true;
}

bool DbHelper::BindParamDouble( double param )
{

	if ( sqlite3_bind_double( m_Result, ++m_BindColCount, param ) != SQLITE_OK )
	{
		printf( "DbHelper Bind Double failed: %s\n", sqlite3_errmsg( m_Sqlite ) );
		return false;
	}

	return true;
}

bool DbHelper::BindParamText( const char* text, int count )
{
	// 네번째 인자는 세번째 데이터의 크기를 정한다
	// -1(음수)를 넣으면 전체 데이터를 넣는다.
	//
	// 다섯번째 인자는 특별한 인자 이다.
	// 들어가는 값으로 SQLITE_STATIC 과 SQLITE_TRANSIENT가 있다.
	//
	// SQLITE_STATIC의 경우 바인딩 되는 변수를 static 변수(free가 될 일이 없는)로 사용한다는 의미
	// 변수가 중간에 변경되거나 메모리가 해제 되면 문제가 발생할 수 있다.
	//
	// SQLITE_TRANSIENT는 바인딩 변수가중간에 변경이 될 수도 있기에 해당 변수값을 복사하여 사용한다.
	// 중간에 변수가 변경이 되어도 복사한 값으로 사용되기에 문제 없다.
	// 다만 복사과정이 들어가기에 안전하나 SQLITE_STATIC 보다는 속도가 느리다
	if ( sqlite3_bind_text( m_Result, ++m_BindColCount, text, static_cast<int>( strlen( text ) ), NULL ) != SQLITE_OK )
	{
		printf( "DbHelper Bind Text failed: %s\n", sqlite3_errmsg( m_Sqlite ) );
		return false;
	}

	return true;
}

RESULT_TYPE DbHelper::FetchRow()
{
	// sqlite3_step() SQL 문장을 prepared statement 객체로 전환한 mResult를 매개인자로 넣어서
	// 실제 쿼리문에 대해서 평가함
	//
	// sqlite3_prepare_v2()	: SQL 명령문 형식에 대한 평가
	// sqlite3_step()		: 실제 명령문에 인자를 bind 한 이후 적용에 대한 평가

	int result = sqlite3_step( m_Result );
	if ( result != SQLITE_ROW && result != SQLITE_DONE )
	{
		printf( "DbHelper FetchRow failed: %s\n", sqlite3_errmsg( m_Sqlite ) );
		return RESULT_ERROR;
	}

	/// 결과셋으로 얻어올 데이터가 없다. (그냥 쿼리 실행만 된 것)
	if ( result == SQLITE_DONE )
	{
		return RESULT_DONE;
	}

	// 결과를 반환해야 하므로 GetResultParam() 으로 얻어오려면 현재 열을 카운트 해야 됨. 초기화
	m_ResultColCount = sqlite3_column_count( m_Result );
	m_ResultCurrentCol = 0;

	return RESULT_ROW;
}

int DbHelper::GetResultParamInt()
{
	// 최대 열 수를 초과하면 문제가 있는 거
	// 최대 열이 10개인데 GetResult를 11번 호출했다던가...
	CRASH_ASSERT( m_ResultCurrentCol < m_ResultColCount );

	// mResultCurrentCol로 현재 열 수를 카운트하자
	return sqlite3_column_int( m_Result, m_ResultCurrentCol++ );
}

double DbHelper::GetResultParamDouble()
{
	CRASH_ASSERT( m_ResultCurrentCol < m_ResultColCount );

	return sqlite3_column_double( m_Result, m_ResultCurrentCol++ );
}

const unsigned char* DbHelper::GetResultParamText()
{
	CRASH_ASSERT( m_ResultCurrentCol < m_ResultColCount );

	return sqlite3_column_text( m_Result, m_ResultCurrentCol++ );
}