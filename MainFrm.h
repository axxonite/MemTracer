// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__E054B6F0_8B33_4C19_B050_2889C1EB2ED3__INCLUDED_)
#define AFX_MAINFRM_H__E054B6F0_8B33_4C19_B050_2889C1EB2ED3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MainTracer.h"

/////////////////////////////////////////////////////////////////////////////
// CMainToolbar

class CMainToolbar : public CToolBar
{

protected :

	CComboBox	m_ProcessesCombo;
	CComboBox	m_AddressCombo;
	CFont		m_Font;

public :
	
	bool Create( CFrameWnd* pWnd );

	CComboBox*	GetProcessesCombo()		{ return &m_ProcessesCombo;	};
	CComboBox*	GetAddressCombo()		{ return &m_AddressCombo;	};

};


/////////////////////////////////////////////////////////////////////////////
// CMainFrame

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

	static void ConsoleOutput( char* pString );

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar		m_StatusBar;
	CMainToolbar	m_Toolbar;
	CMainTracer		m_Tracer;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	afx_msg void OnEditupdateAddress();
	afx_msg	void OnGo();
	afx_msg void OnRefreshProcesses();
	afx_msg void OnUpdateGo(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__E054B6F0_8B33_4C19_B050_2889C1EB2ED3__INCLUDED_)
