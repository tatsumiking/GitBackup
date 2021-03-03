// DSizeTrns.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "smsout.h"
#include "DSizeTrns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeTrns ダイアログ
extern "C" {
	void senvgetexepath(LPSTR path);
	void senvgettmppath(LPSTR path);
	void scrngetmmsize(LPDBL mmxsize, LPDBL mmysize);
	void scrnsetmmsize(DBL mmxsize, DBL mmysize);
	void poutploternameset(LPSTR pltname);
	void poutploternameget(LPSTR pltname);
	void basechangesafix(LPSTR FileName, LPSTR Safix);
	void wakuenvsaveoutputenv(LPSTR wakusavefname);
}


CDialogSizeTrns::CDialogSizeTrns(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSizeTrns::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	//{{AFX_DATA_INIT(CDialogSizeTrns)
	m_csDstXSize = _T("");
	m_csDstYSize = _T("");
	m_csSrcXSize = _T("");
	m_csSrcYSize = _T("");
	m_csPlotName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogSizeTrns::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSizeTrns)
	DDX_Control(pDX, IDC_BTNPROPERTY, m_btnProperty);
	DDX_Control(pDX, IDC_CMBPLOTNAME, m_cmbPlotName);
	DDX_Text(pDX, IDC_EDDSTXSIZE, m_csDstXSize);
	DDX_Text(pDX, IDC_EDDSTYSIZE, m_csDstYSize);
	DDX_Text(pDX, IDC_EDSRCXSIZE, m_csSrcXSize);
	DDX_Text(pDX, IDC_EDSRCYSIZE, m_csSrcYSize);
	DDX_CBString(pDX, IDC_CMBPLOTNAME, m_csPlotName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSizeTrns, CDialog)
	//{{AFX_MSG_MAP(CDialogSizeTrns)
	ON_CBN_SELCHANGE(IDC_CMBPLOTNAME, OnSelchangeCmbplotname)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDSTXSIZE, OnDeltaposSpndstxsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNDSTYSIZE, OnDeltaposSpndstysize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNSRCXSIZE, OnDeltaposSpnsrcxsize)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPNSRCYSIZE, OnDeltaposSpnsrcysize)
	ON_BN_CLICKED(IDC_BTNPROPERTY, OnBtnproperty)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeTrns メッセージ ハンドラ

BOOL CDialogSizeTrns::OnInitDialog() 
{
	DBL		xsize, ysize;

	CDialog::OnInitDialog();
	InitLoadFirstWakuData();
	InitCmbPlotName();
	SetBtnEnable();
	scrngetmmsize(&xsize, &ysize);
	m_csDstXSize.Format("%1.1lf", xsize);
	m_csDstYSize.Format("%1.1lf", ysize);
	m_csSrcXSize.Format("%1.1lf", xsize);
	m_csSrcYSize.Format("%1.1lf", ysize);
	UpdateData(FALSE);
	return TRUE;
}

void CDialogSizeTrns::OnSelchangeCmbplotname() 
{
	int		idx;
	char	plotname[QRECMAX];

	idx = m_cmbPlotName.GetCurSel();
	UpdateData(TRUE);
	m_cmbPlotName.GetLBText(idx, plotname);
	poutploternameset(plotname);
	m_csPlotName = _T(plotname);
	SetBtnEnable();
	UpdateData(FALSE);
}

void CDialogSizeTrns::InitLoadFirstWakuData()
{
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		EndCrctDB();	
		return;
	}
	m_pcRdsOutCrct->MoveFirst();
	m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
	EndCrctDB();	
}

void CDialogSizeTrns::InitCmbPlotName()
{
	char	exepath[FNAMEMAX];
	char	fname[FNAMEMAX];
	FILE	*fp;
	char	buf[RECMAX];
	char	plotname[QRECMAX];

	senvgetexepath(exepath);

	sprintf(fname, "%s\\plotname.env", exepath);
	if((fp = fopen(fname, "r")) == NULL){
		return;
	}
	while(1){
		if(fgets(buf, RECMAX, fp) == NULL){
			break;
		}
		buf[strlen(buf)-1] = '\0';
		sscanf(buf, "%s", plotname);
		m_cmbPlotName.AddString(plotname);
	}
	fclose(fp);
	poutploternameget(plotname);
	m_csPlotName = _T(plotname);
	UpdateData(FALSE);
}
void CDialogSizeTrns::InitCrctDB()
{
	char	szExePath[FNAMEMAX];
	CString	csDBName;
	CString	csSQL;

	senvgetexepath(szExePath);
	csDBName.Format("%s\\outcrect.mdb", szExePath);
	csSQL.Format("select * from [出力テーブル]");
	m_dbOutcrct.Open(csDBName, FALSE, FALSE);
	m_pcRdsOutCrct = new CRdsOutCrct(&m_dbOutcrct);
    m_pcRdsOutCrct->Open(dbOpenDynaset, csSQL, dbDenyWrite);
}

void CDialogSizeTrns::EndCrctDB()
{
	m_pcRdsOutCrct->Close();
	delete(m_pcRdsOutCrct);
	m_dbOutcrct.Close();
}

void CDialogSizeTrns::SetBtnEnable()
{
	char	plotername[QRECMAX];

	poutploternameget(plotername);
	if(strcmp(plotername, "プリンタ") == 0){
		m_btnProperty.EnableWindow(TRUE);
	}else{
		m_btnProperty.EnableWindow(FALSE);
	}
}

void CDialogSizeTrns::OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csDstXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstXSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csDstYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstYSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csSrcXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcXSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = atof(m_csSrcYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcYSize.Format("%1.1lf", num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnOK() 
{
	DBL	srcxsize, srcysize;
	DBL	dstxsize, dstysize;
	DBL	dxtime, dytime;
	char	tmppath[FNAMEMAX];
	char	szWakuName[FNAMEMAX];
	char	szPrnBinName[FNAMEMAX];

	UpdateData(TRUE);

	senvgettmppath(tmppath);
	sprintf(szPrnBinName, "%s\\crct.bin", tmppath);
	m_pcView->PrintEnvSave(szPrnBinName);

	dstxsize = atof(m_csDstXSize);
	dstysize = atof(m_csDstYSize);
	srcxsize = atof(m_csSrcXSize);
	srcysize = atof(m_csSrcYSize);
	dxtime = dstxsize / srcxsize;
	dytime = dstysize / srcysize;

	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		EndCrctDB();	
		return;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		strcpy(szWakuName, m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
		m_pcDoc->DataAllCls();
		m_pcDoc->WakuLoad(szWakuName);

		scrngetmmsize(&srcxsize, &srcysize);
		m_pcDoc->OnFlag(ACTIVEFLAG);
		m_pcDoc->SizeTrns(0.0, 0.0, dxtime, dytime);
		dstxsize = srcxsize * dxtime;
		dstysize = srcysize * dytime;
		scrnsetmmsize(dstxsize, dstysize);
		m_pcView->WorkSizeReset();
		m_pcView->WorkAreaReset();

		poutploternameset(m_csPlotName.GetBuffer(QRECMAX));
		m_pcView->PrintEnvLoad(szPrnBinName);

		m_pcDoc->WakuSave(szWakuName);
		basechangesafix(szWakuName, "env");
		wakuenvsaveoutputenv(szWakuName);
		basechangesafix(szWakuName, "bin");
		m_pcView->PrintEnvSave(szWakuName);

		m_pcRdsOutCrct->MoveNext();
	}
	EndCrctDB();


	CDialog::OnOK();
}

void CDialogSizeTrns::OnBtnproperty() 
{
	CTonApp*	pcApp;

	UpdateData(TRUE);

	pcApp = (CTonApp*)::AfxGetApp();
	pcApp->SetupPrinter();	
}
