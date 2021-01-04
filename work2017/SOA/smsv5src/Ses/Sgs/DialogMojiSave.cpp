// DlgMjSv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogMojiSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//extern "C" {
	#include "../../ObjLib/Mojienv.h"
//}

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSave ダイアログ


CDialogMojiSave::CDialogMojiSave(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMojiSave::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMojiSave)
	m_csFontName = _T("");
	m_csMojiCode = _T("");
	//}}AFX_DATA_INIT
}


void CDialogMojiSave::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMojiSave)
	DDX_CBString(pDX, IDC_COMBO1, m_csFontName);
	DDX_Text(pDX, IDC_EDIT1, m_csMojiCode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMojiSave, CDialog)
	//{{AFX_MSG_MAP(CDialogMojiSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSave メッセージ ハンドラ
void CDialogMojiSave::FontCBInit()
{
	int		idx;
	int		fonttype;
	TCHAR	fontname[FNAMEMAX];
	TCHAR	accessname[FNAMEMAX];

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
	idx = 0;
	mojienvflistopen(_T("r+b"));
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == _T('\0') && accessname[0] == _T('\0')){
			break;
		}
		pcCB->AddString(fontname);
		idx++;
	}
	mojienvflistclose();
	mojienvgetfontname(fontname);
	m_csFontName = fontname;
	idx = pcCB->FindString(-1, fontname);
	if(idx == -1){
		_tcscpy_s(fontname, _T("翠舟楷書体"));
		idx = pcCB->FindString(-1, fontname);
		if(idx == -1){
			idx = 0;
			pcCB->GetLBText(idx, fontname);
		}
		mojienvsetfontname(fontname);
		m_csFontName = fontname;
	}
	pcCB->SetCurSel(idx);
}

BOOL CDialogMojiSave::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	FontCBInit();
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}
