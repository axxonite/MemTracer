// MemTracerDoc.h : interface of the CMemTracerDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MEMTRACERDOC_H__FA33E5DF_5605_4601_A201_09255625E56C__INCLUDED_)
#define AFX_MEMTRACERDOC_H__FA33E5DF_5605_4601_A201_09255625E56C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMemTracerDoc : public CDocument
{
protected: // create from serialization only
	CMemTracerDoc();
	DECLARE_DYNCREATE(CMemTracerDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMemTracerDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMemTracerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMemTracerDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MEMTRACERDOC_H__FA33E5DF_5605_4601_A201_09255625E56C__INCLUDED_)
