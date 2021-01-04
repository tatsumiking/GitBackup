// DSizeTrns.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogSizeTrns.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeTrns ダイアログ

//extern "C" {
	#include "../../ObjLib/Baselib.h"
	#include "../../ObjLib/Pout.h"
	#include "../../ObjLib/Scrnlib.h"
	#include "../../ObjLib/Senvlib.h"
	#include "../../ObjLib/Wakuenv.h"
//}


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
	m_csDstXSize.Format(_T("%1.1lf"), xsize);
	m_csDstYSize.Format(_T("%1.1lf"), ysize);
	m_csSrcXSize.Format(_T("%1.1lf"), xsize);
	m_csSrcYSize.Format(_T("%1.1lf"), ysize);
	UpdateData(FALSE);
	return TRUE;
}

void CDialogSizeTrns::OnSelchangeCmbplotname() 
{
	int		idx;
	TCHAR	plotname[QRECMAX];

	idx = m_cmbPlotName.GetCurSel();
	UpdateData(TRUE);
	m_cmbPlotName.GetLBText(idx, plotname);
	poutploternameset(plotname);
	m_csPlotName = plotname;
	SetBtnEnable();
	UpdateData(FALSE);
}

void CDialogSizeTrns::InitLoadFirstWakuData()
{
	POSITION	pos;
	CCrctWaku	*pcCWCrt;

	pos = m_pcDoc->m_lstCWRead.GetHeadPosition();
	pcCWCrt = m_pcDoc->m_lstCWRead.GetNext(pos);
	m_pcDoc->WakuLoad(pcCWCrt->m_csFileName.GetBuffer(FNAMEMAX));
}

void CDialogSizeTrns::InitCmbPlotName()
{
	TCHAR	exepath[FNAMEMAX];
	TCHAR	fname[FNAMEMAX];
	FILE	*fp;
	TCHAR	buf[RECMAX];
	TCHAR	plotname[QRECMAX];

	senvgetexepath(exepath);

	_stprintf_s(fname, _T("%s\\plotname.env"), exepath);
	_tfopen_s(&fp, fname, _T("r"));
	if (fp == NULL) {
		return;
	}
	while(1){
		if(baselibCrDelFgets(buf, RECMAX, fp) == NULL){
			break;
		}
		_stscanf_s(buf, _T("%s"), plotname);
		m_cmbPlotName.AddString(plotname);
	}
	fclose(fp);
	poutploternameget(plotname);
	m_csPlotName = plotname;
	UpdateData(FALSE);
}

void CDialogSizeTrns::SetBtnEnable()
{
	TCHAR	plotername[QRECMAX];

	poutploternameget(plotername);
	if(_tcscmp(plotername, _T("プリンタ")) == 0){
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
	num = _ttof(m_csDstXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstXSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csDstYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csDstYSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csSrcXSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcXSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult) 
{
	DBL		num;

	UpdateData(TRUE);
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	num = _ttof(m_csSrcYSize);
	num -=  ((DBL)(pNMUpDown->iDelta) * 0.1);
	if(num < 1.0){
		num = 1.0;
	}
	m_csSrcYSize.Format(_T("%1.1lf"), num);
	*pResult = 0;
	UpdateData(FALSE);
}

void CDialogSizeTrns::OnOK() 
{
	POSITION pos;
	CCrctWaku *pcCrctWaku;
	DBL	srcxsize, srcysize;
	DBL	dstxsize, dstysize;
	DBL	dxtime, dytime;
	TCHAR	tmppath[FNAMEMAX];
	TCHAR	szWakuName[FNAMEMAX];
	TCHAR	szPrnBinName[FNAMEMAX];

	UpdateData(TRUE);

	senvgettmppath(tmppath);
	_stprintf_s(szPrnBinName, _T("%s\\crct.bin"), tmppath);
	m_pcView->PrintEnvSave(szPrnBinName);

	dstxsize = _ttof(m_csDstXSize);
	dstysize = _ttof(m_csDstYSize);
	srcxsize = _ttof(m_csSrcXSize);
	srcysize = _ttof(m_csSrcYSize);
	dxtime = dstxsize / srcxsize;
	dytime = dstysize / srcysize;

	pos = m_pcDoc->m_lstCWRead.GetHeadPosition();
	while (pos != NULL)
	{
		pcCrctWaku = m_pcDoc->m_lstCWRead.GetNext(pos);
		_tcscpy_s(szWakuName, pcCrctWaku->m_csFileName.GetBuffer(FNAMEMAX));
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
		basechangesafix(szWakuName, _T("env"));
		wakuenvsaveoutputenv(szWakuName);
		basechangesafix(szWakuName, _T("bin"));
		m_pcView->PrintEnvSave(szWakuName);
	}
	CDialog::OnOK();
}

void CDialogSizeTrns::OnBtnproperty() 
{
	CTonApp*	pcApp;

	UpdateData(TRUE);

	pcApp = (CTonApp*)::AfxGetApp();
	pcApp->SetupPrinter();	
}
