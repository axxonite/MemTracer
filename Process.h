// Process.h: interface for the CProcess class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROCESS_H__8CBB9D21_29B9_460E_B301_CE99B58715C4__INCLUDED_)
#define AFX_PROCESS_H__8CBB9D21_29B9_460E_B301_CE99B58715C4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CProcess  
{
public:
	CProcess();
	virtual ~CProcess();

	static void EnumProcesses( CComboBox* pComboBox );

};

#endif // !defined(AFX_PROCESS_H__8CBB9D21_29B9_460E_B301_CE99B58715C4__INCLUDED_)
