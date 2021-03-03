// DlgMjSiz.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgMjSiz.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSize ダイアログ


CDialogMojiSize::CDialogMojiSize(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMojiSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMojiSize)
	m_edStepmm = 0.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_btnBig.LoadBitmaps(IDB_SIZEBIG1, IDB_SIZEBIG2, IDB_SIZEBIG3);
	m_btnSmall.LoadBitmaps(IDB_SIZESMALL1, IDB_SIZESMALL2, IDB_SIZESMALL3);
}


void CDialogMojiSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMojiSize)
	DDX_Text(pDX, IDC_ED_STEP, m_edStepmm);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMojiSize, CDialog)
	//{{AFX_MSG_MAP(CDialogMojiSize)
	ON_BN_CLICKED(IDC_DLGRESET, OnDlgReset)
	ON_BN_CLICKED(IDC_DLGSET, OnDlgSet)
	ON_BN_CLICKED(IDC_BTN_SIZEBIG, OnBtnSizebig)
	ON_BN_CLICKED(IDC_BTN_SIZESMALL, OnBtnSizesmall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSize メッセージ ハンドラ

BOOL CDialogMojiSize::OnInitDialog() 
{
	CDialog::OnInitDialog();

	VERIFY(m_btnSmall.SubclassDlgItem(IDC_BTN_SIZESMALL, this));
	m_btnSmall.SizeToContent();
	VERIFY(m_btnBig.SubclassDlgItem(IDC_BTN_SIZEBIG, this));
	m_btnBig.SizeToContent();
	// TODO: この位置に初期化の補足処理を追加してください
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogMojiSize::OnDlgReset() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
}

void CDialogMojiSize::OnDlgSet() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogMojiSize::GetItemElement()
{
	UpdateData(TRUE);
}

void CDialogMojiSize::SetItemElement()
{
	UpdateData(FALSE);
}

void CDialogMojiSize::OnBtnSizebig() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(USER1EVENT, 0, 0);
}

void CDialogMojiSize::OnBtnSizesmall() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(USER2EVENT, 0, 0);
}
