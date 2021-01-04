// DlgTwPnt.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "Dialog2PointSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog2PointSize ダイアログ


CDialog2PointSize::CDialog2PointSize(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2PointSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog2PointSize)
	m_dXSize = 0.0;
	m_dYSize = 0.0;
	m_dLength = 0.0;
	//}}AFX_DATA_INIT
}


void CDialog2PointSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog2PointSize)
	DDX_Text(pDX, IDC_EDXSIZE, m_dXSize);
	DDX_Text(pDX, IDC_EDYSIZE, m_dYSize);
	DDX_Text(pDX, IDC_EDLENGTH, m_dLength);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog2PointSize, CDialog)
	//{{AFX_MSG_MAP(CDialog2PointSize)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog2PointSize メッセージ ハンドラ
