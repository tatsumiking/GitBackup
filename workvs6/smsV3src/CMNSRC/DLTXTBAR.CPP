// DialogTextInBar.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DlTxtBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define	MMTOPOINT	2.8345
#define POINTTOMM	0.3528

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInBar ダイアログ
extern "C" {
	void mojienvgetfonttypename(LPSTR fontname, LPINT retftype, LPSTR fontfullname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvsetsize(DBL dfwidth, DBL dfhight);
	void mojienvsetspace(DBL dmojispace, DBL dlinespace);
	void mojienvsetstrlen(DBL slen);
	void mojienvsetorder(int mode);
	void mojienvsettategakiflag(int flag);
	int mojienvcheckbold();
	int mojienvcheckitric();
	int mojienvcheckunderl();
	void mojienvgetorder(LPINT mode);
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvsettategakiflag(int tateflg);
}

CDialogTextInBar::CDialogTextInBar(CWnd* pParent /*=NULL*/)
	: CMyDialogBar(pParent)
{
	DBL		mm;

	m_pcView = (CTonView*)pParent;
	m_nFirst = ON;
	//{{AFX_DATA_INIT(CDialogTextInBar)
	m_csFontName = _T("ＭＳ 明朝");
	m_csFontSize = _T("10.5pt");
	m_dStrgLen = 5.0;
	//}}AFX_DATA_INIT


	mm = 10.5 * POINTTOMM;
	mojienvsetorder(ODERLEFT);
	mojienvsetfontname("ＭＳ明朝");
	mojienvsettategakiflag(OFF);
	mojienvsetsize(mm, mm);
	mojienvsetspace(0.0, 0.0);
	mojienvsetstrlen(m_dStrgLen);
	mojienvsettategakiflag(OFF);
}

void CDialogTextInBar::DoDataExchange(CDataExchange* pDX)
{
	CMyDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogTextInBar)
	DDX_Control(pDX, ID_CMB_FONTSIZE, m_cbFontSize);
	DDX_Control(pDX, ID_CMB_FONTNAME, m_cbFontName);
	DDX_Control(pDX, ID_BTN_TATEGAKI, m_btnTextTategaki);
	DDX_Control(pDX, ID_BTN_UNDERL, m_btnTextUnderLine);
	DDX_Control(pDX, ID_BTN_RIGHT, m_btnTextRight);
	DDX_Control(pDX, ID_BTN_LEFT, m_btnTextLeft);
	DDX_Control(pDX, ID_BTN_ITRIC, m_btnTextItric);
	DDX_Control(pDX, ID_BTN_EQUAL, m_btnTextEqul);
	DDX_Control(pDX, ID_BTN_CNTR, m_btnTextCntr);
	DDX_Control(pDX, ID_BTN_BOLD, m_btnTextBold);
	DDX_CBString(pDX, ID_CMB_FONTNAME, m_csFontName);
	DDX_CBString(pDX, ID_CMB_FONTSIZE, m_csFontSize);
	DDX_Text(pDX, IDC_EDSTRGLEN, m_dStrgLen);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogTextInBar, CMyDialogBar)
	//{{AFX_MSG_MAP(CDialogTextInBar)
	ON_WM_PAINT()
	ON_BN_CLICKED(ID_BTN_BOLD, OnBtnBold)
	ON_BN_CLICKED(ID_BTN_ITRIC, OnBtnItric)
	ON_BN_CLICKED(ID_BTN_UNDERL, OnBtnUnderl)
	ON_BN_CLICKED(ID_BTN_LEFT, OnBtnLeft)
	ON_BN_CLICKED(ID_BTN_CNTR, OnBtnCntr)
	ON_BN_CLICKED(ID_BTN_RIGHT, OnBtnRight)
	ON_BN_CLICKED(ID_BTN_EQUAL, OnBtnEqul)
	ON_BN_CLICKED(ID_BTN_TATEGAKI, OnBtnTategaki)
	ON_EN_CHANGE(IDC_EDSTRGLEN, OnChangeEdstrglen)
	ON_CBN_SELCHANGE(ID_CMB_FONTNAME, OnSelchangeCmbFontname)
	ON_CBN_SELCHANGE(ID_CMB_FONTSIZE, OnSelchangeCmbFontsize)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogTextInBar メッセージ ハンドラ

void CDialogTextInBar::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト

	if(m_nFirst == ON){
		m_nFirst = OFF;

		VERIFY(m_cbFontName.SubclassDlgItem(ID_CMB_FONTNAME, this));
		VERIFY(m_cbFontSize.SubclassDlgItem(ID_CMB_FONTSIZE, this));
		VERIFY(m_btnTextBold.SubclassDlgItem(ID_BTN_BOLD, this));
		VERIFY(m_btnTextItric.SubclassDlgItem(ID_BTN_ITRIC, this));
		VERIFY(m_btnTextUnderLine.SubclassDlgItem(ID_BTN_UNDERL, this));
		VERIFY(m_btnTextLeft.SubclassDlgItem(ID_BTN_LEFT, this));
		VERIFY(m_btnTextCntr.SubclassDlgItem(ID_BTN_CNTR, this));
		VERIFY(m_btnTextRight.SubclassDlgItem(ID_BTN_RIGHT, this));
		VERIFY(m_btnTextEqul.SubclassDlgItem(ID_BTN_EQUAL, this));
		VERIFY(m_btnTextTategaki.SubclassDlgItem(ID_BTN_TATEGAKI, this));

		SetButtonElement();
		InitFontList();
		InitFontSize();

		UpdateData(FALSE);
	}
}

void CDialogTextInBar::GetItemElement()
{
	UpdateData(TRUE);
	GetCmbFontName();
	GetCmbFontSize();
}

void CDialogTextInBar::SetItemElement()
{
	SetButtonElement();
	Invalidate();
}

void CDialogTextInBar::SetButtonElement()
{
	int		mode;
	int		flag;

	if(mojienvcheckbold() == ON){
		m_btnTextBold.LoadBitmaps(IDB_BTN_BOLD_DOWN, NULL, NULL, NULL);
	}
	else{
		m_btnTextBold.LoadBitmaps(IDB_BTN_BOLD_UP, NULL, NULL, NULL);
	}
	if(mojienvcheckitric() == ON){
		m_btnTextItric.LoadBitmaps(IDB_BTN_ITRIC_DOWN, NULL, NULL);
	}
	else{
		m_btnTextItric.LoadBitmaps(IDB_BTN_ITRIC_UP, NULL, NULL);
	}
	if(mojienvcheckunderl() == ON){
		m_btnTextUnderLine.LoadBitmaps(IDB_BTN_UNDERL_DOWN, NULL, NULL);
	}
	else{
		m_btnTextUnderLine.LoadBitmaps(IDB_BTN_UNDERL_UP, NULL, NULL);
	}

	m_btnTextLeft.LoadBitmaps(IDB_BTN_LEFT_UP, NULL, NULL);
	m_btnTextCntr.LoadBitmaps(IDB_BTN_CNTR_UP, NULL, NULL);
	m_btnTextRight.LoadBitmaps(IDB_BTN_RIGHT_UP, NULL, NULL);
	m_btnTextEqul.LoadBitmaps(IDB_BTN_EQUAL_UP, NULL, NULL);
	mojienvgetorder(&mode);
	switch(mode){
	case ODERLEFT:
		m_btnTextLeft.LoadBitmaps(IDB_BTN_LEFT_DOWN, NULL, NULL);
		break;
	case ODERCNTR:
		m_btnTextCntr.LoadBitmaps(IDB_BTN_CNTR_DOWN, NULL, NULL);
		break;
	case ODERRIGHT:
		m_btnTextRight.LoadBitmaps(IDB_BTN_RIGHT_DOWN, NULL, NULL);
		break;
	case ODEREQUAL:
		m_btnTextEqul.LoadBitmaps(IDB_BTN_EQUAL_DOWN, NULL, NULL);
		break;
	}
	mojienvgettategakiflag(&flag);
	if(flag == ON){
		m_btnTextTategaki.LoadBitmaps(IDB_BTN_TATEGAKI_DOWN, NULL, NULL);
	}
	else{
		m_btnTextTategaki.LoadBitmaps(IDB_BTN_TATEGAKI_UP, NULL, NULL);
	}
}

void CDialogTextInBar::InitFontList()
{
	int		idx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	idx = 0;
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		m_cbFontName.AddString(fontname);
		idx++;
	}
}

void CDialogTextInBar::InitFontSize()
{
	m_dFontSize = 10.5;
	m_csFontSize.Format("%4.1lfpt", m_dFontSize);
	m_cbFontSize.AddString(" 8pt");
	m_cbFontSize.AddString(" 9pt");
	m_cbFontSize.AddString("10pt");
	m_cbFontSize.AddString("11pt");
	m_cbFontSize.AddString("12pt");
	m_cbFontSize.AddString("14pt");
	m_cbFontSize.AddString("16pt");
	m_cbFontSize.AddString("18pt");
	m_cbFontSize.AddString("20pt");
	m_cbFontSize.AddString("22pt");
	m_cbFontSize.AddString("24pt");
	m_cbFontSize.AddString("26pt");
	m_cbFontSize.AddString("28pt");
	m_cbFontSize.AddString("36pt");
	m_cbFontSize.AddString("48pt");
	m_cbFontSize.AddString("72pt");
}

void CDialogTextInBar::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

void CDialogTextInBar::GetCmbFontName() 
{
	char	fontname[RECMAX];

	UpdateData(TRUE);
	strcpy(fontname, m_csFontName.GetBuffer(RECMAX));
	mojienvsetfontname(fontname);
}

void CDialogTextInBar::GetCmbFontSize() 
{
	char	pointstr[RECMAX];
	DBL		point, mm;

	UpdateData(TRUE);
	strcpy(pointstr, m_csFontSize.GetBuffer(RECMAX));
	sscanf(pointstr, "%lf", &point);
	mm = point * POINTTOMM;
	mojienvsetsize(mm, mm);
	mojienvsetspace(0.0, 0.0);
}

void CDialogTextInBar::OnChangeEdstrglen() 
{
	UpdateData(TRUE);
	mojienvsetstrlen(m_dStrgLen);
}

void CDialogTextInBar::OnBtnBold() 
{
	m_pcView->OnBtnBold();
}

void CDialogTextInBar::OnBtnItric() 
{
	m_pcView->OnBtnItric();
}

void CDialogTextInBar::OnBtnUnderl() 
{
	m_pcView->OnBtnUnderl();
}

void CDialogTextInBar::OnBtnLeft() 
{
	m_pcView->OnBtnLeft();
}

void CDialogTextInBar::OnBtnCntr() 
{
	m_pcView->OnBtnCntr();
}

void CDialogTextInBar::OnBtnRight() 
{
	m_pcView->OnBtnRight();
}

void CDialogTextInBar::OnBtnEqul() 
{
	m_pcView->OnBtnEqul();
}

void CDialogTextInBar::OnBtnTategaki() 
{
	m_pcView->OnBtnTategaki();
}

BOOL CDialogTextInBar::OnInitDialog() 
{
	//CMyDialogBar::OnInitDialog();
	
	// TODO: この位置に初期化の補足処理を追加してください
	
	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}


void CDialogTextInBar::OnSelchangeCmbFontname() 
{
	int		idx;
	char	fontname[RECMAX];

	idx = m_cbFontName.GetCurSel();
	m_cbFontName.GetLBText(idx, fontname);
	m_csFontName = _T(fontname);
	UpdateData(FALSE);
	m_pcView->OnCmbFontname();
}

void CDialogTextInBar::OnSelchangeCmbFontsize() 
{
	int		idx;
	char	fontsize[RECMAX];

	idx = m_cbFontSize.GetCurSel();
	m_cbFontSize.GetLBText(idx, fontsize);
	m_csFontSize = _T(fontsize);
	UpdateData(FALSE);
	m_pcView->OnCmbFontsize();
}
