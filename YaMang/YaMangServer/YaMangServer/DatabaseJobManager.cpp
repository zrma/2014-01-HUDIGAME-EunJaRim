#include "stdafx.h"
#include "YamangServer.h"
#include "DatabaseJobContext.h"
#include "DatabaseJobManager.h"
#include "DbHelper.h"

DatabaseJobManager* g_DatabaseJobManager = nullptr;


/// 아래 함수는 DB 처리 쓰레드에서 불려야 한다
// 멤버변수로 갖고 있는, DB 작업 요청 큐(mDbJobRequestQueue) 안의 
// 작업 대기 되어 있는 DB Job Context 들을 하나씩 꺼내서 처리 한 후 
// 작업 완료 큐 (mDbJobResultQueue) 로 집어넣는다.
void DatabaseJobManager::ExecuteDatabaseJobs()
{
	assert( LThreadType == THREAD_DATABASE );

	DatabaseJobContext* jobContext = nullptr;
	while ( true )
	{
		m_DbJobRequestQueue.Consume( jobContext );

		/// 여기서 DB호출해서 처리하고 
		jobContext->m_Success = jobContext->OnExecute();

		/// 그 결과를 result queue에 담아 놓음
		// mDbJobResultQueue.PushBack(jobContext) ;
		m_DbJobResultQueue.Produce( jobContext );
	}
}

/// 아래 함수는 클라이언트 처리 쓰레드에서 불려야 한다
void DatabaseJobManager::PushDatabaseJobRequest( DatabaseJobContext* jobContext )
{
	assert( LThreadType == THREAD_CLIENT );
	m_DbJobRequestQueue.Produce( jobContext );
}

/// 아래 함수는 클라이언트 처리 쓰레드에서 불려야 한다
bool DatabaseJobManager::PopDatabaseJobResult( DatabaseJobContext*& jobContext )
{
	assert( LThreadType == THREAD_CLIENT );

	/// DB 작업 완료는 기다리지 않는다
	return m_DbJobResultQueue.Consume( jobContext, false );
}