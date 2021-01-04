// DWkBar1.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuBar1.h"

#include "DialogWkPaperAdd.h"

//extern "C" {
#include "../../ObjLib/Baselib.h"
#include "../../ObjLib/Senvlib.h"
#include "../../ObjLib/Scrnlib.h"
#include "../../ObjLib/Wakuenv.h"
//}
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar1 ダイアログ

CDialogWakuBar1::CDialogWakuBar1(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	m_pcApp = (CTonApp*)::AfxGetApp();
	m_pcView = NULL;
	m_nFirstFlag = ON;
	//{{AFX_DATA_INIT(CDialogWakuBar1)
	m_csPaperName = _T("");
	//}}AFX_DATA_INIT
}

void CDialogWakuBar1::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuBar1)
	DDX_Control(pDX, IDC_CMBPAPER, m_cmbPaperName);
	DDX_CBString(pDX, IDC_CMBPAPER, m_csPaperName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogWakuBar1, CDialogBar)
	//{{AFX_MSG_MAP(CDialogWakuBar1)
	ON_CBN_SELCHANGE(IDC_CMBPAPER, OnSelchangeCmbpaper)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_WAKU_PAPERADD, OnWakuPaperAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar1 メッセージ ハンドラ

void CDialogWakuBar1::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirstFlag == ON){
		m_nFirstFlag = OFF;
		InitItemElement();
	}
}

void CDialogWakuBar1::InitItemElement()
{
	InitWakuPaperCombo();
}

void CDialogWakuBar1::InitWakuPaperCombo()
{
	int		idx;
	TCHAR	papername[QRECMAX];
	int		ret;

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBPAPER);
	pcCB->ResetContent();
	idx = 0;
	while(1){
		wakuenvlistgetpapername(idx, papername);
		if(papername[0] == _T('\0')){
			break;
		}
		pcCB->AddString(papername);
		idx++;
	}
	wakuenvgetpapername(papername);
	if (papername[0] == _T('\0')) {
		UpdateData(FALSE);
		return;
	}
	m_csPaperName = papername;
	ret = pcCB->FindStringExact(0, m_csPaperName);
	if(ret == CB_ERR){
		wakuenvlistgetpapername(0, papername);
		m_csPaperName = papername;
	}
	// ダイアログの文字列を検索し同じ物があればそのまま
	wakuenvsetpapername(papername);
	wakuenvgetpaperelement();
	UpdateData(FALSE);
}

void CDialogWakuBar1::OnSelchangeCmbpaper() 
{
	int		idx;
	TCHAR	papername[RECMAX];

	idx = m_cmbPaperName.GetCurSel();
	m_cmbPaperName.GetLBText(idx, papername);
	m_csPaperName = papername;
	wakuenvsetpapername(papername);
	wakuenvgetpaperelement();
}

void CDialogWakuBar1::OnWakuPaperAdd() 
{
	CDialogWkPaperAdd*	pDialogWkPaperAdd;
	int		ret;

	pDialogWkPaperAdd = new CDialogWkPaperAdd(m_pcView);
	ret = pDialogWkPaperAdd->DoModal();
	if(ret == IDOK){
		InitWakuPaperCombo();
	}
	delete(pDialogWkPaperAdd);
}

void CDialogWakuBar1::GetItemElement()
{
	UpdateData(TRUE);

}

void CDialogWakuBar1::SetItemElement()
{

	UpdateData(FALSE);
}

void CDialogWakuBar1::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

