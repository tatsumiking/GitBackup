// DlInPair.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlInPair.h"

#define MODAL		0
#define	MODALESS	1

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNumPair ダイアログ


CDialogInputNumPair::CDialogInputNumPair(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInputNumPair::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInputNumPair)
	m_dInputNumX = 0.0;
	m_dInputNumY = 0.0;
	//}}AFX_DATA_INIT
	m_nMode = MODAL;
	m_nFirst = TRUE;
	m_pcView = (CTonView*)pParent;
}


void CDialogInputNumPair::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInputNumPair)
	DDX_Control(pDX, IDC_EDNUMX, m_edInputNumX);
	DDX_Control(pDX, IDC_EDNUMY, m_edInputNumY);
	DDX_Control(pDX, IDOK, m_cbtnOK);
	DDX_Control(pDX, IDCANCEL, m_cbtnCancel);
	DDX_Control(pDX, IDC_STUNITX, m_stUnitX);
	DDX_Control(pDX, IDC_STUNITY, m_stUnitY);
	DDX_Control(pDX, IDC_STTITLEX, m_stTitleX);
	DDX_Control(pDX, IDC_STTITLEY, m_stTitleY);
	DDX_Text(pDX, IDC_EDNUMX, m_dInputNumX);
	DDX_Text(pDX, IDC_EDNUMY, m_dInputNumY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInputNumPair, CDialog)
	//{{AFX_MSG_MAP(CDialogInputNumPair)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInputNumPair メッセージ ハンドラ

void CDialogInputNumPair::SetDialogTitle(LPSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

void CDialogInputNumPair::SetUnitText(LPSTR lpUnitTextX, LPSTR lpUnitTextY)
{
	m_lpUnitTextX = lpUnitTextX;
	m_lpUnitTextY = lpUnitTextY;
}

void CDialogInputNumPair::SetTitleText(LPSTR lpTitleTextX, LPSTR lpTitleTextY)
{
	m_lpTitleTextX = lpTitleTextX;
	m_lpTitleTextY = lpTitleTextY;
}

BOOL CDialogInputNumPair::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	SetWindowText(m_lpTitle);
	m_stUnitX.SetWindowText(m_lpUnitTextX);
	m_stUnitY.SetWindowText(m_lpUnitTextY);
	m_stTitleX.SetWindowText(m_lpTitleTextX);
	m_stTitleY.SetWindowText(m_lpTitleTextY);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogInputNumPair::SetModaless()
{
	//m_cbtnOK->ShowWindow(SW_HIDE);
	m_nMode = MODALESS;
}

void CDialogInputNumPair::SetModal()
{
	m_nMode = MODAL;
}

void CDialogInputNumPair::OnCancel() 
{
	// TODO: この位置に特別な後処理を追加してください。
	
	if(m_nMode == MODAL){
		CDialog::OnCancel();
	}
	else{
		m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
	}
}

void CDialogInputNumPair::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	if(m_nMode == MODAL){
		CDialog::OnOK();
	}
	else{
		m_pcView->CommandExec(DLGSETEVENT, 0, 0);
	}
}

void CDialogInputNumPair::GetItemElement()
{
	UpdateData(TRUE);
}

void CDialogInputNumPair::SetItemElement()
{
	UpdateData(FALSE);
}

void CDialogInputNumPair::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		m_edInputNumX.SetSel(0, -1);
		m_edInputNumX.SetFocus();
	}
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください

	// 描画用メッセージとして CDialog::OnPaint() を呼び出してはいけません
}
