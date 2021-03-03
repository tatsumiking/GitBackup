// DlgPtnCr.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlgPtnCr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnCreate ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvgetsize(LPDBL dw, LPDBL dh);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvgetstrgslop(LPDBL slop);
}

CDialogPtnCreate::CDialogPtnCreate(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogPtnCreate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogPtnCreate)
	m_bTategaki = FALSE;
	m_dFlatPar = 0.0;
	m_dPitch = 0.0;
	m_dSize = 0.0;
	m_dSlopRag = 0.0;
	m_csText = _T("");
	m_dThickPar = 100.0;
	m_bRoundMode = FALSE;
	//}}AFX_DATA_INIT
	m_pcView = (CTonView*)pParent;
	m_hImc = 0;
}

void CDialogPtnCreate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogPtnCreate)
	DDX_Control(pDX, IDC_EDTEXT, m_edText);
	DDX_Control(pDX, IDC_CBSETMODE, m_cbSetMode);
	DDX_Control(pDX, IDC_CBUNIT, m_cbUnit);
	DDX_Control(pDX, IDC_CBODER, m_cbOder);
	DDX_Control(pDX, IDC_CBFONT, m_cbFont);
	DDX_Check(pDX, IDC_CHKTATEGAKI, m_bTategaki);
	DDX_Text(pDX, IDC_EDFLAT, m_dFlatPar);
	DDX_Text(pDX, IDC_EDPITCH, m_dPitch);
	DDX_Text(pDX, IDC_EDSIZE, m_dSize);
	DDX_Text(pDX, IDC_EDSLOP, m_dSlopRag);
	DDX_Text(pDX, IDC_EDTEXT, m_csText);
	DDX_Text(pDX, IDC_EDTHICK, m_dThickPar);
	DDX_Check(pDX, IDC_CHKROUNDMODE, m_bRoundMode);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogPtnCreate, CDialog)
	//{{AFX_MSG_MAP(CDialogPtnCreate)
	ON_BN_CLICKED(IDC_DLGRESET, OnReset)
	ON_CBN_SELCHANGE(IDC_CBUNIT, OnSelchangeCbunit)
	ON_CBN_SELCHANGE(IDC_CBODER, OnSelchangeCboder)
	ON_CBN_SELCHANGE(IDC_CBFONT, OnSelchangeCbfont)
	ON_CBN_SELCHANGE(IDC_CBSETMODE, OnSelchangeCbsetmode)
	ON_EN_SETFOCUS(IDC_EDTEXT, OnSetfocusEdtext)
	ON_EN_KILLFOCUS(IDC_EDTEXT, OnKillfocusEdtext)
	ON_BN_CLICKED(IDC_SETBTN, OnSetbtn)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnCreate メッセージ ハンドラ

void CDialogPtnCreate::OnReset() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

BOOL CDialogPtnCreate::OnInitDialog() 
{
	int		tategakiflag;
	DBL		dw, dh;
	DBL		dmsp, dlsp;
	DBL		slop;

	CDialog::OnInitDialog();
	
	FontListInit();
	OderListInit();
	UnitListInit();
	SetModeListInit();

	mojienvgettategakiflag(&tategakiflag);
	mojienvgetsize(&dw, &dh);
	mojienvgetspace(&dmsp, &dlsp);
	mojienvgetstrgslop(&slop);
	if(tategakiflag == ON){
		m_bTategaki = TRUE;
	}
	else{
		m_bTategaki = FALSE;
	}
	m_dPitch = (dw + dmsp) / dw;
	m_dPitch = FLOOR100(m_dPitch);
	m_dSizemm = dw;
	m_dSize = CnvertMMToUnit(m_dSizemm);
	m_dSize = FLOOR100(m_dSize);
	m_dThickPar = 100;
	m_dFlatPar = (dw / dh) * 100;
	m_dFlatPar = FLOOR100(m_dFlatPar);
	m_dSlopRag = 0;
	m_cbOder.SetCurSel(m_nOderNo);
	m_cbUnit.SetCurSel(m_nUnitNo);
	switch(m_nSetMode){
	case LEFTDOWN:
		m_cbSetMode.SetCurSel(0);
		break;	
	case CNTRCNTR:
		m_cbSetMode.SetCurSel(1);
		break;	
	case RIGHTUP:
	default:
		m_cbSetMode.SetCurSel(2);
		break;
	}
	UpdateData(FALSE);

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogPtnCreate::ElementGet()
{
	UpdateData(TRUE);

	m_dSizemm = CnvertUnitToMM(m_dSize);
}

void CDialogPtnCreate::ElementSet()
{
	FontListInit();
	m_dSize = CnvertMMToUnit(m_dSizemm);
	m_dSize = FLOOR10(m_dSize);
	m_cbOder.SetCurSel(m_nOderNo);
	m_cbUnit.SetCurSel(m_nUnitNo);
	switch(m_nSetMode){
	case LEFTDOWN:
		m_cbSetMode.SetCurSel(0);
		break;	
	case CNTRCNTR:
		m_cbSetMode.SetCurSel(1);
		break;	
	case RIGHTUP:
		m_cbSetMode.SetCurSel(2);
		break;
	}
	UpdateData(FALSE);
}

void CDialogPtnCreate::FontListInit()
{
	int		idx, slctidx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	m_cbFont.Clear();
	idx = 0;
	mojienvgetfontname(m_szFontName);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		if(strcmp(fontname, m_szFontName) == 0){
			slctidx = idx;
		}
		m_cbFont.AddString(fontname);
		idx++;
	}
	m_cbFont.SetCurSel(slctidx);
}

void CDialogPtnCreate::OnSelchangeCbunit() 
{
	int		idx;

	idx = m_cbUnit.GetCurSel();

	UpdateData(TRUE);

	m_dSizemm = CnvertUnitToMM(m_dSize);
	m_nUnitNo = idx;
	m_dSize = CnvertMMToUnit(m_dSizemm);
	m_dSize = FLOOR10(m_dSize);
	UpdateData(FALSE);
}

void CDialogPtnCreate::OnSelchangeCboder() 
{
	int		idx;

	idx = m_cbOder.GetCurSel();
	m_nOderNo =  idx;
}

void CDialogPtnCreate::OnSelchangeCbfont() 
{
	int		idx;

	idx = m_cbFont.GetCurSel();
	m_cbFont.GetLBText(idx, m_szFontName);
}

void CDialogPtnCreate::OnSelchangeCbsetmode() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	int		idx;

	idx = m_cbSetMode.GetCurSel();
	switch(idx){
	case 0:
		m_nSetMode = LEFTDOWN;
		break;	
	case 1:
		m_nSetMode = CNTRCNTR;
		break;	
	case 2:
		m_nSetMode = RIGHTUP;
		break;
	}
}

void CDialogPtnCreate::OderListInit()
{
	m_cbOder.AddString("均等割付");
	m_cbOder.AddString("左(上)揃え");
	m_cbOder.AddString("中央揃え");
	m_cbOder.AddString("右(下)揃え");
}

void CDialogPtnCreate::UnitListInit()
{
	m_cbUnit.AddString("ポイント");
	m_cbUnit.AddString("級数");
	m_cbUnit.AddString("ｍｍ");
}

void CDialogPtnCreate::SetModeListInit()
{
	m_cbSetMode.AddString("右(下)基準");
	m_cbSetMode.AddString("中央基準");
	m_cbSetMode.AddString("左(上)基準");
}

DBL CDialogPtnCreate::CnvertMMToUnit(DBL mm)
{
	DBL		unit;

	switch(m_nUnitNo){
	case UNITPOINT:	// 0
		unit = mm * 2.8345;
		break;
	case UNITKYUSU:
		unit = mm * 4.0;
		break;
	case UNITMM:
	default:
		unit = mm;
		break;
	}
	//unit = FLOOR100(unit);
	return(unit);
}

DBL CDialogPtnCreate::CnvertUnitToMM(DBL unit)
{
	DBL		mm;

	switch(m_nUnitNo){
	case UNITPOINT:	// 0
		mm = unit * 0.3528;
		break;
	case UNITKYUSU:
		mm = unit * 0.25;
		break;
	case UNITMM:
	default:
		mm = unit;
		break;
	}
	//mm = FLOOR100(mm);
	return(mm);
}


void CDialogPtnCreate::OnSetfocusEdtext() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(m_hImc == 0){
		m_hImc = ::ImmCreateContext();
		::ImmAssociateContext(m_edText.m_hWnd, m_hImc);
		::ImmSetOpenStatus(m_hImc, true);	
	}
}

void CDialogPtnCreate::OnKillfocusEdtext() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	if(m_hImc != 0){
		::ImmAssociateContext(m_edText.m_hWnd, NULL);
		::ImmDestroyContext(m_hImc);
		m_hImc = 0;
	}		
}

void CDialogPtnCreate::OnSetbtn() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}


