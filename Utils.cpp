// Utils.cpp
//

#include "stdafx.h"
#include "Utils.h"
#include <math.h>
#include <assert.h>

// ==========================================================================
DWORD HexStringToDWORD( char* pString )
{
	DWORD dwLength = strlen( pString );

	DWORD dwResult = 0;
	int i , j;
	for ( i = dwLength - 1, j = 0; i >= 0; i--, j++ )
	{
		if ( pString[i] >= '0' && pString[i] <= '9' ) dwResult += (DWORD)(( pString[i] - '0' ) * pow( 16.0f, j ));
		else if ( pString[i] >= 'A' && pString[i] <= 'F' ) dwResult += (DWORD)(( pString[i] - 'A' + 10 ) * pow( 16.0f, j ));
		else assert( false );
	}
	return dwResult;
}