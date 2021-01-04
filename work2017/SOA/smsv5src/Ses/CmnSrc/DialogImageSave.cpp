// DImgSave.cpp : ƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“ ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "DialogImageSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSave

IMPLEMENT_DYNAMIC(CDialogImageSave, CFileDialog)

static TCHAR	BASED_CODE szFilter[] = _T("‰æ‘œƒtƒ@ƒCƒ‹(*.JPG)|*.JPG||");

CDialogImageSave::CDialogImageSave(LPTSTR lpInitPath) :
		CFileDialog(FALSE, NULL, NULL, 0, szFilter, NULL)
{
	m_ofn.lpTemplateName = MAKEINTRESOURCE(IDD_IMAGESAVE);
	m_ofn.Flags |= (OFN_ENABLETEMPLATE | OFN_HIDEREADONLY);
	m_ofn.lpstrInitialDir = lpInitPath;
	m_ofn.lpstrFile = lpInitPath;
}

CDialogImageSave::~CDialogImageSave()
{
}

BEGIN_MESSAGE_MAP(CDialogImageSave, CFileDialog)
	//{{AFX_MSG_MAP(CDialogImageSave)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CDialogImageSave::OnInitDialog() 
{
	CString		csStr;		
	CEdit	*pcEdit;
	CComboBox	*pcCmb;

	CFileDialog::OnInitDialog();

	csStr.Format(_T("%d"), m_nXSize);
	pcEdit = (CEdit*)GetDlgItem(IDC_EDXSIZE);
	pcEdit->SetWindowText(csStr);

	csStr.Format(_T("%d"), m_nYSize);
	pcEdit = (CEdit*)GetDlgItem(IDC_EDYSIZE);
	pcEdit->SetWindowText(csStr);

	csStr.Format(_T("%d"), m_nQuality);
	pcEdit = (CEdit*)GetDlgItem(IDC_EDQUALITY);
	pcEdit->SetWindowText(csStr);

	pcCmb = (CComboBox*)GetDlgItem(IDC_CMBROUND);
	pcCmb->AddString(_T("‰ñ“]‚È‚µ"));
	pcCmb->AddString(_T("‚X‚O“x‰ñ“]"));
	pcCmb->AddString(_T("‚P‚W‚O“x‰ñ“]"));
	pcCmb->AddString(_T("‚Q‚V‚O“x‰ñ“]"));
	pcCmb->SetCurSel(m_nRotate);

	return TRUE;
}

BOOL CDialogImageSave::OnFileNameOK()
{
	int		ret;
	CString		csStr;		
	CEdit	*pcEdit;
	CComboBox	*pcCmb;

	ret = CFileDialog::OnFileNameOK();

	pcEdit = (CEdit*)GetDlgItem(IDC_EDXSIZE);
	pcEdit->GetWindowText(csStr);
	m_nXSize = _ttoi(csStr);

	pcEdit = (CEdit*)GetDlgItem(IDC_EDYSIZE);
	pcEdit->GetWindowText(csStr);
	m_nYSize = _ttoi(csStr);
	
	pcEdit = (CEdit*)GetDlgItem(IDC_EDQUALITY);
	pcEdit->GetWindowText(csStr);
	m_nQuality = _ttoi(csStr);

	pcCmb = (CComboBox*)GetDlgItem(IDC_CMBROUND);
	m_nRotate = pcCmb->GetCurSel();
	return(ret);
}
