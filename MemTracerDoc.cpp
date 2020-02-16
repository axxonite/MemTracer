// MemTracerDoc.cpp : implementation of the CMemTracerDoc class
//

#include "stdafx.h"
#include "MemTracer.h"

#include "MemTracerDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemTracerDoc

IMPLEMENT_DYNCREATE(CMemTracerDoc, CDocument)

BEGIN_MESSAGE_MAP(CMemTracerDoc, CDocument)
	//{{AFX_MSG_MAP(CMemTracerDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemTracerDoc construction/destruction

CMemTracerDoc::CMemTracerDoc()
{
	// TODO: add one-time construction code here

}

CMemTracerDoc::~CMemTracerDoc()
{
}

BOOL CMemTracerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CMemTracerDoc serialization

void CMemTracerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMemTracerDoc diagnostics

#ifdef _DEBUG
void CMemTracerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMemTracerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMemTracerDoc commands
