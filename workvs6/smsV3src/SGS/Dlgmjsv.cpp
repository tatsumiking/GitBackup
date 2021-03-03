// DlgMjSv.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgMjSv.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
}

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
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_COMBO1);
	idx = 0;
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		pcCB->AddString(fontname);
		idx++;
	}
	mojienvgetfontname(fontname);
	m_csFontName = _T(fontname);
	idx = pcCB->FindString(-1, fontname);
	if(idx == -1){
		strcpy(fontname, "翠舟楷書体");
		idx = pcCB->FindString(-1, fontname);
		if(idx == -1){
			idx = 0;
			pcCB->GetLBText(idx, fontname);
		}
		mojienvsetfontname(fontname);
		m_csFontName = _T(fontname);
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
