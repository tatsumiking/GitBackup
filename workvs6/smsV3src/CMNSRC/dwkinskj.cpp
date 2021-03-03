// dwkinskj.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"

#include "io.h"

#include "DWkInSkj.h"
#include "DlgWkLd.h"
#include "Dgjslct.h"
#include "DlCrctLd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInSekiji ダイアログ

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

CDialogWakuInSekiji::CDialogWakuInSekiji(CWnd* pParent /*=NULL*/)
	: CMyDialog(CDialogWakuInSekiji::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogWakuInSekiji)
	//}}AFX_DATA_INIT
	
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_pcCmdBase = NULL;
}


void CDialogWakuInSekiji::DoDataExchange(CDataExchange* pDX)
{
	CMyDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuInSekiji)
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
	DDX_Control(pDX, IDC_CHKKTGK1, m_chkKtgk[0]);
	DDX_Control(pDX, IDC_CHKKTGK2, m_chkKtgk[1]);
	DDX_Control(pDX, IDC_CHKKTGK3, m_chkKtgk[2]);
	DDX_Control(pDX, IDC_CHKKTGK4, m_chkKtgk[3]);
	DDX_Control(pDX, IDC_CHKKTGK5, m_chkKtgk[4]);
	DDX_Control(pDX, IDC_CHKKTGK6, m_chkKtgk[5]);
	DDX_Control(pDX, IDC_CHKKTGK7, m_chkKtgk[6]);
	DDX_Control(pDX, IDC_CHKKTGK8, m_chkKtgk[7]);
	DDX_Control(pDX, IDC_CHKKTGK9, m_chkKtgk[8]);
	DDX_Control(pDX, IDC_CHKKTGK10, m_chkKtgk[9]);
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
	DDX_Control(pDX, IDC_EDIT11, m_edText[10]);
	DDX_Control(pDX, IDC_EDIT12, m_edText[11]);
	DDX_Control(pDX, IDC_EDIT13, m_edText[12]);
	DDX_Control(pDX, IDC_EDIT14, m_edText[13]);
	DDX_Control(pDX, IDC_EDIT15, m_edText[14]);
	DDX_Control(pDX, IDC_EDIT16, m_edText[15]);
	DDX_Control(pDX, IDC_EDIT17, m_edText[16]);
	DDX_Control(pDX, IDC_EDIT18, m_edText[17]);
	DDX_Control(pDX, IDC_EDIT19, m_edText[18]);
	DDX_Control(pDX, IDC_EDIT20, m_edText[19]);
	DDX_Control(pDX, IDC_EDIT21, m_edText[20]);
	DDX_Control(pDX, IDC_EDIT22, m_edText[21]);
	DDX_Control(pDX, IDC_EDIT23, m_edText[22]);
	DDX_Control(pDX, IDC_EDIT24, m_edText[23]);
	DDX_Control(pDX, IDC_EDIT25, m_edText[24]);
	DDX_Control(pDX, IDC_EDIT26, m_edText[25]);
	DDX_Control(pDX, IDC_EDIT27, m_edText[26]);
	DDX_Control(pDX, IDC_EDIT28, m_edText[27]);
	DDX_Control(pDX, IDC_EDIT29, m_edText[28]);
	DDX_Control(pDX, IDC_EDIT30, m_edText[29]);
}


BEGIN_MESSAGE_MAP(CDialogWakuInSekiji, CMyDialog)
	//{{AFX_MSG_MAP(CDialogWakuInSekiji)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_OUT, OnOut)
	ON_BN_CLICKED(IDC_DISP, OnDisp)
	ON_BN_CLICKED(IDC_GAIJI, OnGaiji)
	ON_BN_CLICKED(IDC_EXIT, OnExit)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuInSekiji メッセージ ハンドラ
int CDialogWakuInSekiji::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMyDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

BOOL CDialogWakuInSekiji::OnInitDialog() 
{
	CRect	rect;
	int		idx;

	CMyDialog::OnInitDialog();

	m_nCrtIdxTop = 0;
	m_nNextFocus = -1;
	for(idx = 0; idx < INSEKIJIMAX*3; idx++){
		m_edText[idx].m_nID = IDC_EDIT1+idx;
		m_edText[idx].m_pcMyDialog = this;
		m_edText[idx].SetIMEMode(ON);
	}
	GetGrupMax();
	LoadCrtGrupText();
	m_fFirstFlag = TRUE;
	return TRUE;
}

void CDialogWakuInSekiji::OnDestroy() 
{
	CDialog::OnDestroy();
}

void CDialogWakuInSekiji::OnDisp() 
{
	// TODO: この位置に初期化の補足処理を追加してください
	SaveCrtGrupText();
	m_pcView->CommandExec(DLGDRAWEVENT, 0, 0);
	KeyIn(VK_DOWN, IDC_EDIT1+m_nNextFocus);
}

void CDialogWakuInSekiji::LoadCrtGrupText()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAXAD];
	LPSTR	lpret;
	LPSTR	csvsp[RECMAX];
	int		cnt;
	char	strg[RECMAXAD];
	int		idx;

	senvgetexepath(exepath);
	sprintf(fname, "%s\\sekiji.lst", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);
	for(idx = 0; idx < m_nCrtIdxTop; idx++){
		lpret = fgets(buf, RECMAX, fp);
		if(lpret == NULL){
			break;
		}
	}
	if(lpret == NULL){
		fclose(fp);
		return;
	}
	for(idx = 0; idx < INSEKIJIMAX; idx++){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(buf[0] == '\0'){
			break;
		}
		cnt = basedivcsvdata(csvsp, buf);
		SetDlgItemText(IDC_EDIT1+idx*3, csvsp[0]);
		SetDlgItemText(IDC_EDIT1+idx*3+1, csvsp[1]);
		SetDlgItemText(IDC_EDIT1+idx*3+2, csvsp[2]);
	}
	strg[0] = '\0';
	for(;idx < INSEKIJIMAX; idx++){
		SetDlgItemText(IDC_EDIT1+idx*3, strg);
		SetDlgItemText(IDC_EDIT1+idx*3+1, strg);
		SetDlgItemText(IDC_EDIT1+idx*3+2, strg);
	}
	fclose(fp);
}

void CDialogWakuInSekiji::SaveCrtGrupText()
{
	char	exepath[FNAMEMAX];
	char	srcfname[FNAMEMAX];
	char	bakfname[FNAMEMAX];
	FILE	*infp, *outfp;
	char	buf[RECMAXAD];
	LPSTR	lpret;
	int		idx;
	char	szStr1[RECMAXAD];
	char	szStr2[RECMAXAD];
	char	szStr3[RECMAXAD];

	UpdateData(TRUE);
	senvgetexepath(exepath);
	sprintf(srcfname, "%s\\sekiji.lst", exepath);
	sprintf(bakfname, "%s\\sekiji.bak", exepath);
	CopyFile(srcfname, bakfname, FALSE);
	infp = fopen(bakfname, "r");
	if(infp == NULL){
		return;
	}
	outfp = fopen(srcfname, "w");
	lpret = fgets(buf, RECMAX, infp);
	fputs(buf, outfp);
	for(idx = 0; idx < m_nCrtIdxTop; idx++){
		lpret = fgets(buf, RECMAX, infp);
		if(lpret == NULL){
			break;
		}
		fputs(buf, outfp);
	}
	if(lpret == NULL){
		fclose(infp);
		fclose(outfp);
		return;
	}
	for(idx = 0; idx < INSEKIJIMAX; idx++){
		lpret = fgets(buf, RECMAX, infp);
		if(lpret == NULL){
			fclose(infp);
			fclose(outfp);
			return;
		}
		if(buf[0] == '\0'){
			fclose(infp);
			fclose(outfp);
			return;
		}
		GetDlgItemText(IDC_EDIT1+idx*3, szStr1, RECMAX);
		GetDlgItemText(IDC_EDIT1+idx*3+1, szStr2, RECMAX);
		GetDlgItemText(IDC_EDIT1+idx*3+2, szStr3, RECMAX);
		fprintf(outfp, "%s,%s,%s,\n", szStr1, szStr2, szStr3);
	}
	while(1){
		lpret = fgets(buf, RECMAX, infp);
		if(lpret == NULL){
			break;
		}
		fputs(buf, outfp);
	}
	fclose(infp);
	fclose(outfp);
}

void CDialogWakuInSekiji::KeyIn(UINT nChar, UINT nID)
{
	int		next;
	int		step;


	step = (INSEKIJIMAX-1);
	switch(nChar){
	//case VK_RETURN:
	case VK_DOWN:
		next = m_nNextFocus + 1;
		if(INSEKIJIMAX*3 <= next){
			m_nCrtIdxTop = m_nCrtIdxTop + step;
			if(m_nCrtIdxTop + step > m_nGrupMax){
				m_nCrtIdxTop = m_nGrupMax - step;
				next = 0;
			}
			else{
				next = step+1;
			}
			LoadCrtGrupText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
 	case VK_UP:
		next = m_nNextFocus - 1;
		if(next < 0){
			m_nCrtIdxTop = m_nCrtIdxTop - step;
			if(m_nCrtIdxTop < 0){
				m_nCrtIdxTop = 0;
			}
			next = 0;
			LoadCrtGrupText();
		}
		WakuSetFocus(next);
		Invalidate(TRUE);
		break;
	}
}
void CDialogWakuInSekiji::SelectEdit(UINT nID)
{
	int		nNo;
	CDC		*pcDC;

	nNo = nID - IDC_EDIT1;
	if(m_nNextFocus == nNo){
		return;
	}
	m_nNextFocus = nNo;
	/* // 処理をグループに変更する必要あり
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
	*/
}
void CDialogWakuInSekiji::OnOut() 
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

void CDialogWakuInSekiji::OnExit() 
{
	m_pcView->m_nCrtWaku = -1;
	m_pcView->CommandExec(ENDEVENT, 0L, 0L);
}

void CDialogWakuInSekiji::GetGrupMax()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		idx;
	char	buf[RECMAX+16];

	senvgetexepath(exepath);
	sprintf(fname, "%s\\sekiji.lst", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);
	idx = 0;
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		if(buf[0] == '\0'){
			break;
		}
		idx++;
	}
	m_nGrupMax = idx;
	fclose(fp);
}

void CDialogWakuInSekiji::OnPaint() 
{
	int	no;
	CRect	rect;
	char	numstr[QRECMAX];

	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_fFirstFlag == TRUE){
		m_fFirstFlag = FALSE;
		m_pcView->m_nCrtWaku = 0;
		WakuSetFocus(0);
	}
	for(no = 0; no < INSEKIJIMAX; no++){
		if(m_nGrupMax <= (m_nCrtIdxTop + no)){
			break;
		}
		sprintf(numstr, "%03d", m_nCrtIdxTop+no+1);
		m_stWkno[no].SetWindowText(numstr);
	}
	strcpy(numstr, "   ");
	for(; no < INSEKIJIMAX; no++){
		m_stWkno[no].SetWindowText(numstr);
	}
}

void CDialogWakuInSekiji::WakuSetFocus(int next)
{
	CDC	*pcDC;

	m_edText[next].SetFocus();
	/* // 処理をグループに変更する必要あり
	m_pcView->m_nCrtWaku = m_nNextFocus+m_nCrtIdxTop;
	pcDC = m_pcView->GetDC();
	m_pcView->DrawSelectWaku(pcDC);
	m_pcView->ReleaseDC(pcDC);
	*/
}
void CDialogWakuInSekiji::OnGaiji() 
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
void CDialogWakuInSekiji::SaveCrtWaku(int crctno, LPSTR paperpath, LPSTR wakuname)
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

void CDialogWakuInSekiji::SaveRecString(int crctcnt, FILE *crctfp)
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
	if((textfp = fopen(textfname, "r")) == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(m_nGrupMax <= idx){
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

void CDialogWakuInSekiji::SetString(LPSTR lpStr)
{
	CEdit	*pcEdit;

	pcEdit = (CEdit*)GetDlgItem(IDC_EDIT1+m_nNextFocus);
	pcEdit->ReplaceSel(lpStr, FALSE);
}
