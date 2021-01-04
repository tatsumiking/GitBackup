// DialogEditBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogEditBar.h"

//extern "C" {
#include "../../ObjLib/Mojienv.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEditBar ダイアログ

CDialogEditBar::CDialogEditBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CDialogEditBar)
	m_csEditStrg = _T("");
	//}}AFX_DATA_INIT
}

void CDialogEditBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEditBar)
	DDX_Text(pDX, IDC_EDITSTRG, m_csEditStrg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEditBar, CDialogBar)
	//{{AFX_MSG_MAP(CDialogEditBar)
	ON_EN_SETFOCUS(IDC_EDITSTRG, OnSetfocusEditstrg)
	ON_EN_CHANGE(IDC_EDITSTRG, OnChangeEditstrg)
	ON_EN_KILLFOCUS(IDC_EDITSTRG, OnKillfocusEditstrg)
	ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEditBar メッセージ ハンドラ

void CDialogEditBar::InitItemElement()
{
	UpdateData(FALSE);
}

void CDialogEditBar::SetItemElement()
{
	TCHAR	szStr[RECMAX];

	mojienvgetstring(szStr);
	m_csEditStrg = szStr;
	UpdateData(FALSE);
}

void CDialogEditBar::GetItemElement()
{
	UpdateData(TRUE);
	mojienvsetstring(m_csEditStrg.GetBuffer(RECMAX));
}

void CDialogEditBar::OnSetfocusEditstrg() 
{
	CEdit *pcEd = (CEdit*)GetDlgItem(IDC_EDITSTRG);
	m_hImc = ::ImmCreateContext();
	::ImmAssociateContext(pcEd->m_hWnd, m_hImc);
	::ImmSetOpenStatus(m_hImc, true);	
}

void CDialogEditBar::OnChangeEditstrg() 
{
	// TODO: これが RICHEDIT コントロールの場合、コントロールは、 lParam マスク
	// 内での論理和の ENM_CHANGE フラグ付きで EM_SETEVENTMASK
	// メッセージをコントロールへ送るために CDialogBar::OnInitDialog() 関数をオーバー
	// ライドしない限りこの通知を送りません。
	
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
	UpdateData(TRUE);
}

void CDialogEditBar::OnKillfocusEditstrg() 
{
	CEdit* pcEd = (CEdit*)GetDlgItem(IDC_EDITSTRG);
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	::ImmAssociateContext(pcEd->m_hWnd, NULL);
	::ImmDestroyContext(m_hImc);
}

BOOL CDialogEditBar::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	// TODO: この位置にメッセージ ハンドラ用のコードを追加するかまたはデフォルトの処理を呼び出してください
	
	return CDialogBar::OnSetCursor(pWnd, nHitTest, message);
}
