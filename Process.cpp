// Process.cpp: implementation of the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MemTracer.h"
#include "Process.h"
#include <tlhelp32.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcess::CProcess()
{

}

CProcess::~CProcess()
{

}

void CProcess::EnumProcesses( CComboBox* pComboBox )
{
	pComboBox->ResetContent();

	pComboBox->AddString( "<No process selected>" );
	pComboBox->SetCurSel( 0 );

	HANDLE hSnapshot = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0 );
	if ( !hSnapshot ) return;

	PROCESSENTRY32 process;
	process.dwSize = sizeof( process );
	BOOL bResult = Process32First( hSnapshot, &process );

	while ( bResult )
	{
		char temp[255];
		sprintf( temp, "[%.8x] - %s", process.th32ProcessID, process.szExeFile );
		pComboBox->AddString( temp );
		pComboBox->SetItemData( pComboBox->GetCount() - 1, process.th32ProcessID );

		bResult = Process32Next( hSnapshot, &process );
	}

	CloseHandle( hSnapshot );
}
