// MemTracerView.cpp : implementation of the CMemTracerView class
//

#include "stdafx.h"
#include "MemTracer.h"

#include "MemTracerDoc.h"
#include "MemTracerView.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemTracerView

IMPLEMENT_DYNCREATE(CMemTracerView, CFormView)

BEGIN_MESSAGE_MAP(CMemTracerView, CFormView)
	//{{AFX_MSG_MAP(CMemTracerView)
	ON_NOTIFY(NM_DBLCLK, IDC_MAINTREE, OnDblclkMaintree)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemTracerView construction/destruction

CMemTracerView::CMemTracerView()
	: CFormView(CMemTracerView::IDD)
{
	//{{AFX_DATA_INIT(CMemTracerView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// TODO: add construction code here
}

CMemTracerView::~CMemTracerView()
{
}

void CMemTracerView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMemTracerView)
	DDX_Control(pDX, IDC_MAINTREE, m_MainTree);
	DDX_Control(pDX, IDC_LOG, m_LogEdit);
	//}}AFX_DATA_MAP
}

BOOL CMemTracerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CMemTracerView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	CMainFrame::ConsoleOutput( "Data Path Tracer v. 1.0\r\n" );
	CMainFrame::ConsoleOutput( "Tracer ready.\r\n" );
	CMainFrame::ConsoleOutput( "\r\n" );
}

/////////////////////////////////////////////////////////////////////////////
// CMemTracerView diagnostics

#ifdef _DEBUG
void CMemTracerView::AssertValid() const
{
	CFormView::AssertValid();
}

void CMemTracerView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CMemTracerDoc* CMemTracerView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMemTracerDoc)));
	return (CMemTracerDoc*)m_pDocument;
}
#endif //_DEBUG
void CMemTracerView::OnDblclkMaintree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if ( m_MainTree.GetSelectedItem() ) 
	{
		m_MainTree.Select( (HTREEITEM)m_MainTree.GetItemData( m_MainTree.GetSelectedItem() ), TVGN_CARET );
	}
	*pResult = 0;
}
