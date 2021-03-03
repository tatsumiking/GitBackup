// DlgStrIn.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "MyEdit.h"
#include "DlgStrIn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInput ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgetbkclr(int *rval, int *gval, int *bval);

	void mojienvsetbold(int flag);
	int mojienvcheckbold();
	void mojienvsetitric(int flag);
	int mojienvcheckitric();

	void mojienvsetmaxlegal(int flag);
	int	mojienvcheckmaxlegal();
	void mojienvgettrnslimit(LPDBL dlimit);
	void mojienvsettrnslimit(DBL dlimit);

	void mojienvgetfonttypename(LPSTR fontname, LPINT retftype, LPSTR fontfullname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvgettategakiflag(LPINT tateflg);
	void mojienvgetspacekind(LPINT spkflg);
	void mojienvgetstart(LPDBL dstartx, LPDBL dstarty);
	void mojienvgetsize(LPDBL dw, LPDBL dh);
	void mojienvgetspace(LPDBL dmsp, LPDBL dlsp);
	void mojienvgetstrlen(LPDBL dlen);
	void mojienvgetgyakukind(LPINT nkind);
	void mojienvgetsmpltext(LPSTR smpl);
	void mojienvgetarngkind(LPINT nkind);
	void mojienvgetorder(LPINT nkind);
	void mojienvgetstrgslop(LPDBL dslop);
	void mojienvgetmojiround(LPDBL dround);

	void mojienvsetfontname(LPSTR fontname);
	void mojienvsettategakiflag(int tateflg);
	void mojienvsetspacekind(int spkflg);
	void mojienvsetstart(DBL dstartx, DBL dstarty);
	void mojienvsetsize(DBL dw, DBL dh);
	void mojienvsetspace(DBL dmsp, DBL dlsp);
	void mojienvsetstrlen(DBL dlen);
	void mojienvsetstrgslop(DBL slop);
	void mojienvsetmojiround(DBL round);
	void mojienvsetgyakukind(int nkind);
	void mojienvsetsmpltext(LPSTR smpl);
	void mojienvsetarngkind(int nkind);
	void mojienvsetorder(int nkind);
	void mojienvsetstrgslop(DBL dslop);
	void mojienvsetmojiround(DBL dround);

	DBL scrnsetxytime(DBL timex, DBL timey);
	void scrngetmmsize(LPDBL bxmm, LPDBL bymm);
	void scrnsetmmsize(DBL xsize, DBL ysize);
	void scrnsetrltvelement();
	void scrngetrltvtop(LPDBL sx, LPDBL sy); 
	void scrnsetrltvtop(DBL sx, DBL sy); 
	void scrnrltvareaset(DBL sx, DBL sy, DBL ex, DBL ey);
	void scrnsetdisprltvmove(DBL xmove, DBL ymove);
	void scrngetdisprltvmove(LPDBL xmove, LPDBL ymove);
}

CDialogStrgInput::CDialogStrgInput(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogStrgInput::IDD, pParent)
{
	long	lsize;
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	//{{AFX_DATA_INIT(CDialogStrgInput)
	m_fCheckGyaku = FALSE;
	m_fCheckTategaki = FALSE;
	m_dHight = 0.0;
	m_dMojiSpace = 0.0;
	m_csSmplText = _T("");
	m_csFontName = _T("");
	m_dStartX = 0.0;
	m_dStartY = 0.0;
	m_dStrgLen = 0.0;
	m_dStrgSlop = 0.0;
	m_dStrgSpace = 0.0;
	m_dWidth = 0.0;
	m_csStrgOrder = _T("");
	m_fCheckMojiData = FALSE;
	m_stStrgSize = _T("");
	m_dTrnsLimit = 0.0;
	m_fBold = FALSE;
	m_fItric = FALSE;
	//}}AFX_DATA_INIT

	mojienvsetsmpltext("ＡＢａｂあア８亜");
	// mojienvsetsmpltext("123ABCabc");
	m_pcView = (CTonView*)pParent;
	lsize = 8192;
	hEditBuf = ::GlobalAlloc(GHND, lsize);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\strgtext.lst", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		fclose(fp);
	}
}

CDialogStrgInput::~CDialogStrgInput()
{
	int		aa;

	aa = 0;
}

void CDialogStrgInput::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogStrgInput)
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_CREATE, m_btnCreate);
	DDX_Check(pDX, IDC_CKGYAKU, m_fCheckGyaku);
	DDX_Check(pDX, IDC_CKTATEGAKI, m_fCheckTategaki);
	DDX_Text(pDX, IDC_EDHIGHT, m_dHight);
	DDX_Text(pDX, IDC_EDMOJISPACE, m_dMojiSpace);
	DDX_Text(pDX, IDC_EDSMPLTEXT, m_csSmplText);
	DDX_CBString(pDX, IDC_CBFONTNAME, m_csFontName);
	DDX_Text(pDX, IDC_EDSTARTX, m_dStartX);
	DDX_Text(pDX, IDC_EDSTARTY, m_dStartY);
	DDX_Text(pDX, IDC_EDSTRGLEN, m_dStrgLen);
	DDX_Text(pDX, IDC_EDSTRGSLOP, m_dStrgSlop);
	DDX_Text(pDX, IDC_EDSTRGSPACE, m_dStrgSpace);
	DDX_Text(pDX, IDC_EDWIDTH, m_dWidth);
	DDX_CBString(pDX, IDC_CBSTRGORDER, m_csStrgOrder);
	DDX_Check(pDX, IDC_CKMOJIDATA, m_fCheckMojiData);
	DDX_Text(pDX, IDC_STSTRGSIZE, m_stStrgSize);
	DDX_Text(pDX, IDC_EDTRNSLIMIT, m_dTrnsLimit);
	DDX_Check(pDX, IDC_CHKBOLD, m_fBold);
	DDX_Check(pDX, IDC_CHKITRIC, m_fItric);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogStrgInput, CMyDialog)
	//{{AFX_MSG_MAP(CDialogStrgInput)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CREATE, OnCreate)
	ON_BN_CLICKED(IDC_CKGYAKU, OnCkgyaku)
	ON_CBN_SELCHANGE(IDC_CBFONTNAME, OnSelchangeCbfontname)
	ON_EN_UPDATE(IDC_EDSMPLTEXT, OnUpdateEdsmpltext)
	ON_WM_PAINT()
	ON_CBN_EDITCHANGE(IDC_CBFONTNAME, OnEditchangeCbfontname)
	ON_BN_CLICKED(IDC_CKMOJIDATA, OnCkmojidata)
	ON_BN_CLICKED(IDC_INPEND, OnInpend)
	ON_BN_CLICKED(IDC_CANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgInput メッセージ ハンドラ

BOOL CDialogStrgInput::OnInitDialog() 
{
	CRect	rect, toprect;
	int		xsize, ysize;
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	strg[RECMAX];
	char	dda[5];

	CMyDialog::OnInitDialog();

	CStatic *pcST = (CStatic*)GetDlgItem(IDC_AREA);
	CStatic *pcSTTop = (CStatic*)GetDlgItem(IDC_STTOP);

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CBSTRGORDER);
	pCB->AddString("左寄せ");
	pCB->AddString("中央");
	pCB->AddString("右寄せ");

	pcST->GetWindowRect(&rect);
	pcSTTop->GetWindowRect(&toprect);
	xsize = rect.right - rect.left;
	ysize = rect.bottom - rect.top;
	rect.left = rect.left - toprect.left;
	rect.top = rect.top - toprect.top;
	rect.right = rect.left + xsize;
	rect.bottom = rect.top + ysize;
	m_pcEdit = new CMyEdit();
	m_pcEdit->Create(EDITMLTSTYLE, rect, this, IDC_EDINPUT);
	m_pcEdit->SetIMEMode(ON);

	dda[0] = 0x0d;
	dda[1] = 0x0d;
	dda[2] = 0x0a;
	dda[3] = 0x00;
	// TODO: この位置に初期化の補足処理を追加してください
	senvgetexepath(exepath);
	sprintf(fname, "%s\\strgtext.lst", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return TRUE;
	}
	lpEditBuf = (LPSTR)::GlobalLock(hEditBuf);
	if(lpEditBuf != 0){
		lpEditBuf[0] = '\0';
		while(1){
			if(CrDelFgets(strg, RECMAX, fp) == NULL){
				break;
			}
			strcat(lpEditBuf, strg);
			strcat(lpEditBuf, dda);
		}
		SetDlgItemText(IDC_EDINPUT, lpEditBuf);
		::GlobalUnlock(hEditBuf);
	}
	fclose(fp);
	
	InitFontList();

	m_nFirstFlag = ON;

	if(mojienvcheckbold() == ON){
		m_fBold = TRUE;
	}
	else{
		m_fBold = FALSE;
	}
	if(mojienvcheckitric() == ON){
		m_fItric = TRUE;
	}
	else{
		m_fItric = FALSE;
	}

	return TRUE;  // コントロールにフォーカスを設定しないとき、戻り値は TRUE となります
	              // 例外: OCX プロパティ ページの戻り値は FALSE となります
}

void CDialogStrgInput::OnDestroy() 
{

	if(hEditBuf != NULL){
		::GlobalFree(hEditBuf);
		hEditBuf = NULL;
	}
	delete(m_pcEdit);
	CMyDialog::OnDestroy();
	// TODO: この位置にメッセージ ハンドラ用のコードを追加してください
}

void CDialogStrgInput::KeyIn(UINT nChar, UINT nID) 
{
	if(nChar == VK_RETURN){
		SetStrgTextToFile();
		//m_pcView->CommandExec(DLGRESETEVENT, 0, 0);
	}
}

void CDialogStrgInput::OnCreate() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	SetStrgTextToFile();
	CEdit *pcEd = (CEdit*)GetDlgItem(IDC_EDINPUT);
	pcEd->SetSel(0, -1, FALSE);
	pcEd->Clear();
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

LPSTR CDialogStrgInput::CrDelFgets(char * buf, int max, FILE * fp)
{
	char	*ret;
	int		len;

	ret = fgets(buf, max, fp);
	if(ret != NULL){
		len = strlen(buf);
		if(buf[len-1] == 0x0a){
			if(buf[len-2] == 0x0d){
				if(buf[len-3] == 0x0d){
					buf[len-3] = '\0';
				}
				else{
					buf[len-2] = '\0';
				}
			}
			else{
				buf[len-1] = '\0';
			}
		}
		else if(buf[len-1] == 0x0d){
			buf[len-1] = '\0';
		}
	}
	return(ret);
}

void CDialogStrgInput::SetFileToStrgText()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	strg[RECMAX];

	CEdit *pcEd = (CEdit*)GetDlgItem(IDC_EDINPUT);
	pcEd->SetSel(0, -1, FALSE);
	pcEd->Clear();

	senvgetexepath(exepath);
	sprintf(fname, "%s\\strgtext.lst", exepath);
	if((fp = fopen(fname, "r")) != NULL){
		lpEditBuf = (LPSTR)::GlobalLock(hEditBuf);
		if(lpEditBuf != 0){
			CrDelFgets(strg, RECMAX, fp);
			pcEd->SetWindowText(strg); 
			::GlobalUnlock(hEditBuf);
		}
		fclose(fp);
	}
	UpdateData(FALSE);
}

void CDialogStrgInput::SetStrgTextToFile()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		linemax, len;

	UpdateData(TRUE);
	CEdit *pcEd = (CEdit*)GetDlgItem(IDC_EDINPUT);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\strgtext.lst", exepath);
	if((fp = fopen(fname, "w")) != NULL){
		lpEditBuf = (LPSTR)::GlobalLock(hEditBuf);
		if(lpEditBuf != 0){

			linemax = pcEd->GetLineCount();
			if(linemax == 1){
				pcEd->GetWindowText(lpEditBuf, RECMAX);
				len = strlen(lpEditBuf);
				lpEditBuf[len] = '\n';
				lpEditBuf[len+1] = '\0';
				fputs(lpEditBuf, fp); 
			}
			else{
				pcEd->GetWindowText(lpEditBuf, RECMAX);
				fputs(lpEditBuf, fp); 
				/*
				int		idx;
				for(idx = 0; idx < linemax; idx++){
					len = pcEd->GetLine(idx, lpEditBuf);
					lpEditBuf[len] = '\n';
					lpEditBuf[len+1] = '\0';
				}
				*/
			}
			::GlobalUnlock(hEditBuf);
		}
		fclose(fp);
	}
}

void CDialogStrgInput::GetItemElement()
{
	char	buf[RECMAX];

	UpdateData(TRUE);
	mojienvsetfontname(m_csFontName.GetBuffer(RECMAX));
	if(m_fCheckTategaki){
		mojienvsettategakiflag(ON);
	}
	else{
		mojienvsettategakiflag(OFF);
	}
	if(m_fCheckMojiData){
		mojienvsetspacekind(ON);
	}
	else{
		mojienvsetspacekind(OFF);
	}
	mojienvsetstart(m_dStartX, m_dStartY);
	mojienvsetsize(m_dWidth, m_dHight);
	mojienvsetspace(m_dMojiSpace, m_dStrgSpace);

	if(m_fCheckGyaku){
		mojienvsetgyakukind(ON);
	}
	else{
		mojienvsetgyakukind(OFF);
	}
	mojienvsetsmpltext(m_csSmplText.GetBuffer(RECMAX));

	mojienvsetstrlen(m_dStrgLen);
	mojienvsetstrgslop(m_dStrgSlop);

	strcpy(buf, m_csStrgOrder.GetBuffer(RECMAX));
	if(strcmp(buf, "左寄せ") == 0){
		mojienvsetorder(ODERLEFT);
	}
	else if(strcmp(buf, "右寄せ") == 0){
		mojienvsetorder(ODERRIGHT);
	}
	else if(strcmp(buf, "中央") == 0){
		mojienvsetorder(ODERCNTR);
	}
	else{
		mojienvsetorder(ODERLEFT);
	}
	mojienvsettrnslimit(m_dTrnsLimit);

	if(m_fBold == TRUE){
		mojienvsetbold(ON);
	}
	else{
		mojienvsetbold(OFF);
	}
	if(m_fItric == TRUE){
		mojienvsetitric(ON);
	}
	else{
		mojienvsetitric(OFF);
	}
}

void CDialogStrgInput::SetItemElement()
{
	char	buf[RECMAX];
	int		flag;
	DBL		num;
	DBL		numx, numy;

	mojienvgetfontname(buf);
	m_csFontName = _T(buf);
	mojienvgettategakiflag(&flag);
	if(flag == ON){
		m_fCheckTategaki = TRUE;
	}
	else{
		m_fCheckTategaki = FALSE;
	}
	mojienvgetspacekind(&flag);
	if(flag == ON){
		m_fCheckMojiData = TRUE;
	}
	else{
		m_fCheckMojiData = FALSE;
	}
	mojienvgetstart(&numx, &numy);
	m_dStartX = FLOOR100(numx);
	m_dStartY = FLOOR100(numy);
	mojienvgetsize(&numx, &numy);
	m_dWidth = FLOOR100(numx);
	m_dHight = FLOOR100(numy);
	mojienvgetspace(&numx, &numy);
	m_dMojiSpace = FLOOR100(numx);
	m_dStrgSpace = FLOOR100(numy);
	//mojienvgetstart(&m_dStartX, &m_dStartY);
	//mojienvgetsize(&m_dWidth, &m_dHight);
	//mojienvgetspace(&m_dMojiSpace, &m_dStrgSpace);

	mojienvgetgyakukind(&flag);
	if(flag == ON){
		m_fCheckGyaku = TRUE;
	}
	else{
		m_fCheckGyaku = FALSE;
	}
	mojienvgetsmpltext(buf);
	m_csSmplText = _T(buf);
	
	mojienvgetstrlen(&num);
	m_dStrgLen = FLOOR100(num);
	mojienvgetstrgslop(&num);
	m_dStrgSlop = FLOOR100(num);

	mojienvgetorder(&flag);
	switch(flag){
	case ODERLEFT:
		m_csStrgOrder = _T("左寄せ");
		break;
	case ODERRIGHT:
		m_csStrgOrder = _T("右寄せ");
		break;
	case ODERCNTR:
		m_csStrgOrder = _T("中央");
		break;
	default:
		m_csStrgOrder = _T("左寄せ");
		break;
	}

	mojienvgettrnslimit(&num);
	m_dTrnsLimit = FLOOR100(num);

	UpdateData(FALSE);
}

void CDialogStrgInput::InitItemElement()
{
	CSpinButtonCtrl *pcSpin;

	static UDACCEL aSpinAccel[] = {
		{1,1},
		{2,5},
		{4,10},
		{8,50},
		{12,100},
	};

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPSTARTX);
	pcSpin->SetRange(0, 10000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPSTARTY);
	pcSpin->SetRange(0, 10000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPWIDTH);
	pcSpin->SetRange(0, 10000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPHIGHT);
	pcSpin->SetRange(0, 10000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPMOJISPACE);
	pcSpin->SetRange(-10000, 10000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPSTRGSPACE);
	pcSpin->SetRange(-10000, 10000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPSTRGLEN);
	pcSpin->SetRange(0, 30000);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPSTRGSLOP);
	pcSpin->SetRange(-80, 80);
	pcSpin->SetAccel(5, aSpinAccel);

	pcSpin = (CSpinButtonCtrl*)GetDlgItem(IDC_SPTRNSLIMIT);
	pcSpin->SetRange(0, 100);
	pcSpin->SetAccel(5, aSpinAccel);
}

void CDialogStrgInput::OnCkgyaku() 
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	
}

void CDialogStrgInput::InitFontList()
{
	int		idx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CBFONTNAME);
	idx = 0;
	mojienvgetfontname(fontname);
	m_csFontName = _T(fontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		pCB->AddString(fontname);
		idx++;
	}
}

void CDialogStrgInput::SampleTextOut()
{
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CTonDoc*	pcDoc;
	CStrgVect* pcStrgVect;
	char	fontname[FNAMEMAX];
	int		type;
	char	sname[FNAMEMAX];
	char	smpltext[RECMAX];
	DBL		dstartx, dstarty;
	DBL		dwidth, dhight;
	DBL		dstrlen;
	DBL		dmojispace, dstrgspace;
	CRect	rect, toprect;
	DBL		xsize, ysize;
	DBL		sx, sy, ex, ey;
	DBL		dtime, btime;
	DBL		bxmm, bymm;
	DBL		dxmm, dymm;
	DBL		bsx, bsy;
	DBL		bmvx, bmvy;
	int		rval, gval, bval;

	UpdateData(TRUE);
	CStatic *pcST = (CStatic*)GetDlgItem(IDC_STSMPLE);
	CStatic *pcSTTop = (CStatic*)GetDlgItem(IDC_STTOP);
	pcST->GetWindowRect(&rect);
	pcSTTop->GetWindowRect(&toprect);
	xsize = rect.right - rect.left;
	ysize = rect.bottom - rect.top;
	sx = rect.left - toprect.left;
	sy = rect.top - toprect.top;
	ex = sx + xsize;
	ey = sy + ysize;
	dxmm = rect.right - toprect.left;
	dymm = rect.bottom - toprect.top;
	pcDoc = (CTonDoc*)(m_pcView->GetDocument());
	pcStrgVect = (CStrgVect*)(pcDoc->NewData(STRGVECTID));
	if(pcStrgVect == NULL){
		return;
	}

	dmojispace = 0;
	dstrgspace = 0;
	// dymm - yy で右下からの座標位置
	dstartx = sx+8;
	dstarty = dymm-(sy+ysize/4);
	dwidth = ysize/2;
	dhight = ysize/2;
	dstrlen = 0;
	pcStrgVect->SetSpaceEnv(dmojispace, dstrgspace);
	pcStrgVect->SetStrStartXY(dstartx, dstarty);
	pcStrgVect->SetFontSize(dwidth, dhight);
	pcStrgVect->SetStrLen(dstrlen);
	pcStrgVect->SetTateYoko(YOKOGAKI);
	strcpy(fontname, m_csFontName.GetBuffer(RECMAX));
	strcpy(smpltext, m_csSmplText.GetBuffer(RECMAX));
	mojienvgetfonttypename(fontname, &type, sname);
	if(type == TT3FONT){
		strcpy(smpltext, "123ABCabc");
	}
	pcStrgVect->SetFontName(fontname);
	pcStrgVect->SetString(smpltext);

	CDC* pcDC = GetDC();
	rect.left = (long)sx;
	rect.top = (long)sy;
	rect.right = (long)ex;
	rect.bottom = (long)ey;

	senvgetbkclr(&rval, &gval, &bval);
	CBrush *pcBrush = new CBrush(RGB(rval, gval, bval));
	pcDC->FillRect(&rect, pcBrush);
	delete(pcBrush);

	dtime = 1.0;
	btime = scrnsetxytime(dtime, dtime);
	scrngetmmsize(&bxmm, &bymm);
	scrngetrltvtop(&bsx, &bsy); 
	scrnsetmmsize(dxmm, dymm);
	scrngetdisprltvmove(&bmvx, &bmvy);
	scrnsetrltvelement();
	scrnsetrltvtop(0, 0); 
	scrnrltvareaset(0.0, 0.0, INTMAX, INTMAX);
	scrnsetdisprltvmove(0.0, 0.0);

	pcStrgVect->OnDraw(pcDC);
	ReleaseDC(pcDC);

	scrnsetxytime(btime, btime);
	scrnsetmmsize(bxmm, bymm);
	scrnsetrltvelement();
	scrnsetrltvtop(bsx, bsy); 
	scrnsetdisprltvmove(bmvx, bmvy);

	delete(pcStrgVect);
}

void CDialogStrgInput::OnSelchangeCbfontname() 
{
	int		idx;
	char	fontname[RECMAX];
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	CComboBox* pCB = (CComboBox*)GetDlgItem(IDC_CBFONTNAME);
	idx = pCB->GetCurSel();
	pCB->GetLBText(idx, fontname);
	UpdateData(TRUE);
	m_csFontName = _T(fontname);
	UpdateData(FALSE);
	SampleTextOut();
}

void CDialogStrgInput::OnUpdateEdsmpltext() 
{
	SampleTextOut();
}

void CDialogStrgInput::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	
	if(m_nFirstFlag == ON){
		SampleTextOut();
		m_pcEdit->SetFocus();
		m_nFirstFlag = OFF;
	}
}

void CDialogStrgInput::OnEditchangeCbfontname() 
{
	UpdateData(TRUE);
	SampleTextOut();
}

void CDialogStrgInput::OnCkmojidata() 
{
	UpdateData(TRUE);
	if(m_fCheckMojiData){
		mojienvsetspacekind(ON);
	}
	else{
		mojienvsetspacekind(OFF);
	}
}

void CDialogStrgInput::OnCancel() 
{
}

void CDialogStrgInput::OnInpend() 
{
	SetStrgTextToFile();
	m_pcView->CommandExec(ENDEVENT, 0, 0);
}

