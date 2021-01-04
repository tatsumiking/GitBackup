// DlgImgSize.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogImageSize.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSize ダイアログ


CDialogImageSize::CDialogImageSize(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogImageSize::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogImageSize)
	m_nNoWakuOut = FALSE;
	m_dPrntBlkXStart = 0.0;
	m_dPrntBlkYStart = 0.0;
	m_dPrntBlkXSize = 0.0;
	m_dPrntBlkYSize = 0.0;
	m_dPrntBlkXSpace = 0.0;
	m_dPrntBlkYSpace = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogImageSize::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogImageSize)
	DDX_Check(pDX, IDC_CHKNOWAKUOUT, m_nNoWakuOut);
	DDX_Text(pDX, IDC_EDPRNTBLKXSTART, m_dPrntBlkXStart);
	DDX_Text(pDX, IDC_EDPRNTBLKYSTART, m_dPrntBlkYStart);
	DDX_Text(pDX, IDC_EDPRNTBLKXSIZE, m_dPrntBlkXSize);
	DDX_Text(pDX, IDC_EDPRNTBLKYSIZE, m_dPrntBlkYSize);
	DDX_Text(pDX, IDC_EDPRNTBLKXSPACE, m_dPrntBlkXSpace);
	DDX_Text(pDX, IDC_EDPRNTBLKYSPACE, m_dPrntBlkYSpace);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogImageSize, CDialog)
	//{{AFX_MSG_MAP(CDialogImageSize)
		// メモ - ClassWizard はこの位置にマッピング用のマクロを追加または削除します。
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSize メッセージ ハンドラ
