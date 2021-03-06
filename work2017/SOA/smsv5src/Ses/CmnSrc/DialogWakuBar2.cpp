// DWkBar2.cpp : インプリメンテーション ファイル
//

#include "stdafx.h"
#include "DialogWakuBar2.h"

//extern "C" {
#include "../../ObjLib/Mojienv.h"
//}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar2 ダイアログ

CDialogWakuBar2::CDialogWakuBar2(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	m_pcApp = (CTonApp*)::AfxGetApp();
	m_pcView = NULL;
	m_nFirstFlag = ON;
	//{{AFX_DATA_INIT(CDialogWakuBar2)
	m_csFont = _T("");
	m_csFlatPar = _T("");
	m_csSpaceMM = _T("");
	m_csOderKind = _T("");
	m_csKanaPar = _T("");
	m_csHanten = _T("");
	m_csMojiCount = _T("");
	m_csFildName = _T("");
	//}}AFX_DATA_INIT
}

void CDialogWakuBar2::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogWakuBar2)
	DDX_CBString(pDX, IDC_CMBFONT, m_csFont);
	DDX_CBString(pDX, IDC_CMBFLATPAR, m_csFlatPar);
	DDX_CBString(pDX, IDC_CMBSPACEMM, m_csSpaceMM);
	DDX_CBString(pDX, IDC_CMBODERKIND, m_csOderKind);
	DDX_CBString(pDX, IDC_CMBKANAPAR, m_csKanaPar);
	DDX_CBString(pDX, IDC_CMBHANTEN, m_csHanten);
	DDX_Text(pDX, IDC_EDMOJICOUNT, m_csMojiCount);
	DDX_CBString(pDX, IDC_CMBFILD, m_csFildName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDialogWakuBar2, CDialogBar)
	//{{AFX_MSG_MAP(CDialogWakuBar2)
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_CMBFONT, OnSelchangeCmbfont)
	ON_CBN_SELCHANGE(IDC_CMBFLATPAR, OnSelchangeCmbflatpar)
	ON_CBN_SELCHANGE(IDC_CMBHANTEN, OnSelchangeCmbhanten)
	ON_CBN_SELCHANGE(IDC_CMBKANAPAR, OnSelchangeCmbkanapar)
	ON_CBN_SELCHANGE(IDC_CMBODERKIND, OnSelchangeCmboderkind)
	ON_CBN_SELCHANGE(IDC_CMBSPACEMM, OnSelchangeCmbspacemm)
	ON_BN_CLICKED(IDC_BTNSPACEKIND, OnBtnspacekind)
	ON_BN_CLICKED(IDC_CHKFIXED, OnChkfixed)
	ON_BN_CLICKED(IDC_CHKYOKOGUMI, OnChkyokogumi)
	ON_CBN_SELCHANGE(IDC_CMBFILD, OnSelchangeCmbfild)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBar2 メッセージ ハンドラ

void CDialogWakuBar2::OnPaint() 
{
	CPaintDC dc(this); // 描画用のデバイス コンテキスト
	if(m_nFirstFlag == ON){
		m_nFirstFlag = OFF;
		InitItemElement();
	}
}

void CDialogWakuBar2::InitItemElement()
{
	TCHAR	szFont[RECMAX];

	InitCmbFont();
	InitCmbFildName();
	InitCmbFlatPar();
	InitCmbSpace();
	InitCmbOderKind();
	InitCmbKanaPar();
	InitCmbHanten();
	mojienvgetfontname(szFont);
	m_csFont = szFont;
	UpdateData(FALSE);
}

void CDialogWakuBar2::GetItemElement()
{
	CString		csStr;
	CSysEnv		*pcSysEnv;
	CButton		*pcBtn;

	UpdateData(TRUE);

	pcSysEnv = m_pcApp->m_pcSysEnv;
	_tcscpy_s(pcSysEnv->m_szFont, m_csFont.GetBuffer(RECMAX));

	if(m_csFildName == _T("未設定")){
		pcSysEnv->m_szFildName[0] = _T('\0');
	}else{
		_tcscpy_s(pcSysEnv->m_szFildName, m_csFildName.GetBuffer(RECMAX));
	}

	if(m_csFlatPar.IsEmpty()){
	}else if(m_csFlatPar == _T("標準")){
		pcSysEnv->m_dFlatPar = MJFREEFLATPAR;
	}
	else{
		pcSysEnv->m_dFlatPar = _ttoi(m_csFlatPar);
	}


	pcBtn = (CButton*)GetDlgItem(IDC_BTNSPACEKIND);
	if(pcBtn->IsWindowEnabled() == FALSE){
		pcSysEnv->m_nSpaceKind = -1;
	}
	else if(pcBtn->GetCheck() == FALSE){
		pcSysEnv->m_nSpaceKind = SPACEWAKU;
	}
	else{
		pcSysEnv->m_nSpaceKind = SPACEDATA;
	}

	if(m_csSpaceMM.IsEmpty()){
	}
	else if(m_csSpaceMM == _T("標準")){
		pcSysEnv->m_dSpaceMM = MJEQUALSP;
	}
	else{
		pcSysEnv->m_dSpaceMM = _ttoi(m_csSpaceMM.GetBuffer(RECMAX));
	}

	if(m_csOderKind == _T("均等")){
		pcSysEnv->m_nOderKind = ODEREQUAL;
	}
	else if(m_csOderKind == _T("上(左)")){
		pcSysEnv->m_nOderKind = ODERLEFT;
	}
	else if(m_csOderKind == _T("中央")){
		pcSysEnv->m_nOderKind = ODERCNTR;
	}
	else if(m_csOderKind == _T("下(右)")){
		pcSysEnv->m_nOderKind = ODERRIGHT;
	}
	
	if(m_csMojiCount.IsEmpty()){
	}else{
		pcSysEnv->m_dTCHARCount = _ttof(m_csMojiCount);
	}

	if(m_csKanaPar.IsEmpty()){
	}
	else if(m_csKanaPar == _T("標準")){
		pcSysEnv->m_dKanaPar = 100;
	}
	else{
		pcSysEnv->m_dKanaPar = 100 - _ttoi(m_csKanaPar.GetBuffer(RECMAX));
	}

	pcBtn = (CButton*)GetDlgItem(IDC_CHKYOKOGUMI);
	if(pcBtn->IsWindowEnabled() == FALSE){
		pcSysEnv->m_nYokoGumi = -1;
	}
	else{
		pcSysEnv->m_nYokoGumi = pcBtn->GetCheck();
	}

	pcBtn = (CButton*)GetDlgItem(IDC_CHKFIXED);
	if(pcBtn->IsWindowEnabled() == FALSE){
		pcSysEnv->m_nFixFlag = -1;
	}
	else{
		pcSysEnv->m_nFixFlag = pcBtn->GetCheck();
	}
	if(m_csHanten.IsEmpty()){
	}
	else if(m_csHanten == _T("標準")){
		pcSysEnv->m_nMirror = NOMIRROR;
	}
	/*
	else if(m_csHanten == _T("左右")){
		pcSysEnv->m_nMirror = RLMIRROR;
	}
	else if(m_csHanten == _T("上下")){
		pcSysEnv->m_nMirror = TBMIRROR;
	}
	*/
	else if(m_csHanten == _T("反転")){
		pcSysEnv->m_nMirror = PTMIRROR;
	}
}

void CDialogWakuBar2::SetItemElement()
{
	CSysEnv		*pcSysEnv;
	CButton		*pcBtn;

	pcSysEnv = m_pcApp->m_pcSysEnv;
	m_csFont.Format(_T("%s"), pcSysEnv->m_szFont);
	if(pcSysEnv->m_szFildName[0] == _T('\0')){
		m_csFildName = _T("未設定");
	}else{
		m_csFildName.Format(_T("%s"), pcSysEnv->m_szFildName);
	}
	if(pcSysEnv->m_dFlatPar == NOSETPAR){
		m_csFlatPar = _T("");
	}
	else if(pcSysEnv->m_dFlatPar == MJFREEFLATPAR){
		m_csFlatPar = _T("標準");
	}
	else{
		m_csFlatPar.Format(_T("%1.1lf%%"), pcSysEnv->m_dFlatPar);
	}

	pcBtn = (CButton*)GetDlgItem(IDC_BTNSPACEKIND);
	pcBtn->EnableWindow(TRUE);
	if(pcSysEnv->m_nSpaceKind == SPACEWAKU){
		pcBtn->SetCheck(FALSE);
	}
	else if(pcSysEnv->m_nSpaceKind == SPACEDATA){
		pcBtn->SetCheck(TRUE);
	}
	else{
		pcBtn->EnableWindow(FALSE);
	}

	if(pcSysEnv->m_dSpaceMM == NOSETMM){
		m_csSpaceMM = _T("");
	}
	else if(pcSysEnv->m_dSpaceMM == MJEQUALSP){
		m_csSpaceMM = _T("標準");
	}
	else{
		m_csSpaceMM.Format(_T("%1.1lfmm"), pcSysEnv->m_dSpaceMM);
	}

	if(pcSysEnv->m_nOderKind == -1){
		m_csOderKind = _T("");
	}
	else if(pcSysEnv->m_nOderKind == ODEREQUAL){
		m_csOderKind = _T("均等");
	}
	else if(pcSysEnv->m_nOderKind == ODERLEFT){
		m_csOderKind = _T("上(左)");
	}
	else if(pcSysEnv->m_nOderKind == ODERCNTR){
		m_csOderKind = _T("中央");
	}
	else if(pcSysEnv->m_nOderKind == ODERRIGHT){
		m_csOderKind = _T("下(右)");
	}
	
	if(pcSysEnv->m_dKanaPar == NOSETPAR){
		m_csKanaPar = _T("");
	}
	else if(pcSysEnv->m_dKanaPar == 100){
		m_csKanaPar = _T("標準");
	}
	else{
		m_csKanaPar.Format(_T("%1.1lf%%"), 100.0 - pcSysEnv->m_dKanaPar);
	}

	if(pcSysEnv->m_dTCHARCount == NOSETMM){
		m_csMojiCount = _T("");
	}else{
		m_csMojiCount.Format(_T("%1.1lf"), pcSysEnv->m_dTCHARCount);
	}

	pcBtn = (CButton*)GetDlgItem(IDC_CHKYOKOGUMI);
	pcBtn->EnableWindow(TRUE);
	if(pcSysEnv->m_nYokoGumi == -1){
		pcBtn->EnableWindow(FALSE);
	}
	else{
		pcBtn->SetCheck(pcSysEnv->m_nYokoGumi);
	}

	pcBtn = (CButton*)GetDlgItem(IDC_CHKFIXED);
	pcBtn->EnableWindow(TRUE);
	if(pcSysEnv->m_nFixFlag == -1){
		pcBtn->EnableWindow(FALSE);
	}
	else{
		pcBtn->SetCheck(pcSysEnv->m_nFixFlag);
	}

	if(pcSysEnv->m_nMirror == -1){
		m_csHanten = _T("");
	}
	else if(pcSysEnv->m_nMirror == NOMIRROR){
		m_csHanten = _T("標準");
	}
	/*
	else if(pcSysEnv->m_nMirror == RLMIRROR){
		m_csHanten = _T("左右");
	}
	else if(pcSysEnv->m_nMirror == TBMIRROR){
		m_csHanten = _T("上下");
	}
	*/
	else if(pcSysEnv->m_nMirror == PTMIRROR){
		m_csHanten = _T("反転");
	}

	UpdateData(FALSE);
}

void CDialogWakuBar2::SetCrtView(CTonView * pcView)
{
	m_pcView = pcView;
}

void CDialogWakuBar2::InitCmbFont()
{
	int		sidx, idx;
	int		fonttype;
	TCHAR	crtfontname[RECMAX];
	TCHAR	fontname[RECMAX];
	TCHAR	accessname[RECMAX];

	idx = 0;
	sidx = 0;
	mojienvflistopen(_T("rb"));
	mojienvgetfontname(crtfontname);
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBFONT);
	pcCB->ResetContent();
	while(1){
		mojienvgetflist(idx, &fonttype, fontname, accessname);
		if(fontname[0] == _T('\0') && accessname[0] == _T('\0')){
			break;
		}
		if(_tcscmp(fontname, crtfontname) == 0){
			sidx = idx;
		}
		pcCB->AddString(fontname);
		idx++;
	}
	pcCB->SetCurSel(sidx);
	mojienvflistclose();
}

void CDialogWakuBar2::InitCmbFildName()
{
	int		nFildNo, nFildMax;
	TCHAR	szFildName[RECMAX];

	CStatic	*pcSt = (CStatic*)GetDlgItem(IDC_STFILD);
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBFILD);
	m_pcDoc = m_pcView->GetDocument();
	if(m_pcDoc->m_pcDialogDBEdit == NULL){
		pcSt->EnableWindow(FALSE);
		pcCB->EnableWindow(FALSE);
		return;
	}else{
		pcSt->EnableWindow(TRUE);
		pcCB->EnableWindow(TRUE);
	}
	pcCB->ResetContent();
	pcCB->AddString(_T("未設定"));
	nFildMax = m_pcDoc->m_pcDialogDBEdit->m_nFildMax;
	for(nFildNo = 0; nFildNo < nFildMax; nFildNo++){
		m_pcDoc->m_pcDialogDBEdit->GetFildName(nFildNo, szFildName);
		pcCB->AddString(szFildName);
	}
	pcCB->SetCurSel(0);
}

void CDialogWakuBar2::InitCmbFlatPar()
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBFLATPAR);
	pcCB->ResetContent();
	pcCB->AddString(_T("標準"));
	pcCB->AddString(_T("100%"));
	pcCB->AddString(_T("90%"));
	pcCB->AddString(_T("80%"));
	pcCB->AddString(_T("70%"));
	pcCB->AddString(_T("60%"));
	pcCB->AddString(_T("50%"));
	pcCB->AddString(_T("40%"));
	pcCB->AddString(_T("30%"));
	pcCB->AddString(_T("20%"));
	pcCB->AddString(_T("10%"));
	pcCB->AddString(_T("110%"));
	pcCB->AddString(_T("120%"));
	pcCB->AddString(_T("130%"));
	pcCB->AddString(_T("140%"));
	pcCB->AddString(_T("150%"));
	pcCB->AddString(_T("160%"));
	pcCB->AddString(_T("170%"));
	pcCB->AddString(_T("180%"));
	pcCB->AddString(_T("190%"));
	pcCB->AddString(_T("200%"));
	pcCB->SetCurSel(0);
}

void CDialogWakuBar2::InitCmbSpace()
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBSPACEMM);
	pcCB->ResetContent();
	pcCB->AddString(_T("標準"));
	pcCB->AddString(_T("0mm"));
	pcCB->AddString(_T("1mm"));
	pcCB->AddString(_T("2mm"));
	pcCB->AddString(_T("3mm"));
	pcCB->AddString(_T("4mm"));
	pcCB->AddString(_T("5mm"));
	pcCB->AddString(_T("10mm"));
	pcCB->SetCurSel(0);
}

void CDialogWakuBar2::InitCmbOderKind()
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBODERKIND);
	pcCB->ResetContent();
	pcCB->AddString(_T("均等"));
	pcCB->AddString(_T("上(左)"));
	pcCB->AddString(_T("中央"));
	pcCB->AddString(_T("下(右)"));
	pcCB->SetCurSel(0);
}

void CDialogWakuBar2::InitCmbKanaPar()
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBKANAPAR);
	pcCB->ResetContent();
	pcCB->AddString(_T("標準"));
	pcCB->AddString(_T("10%"));
	pcCB->AddString(_T("20%"));
	pcCB->AddString(_T("30%"));
	pcCB->AddString(_T("40%"));
	pcCB->AddString(_T("50%"));
	pcCB->SetCurSel(0);
}

void CDialogWakuBar2::InitCmbHanten()
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBHANTEN);
	pcCB->ResetContent();
	pcCB->AddString(_T("標準"));
	//pcCB->AddString(_T("左右"));
	//pcCB->AddString(_T("上下"));
	pcCB->AddString(_T("反転"));
	pcCB->SetCurSel(0);
}

void CDialogWakuBar2::OnSelchangeCmbfont() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBFONT);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csFont);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnSelchangeCmbfild() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBFILD);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csFildName);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnSelchangeCmbflatpar() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBFLATPAR);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csFlatPar);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnSelchangeCmbspacemm() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBSPACEMM);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csSpaceMM);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnSelchangeCmbkanapar() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBKANAPAR);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csKanaPar);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnSelchangeCmbhanten() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBHANTEN);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csHanten);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

BOOL CDialogWakuBar2::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN){
		if(pMsg->wParam == VK_RETURN){
			UpdateData(TRUE);
			if(m_pcView != NULL){
				GetItemElement();
				m_pcView->CallOnWakuApply();
			}
		}
	}
	return CDialogBar::PreTranslateMessage(pMsg);
}

void CDialogWakuBar2::OnSelchangeCmboderkind() 
{
	CComboBox* pcCB = (CComboBox*)GetDlgItem(IDC_CMBODERKIND);
	int idx = pcCB->GetCurSel();
	pcCB->GetLBText(idx, m_csOderKind);
	UpdateData(FALSE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnBtnspacekind() 
{
	//UpdateData(TRUE);
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnChkfixed() 
{
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}

void CDialogWakuBar2::OnChkyokogumi() 
{
	if(m_pcView != NULL){
		GetItemElement();
		m_pcView->CallOnWakuApply();
	}
}
