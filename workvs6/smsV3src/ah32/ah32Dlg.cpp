// ah32Dlg.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "dleddata.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAh32Dlg ダイアログ

CAh32Dlg::CAh32Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAh32Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAh32Dlg)
		// メモ: この位置に ClassWizard によってメンバの初期化が追加されます。
	//}}AFX_DATA_INIT
	// メモ: LoadIcon は Win32 の DestroyIcon のサブシーケンスを要求しません。
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nFirstFlag = ON;
}

void CAh32Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAh32Dlg)
	DDX_Control(pDX, IDC_BTN_OUTPUT10, m_btnOutput10);
	DDX_Control(pDX, IDC_BTN_OUTPUT9, m_btnOutput9);
	DDX_Control(pDX, IDC_BTN_OUTPUT8, m_btnOutput8);
	DDX_Control(pDX, IDC_BTN_OUTPUT7, m_btnOutput7);
	DDX_Control(pDX, IDC_BTN_OUTPUT6, m_btnOutput6);
	DDX_Control(pDX, IDC_BTN_OUTPUT5, m_btnOutput5);
	DDX_Control(pDX, IDC_BTN_OUTPUT4, m_btnOutput4);
	DDX_Control(pDX, IDC_BTN_OUTPUT3, m_btnOutput3);
	DDX_Control(pDX, IDC_BTN_OUTPUT2, m_btnOutput2);
	DDX_Control(pDX, IDC_BTN_OUTPUT1, m_btnOutput1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAh32Dlg, CDialog)
	//{{AFX_MSG_MAP(CAh32Dlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXIT, OnBtnExit)
	ON_BN_CLICKED(IDC_BTN_INPUTEDIT, OnBtnInputedit)
	ON_BN_CLICKED(IDC_BTN_OUTPUT1, OnBtnOutput1)
	ON_BN_CLICKED(IDC_BTN_OUTPUT2, OnBtnOutput2)
	ON_BN_CLICKED(IDC_BTN_OUTPUT3, OnBtnOutput3)
	ON_BN_CLICKED(IDC_BTN_OUTPUT4, OnBtnOutput4)
	ON_BN_CLICKED(IDC_BTN_OUTPUT5, OnBtnOutput5)
	ON_BN_CLICKED(IDC_BTN_OUTPUT6, OnBtnOutput6)
	ON_BN_CLICKED(IDC_BTN_OUTPUT7, OnBtnOutput7)
	ON_BN_CLICKED(IDC_BTN_OUTPUT8, OnBtnOutput8)
	ON_BN_CLICKED(IDC_BTN_OUTPUT9, OnBtnOutput9)
	ON_BN_CLICKED(IDC_BTN_OUTPUT10, OnBtnOutput10)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAh32Dlg メッセージ ハンドラ

BOOL CAh32Dlg::OnInitDialog()
{
	char	szExePath[FNAMEMAX];
	char	envfname[FNAMEMAX];
	FILE	*envfp;
	char	envbuf[RECMAX];

	CDialog::OnInitDialog();

	CAh32App *theApp = (CAh32App*)AfxGetApp();
	m_pcSysEnv = theApp->m_pcSysEnv;
	m_pcFuncLib = theApp->m_pcFuncLib;
	InitExePath(szExePath);
	strcpy(m_pcSysEnv->m_szExePath, szExePath);

	LoadSeting();
	if(m_szWakuPath[0] == '\0'){
		return(FALSE);
	}

	strcpy(m_szDataFname, "ah32.rec");
	sprintf(envfname, "%s\\ah32.env", szExePath);
	envfp = fopen(envfname, "r");
	if(envfp == NULL){
		return(FALSE);
	}
	fgets(envbuf, RECMAX, envfp);
	sscanf(envbuf, "%s", m_szDataFname);
	fclose(envfp);

	InitBtnName();

	ConvertAhoutRec();

	// このダイアログ用のアイコンを設定します。フレームワークはアプリケーションのメイン
	// ウィンドウがダイアログでない時は自動的に設定しません。
	SetIcon(m_hIcon, TRUE);			// 大きいアイコンを設定
	SetIcon(m_hIcon, FALSE);		// 小さいアイコンを設定
	
	// TODO: 特別な初期化を行う時はこの場所に追加してください。
	
	return TRUE;
}

// もしダイアログボックスに最小化ボタンを追加するならば、アイコンを描画する
// コードを以下に記述する必要があります。MFC アプリケーションは document/view
// モデルを使っているので、この処理はフレームワークにより自動的に処理されます。

void CAh32Dlg::OnPaint() 
{
	BOOL	ret;

	if (IsIconic())
	{
		CPaintDC dc(this); // 描画用のデバイス コンテキスト

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// クライアントの矩形領域内の中央
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// アイコンを描画します。
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// システムは、ユーザーが最小化ウィンドウをドラッグしている間、
// カーソルを表示するためにここを呼び出します。
HCURSOR CAh32Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CAh32Dlg::OnOK() 
{
	CDialog::OnOK();
}

void CAh32Dlg::InitExePath(LPSTR lpExePath)
{
	int		len;

	len = ::GetModuleFileName(AfxGetInstanceHandle( ), lpExePath, FNAMEMAX);
	while(1){
		if(len <= 0){
			lpExePath[0] = '\0';
			break;
		}
		if(lpExePath[len] == ':' || lpExePath[len] == '\\'){
			lpExePath[len] = '\0';
			break;
		}
		len--;
	}
}

void CAh32Dlg::InitBtnName()
{
	char	szExePath[FNAMEMAX];
	char	lstfname[FNAMEMAX];
	FILE	*lstfp;
	char	lstbuf[RECMAX];
	char	btnname[QRECMAX];
	char	crctpath[FNAMEMAX];
	char	wakufname[FNAMEMAX];
	int		idx;

	m_pcSysEnv->GetExePath(szExePath);
	sprintf(lstfname, "%s\\ah32.lst", szExePath);
	lstfp = fopen(lstfname, "r");
	if(lstfp == NULL){
		return;
	}
	idx = 0;
	while(1){
		if(fgets(lstbuf, RECMAX, lstfp) == NULL){
			break;
		}
		sscanf(lstbuf, "%s %s %s %s", btnname, crctpath, m_szPaper, wakufname);
		SetDlgItemText(IDC_BTN_OUTPUT1+idx, btnname);
		idx++;
	}
	fclose(lstfp);
	btnname[0] = '\0';
	for(;idx < 10; idx++){
		SetDlgItemText(IDC_BTN_OUTPUT1+idx, btnname);
	}
}

BOOL CAh32Dlg::ConvertAhoutRec()
{
	char	szExePath[FNAMEMAX];
	char	lstfname[FNAMEMAX];
	FILE	*lstfp;
	char	lstbuf[RECMAX];
	char	btnname[QRECMAX];
	char	crctpath[FNAMEMAX];
	char	wakufname[FNAMEMAX];
	char	recfname[FNAMEMAX];
	char	crctfname[FNAMEMAX];
	FILE	*recfp;
	FILE	*crctfp;
	char	recbuf[RECMAX];
	char	crctbuf[RECMAX];
	int		len;

	m_pcSysEnv->GetExePath(szExePath);
	sprintf(lstfname, "%s\\ah32.lst", szExePath);
	lstfp = fopen(lstfname, "r");
	if(lstfp == NULL){
		return(FALSE);
	}
	while(1){
		if(fgets(lstbuf, RECMAX, lstfp) == NULL){
			break;
		}
		sscanf(lstbuf, "%s %s %s %s", btnname, crctpath, m_szPaper, wakufname);

		sprintf(crctfname, "%s\\outcrect.dat", crctpath);
		crctfp = fopen(crctfname, "w");
		if(crctfp == NULL){
			continue;
		}

		recfp = fopen(m_szDataFname, "r");
		if(recfp == NULL){
			fclose(crctfp);
			break;
		}
		while(1){
			if(fgets(recbuf, RECMAX, recfp) == NULL){
				break;
			}
			len = strlen(recbuf);
			recbuf[len-1] = '\0';
			fprintf(crctfp, "\"未\",\"%s\",\"%s\",\"%s\",\"1\",\"/%s/\"\n"
				, m_szGrup, m_szPaper, wakufname, recbuf);
		}
		fclose(recfp);
		fclose(crctfp);
	}
	fclose(lstfp);
	return(TRUE);
}

void CAh32Dlg::LoadSeting()
{
	char	szExePath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	*csvsp[10];
	char	szDmy1[FNAMEMAX];
	char	szDmy2[FNAMEMAX];
	char	szDmy3[FNAMEMAX];

	strcpy(m_szGrup, "ソア");
	strcpy(m_szPaper, "");
	strcpy(m_szWakuPath, "");
	m_pcSysEnv->GetExePath(szExePath);
	sprintf(fname, "%s\\soainfo.env", szExePath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);	//会社名
	fgets(buf, RECMAX, fp);	//書体位置
	fgets(buf, RECMAX, fp);	//グループ名
	m_pcFuncLib->DivCsvData(csvsp, buf);
	strcpy(m_szGrup, csvsp[0]);
	//sscanf(buf, "\"%s\"", m_szGrup); 
	fgets(buf, RECMAX, fp);	//枠位置（グループ名）
	fgets(buf, RECMAX, fp);	//標準書体
	fgets(buf, RECMAX, fp);	//外字プログラム名
	fgets(buf, RECMAX, fp);	//枠作成プログラム名
	fgets(buf, RECMAX, fp);	//標準出力機
	fgets(buf, RECMAX, fp);	//用紙名
	m_pcFuncLib->DivCsvData(csvsp, buf);
	strcpy(m_szPaper, csvsp[0]);
	//sscanf(buf, "\"%s\"", m_szPaper); 
	fgets(buf, RECMAX, fp);	//縦書書体回転角度
	fgets(buf, RECMAX, fp);	//標準出力機
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//
	fgets(buf, RECMAX, fp);	//画面表示倍率
	fclose(fp);

	sprintf(fname, "%s\\waku.env", szExePath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);	//用紙名
	sscanf(buf, "%s", m_szPaper);
	fclose(fp);

	sprintf(fname, "%s\\ses_sys.env", szExePath);
	fp = fopen(fname, "r");
	if(fp == NULL){
		return;
	}
	fgets(buf, RECMAX, fp);	//会社名
	fgets(buf, RECMAX, fp);	//作図領域
	fgets(buf, RECMAX, fp);	//出力方向
	fgets(buf, RECMAX, fp);	//ポート
	fgets(buf, RECMAX, fp);	//出力機設定
	fgets(buf, RECMAX, fp);	//出力機フラグ
	fgets(buf, RECMAX, fp);	//？？？
	fgets(buf, RECMAX, fp);	//書体位置
	fgets(buf, RECMAX, fp);	//（データ保存　貯め込み　家紋　枠）位置
	sscanf(buf, "%s %s %s %s", szDmy1, szDmy2, szDmy3, m_szWakuPath);
	fgets(buf, RECMAX, fp);	//作業位置　ＢＭＰ作業位置
	fclose(fp);
}

void CAh32Dlg::OnBtnExit() 
{
	CDialog::OnOK();
}

void CAh32Dlg::OnBtnInputedit() 
{
	int		ret;

	CDialogEditData	*pcDialogEditData;

	pcDialogEditData = new CDialogEditData(this);
	ret = pcDialogEditData->DoModal();
	if(ret == IDOK){
		ConvertAhoutRec();
	}
}

void CAh32Dlg::OnBtnOutput1() 
{
	ProgramExec(0);
}

void CAh32Dlg::OnBtnOutput2() 
{
	ProgramExec(1);
}

void CAh32Dlg::OnBtnOutput3() 
{
	ProgramExec(2);
}

void CAh32Dlg::OnBtnOutput4() 
{
	ProgramExec(3);
}

void CAh32Dlg::OnBtnOutput5() 
{
	ProgramExec(4);
}

void CAh32Dlg::OnBtnOutput6() 
{
	ProgramExec(5);
}

void CAh32Dlg::OnBtnOutput7() 
{
	ProgramExec(6);
}

void CAh32Dlg::OnBtnOutput8() 
{
	ProgramExec(7);
}

void CAh32Dlg::OnBtnOutput9() 
{
	ProgramExec(8);
}

void CAh32Dlg::OnBtnOutput10() 
{
	ProgramExec(9);
}

void CAh32Dlg::ProgramExec(int no)
{
	char	szExePath[FNAMEMAX];
	char	lstfname[FNAMEMAX];
	FILE	*lstfp;
	char	lstbuf[RECMAX];
	char	btnname[QRECMAX];
	char	crctpath[FNAMEMAX];
	char	wakufname[FNAMEMAX];
	int		idx;
	char	szprog[RECMAX];
	char	szExecProg[RECMAX];

	m_pcSysEnv->GetExePath(szExePath);
	sprintf(lstfname, "%s\\ah32.lst", szExePath);
	lstfp = fopen(lstfname, "r");
	if(lstfp == NULL){
		return;
	}
	for(idx = 0; idx < no; idx++){
		if(fgets(lstbuf, RECMAX, lstfp) == NULL){
			break;
		}
	}
	if(fgets(lstbuf, RECMAX, lstfp) == NULL){
		return;
	}
	sscanf(lstbuf, "%s %s %s %s", btnname, crctpath, m_szPaper, wakufname);

	GetProgranName(1, szprog);
	if(szprog[1] == ':' || szprog[1] == '.'){
		strcpy(szExecProg, szprog);		
	}
	else{
		sprintf(szExecProg, "%s\\%s", crctpath, szprog);
	}
	::WinExec(szExecProg, SW_SHOW);
}

void CAh32Dlg::GetProgranName(int no, char *lpprog)
{
	char	szExePath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	int		i, len;

	*lpprog = '\0';
	switch(no){
	case 0:
		strcpy(lpprog, "sms.exe");
		break;
	case 1:
		strcpy(lpprog, "smsout.exe");
		break;
	}

	m_pcSysEnv->GetExePath(szExePath);
	sprintf(fname, "%s\\prntprog.env", szExePath);
	if((fp = fopen(fname, "r")) != NULL){
		for(i = 0; i < no; i++){
			fgets(lpprog, RECMAX, fp);
		}
		fgets(lpprog, RECMAX, fp);
		len = strlen(lpprog);
		lpprog[len-1] = '\0';
		fclose(fp);
	}
}


