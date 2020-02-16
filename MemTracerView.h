// MemTracerView.h : interface of the CMemTracerView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMTRACERVIEW_H__18F60298_21AA_4CD3_A7E0_AE6DCF4E1932__INCLUDED_)
#define AFX_MEMTRACERVIEW_H__18F60298_21AA_4CD3_A7E0_AE6DCF4E1932__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "MemTracerDoc.h"

struct CTemp
{
	CTemp* pNext;
};

class CMemTracerView : public CFormView
{
protected: // create from serialization only
	CMemTracerView();
	DECLARE_DYNCREATE(CMemTracerView)

public:
	//{{AFX_DATA(CMemTracerView)
	enum { IDD = IDD_MEMTRACER_FORM };
	CTreeCtrl	m_MainTree;
	CEdit	m_LogEdit;
	//}}AFX_DATA

// Attributes
public:
	CMemTracerDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemTracerView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMemTracerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


	char* pString;
	CTemp* pLast;
	

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMemTracerView)
	afx_msg void OnDblclkMaintree(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in MemTracerView.cpp
inline CMemTracerDoc* CMemTracerView::GetDocument()
   { return (CMemTracerDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMTRACERVIEW_H__18F60298_21AA_4CD3_A7E0_AE6DCF4E1932__INCLUDED_)
