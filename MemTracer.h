// MemTracer.h : main header file for the MEMTRACER application
//

#if !defined(AFX_MEMTRACER_H__FE0CCB8B_5390_4050_A42F_3C2766AAD467__INCLUDED_)
#define AFX_MEMTRACER_H__FE0CCB8B_5390_4050_A42F_3C2766AAD467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMemTracerApp:
// See MemTracer.cpp for the implementation of this class
//

class CMemTracerApp : public CWinApp
{
public:
	CMemTracerApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemTracerApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMemTracerApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMTRACER_H__FE0CCB8B_5390_4050_A42F_3C2766AAD467__INCLUDED_)
