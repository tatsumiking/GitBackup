// DlInBshi.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlInBshi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogInnumBoshi ダイアログ


CDialogInnumBoshi::CDialogInnumBoshi(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogInnumBoshi::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogInnumBoshi)
	m_dInputNum = 0.0;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_nFirst = TRUE;
}


void CDialogInnumBoshi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogInnumBoshi)
	DDX_Control(pDX, IDRETRY, m_btnRetry);
	DDX_Control(pDX, IDC_EDNUM, m_edInputNum);
	DDX_Control(pDX, IDC_STUNIT, m_stUnit);
	DDX_Control(pDX, IDC_STTITLE, m_stTitle);
	DDX_Control(pDX, IDC_STINFO, m_stInfo);
	DDX_Text(pDX, IDC_EDNUM, m_dInputNum);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogInnumBoshi, CDialog)
	//{{AFX_MSG_MAP(CDialogInnumBoshi)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDRETRY, OnRetry)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogInnumBoshi メッセージ ハンドラ
void CDialogInnumBoshi::SetDialogTitle(LPSTR lpTitle)
{
	m_lpTitle = lpTitle;
}

void CDialogInnumBoshi::SetUnitText(LPSTR lpUnitText)
{
	m_lpUnitText = lpUnitText;
}

void CDialogInnumBoshi::SetTitleText(LPSTR lpTitleText)
{
	m_lpTitleText = lpTitleText;
}

void CDialogInnumBoshi::SetInfoText(LPSTR lpInfoText)
{
	m_lpInfoText = lpInfoText;
}

void CDialogInnumBoshi::SetRetryText(LPSTR lpRetryText)
{
	m_lpRetryText = lpRetryText;
}

BOOL CDialogInnumBoshi::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetWindowText(m_lpTitle);
	m_stUnit.SetWindowText(m_lpUnitText);
	m_stTitle.SetWindowText(m_lpTitleText);
	m_stInfo.SetWindowText(m_lpInfoText);
	m_btnRetry.SetWindowText(m_lpRetryText);
	return TRUE;
}

void CDialogInnumBoshi::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		m_edInputNum.SetSel(0, -1);
		m_edInputNum.SetFocus();
	}
}

void CDialogInnumBoshi::OnRetry() 
{
	CDialog::EndDialog(IDRETRY);
}
