// DlgWkInp.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "io.h"

#include "DlgWkInp.h"
#include "DlgWkLd.h"
#include "Dgjslct.h"
#include "DlCrctLd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void poutploternameget(LPSTR plotername);
	void wakuenvgetgrupname(LPSTR grupname);
	void wakuenvgetpapername(LPSTR papername);
	void wakuenvgetfullpaperpath(LPSTR paperpath);
	void wakuenvgetselectwakuidx(int *idx);
	void wakuenvsetselectwakuidx(int idx);
	void wakuenvloadoutputenv(LPSTR wakuloadfname);
	void wakuenvlistgetwakuname(int slctidx, LPSTR wakufname);
	void wakuenvsaveoutputenv(LPSTR wakusavefname);
	int basedivcsvdata(LPSTR csvsp[], LPSTR csvbuf);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	LPSTR basedivstrg(LPSTR sp, LPSTR retsp, char spltr);
	int basegetparentpath(LPSTR lpPath);
	int DispatchWindows(HWND hWin);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvgetfonttypename(LPSTR fontname, LPINT retftype, LPSTR fontfullname);
}

CDialogWakuInput::CDialogWakuInput(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWakuInput)
	m_csCrctCnt = _T("");
	//}}AFX_DATA_INIT

	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_pcCmdBase = NULL;
}

void CDialogWakuInput::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuInput)
	DDX_Text(pDX, IDC_STCRCTCNT, m_csCrctCnt);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_STATIC1, m_stWkno[0]);
	DDX_Control(pDX, IDC_STATIC2, m_stWkno[1]);
	DDX_Control(pDX, IDC_STATIC3, m_stWkno[2]);
	DDX_Control(pDX, IDC_STATIC4, m_stWkno[3]);
	DDX_Control(pDX, IDC_STATIC5, m_stWkno[4]);
	DDX_Control(pDX, IDC_STATIC6, m_stWkno[5]);
	DDX_Control(pDX, IDC_STATIC7, m_stWkno[6]);
	DDX_Control(pDX, IDC_STATIC8, m_stWkno[7]);
	DDX_Control(pDX, IDC_STATIC9, m_stWkno[8]);
	DDX_Control(pDX, IDC_STATIC10, m_stWkno[9]);
	DDX_Control(pDX, IDC_EDIT1, m_edText[0]);
	DDX_Control(pDX, IDC_EDIT2, m_edText[1]);
	DDX_Control(pDX, IDC_EDIT3, m_edText[2]);
	DDX_Control(pDX, IDC_EDIT4, m_edText[3]);
	DDX_Control(pDX, IDC_EDIT5, m_edText[4]);
	DDX_Control(pDX, IDC_EDIT6, m_edText[5]);
	DDX_Control(pDX, IDC_EDIT7, m_edText[6]);
	DDX_Control(pDX, IDC_EDIT8, m_edText[7]);
	DDX_Control(pDX, IDC_EDIT9, m_edText[8]);
	DDX_Control(pDX, IDC_EDIT10, m_edText[9]);
}


BEGIN_MESSAGE_MAP(CDialogWakuInput, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuInput)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_DISP, OnDisp)
	ON_BN_CLICKED(IDC_CRCT, OnCrct)
	ON_BN_CLICKED(IDC_LOAD, OnLoad)
	ON_BN_CLICKED(IDC_OUT, OnOut)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_PLOTOUT, OnPlotout)
	ON_BN_CLICKED(IDC_GAIJI, OnGaiji)
	ON_BN_CLICKED(IDC_CRCTLOAD, OnCrctload)
	ON_BN_CLICKED(IDC_CRCTSAVE, OnCrctsave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInput メッセージ ハンドラ

int CDialogWakuInput::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CDialogWakuInput::OnInitDialog() 
{
	CRect	rect;
	int		idx;

	CMyDialog::OnInitDialog();
	
	m_nCrtIdxTop = 0;
	m_nNextFocus = -1;
	for(idx = 0; idx < INPUTEDITMAX; idx++){
		m_edText[idx].m_nID = IDC_EDIT1+idx;
		m_edText[idx].m_pcMyDialog = this;
		m_edText[idx].SetIMEMode(ON);
	}
	GetWakuMax();
	LoadCrtWakuText();
	m_fFirstFlag = TRUE;
	//WakuSetFocus(0);
	return TRUE;
}

void CDialogWakuInput::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDialogWakuInput::OnDisp() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	KeyIn(VK_DOWN, IDC_EDIT1+m_nNextFocus);
	//WakuSetFocus();
}

void CDialogWakuInput::LoadCrtWakuText()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX+16];
	char	strg[RECMAX+16];
	int		idx;
	long	seekp;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "rb")) == NULL){
		return;
	}
	seekp = m_nCrtIdxTop * RECMAX;
	fseek(fp, seekp, 0);
	for(idx = 0; idx < INPUTEDITMAX; idx++){
		if(fread(buf, RECMAX, 1, fp) != 1){
			break;
		}
		strcpy(strg, buf);
		SetDlgItemText(IDC_EDIT1+idx, strg);
	}
	strg[0] = '\0';
	for(;idx < INPUTEDITMAX; idx++){
		SetDlgItemText(IDC_EDIT1+idx, strg);
	}
	fclose(fp);
}

void CDialogWakuInput::SaveCrtWakuText()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	strg[RECMAX+16];
	int		idx;
	long	seekp;

	idx = 0;
	UpdateData(TRUE);
	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "r+b")) != NULL){
		seekp = m_nCrtIdxTop * RECMAX;
		fseek(fp, seekp, 0);
		idx = 0;
		for(idx = 0; idx < INPUTEDITMAX; idx++){
			memset(strg, ' ', RECMAX);
			strg[RECMAX-1] = 0x0a;
			strg[RECMAX-2] = 0x0d;
			GetDlgItemText(IDC_EDIT1+idx, strg, RECMAX);
			fwrite(strg, RECMAX, 1, fp);
		}
		fclose(fp);
	}
}

void CDialogWakuInput::KeyIn(UINT nChar, UINT nID)
{
	int		next;

	switch(nChar){
	//case VK_RETURN:
	case VK_DOWN:
		next = m_nNextFocus + 1;
		if(INPUTEDITMAX <= next){
			m_nCrtIdxTop = m_nCrtIdxTop + 5;
			if(m_nCrtIdxTop + 5 > m_nWakuMax){
				m_nCrtIdxTop = m_nWakuMax - 5;
				next = 0;
			}
			else{
				next = 5;
			}
			LoadCrtWakuText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
 	case VK_UP:
		next = m_nNextFocus - 1;
		if(next < 0){
			m_nCrtIdxTop = m_nCrtIdxTop - 5;
			if(m_nCrtIdxTop < 0){
				m_nCrtIdxTop = 0;
			}
			next = 0;
			LoadCrtWakuText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
	}
}

void CDialogWakuInput::SelectEdit(UINT nID)
{
	int		nNo;
	CDC		*pcDC;

	nNo = nID - IDC_EDIT1;
	if(m_nNextFocus == nNo){
		return;
	}
	m_nNextFocus = nNo;
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
}

void CDialogWakuInput::OnCrct() 
{
	char	exepath[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	FILE	*crctfp;
	int		crctcnt;
	char	buf[RECMAX8+16];

	SaveCrtWakuText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);

	senvgetexepath(exepath);
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	if((crctfp = fopen(crctfname, "r+")) == NULL){
		if((crctfp = fopen(crctfname, "w+")) == NULL){
			return;
		}
	}
	crctcnt = 0;
	while(1){
		if(fgets(buf, RECMAX8, crctfp) == NULL){
			break;
		}
		crctcnt++;
	}
	//fseek(crctfp, 0L, 2);

	SaveRecString(crctcnt, crctfp);

	fclose(crctfp);

	crctcnt++;
	sprintf(buf, "貯込%d枚", crctcnt);
	m_csCrctCnt = _T(buf);
	UpdateData(FALSE);
}

void CDialogWakuInput::OnLoad() 
{
	char	paperpath[FNAMEMAX];
	char	wakuname[FNAMEMAX];
	char	wakufilename[FNAMEMAX];
	int		slctidx;
	int		ret;
	CDialogWakuLoad*	pDialogWakuLoad;

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument(); 
	//pcDoc->WakuLoad(wakupath);
	// TODO: この位置にコントロール通知ハンドラ用のコードを追加してください
	ShowWindow(SW_HIDE);
	pDialogWakuLoad = new CDialogWakuLoad(m_pcView);
	ret = pDialogWakuLoad->DoModal();
	if(ret == IDOK){
		wakuenvgetfullpaperpath(paperpath);
		wakuenvgetselectwakuidx(&slctidx);
		wakuenvlistgetwakuname(slctidx, wakuname);
		pcDoc->SetTitle(wakuname);
		sprintf(wakufilename, "%s\\%s", paperpath, wakuname);

		WakuLoad(wakufilename);
		pcDoc->OnFlag(ACTIVEFLAG);
		pcDoc->SaveStringStoreFile(SWAKVECTID);
		GetWakuMax();
		LoadCrtWakuText();
		wakuenvsetselectwakuidx(-1);
	}
	delete(pDialogWakuLoad);
	//m_pcView->CommandExec(DLGSETEVENT, 0, 0);
	m_pcView->Invalidate();
	DispatchWindows(m_pcView->m_hWnd);	
	WakuSetFocus(0);
	ShowWindow(SW_SHOW);
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogWakuInput::OnOut() 
{
	CTonApp *pcApp = (CTonApp*)AfxGetApp();
	CMainFrame *pcWnd = (CMainFrame*)pcApp->GetMainWnd();
	m_pcView->CommandExec(DLGSETEVENT, 0L, 0L);
	pcWnd->ShowWindow(SW_HIDE);
	ShowWindow(SW_HIDE);
	pcWnd->EnableWindow(FALSE);
	EnableWindow(FALSE);
	m_pcDoc->SaveUndo();
	m_pcView->ExecSmsout();
	m_pcDoc->LoadUndo();
	pcWnd->EnableWindow(TRUE);
	EnableWindow(TRUE);
	ShowWindow(SW_SHOW);
	pcWnd->ShowWindow(SW_SHOW);
}

void CDialogWakuInput::OnExit() 
{
	m_pcView->m_nCrtWaku = -1;
	m_pcView->CommandExec(ENDEVENT, 0L, 0L);
}

void CDialogWakuInput::GetWakuMax()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX+16];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "rb")) == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(fread(buf, RECMAX, 1, fp) != 1){
			break;
		}
		idx++;
	}
	m_nWakuMax = idx;
	fclose(fp);
}

void CDialogWakuInput::OnPaint() 
{
	int	idx;
	CRect	rect;
	char	numstr[QRECMAX];

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_fFirstFlag == TRUE){
		m_fFirstFlag = FALSE;
		m_pcView->m_nCrtWaku = 0;
		WakuSetFocus(0);
	}
	for(idx = 0; idx < INPUTEDITMAX; idx++){
		if(m_nWakuMax <= m_nCrtIdxTop + idx){
			break;
		}
		sprintf(numstr, "%03d", m_nCrtIdxTop + idx + 1);
		m_stWkno[idx].SetWindowText(numstr);
	}
	strcpy(numstr, "   ");
	for(; idx < INPUTEDITMAX; idx++){
		m_stWkno[idx].SetWindowText(numstr);
	}
}

void CDialogWakuInput::WakuSetFocus(int next)
{
	CDC	*pcDC;

	m_edText[next].SetFocus();
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
}

void CDialogWakuInput::OnPlotout() 
{
	char	plotername[QRECMAX];

	poutploternameget(plotername);
	if(strcmp(plotername, "プリンタ") == 0){
		m_pcView->PrinterOutput();
	}
	else{
		m_pcView->PloterOutput();
	}
}

void CDialogWakuInput::OnGaiji() 
{
	CDialogGaijiSlct *pcDialogGaijiSlct;
	char	szFontName[RECMAX];
	char	szFolderName[RECMAX];
	int		idx, retftype;
	char	strg[RECMAX];
	char	setstrg[RECMAX];
	int		slen, elen;
	
	idx = m_nNextFocus;
	mojienvgetfontname(szFontName);
	mojienvgetfonttypename(szFontName, &retftype, szFolderName);
	pcDialogGaijiSlct = new CDialogGaijiSlct(this);
	strcpy(pcDialogGaijiSlct->m_szFolderName, szFolderName);
	if(pcDialogGaijiSlct->DoModal() == IDOK){
		GetDlgItemText(IDC_EDIT1+idx, strg, RECMAX);
		m_edText[idx].GetSel(slen, elen);
		setstrg[0] = '\0';
		if(slen != 0){
			strncpy(setstrg, strg, slen);
			setstrg[slen] = '\0';
		}
		strcat(setstrg, pcDialogGaijiSlct->m_szGaijiText);
		strcat(setstrg, &(strg[elen]));
		SetDlgItemText(IDC_EDIT1+idx, setstrg);
	}
	delete(pcDialogGaijiSlct);
	UpdateData(FALSE);
}

void CDialogWakuInput::OnCrctload() 
{
	CDialogCrctLoad*		pcDialogCrctLoad;
	char	exepath[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	FILE	*crctfp;
	char	csvbuf[RECMAX8+16];
	LPSTR	csvsp[10];
	LPSTR	sp;
	int		idx;
	char	wakufilename[FNAMEMAX];
	int		slctidx;
	char	fname[FNAMEMAX];
	char	szStr[RECMAX];
	FILE	*fp;

	slctidx = -1;
	pcDialogCrctLoad = new CDialogCrctLoad(this);
	if(pcDialogCrctLoad->DoModal() == IDOK){
		slctidx = pcDialogCrctLoad->m_nSlctIdx;
	}
	delete(pcDialogCrctLoad);
	if(slctidx == -1){
		return;
	}
	senvgetexepath(exepath);
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	if((crctfp = fopen(crctfname, "r+")) == NULL){
		return;
	}
	csvbuf[0] = '\0';
	idx = 0;
	while(1){
		if(fgets(csvbuf, RECMAX8, crctfp) == NULL){
			break;
		}
		if(idx == slctidx){
			break;
		}
		idx++;
	}
	fclose(crctfp);
	if(csvbuf[0] == '\0'){
		return;
	}

	m_pcView->m_nCrctIdx = slctidx;

	basedivcsvdata(csvsp, csvbuf);
	strcpy(wakufilename, csvsp[3]);

	WakuLoad(wakufilename);

	senvgetexepath(exepath);
	sprintf(fname, "%s\\string.lst", exepath);
	if((fp = fopen(fname, "wb")) == NULL){
		return;
	}
	sp = csvsp[5];
	sp++;
	while(1){
		// 2001.10.18 立身 途中に文字が入っていない枠があると以降処理されない
		if(*sp == '\0'){
			break;
		}
		memset(szStr, ' ', RECMAX);
		szStr[RECMAX-1] = 0x0a;
		szStr[RECMAX-2] = 0x0d;
		sp = basedivstrg(sp, szStr, '/');
		fwrite(szStr, RECMAX, 1, fp);
	}
	fclose(fp);

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->LoadStringStoreFile(SWAKVECTID);

	m_pcView->m_nCrtWaku = 0;
	pcDoc->SaveStringStoreFile(SWAKVECTID);
	GetWakuMax();
	LoadCrtWakuText();
	m_pcView->CommandExec(DLGSETEVENT, 0, 0);
}

void CDialogWakuInput::OnCrctsave() 
{
	char	exepath[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	char	backfname[FNAMEMAX];
	FILE	*crctfp;
	FILE	*backfp;
	char	csvbuf[RECMAX8+16];
	int		idx;

	// wakuenvsetselectwakuidx(-1);
	senvgetexepath(exepath);
	sprintf(crctfname, "%s\\outcrect.dat", exepath);
	sprintf(backfname, "%s\\outcrect.bak", exepath);
	if((crctfp = fopen(crctfname, "r")) == NULL){
		return;
	}
	if((backfp = fopen(backfname, "w")) == NULL){
		return;
	}
	while(1){
		if(fgets(csvbuf, RECMAX, crctfp) == NULL){
			break;
		}
		fputs(csvbuf, backfp);
	}
	fclose(backfp);
	fclose(crctfp);

	if((crctfp = fopen(crctfname, "w")) == NULL){
		return;
	}
	if((backfp = fopen(backfname, "r")) == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(fgets(csvbuf, RECMAX8, backfp) == NULL){
			break;
		}
		if(m_pcView->m_nCrctIdx == idx){
			SaveRecString(idx, crctfp);
		}
		else{
			fputs(csvbuf, crctfp);
		}
		idx++;
	}
	fclose(backfp);
	fclose(crctfp);

}

void CDialogWakuInput::SaveCrtWaku(int crctno, LPSTR paperpath, LPSTR wakuname)
{
	char	filename[FNAMEMAX];

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument();
	sprintf(wakuname, "crct$%03d.wak", crctno);
	sprintf(filename, "%s\\%s", paperpath, wakuname);
	pcDoc->WakuSave(filename);
	basechangesafix(filename, "env");
	wakuenvsaveoutputenv(filename);
	basechangesafix(filename, "bin");
	((CTonView*)m_pcView)->PrintEnvSave(filename);
}

void CDialogWakuInput::WakuLoad(LPSTR wakufilename)
{
	int		len;
	char	basefname[FNAMEMAX];
	char	printenvfname[FNAMEMAX];
	char	outputenvfname[FNAMEMAX];

	CTonDoc* pcDoc = (CTonDoc*)m_pcView->GetDocument();
	pcDoc->DataAllCls();

	strcpy(basefname, wakufilename);
	len = strlen(basefname);
	basefname[len-4] = '\0';

	sprintf(outputenvfname, "%s.env", basefname);
    if((_access(outputenvfname, 0 )) == 0){
		wakuenvloadoutputenv(outputenvfname);
	}
	m_pcView->WorkSizeReset();
	m_pcView->WorkAreaReset();

	pcDoc->WakuLoad(wakufilename);
	pcDoc->OnFlag(ACTIVEFLAG);

	sprintf(printenvfname, "%s.bin", basefname);
    if((_access(printenvfname, 0 )) == 0){
		m_pcView->PrintEnvLoad(printenvfname);
	}
	else{
		basegetparentpath(printenvfname);
		strcat(printenvfname, "\\defprnt.bin");
		m_pcView->PrintEnvLoad(printenvfname);
	}
}

void CDialogWakuInput::SaveRecString(int crctcnt, FILE *crctfp)
{
	char	exepath[FNAMEMAX];
	char	textfname[FNAMEMAX];
	FILE	*textfp;
	char	grupname[RECMAX];
	char	papername[RECMAX];
	char	paperpath[FNAMEMAX];
	char	wakufname[FNAMEMAX];
	char	wakupath[FNAMEMAX];
	char	buf[RECMAX];
	char	strg[RECMAX];
	int		slctidx;
	int		idx;

	senvgetexepath(exepath);
	wakuenvgetgrupname(grupname);
	wakuenvgetpapername(papername);
	wakuenvgetfullpaperpath(paperpath);
	// 必ず出力時枠サイズ変更のため一時ファイルを作成するように修正
	//wakuenvgetselectwakuidx(&slctidx);
	//if(slctidx != -1){
	//	wakuenvlistgetwakuname(slctidx, wakufname);
	//}
	//else{
		SaveCrtWaku(crctcnt, paperpath, wakufname);	
	//}
	sprintf(wakupath, "%s\\%s", paperpath, wakufname);

	sprintf(buf, "\"未\",\"%s\",\"%s\",\"%s\",\"1\",\"",
		grupname, papername, wakupath);
	fputs(buf, crctfp);

	sprintf(textfname, "%s\\string.lst", exepath);
	if((textfp = fopen(textfname, "r+b")) == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(m_nWakuMax <= idx){
			break;
		}
		if(fread(buf, RECMAX, 1, textfp) != 1){
			break;
		}
		strcpy(strg, buf);
		sprintf(buf, "/%s", strg);
		fputs(buf, crctfp);
		idx++;
	}
	sprintf(buf, "\"\n");
	fputs(buf, crctfp);

	fclose(textfp);
}

void CDialogWakuInput::SetString(LPSTR lpStr)
{
	CEdit	*pcEdit;

	pcEdit = (CEdit*)GetDlgItem(IDC_EDIT1+m_nNextFocus);
	pcEdit->ReplaceSel(lpStr, FALSE);
}
