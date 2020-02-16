// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MemTracer.h"

#include "MainFrm.h"
#include "Process.h"
#include "Utils.h"
#include "MemTracerView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
	ON_CBN_EDITUPDATE(ID_ADDRESS, OnEditupdateAddress)
	ON_BN_CLICKED(ID_REFRESH_PROCESSES, OnRefreshProcesses)
	ON_BN_CLICKED(ID_GO, OnGo)
	ON_UPDATE_COMMAND_UI(ID_GO, OnUpdateGo)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

static CMainFrame* ms_MainFrame = NULL;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

// ==========================================================================
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	ms_MainFrame = this;
}

// ==========================================================================
CMainFrame::~CMainFrame()
{
}

void CMainFrame::ConsoleOutput( char* pString )
{
	CMemTracerView* pView = (CMemTracerView*)ms_MainFrame->GetActiveView();
	pView->m_LogEdit.SetLimitText( 0xFFFFFFFF );			// <-- TODO Verify this
	pView->m_LogEdit.ReplaceSel( pString );
}

// ==========================================================================
int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_StatusBar.Create(this) ||
		!m_StatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if ( !m_Toolbar.Create( this ) ) return -1;
	OnRefreshProcesses();

	return 0;
}

// ==========================================================================
BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.style &= ~( WS_MAXIMIZEBOX | WS_THICKFRAME);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
// ==========================================================================
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

// ==========================================================================
void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


// ==========================================================================
void CMainFrame::OnEditupdateAddress()
{
	char src[100];
	char dest[100];
	int iLength;
	DWORD dwSel = m_Toolbar.GetAddressCombo()->GetEditSel();
	iLength = m_Toolbar.GetAddressCombo()->GetWindowText( src, 100 );

	int j = 0;
	bool bRemoved = ( iLength > 8 );
	if ( iLength > 8 ) iLength = 8;
	for ( BYTE i = 0; i < iLength; i++ )
	{
		char x = src[i];
		if ( ( x >= '0' && x <= '9' ) || ( x >= 'A' && x <= 'F' ) ) dest[j++] = x;
		else if ( x >= 'a' && x <= 'f' ) dest[j++] = x - 32;
		else bRemoved = true;
	}
	dest[j] = 0;

	m_Toolbar.GetAddressCombo()->SetWindowText( dest );
	if ( bRemoved ) m_Toolbar.GetAddressCombo()->SetEditSel( LOWORD( dwSel ) - 1, HIWORD( dwSel ) - 1 );
	else m_Toolbar.GetAddressCombo()->SetEditSel( LOWORD( dwSel ), HIWORD( dwSel ) );

	//m_Toolbar.GetToolBarCtrl().EnableButton( ID_GO, false ); //j != 0 );
}

// ==========================================================================
void CMainFrame::OnGo()
{
	char src[100];
	int iLength;
	iLength = m_Toolbar.GetAddressCombo()->GetWindowText( src, 100 );
	src[iLength] = 0;
	DWORD dwAddress = HexStringToDWORD( src );
	CMemTracerView* pView = (CMemTracerView*)ms_MainFrame->GetActiveView();
	m_Tracer.StartTrace( m_Toolbar.GetProcessesCombo()->GetItemData( m_Toolbar.GetProcessesCombo()->GetCurSel() ), dwAddress, &pView->m_MainTree );
}

// ==========================================================================
void CMainFrame::OnRefreshProcesses()
{
	CProcess::EnumProcesses( m_Toolbar.GetProcessesCombo() );
}

// ==========================================================================
void CMainFrame::OnUpdateGo(CCmdUI* pCmdUI)
{
	char src[4];
	DWORD iLength = m_Toolbar.GetAddressCombo()->GetWindowText( src, 4 );
	DWORD sel = m_Toolbar.GetProcessesCombo()->GetCurSel();
	pCmdUI->Enable( iLength > 0 && sel > 0 );
}


/////////////////////////////////////////////////////////////////////////////
// CMainToolbar


static UINT BASED_CODE buttons[] =
{
	// same order as in the bitmap 'styles.bmp'
	ID_REFRESH_PROCESSES,
	ID_SEPARATOR,
	ID_PROCESSES,
	ID_SEPARATOR,
	ID_ADDRESS,
	ID_SEPARATOR,
	ID_GO
};

// ==========================================================================
bool CMainToolbar::Create( CFrameWnd* pWnd )
{
	const int nDropHeight = 250;

	if (!CToolBar::Create(pWnd, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS | CBRS_FLYBY ) ||
		!LoadBitmap(IDB_MAINTOOLBAR) || !SetButtons(buttons, sizeof(buttons)/sizeof(UINT)))
	{
		TRACE0("Failed to create main toolbar\n");
		return FALSE;       // fail to create
	}

	SetButtonInfo(1, ID_SEPARATOR, TBBS_SEPARATOR, 6);
	SetButtonInfo(2, ID_PROCESSES, TBBS_SEPARATOR, 200);
	SetButtonInfo(3, ID_SEPARATOR, TBBS_SEPARATOR, 12);
	SetButtonInfo(4, ID_ADDRESS, TBBS_SEPARATOR, 100);
	SetButtonInfo(5, ID_SEPARATOR, TBBS_SEPARATOR, 6);
	SetButtonInfo(6, ID_GO, TBBS_BUTTON, 1);

	CRect rect;

	GetItemRect(2, &rect);
	rect.top = 0;
	rect.bottom = nDropHeight;
	rect.right = rect.left + 200;
	if (!m_ProcessesCombo.Create( CBS_DROPDOWNLIST | WS_VISIBLE | WS_VSCROLL | WS_CHILD |WS_TABSTOP, rect, this, ID_PROCESSES))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}

	GetItemRect(4, &rect);
	rect.top = 0;
	rect.bottom = nDropHeight;
	rect.right = rect.left + 100;
	if (!m_AddressCombo.Create( CBS_DROPDOWN | WS_VISIBLE | WS_VSCROLL | WS_CHILD |WS_TABSTOP, rect, this, ID_ADDRESS))
	{
		TRACE0("Failed to create combo-box\n");
		return FALSE;
	}

	//  Create a font for the combobox
	LOGFONT logFont;
	memset(&logFont, 0, sizeof(logFont));
	if (!::GetSystemMetrics(SM_DBCSENABLED))
	{
		// Since design guide says toolbars are fixed height so is the font.
		logFont.lfHeight = -12;
		logFont.lfWeight = FW_BOLD;
		logFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
		CString strDefaultFont;
		strDefaultFont.LoadString(IDS_DEFAULT_FONT);
		lstrcpy(logFont.lfFaceName, strDefaultFont);
		if (!m_Font.CreateFontIndirect(&logFont)) TRACE0("Could Not create font for combo\n");
		else m_ProcessesCombo.SetFont(&m_Font);
	}
	else
	{
		m_Font.Attach(::GetStockObject(SYSTEM_FONT));
		m_ProcessesCombo.SetFont(&m_Font);
	}

	return TRUE;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ( wParam == 1 && lParam == 0 && m_Toolbar.GetToolBarCtrl().IsButtonEnabled( ID_GO ) ) OnGo();
	return CFrameWnd::OnCommand(wParam, lParam);
}
