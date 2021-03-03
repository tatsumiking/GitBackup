// DlgOutput.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DlgOutput.h"
#include "..\cmnsrc\dlginnum.h"
#include "dlgnumarea.h"
#include "dlgchkout.h"
#include "dlgimgdsp.h"
#include "dlglstdsp.h"
#include "dsizetrns.h"
#include "..\cmnsrc\cmd_base.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern "C" {
	LPSTR basecsvstrdivcpy(LPSTR sp, LPSTR retstr);
	int  baseiskanji(int far hc);
	void senvgetexepath(LPSTR path);
	void poutploternameget(LPSTR pltname);
	void mojienvgetfontname(LPSTR fontname);
	void mojienvsetfontname(LPSTR fontname);
	void mojienvgetflist(int idx, LPINT type, LPSTR sfname, LPSTR ffname);
	int senvgetpennoaddname(int rval, int gval, int bval);
	void senvsetcrtfillpenno(int penno);
	void senvsetcrtlinepenno(int penno);
}
/////////////////////////////////////////////////////////////////////////////
// CDialogOutput �_�C�A���O

CDialogOutput::CDialogOutput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogOutput::IDD, pParent)
{
	int		i;

	m_pcView = (CTonView*)pParent;
	m_pcDoc = m_pcView->GetDocument();
	m_nFirst = TRUE;
	//{{AFX_DATA_INIT(CDialogOutput)
	m_csFont = _T("");
	m_nInSort = FALSE;
	m_csSumCount = _T("");
	m_csTitle = _T("");
	m_nDefPrinter = 0;
	m_nTitleOut = FALSE;
	//}}AFX_DATA_INIT
	m_nCrtTop = 0;
	for(i = 0; i < OUTLISTMAX; i++){
		m_csOutNo[i] = _T("");
		m_nChkOut[i] = 0;
		m_csPaper[i] = _T("");
		m_nOut[i] = 0;
		m_csText[i] = _T("");
		m_csCpyCnt[i] = _T("");
	}
	m_clrSlct = (COLORREF)-1;
	m_csFont = _T("");
}


void CDialogOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogOutput)
	DDX_Control(pDX, IDC_CHKDEFPRINTER, m_chkDefPrinter);
	DDX_Control(pDX, IDC_STCLR, m_stClr);
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_CMBFONT, m_cmbFont);
	DDX_CBString(pDX, IDC_CMBFONT, m_csFont);
	DDX_Check(pDX, IDC_CHKINSORT, m_nInSort);
	DDX_Text(pDX, IDC_EDSUMCOUNT, m_csSumCount);
	DDX_Text(pDX, IDC_EDTITLE, m_csTitle);
	DDX_Check(pDX, IDC_CHKDEFPRINTER, m_nDefPrinter);
	DDX_Check(pDX, IDC_CHKTITLEOUT, m_nTitleOut);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_STOUTNO1, m_csOutNo[0]);
	DDX_Text(pDX, IDC_STOUTNO2, m_csOutNo[1]);
	DDX_Text(pDX, IDC_STOUTNO3, m_csOutNo[2]);
	DDX_Text(pDX, IDC_STOUTNO4, m_csOutNo[3]);
	DDX_Text(pDX, IDC_STOUTNO5, m_csOutNo[4]);
	DDX_Text(pDX, IDC_STOUTNO6, m_csOutNo[5]);
	DDX_Text(pDX, IDC_STOUTNO7, m_csOutNo[6]);
	DDX_Text(pDX, IDC_STOUTNO8, m_csOutNo[7]);
	DDX_Text(pDX, IDC_STOUTNO9, m_csOutNo[8]);
	DDX_Text(pDX, IDC_STOUTNO10, m_csOutNo[9]);
	DDX_Text(pDX, IDC_STOUTNO11, m_csOutNo[10]);
	DDX_Text(pDX, IDC_STOUTNO12, m_csOutNo[11]);
	DDX_Check(pDX, IDC_CHKOUT1, m_nChkOut[0]);
	DDX_Check(pDX, IDC_CHKOUT2, m_nChkOut[1]);
	DDX_Check(pDX, IDC_CHKOUT3, m_nChkOut[2]);
	DDX_Check(pDX, IDC_CHKOUT4, m_nChkOut[3]);
	DDX_Check(pDX, IDC_CHKOUT5, m_nChkOut[4]);
	DDX_Check(pDX, IDC_CHKOUT6, m_nChkOut[5]);
	DDX_Check(pDX, IDC_CHKOUT7, m_nChkOut[6]);
	DDX_Check(pDX, IDC_CHKOUT8, m_nChkOut[7]);
	DDX_Check(pDX, IDC_CHKOUT9, m_nChkOut[8]);
	DDX_Check(pDX, IDC_CHKOUT10, m_nChkOut[9]);
	DDX_Check(pDX, IDC_CHKOUT11, m_nChkOut[10]);
	DDX_Check(pDX, IDC_CHKOUT12, m_nChkOut[11]);
	DDX_Text(pDX, IDC_EDPAPER1, m_csPaper[0]);
	DDX_Text(pDX, IDC_EDPAPER2, m_csPaper[1]);
	DDX_Text(pDX, IDC_EDPAPER3, m_csPaper[2]);
	DDX_Text(pDX, IDC_EDPAPER4, m_csPaper[3]);
	DDX_Text(pDX, IDC_EDPAPER5, m_csPaper[4]);
	DDX_Text(pDX, IDC_EDPAPER6, m_csPaper[5]);
	DDX_Text(pDX, IDC_EDPAPER7, m_csPaper[6]);
	DDX_Text(pDX, IDC_EDPAPER8, m_csPaper[7]);
	DDX_Text(pDX, IDC_EDPAPER9, m_csPaper[8]);
	DDX_Text(pDX, IDC_EDPAPER10, m_csPaper[9]);
	DDX_Text(pDX, IDC_EDPAPER11, m_csPaper[10]);
	DDX_Text(pDX, IDC_EDPAPER12, m_csPaper[11]);
	DDX_Control(pDX, IDC_STOUT1, m_stOut[0]);
	DDX_Control(pDX, IDC_STOUT2, m_stOut[1]);
	DDX_Control(pDX, IDC_STOUT3, m_stOut[2]);
	DDX_Control(pDX, IDC_STOUT4, m_stOut[3]);
	DDX_Control(pDX, IDC_STOUT5, m_stOut[4]);
	DDX_Control(pDX, IDC_STOUT6, m_stOut[5]);
	DDX_Control(pDX, IDC_STOUT7, m_stOut[6]);
	DDX_Control(pDX, IDC_STOUT8, m_stOut[7]);
	DDX_Control(pDX, IDC_STOUT9, m_stOut[8]);
	DDX_Control(pDX, IDC_STOUT10, m_stOut[9]);
	DDX_Control(pDX, IDC_STOUT11, m_stOut[10]);
	DDX_Control(pDX, IDC_STOUT12, m_stOut[11]);
	DDX_Text(pDX, IDC_EDTEXT1, m_csText[0]);
	DDX_Text(pDX, IDC_EDTEXT2, m_csText[1]);
	DDX_Text(pDX, IDC_EDTEXT3, m_csText[2]);
	DDX_Text(pDX, IDC_EDTEXT4, m_csText[3]);
	DDX_Text(pDX, IDC_EDTEXT5, m_csText[4]);
	DDX_Text(pDX, IDC_EDTEXT6, m_csText[5]);
	DDX_Text(pDX, IDC_EDTEXT7, m_csText[6]);
	DDX_Text(pDX, IDC_EDTEXT8, m_csText[7]);
	DDX_Text(pDX, IDC_EDTEXT9, m_csText[8]);
	DDX_Text(pDX, IDC_EDTEXT10, m_csText[9]);
	DDX_Text(pDX, IDC_EDTEXT11, m_csText[10]);
	DDX_Text(pDX, IDC_EDTEXT12, m_csText[11]);
	DDX_Text(pDX, IDC_EDCPYCNT1, m_csCpyCnt[0]);
	DDX_Text(pDX, IDC_EDCPYCNT2, m_csCpyCnt[1]);
	DDX_Text(pDX, IDC_EDCPYCNT3, m_csCpyCnt[2]);
	DDX_Text(pDX, IDC_EDCPYCNT4, m_csCpyCnt[3]);
	DDX_Text(pDX, IDC_EDCPYCNT5, m_csCpyCnt[4]);
	DDX_Text(pDX, IDC_EDCPYCNT6, m_csCpyCnt[5]);
	DDX_Text(pDX, IDC_EDCPYCNT7, m_csCpyCnt[6]);
	DDX_Text(pDX, IDC_EDCPYCNT8, m_csCpyCnt[7]);
	DDX_Text(pDX, IDC_EDCPYCNT9, m_csCpyCnt[8]);
	DDX_Text(pDX, IDC_EDCPYCNT10, m_csCpyCnt[9]);
	DDX_Text(pDX, IDC_EDCPYCNT11, m_csCpyCnt[10]);
	DDX_Text(pDX, IDC_EDCPYCNT12, m_csCpyCnt[11]);
}

BEGIN_MESSAGE_MAP(CDialogOutput, CDialog)
	//{{AFX_MSG_MAP(CDialogOutput)
	ON_BN_CLICKED(IDC_BTNCMD1, OnBtncmd1)
	ON_BN_CLICKED(IDC_BTNCMD2, OnBtncmd2)
	ON_BN_CLICKED(IDC_BTNCMD3, OnBtncmd3)
	ON_BN_CLICKED(IDC_BTNCMD4, OnBtncmd4)
	ON_BN_CLICKED(IDC_BTNCMD5, OnBtncmd5)
	ON_BN_CLICKED(IDC_BTNCMD6, OnBtncmd6)
	ON_BN_CLICKED(IDC_BTNCMD7, OnBtncmd7)
	ON_BN_CLICKED(IDC_BTNCMD8, OnBtncmd8)
	ON_BN_CLICKED(IDC_BTNCMD9, OnBtncmd9)
	ON_BN_CLICKED(IDC_BTNCMD10, OnBtncmd10)
	ON_BN_CLICKED(IDC_BTNCMD11, OnBtncmd11)
	ON_BN_CLICKED(IDC_BTNCMD12, OnBtncmd12)
	ON_BN_CLICKED(IDC_STCLR, OnStclr)
	ON_BN_CLICKED(IDC_BTNNEXTPAGE, OnBtnnextpage)
	ON_BN_CLICKED(IDC_BTNPREPAGE, OnBtnprepage)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHKDEFPRINTER, OnChkdefprinter)
	ON_BN_CLICKED(IDC_BTNSIZETRNS, OnBtnsizetrns)
	ON_BN_CLICKED(IDC_BTNCMD13, OnBtncmd13)
	ON_BN_CLICKED(IDC_BTNCMD14, OnBtncmd14)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogOutput ���b�Z�[�W �n���h��

BOOL CDialogOutput::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InitFontList();
	InitSetDB();
	m_nCrtTop = 0;
	UpdateOutList();
	return TRUE;
}

BOOL CDialogOutput::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN
	|| pMsg->message == WM_SYSKEYDOWN){
		switch(pMsg->wParam){
		case VK_PRIOR:
			OnBtnprepage();
			break;
		case VK_NEXT:
			OnBtnnextpage();
			break;
		case VK_UP:
			m_nCrtTop = LegalCrtTop(m_nCrtTop - OUTLISTMAX/2);
			UpdateOutList();
			break;
		case VK_DOWN:
			m_nCrtTop = LegalCrtTop(m_nCrtTop + OUTLISTMAX/2);
			UpdateOutList();
			break;
		case VK_F1:
			OnBtncmd1();
			break;
		case VK_F2:
			OnBtncmd2();
			break;
		case VK_F3:
			OnBtncmd3();
			break;
		case VK_F4:
			OnBtncmd4();
			break;
		case VK_F5:
			OnBtncmd5();
			break;
		case VK_F6:
			OnBtncmd6();
			break;
		case VK_F7:
			OnBtncmd7();
			break;
		case VK_F8:
			OnBtncmd8();
			break;
		case VK_F9:
			OnBtncmd9();
			break;
		case VK_F10:
			OnBtncmd10();
			break;
		case VK_F11:
			OnBtncmd11();
			break;
		case VK_F12:
			OnBtncmd12();
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogOutput::OnBtncmd1() 
{
	CDialogInputNum	*pDialogInputNum;
	int		cpycnt;

	pDialogInputNum = new CDialogInputNum(m_pcView);
	pDialogInputNum->SetDialogTitle("�o�̓R�s�[�����w��");
	pDialogInputNum->SetUnitText("��");
	pDialogInputNum->SetTitleText("����");
	pDialogInputNum->m_dInputNum = 1;
	if(pDialogInputNum->DoModal() == IDOK){
		cpycnt = (int)pDialogInputNum->m_dInputNum;
		InitCrctDB();
		if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
			goto DBLOADEND;
		}
		m_pcRdsOutCrct->MoveFirst();
		while(1){
			if(m_pcRdsOutCrct->IsEOF() == TRUE){
				break;
			}
			m_pcRdsOutCrct->Edit();
			m_pcRdsOutCrct->m_column7.Format("%d", cpycnt);
			m_pcRdsOutCrct->Update();
			m_pcRdsOutCrct->MoveNext();
		}
DBLOADEND:
		EndCrctDB();
		UpdateOutList();
	}
	delete(pDialogInputNum);
}

void CDialogOutput::OnBtncmd2() 
{
	CDialogNumArea	*pcDialogNumArea;
	int		idx, sidx, eidx;

	pcDialogNumArea = new CDialogNumArea(m_pcView);
	pcDialogNumArea->m_csSNum = _T("1");
	pcDialogNumArea->m_csENum = _T("1");
	if(pcDialogNumArea->DoModal() == IDOK){
		sidx = atoi(pcDialogNumArea->m_csSNum.GetBuffer(QRECMAX));
		eidx = atoi(pcDialogNumArea->m_csENum.GetBuffer(QRECMAX));
		InitCrctDB();
		if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
			goto DBLOADEND;
		}
		m_pcRdsOutCrct->MoveFirst();
		idx = 1;
		while(1){
			if(m_pcRdsOutCrct->IsEOF() == TRUE){
				break;
			}
			m_pcRdsOutCrct->Edit();
			if(sidx <= idx && idx <= eidx){
				m_pcRdsOutCrct->m_column8 = _T("1");
			}
			else{
				m_pcRdsOutCrct->m_column8 = _T("0");
			}
			m_pcRdsOutCrct->Update();
			m_pcRdsOutCrct->MoveNext();
			idx++;
		}
DBLOADEND:
		EndCrctDB();
		UpdateOutList();
	}
	delete(pcDialogNumArea);
}

void CDialogOutput::OnBtncmd3() 
{
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		m_pcRdsOutCrct->Edit();
		m_pcRdsOutCrct->m_column8 = _T("1");
		m_pcRdsOutCrct->Update();
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
}

void CDialogOutput::OnBtncmd4() 
{
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		m_pcRdsOutCrct->Edit();
		m_pcRdsOutCrct->m_column8 = _T("0");
		m_pcRdsOutCrct->Update();
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
}

// �o��
void CDialogOutput::OnBtncmd5() 
{
	int		cpycnt;
	char	szStr[RECMAX8];

	UpdateData(TRUE);
	ResetElement();
	InitChangeElement();
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			m_pcDoc->DataAllCls();
			m_pcDoc->m_nNoLoadDev = m_nDefPrinter;
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			szStr[0] = '\0';
			strcpy(szStr, m_pcRdsOutCrct->m_column5.GetBuffer(RECMAX8));
			m_pcView->AllWakuSetString(szStr);
			SetChangeElement();
			cpycnt = atoi(m_pcRdsOutCrct->m_column7.GetBuffer(QRECMAX));
			OutputData(cpycnt);
			m_pcRdsOutCrct->Edit();
			m_pcRdsOutCrct->m_column8 = _T("0");
			m_pcRdsOutCrct->m_column6 = _T("��");
			m_pcRdsOutCrct->Update();
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
}

void CDialogOutput::OnBtncmd6() 
{
	int		cpycnt;

	UpdateData(TRUE);
	ResetElement();
	InitChangeElement();
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			m_pcDoc->DataAllCls();
			m_pcDoc->m_nNoLoadDev = m_nDefPrinter;
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			m_pcView->AllWakuSetString(m_pcRdsOutCrct->m_column5.GetBuffer(FNAMEMAX));
			SetChangeElement();
			cpycnt = atoi(m_pcRdsOutCrct->m_column7.GetBuffer(QRECMAX));
			if(CheckOutput() == TRUE){
				OutputData(cpycnt);
				m_pcRdsOutCrct->Edit();
				m_pcRdsOutCrct->m_column8 = _T("0");
				m_pcRdsOutCrct->m_column6 = _T("��");
				m_pcRdsOutCrct->Update();
			}
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
}

// �x�^����
void CDialogOutput::OnBtncmd7() 
{
	char	szStr[RECMAX8];
	int		len;

	UpdateData(TRUE);
	ResetElement();

	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		strcpy(szStr, m_pcRdsOutCrct->m_column5.GetBuffer(RECMAX8));
		len = strlen(szStr);
		if(len > (RECMAX4-1)){
			break;
		}
		m_pcRdsOutCrct->Edit();
		m_pcRdsOutCrct->m_column5.Format("%s%s", szStr, szStr);
		m_pcRdsOutCrct->Update();
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
}

void CDialogOutput::OnBtncmd8() 
{
	int		nExitCall;
	CDialogListDisp	*pcDialogListDisp;

	UpdateData(TRUE);
	ResetElement();

	nExitCall = 0;
	pcDialogListDisp = new CDialogListDisp(m_pcView);
	pcDialogListDisp->m_nInSort = m_nInSort;
	if(m_nTitleOut == 0){
		pcDialogListDisp->m_szTitle[0] = '\0';
	}
	else{
		strcpy(pcDialogListDisp->m_szTitle, m_csTitle.GetBuffer(HQRECMAX));
	}
	if(pcDialogListDisp->DoModal() == IDOK){
		nExitCall = pcDialogListDisp->m_nExitCall;
	}
	delete(pcDialogListDisp);
	if(nExitCall == 1){
		OnBtncmd9();
	}
}

void CDialogOutput::OnBtncmd9() 
{
	int		nExitCall;
	CDialogImageDisp	*pcDialogImageDisp;

	UpdateData(TRUE);
	ResetElement();

	nExitCall = 0;
	pcDialogImageDisp = new CDialogImageDisp(m_pcView);
	pcDialogImageDisp->m_nInSort = m_nInSort;
	if(m_nTitleOut == 0){
		pcDialogImageDisp->m_szTitle[0] = '\0';
	}
	else{
		strcpy(pcDialogImageDisp->m_szTitle, m_csTitle.GetBuffer(HQRECMAX));
	}
	pcDialogImageDisp->m_clrSlct = m_clrSlct;
	strcpy(pcDialogImageDisp->m_szFont, m_csFont.GetBuffer(RECMAX));
	if(pcDialogImageDisp->DoModal() == IDOK){
		nExitCall = pcDialogImageDisp->m_nExitCall;
	}
	delete(pcDialogImageDisp);
	if(nExitCall == 1){
		OnBtncmd8();
	}
}

void CDialogOutput::OnBtncmd10() 
{
	UpdateData(TRUE);
	ResetElement();
	// �ς݃}�[�N��ݒ�
	SaveOutCrctDat();
	CDialog::OnOK();
}

void CDialogOutput::OnBtncmd11() 
{
	int		cpycnt;
	char	szStr[RECMAX8];

	UpdateData(TRUE);
	ResetElement();
	FTPSendInit();
	InitChangeElement();
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			m_pcDoc->DataAllCls();
			m_pcDoc->m_nNoLoadDev = m_nDefPrinter;
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			szStr[0] = '\0';
			strcpy(szStr, m_pcRdsOutCrct->m_column5.GetBuffer(RECMAX8));
			m_pcView->AllWakuSetString(szStr);
			SetChangeElement();
			FTPSendData();
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
	FTPSendEnd();
}


void CDialogOutput::OnBtncmd12() 
{
	int		cpycnt;
	char	szStr[RECMAX8];

	UpdateData(TRUE);
	ResetElement();
	EFTPSendInit();
	InitChangeElement();
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			m_pcDoc->DataAllCls();
			m_pcDoc->m_nNoLoadDev = m_nDefPrinter;
			m_pcDoc->WakuLoad(m_pcRdsOutCrct->m_column4.GetBuffer(FNAMEMAX));
			m_pcDoc->OnFlag(ACTIVEFLAG);
			szStr[0] = '\0';
			strcpy(szStr, m_pcRdsOutCrct->m_column5.GetBuffer(RECMAX8));
			m_pcView->AllWakuSetString(szStr);
			SetChangeElement();
			EFTPSendData();
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	UpdateOutList();
	EFTPSendEnd();
}

void CDialogOutput::InitFontList()
{
	int		idx;
	int		fonttype;
	char	fontname[FNAMEMAX];
	char	accessname[FNAMEMAX];

	m_cmbFont.ResetContent();
	idx = 0;
	mojienvgetfontname(fontname);
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == '\0' && accessname[0] == '\0'){
			break;
		}
		m_cmbFont.AddString(fontname);
		idx++;
	}
}

//P ���R�[�h�t�@�C���̓��e���f�[�^�[�x�[�X�ɐݒ肷��
void CDialogOutput::InitSetDB()
{
	char	szExePath[FNAMEMAX];
	char	srcfile[FNAMEMAX];
	char	dstfile[FNAMEMAX];
	char	szCrctFName[FNAMEMAX];
	FILE	*fp;
	char	rbuf[RECMAX8];
	char	buf[RECMAX8];
	LPSTR	sp;
	int		idx;

	senvgetexepath(szExePath);

	m_nRecMax = 0;
	sprintf(srcfile, "%s\\orgcrect.mdb", szExePath);
	sprintf(dstfile, "%s\\outcrect.mdb", szExePath);
	CopyFile(srcfile, dstfile, FALSE);

	sprintf(szCrctFName, "%s\\outcrect.dat", szExePath);
	fp = fopen(szCrctFName, "r");
	if(fp == NULL){
		return;
	}

	m_nInSort = 1;
	InitCrctDB();
	m_nInSort = 0;
	idx = 0;
	while(1){
		if(fgets(rbuf, RECMAX8, fp) == NULL){
			break;
		}
		sp = rbuf;
		m_pcRdsOutCrct->AddNew();
		sp = basecsvstrdivcpy(sp, buf);	// "��"or"��"
		m_pcRdsOutCrct->m_column6 = _T(buf);
		if(m_pcRdsOutCrct->m_column6 == "��"){
			m_pcRdsOutCrct->m_column8 = _T("1");
		}
		else{	// ��
			m_pcRdsOutCrct->m_column8 = _T("0");
		}
		sp = basecsvstrdivcpy(sp, buf);	// �g�O���[�v��
		m_pcRdsOutCrct->m_column2 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // �p����
		m_pcRdsOutCrct->m_column3 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // �g�t���p�X��
		m_pcRdsOutCrct->m_column4 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // ����
		m_pcRdsOutCrct->m_column7 = _T(buf);
		sp = basecsvstrdivcpy(sp, buf); // �g������
		if(buf[0] == '\0'){
			buf[0] = '/';
			buf[1] = '\0';
		}
		DeleteExtraSlash(buf);
		m_pcRdsOutCrct->m_column5 = _T(buf);
		m_pcRdsOutCrct->Update();
		idx++;
	}
	m_nRecMax = idx;
	EndCrctDB();
	fclose(fp);
}

void CDialogOutput::SetElement()
{
	int		idx, cnt, outcnt;

	outcnt = 0;
	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	idx = 0;
	while(1){
		if(idx == m_nCrtTop){
			break;
		}
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			outcnt++;
		}
		m_pcRdsOutCrct->MoveNext();
		idx++;
	}
	idx = 0;
	cnt = m_nCrtTop;
	while(1){
		if(idx >= OUTLISTMAX){
			break;
		}
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		m_csOutNo[idx].Format("%03d", cnt+1);
		if(m_pcRdsOutCrct->m_column8 == "0"){
			m_nChkOut[idx] = 0;
		}
		else{
			m_nChkOut[idx] = 1;
			outcnt++;
		}
		m_csPaper[idx] = m_pcRdsOutCrct->m_column3;
		if(m_pcRdsOutCrct->m_column6 == "��"){
			m_nOut[idx] = 0;
		}
		else{
			m_nOut[idx] = 1;
		}
		m_csText[idx] = m_pcRdsOutCrct->m_column5;
		m_csCpyCnt[idx] = m_pcRdsOutCrct->m_column7;
		m_pcRdsOutCrct->MoveNext();
		idx++;
		cnt++;
	}
	while(1){
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		if(m_pcRdsOutCrct->m_column8 == "1"){
			outcnt++;
		}
		m_pcRdsOutCrct->MoveNext();
	}
DBLOADEND:
	EndCrctDB();
	m_csSumCount.Format("%d/%d", outcnt, m_nRecMax);
}

void CDialogOutput::ResetElement()
{
	int		idx, outcnt;

	InitCrctDB();
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		goto DBLOADEND;
	}
	m_pcRdsOutCrct->MoveFirst();
	idx = 0;
	outcnt = 0;
	while(1){
		if(idx == m_nCrtTop){
			break;
		}
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		m_pcRdsOutCrct->MoveNext();
		idx++;
	}
	idx = 0;
	while(1){
		if(idx >= OUTLISTMAX){
			break;
		}
		if(m_pcRdsOutCrct->IsEOF() == TRUE){
			break;
		}
		m_pcRdsOutCrct->Edit();
		if(m_nChkOut[idx] == 0){
			m_pcRdsOutCrct->m_column8 = _T("0");
		}
		else{
			m_pcRdsOutCrct->m_column8 = _T("1");
		}
		if(m_nOut[idx] == 0){
			m_pcRdsOutCrct->m_column6 = _T("��");
		}
		else{
			m_pcRdsOutCrct->m_column6 = _T("��");
		}
		m_pcRdsOutCrct->m_column5 = _T(m_csText[idx]);
		m_pcRdsOutCrct->m_column7 = _T(m_csCpyCnt[idx]);
		m_pcRdsOutCrct->Update();
		m_pcRdsOutCrct->MoveNext();
		idx++;
	}
DBLOADEND:
	EndCrctDB();
}

void CDialogOutput::OnStclr() 
{
	CColorDialog *pcColorDialog = new CColorDialog(m_clrSlct, CC_FULLOPEN, NULL);
	if(pcColorDialog->DoModal() == IDOK){
		m_clrSlct = pcColorDialog->GetColor();
		Invalidate(TRUE);
	}
	delete(pcColorDialog);
}

void CDialogOutput::OnBtnnextpage() 
{
	UpdateData(TRUE);
	ResetElement();
	m_nCrtTop = LegalCrtTop(m_nCrtTop + OUTLISTMAX);
	UpdateOutList();
}

void CDialogOutput::OnBtnprepage() 
{
	UpdateData(TRUE);
	ResetElement();
	m_nCrtTop = LegalCrtTop(m_nCrtTop - OUTLISTMAX);
	UpdateOutList();
}

void CDialogOutput::OnPaint() 
{
	CFont	cFont;
	CFont	*pcOldFont;
	CBrush	*pcBrush;
	CRect	toprect;
	CRect	outrect;
	CRect	clrrect;
	int		i;
	CRect	rect;
	int		sx, sy;

	if(m_nFirst == TRUE){
		m_nFirst = FALSE;
		CMainFrame	*pcWnd = (CMainFrame*)::AfxGetMainWnd();
		pcWnd->GetWindowRect(&rect);
		sx = rect.left;
		sy = rect.top;
		if(sx < 0){
			sx = 10;
		}
		if(sy < 0){
			sy = 10;
		}
		::SetWindowPos(m_hWnd, HWND_TOP, sx, sy, 0, 0, SWP_NOSIZE);
	}
	CPaintDC dc(this);
	m_stTop.GetWindowRect(&toprect);
	m_stOut[0].GetWindowRect(&outrect);
	cFont.CreateFont(outrect.Height(), outrect.Width()/2, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, "�l�r ����");
	pcOldFont = dc.SelectObject(&cFont);
	dc.SetBkMode(OPAQUE);
	for(i = 0; i < OUTLISTMAX; i++){
		if(m_csPaper[i] == ""){
			break;
		}
		m_stOut[i].GetWindowRect(&outrect);
		outrect.left -= toprect.left;
		outrect.right -= toprect.left;
		outrect.top -= toprect.top;
		outrect.bottom -= toprect.top;
		dc.SetTextColor(RGB(255, 255, 255));
		if(m_nOut[i] == 0){
			dc.SetBkColor(RGB(0, 0, 255));
			dc.TextOut(outrect.left, outrect.top, "��", 2);
		}
		else{
			dc.SetBkColor(RGB(255, 0, 0));
			dc.TextOut(outrect.left, outrect.top, "��", 2);
		}
	}
	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(pcOldFont);
	cFont.DeleteObject();
	dc.SetTextColor(RGB(0, 0, 0));
	dc.SetBkColor(RGB(255, 255, 255));

	if(m_clrSlct != -1){
		m_stClr.GetWindowRect(&clrrect);
		clrrect.left -= toprect.left;
		clrrect.right -= toprect.left;
		clrrect.top -= toprect.top;
		clrrect.bottom -= toprect.top;
		pcBrush = new CBrush(m_clrSlct);
		dc.FillRect(&clrrect, pcBrush);
		delete pcBrush;
	}
}

int CDialogOutput::LegalCrtTop(int crttop)
{
	if(crttop + OUTLISTMAX >= m_nRecMax){
		crttop = m_nRecMax - OUTLISTMAX;	
	}
	if(crttop < 0){
		crttop = 0;	
	}
	return(crttop);
}

void CDialogOutput::InitCrctDB()
{
	char	szExePath[FNAMEMAX];
	CString	csDBName;
	CString	csSQL;

	senvgetexepath(szExePath);
	csDBName.Format("%s\\outcrect.mdb", szExePath);
	if(m_nInSort == 0){
		csSQL.Format("select * from [�\�[�g�N�G���[]");
	}
	else{
		csSQL.Format("select * from [�o�̓e�[�u��]");
	}
	m_dbOutcrct.Open(csDBName, FALSE, FALSE);
	m_pcRdsOutCrct = new CRdsOutCrct(&m_dbOutcrct);
    m_pcRdsOutCrct->Open(dbOpenDynaset, csSQL, dbDenyWrite);
}

void CDialogOutput::EndCrctDB()
{
	m_pcRdsOutCrct->Close();
	delete(m_pcRdsOutCrct);
	m_dbOutcrct.Close();
}

void CDialogOutput::UpdateOutList()
{
	SetElement();
	UpdateData(FALSE);
	Invalidate(TRUE);
}

void CDialogOutput::OutputData(int cpycnt)
{
	char	plotername[QRECMAX];

	poutploternameget(plotername);
	if(strcmp(plotername, "�v�����^") == 0){
		m_pcView->PrinterOutput(cpycnt);
	}
	else{
		m_pcView->PloterOutput(cpycnt);
	}
}

int CDialogOutput::CheckOutput()
{
	int		ret;
	CDialogCheckOutput	*pcDialogCheckOutput;
	
	ret = FALSE;
	pcDialogCheckOutput = new CDialogCheckOutput(m_pcView);
	if(pcDialogCheckOutput->DoModal() == IDOK){
		ret = TRUE;
	}
	delete(pcDialogCheckOutput);
	return(ret);
}


void CDialogOutput::InitChangeElement()
{
	int		r, g, b;
	int		penno;

	if(m_clrSlct != -1){
		r = GetRValue(m_clrSlct); 
		g = GetGValue(m_clrSlct); 
		b = GetBValue(m_clrSlct);
		penno = senvgetpennoaddname(r, g, b);
		senvsetcrtfillpenno(penno);
		senvsetcrtlinepenno(penno);
	}
}

void CDialogOutput::SetChangeElement()
{
	char	szFont[RECMAX];

	if(m_csFont.IsEmpty() == FALSE){
		strcpy(szFont, m_csFont.GetBuffer(RECMAX));
		m_pcView->ChangeFont(szFont);
	}
	if(m_clrSlct != -1){
		m_pcDoc->SetOutLineClrFlagOn(0);
		m_pcDoc->SetFillClrFlagOn(0);
	}
}

void CDialogOutput::OnChkdefprinter() 
{
	CTonApp	*pcApp;
	PRINTDLG	sPrintDlg;
	HANDLE		hDevNames;
	LPDEVNAMES	lpDevNames;
	char		szDevice[QRECMAX];

	UpdateData(TRUE);
	if(m_nDefPrinter == TRUE){
		pcApp = (CTonApp*)AfxGetApp();
		pcApp->GetPrinterDeviceDefaults(&sPrintDlg);
		pcApp->SetupPrinter();
		hDevNames = pcApp->GetDevNames();
		lpDevNames = (LPDEVNAMES)::GlobalLock(hDevNames);
		strcpy(szDevice, ((LPSTR)(lpDevNames) + lpDevNames->wDeviceOffset));
		::GlobalUnlock(hDevNames);
		m_chkDefPrinter.SetWindowText(szDevice);
	}
	else{
		m_chkDefPrinter.SetWindowText("�o�̓v�����^�I��");
	}
	UpdateData(FALSE);
}

void CDialogOutput::DeleteExtraSlash(LPSTR lpBuf)
{
	int		idx;
	BYTE	hc;
	int		lastidx;
	int		flag;


	idx = 0;
	flag = OFF;
	while(1){
		if((lpBuf[idx]) == '\0'){
			break;
		}
		hc = (lpBuf[idx]) & 0x00ff;
		if(baseiskanji(hc) == ON){
			idx += 2;
			lastidx = -1;
			flag = OFF;
		}
		else{
			if(hc == '/'){
				if(flag == OFF){
					lastidx = idx;
					flag = ON;
				}
			}
			else{
				lastidx = -1;
				flag = OFF;
			}
			idx++;
		}
	}

	if(lastidx != -1){
		lpBuf[lastidx] = '\0';
	}
}

void CDialogOutput::SaveOutCrctDat()
{
	char	szExePath[FNAMEMAX];
	char	szBakFileName[FNAMEMAX];
	char	szDatFileName[FNAMEMAX];
	FILE	*ifp, *ofp;
	char	szBuf[RECMAX8];
	CString	csStr;
	CString	csStr1, csStr2, csStr3, csStr4, csStr5, csStr6;

	m_nInSort = 1;
	InitCrctDB();
	m_nInSort = 0;
	if(m_pcRdsOutCrct->IsBOF() && m_pcRdsOutCrct->IsEOF()){
		EndCrctDB();
		return;
	}
	senvgetexepath(szExePath);
	sprintf(szDatFileName, "%s\\outcrect.dat", szExePath);
	sprintf(szBakFileName, "%s\\outcrect.bak", szExePath);
	CopyFile(szDatFileName, szBakFileName, FALSE);
	ifp = fopen(szBakFileName, "r");
	if(ifp == NULL){
		EndCrctDB();
		return;
	}
	ofp = fopen(szDatFileName, "w");
	if(ofp == NULL){
		fclose(ifp);
		EndCrctDB();
		return;
	}
	m_pcRdsOutCrct->MoveFirst();
	while(1){
		if(fgets(szBuf, RECMAX8, ifp) == NULL){
			break;
		}
		csStr = m_pcRdsOutCrct->m_column8;
		if(csStr == _T("1")){
			csStr1 = _T("��");
		}else{
			csStr1 = _T("��");
		}
		csStr2 = m_pcRdsOutCrct->m_column2;
		csStr3 = m_pcRdsOutCrct->m_column3;
		csStr4 = m_pcRdsOutCrct->m_column4;
		csStr5 = m_pcRdsOutCrct->m_column7;
		csStr6 = m_pcRdsOutCrct->m_column5;
		sprintf(szBuf, "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",\n"
			,csStr1.GetBuffer(256),csStr2.GetBuffer(256),csStr3.GetBuffer(256)
			,csStr4.GetBuffer(256),csStr5.GetBuffer(256),csStr6.GetBuffer(256));
		fputs(szBuf, ofp);
		m_pcRdsOutCrct->MoveNext();
	}
	fclose(ifp);
	fclose(ofp);
	EndCrctDB();
}

void CDialogOutput::OnBtnsizetrns() 
{
	CDialogSizeTrns *pcDialogSizeTrns = new CDialogSizeTrns(m_pcView);
	pcDialogSizeTrns->DoModal();
	UpdateOutList();
}

void CDialogOutput::FTPSendInit()
{
	m_pcCmdFtp = new CCmdFtp(m_pcView);
	m_pcCmdFtp->SmsoutCrctFileInit();	
}

void CDialogOutput::FTPSendData()
{
	m_pcCmdFtp->SmsoutSendData();
}

void CDialogOutput::FTPSendEnd()
{
	m_pcCmdFtp->SmsoutCrctFileEnd();
	delete(m_pcCmdFtp);
}

void CDialogOutput::EFTPSendInit()
{
	m_pcCmdEFtp = new CCmdEFtp(m_pcView);
	m_pcCmdEFtp->SmsoutCrctFileInit();	
}

void CDialogOutput::EFTPSendData()
{
	m_pcCmdEFtp->SmsoutSendData();
}

void CDialogOutput::EFTPSendEnd()
{
	m_pcCmdEFtp->SmsoutCrctFileEnd();
	delete(m_pcCmdEFtp);
}

void CDialogOutput::OnBtncmd13() 
{
	char	szExePath[FNAMEMAX];
	char	szProg[FNAMEMAX];

	UpdateData(TRUE);
	ResetElement();
	SaveOutCrctDat();
	senvgetexepath(szExePath);
	sprintf(szProg, "%s\\sden.exe", szExePath);
	ShellExecute(m_pcView->m_hWnd, NULL, szProg,
				NULL, NULL, SW_SHOWNORMAL);
	
}
void CDialogOutput::OnBtncmd14() 
{
	UpdateData(TRUE);
	ResetElement();
	SaveOutCrctDat();
}
