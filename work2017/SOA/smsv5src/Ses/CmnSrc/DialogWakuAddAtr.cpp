// DlgWkAtr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuAddAtr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAddAtr ダイアログ


CDialogWakuAddAtr::CDialogWakuAddAtr(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogWakuAddAtr::IDD, pParent)
{
	m_pcView = (CScrollView*)pParent;
	//{{AFX_DATA_INIT(CDialogWakuAddAtr)
	m_csInputStrg = _T("");
	m_csMatching = _T("");
	m_dTrns = 0.0;
	//}}AFX_DATA_INIT
	// CG: 次の行は 「IME サポート」 コンポーネントにより追加されています。
	m_hImc = ::ImmCreateContext();
}

CDialogWakuAddAtr::~CDialogWakuAddAtr()
{
	// CG: 次の行は 「IME サポート」コンポーネントにより追加されています。
	::ImmAssociateContext(m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
}

void CDialogWakuAddAtr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuAddAtr)
	DDX_Text(pDX, IDC_INSTRG, m_csInputStrg);
	DDX_Text(pDX, IDC_MATCHING, m_csMatching);
	DDX_Text(pDX, IDC_TRNS, m_dTrns);
	DDX_Check(pDX, IDC_CHKFIXED, m_nChkFixed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogWakuAddAtr, CDialog)
	//{{AFX_MSG_MAP(CDialogWakuAddAtr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuAddAtr メッセージ ハンドラ


BOOL CDialogWakuAddAtr::OnInitDialog() 
{
	CRect	rect;
	int		sx, sy, sizex, sizey;

	CDialog::OnInitDialog();

	GetWindowRect(&rect);
	sx = rect.left;
	sy = rect.top;
	sizex = rect.Width();
	sizey = rect.Height();
	::SetWindowPos(m_hWnd, HWND_TOPMOST, sx, sy, sizex, sizey, 0);
	// TODO: この位置に初期化の補足処理を追加してください
	UpdateData(FALSE);
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogWakuAddAtr::OnOK() 
{
	// TODO: この位置にその他の検証用のコードを追加してください
	UpdateData(TRUE);
	CDialog::OnOK();
}

