#pragma once
#include "client/windows/handler/exception_handler.h"
#include "MacroSet.h"

inline void CRASH_ASSERT( bool isOk )
{
	if ( isOk )
	{
		return;
	}

	int* crashVal = 0;
	*crashVal = 0xDEADBEEF;
}

static bool ExceptResult( const wchar_t *dump_path, const wchar_t *id,
						  void *context, EXCEPTION_POINTERS *exinfo, MDRawAssertionInfo *assertion, bool succeeded )
{
	if ( succeeded )
	{
		Log( "Dump guid is %ws \n", id );
	}
	else
	{
		Log( "Dump failed \n" );
	}

	return succeeded;
}