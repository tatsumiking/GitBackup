// DlgStrVS.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgStrVS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgVStruct ダイアログ


CDialogStrgVStruct::CDialogStrgVStruct(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogStrgVStruct::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogStrgVStruct)
	m_chkStrgEqual = FALSE;
	m_chkNoround = FALSE;
	//}}AFX_DATA_INIT
	m_nSetMode = LEFTDOWN;
	m_pcView = (CTonView*)pParent;
}


void CDialogStrgVStruct::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrgVStruct)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Check(pDX, IDC_CHK_EQUAL, m_chkStrgEqual);
	DDX_Control(pDX, IDC_STRGVSTRCT1, m_stSetMode1);
	DDX_Control(pDX, IDC_STRGVSTRCT2, m_stSetMode2);
	DDX_Control(pDX, IDC_STRGVSTRCT3, m_stSetMode3);
	DDX_Check(pDX, IDC_CHK_NOROUND, m_chkNoround);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogStrgVStruct, CDialog)
	//{{AFX_MSG_MAP(CDialogStrgVStruct)
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DLGRESET, OnReset)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgVStruct メッセージ ハンドラ

void CDialogStrgVStruct::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	CRect	toprect;
	CRect	rect;

	m_stTop.GetWindowRect(&toprect);
	point.x += toprect.left;
	point.y += toprect.top;

	m_stSetMode1.GetWindowRect(&rect);
	if(rect.left < point.x && point.x < rect.right
	&& rect.top < point.y && point.y < rect.bottom){
		m_nSetMode = LEFTDOWN;
	}
	m_stSetMode2.GetWindowRect(&rect);
	if(rect.left < point.x && point.x < rect.right
	&& rect.top < point.y && point.y < rect.bottom){
		m_nSetMode = CNTRCNTR;
	}
	m_stSetMode3.GetWindowRect(&rect);
	if(rect.left < point.x && point.x < rect.right
	&& rect.top < point.y && point.y < rect.bottom){
		m_nSetMode = RIGHTUP;
	}
	CDialog::OnLButtonDown(nFlags, point);
	Invalidate();
}

void CDialogStrgVStruct::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	CRect	toprect;
	CRect	rect;
	CPen	*pcPen;
	CPen	*pcTPen;
	int		sx, sy, ex, ey;

	pcPen = new CPen(PS_INSIDEFRAME, 4, RGB(255, 0, 0));
	pcTPen=dc.SelectObject(pcPen);
	m_stTop.GetWindowRect(&toprect);
	switch(m_nSetMode){
	case LEFTDOWN:
		m_stSetMode1.GetWindowRect(&rect);
		break;
	case CNTRCNTR:
		m_stSetMode2.GetWindowRect(&rect);
		break;
	case RIGHTUP:
		m_stSetMode3.GetWindowRect(&rect);
		break;
	}
	sx = rect.left - toprect.left;
	ex = rect.right - toprect.left;
	sy = rect.top - toprect.top;
	ey = rect.bottom - toprect.top;
	dc.MoveTo(sx, sy);
	dc.LineTo(ex, sy);
	dc.LineTo(ex, ey);
	dc.LineTo(sx, ey);
	dc.LineTo(sx, sy);
	dc.SelectObject(pcTPen);
	delete pcPen;
}

void CDialogStrgVStruct::OnReset() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

void CDialogStrgVStruct::GetItemElement()
{
	UpdateData(TRUE);
}

void CDialogStrgVStruct::SetItemElement()
{
	UpdateData(FALSE);
}
