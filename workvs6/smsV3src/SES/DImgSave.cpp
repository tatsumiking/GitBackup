// DImgSave.cpp : ƒCƒ“ƒvƒŠƒƒ“ƒe[ƒVƒ‡ƒ“ ƒtƒ@ƒCƒ‹
//

#include "stdafx.h"
#include "DImgSave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogImageSave

IMPLEMENT_DYNAMIC(CDialogImageSave, CFileDialog)

static char	BASED_CODE szFilter[] = "‰æ‘œƒtƒ@ƒCƒ‹(*.JPG)|*.JPG||";

CDialogImageSave::CDialogImageSave(LPSTR lpInitPath) :
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

	csStr.Format("%d", m_nXSize);
	pcEdit = (CEdit*)GetDlgItem(IDC_EDXSIZE);
	pcEdit->SetWindowText(csStr);

	csStr.Format("%d", m_nYSize);
	pcEdit = (CEdit*)GetDlgItem(IDC_EDYSIZE);
	pcEdit->SetWindowText(csStr);

	csStr.Format("%d", m_nQuality);
	pcEdit = (CEdit*)GetDlgItem(IDC_EDQUALITY);
	pcEdit->SetWindowText(csStr);

	pcCmb = (CComboBox*)GetDlgItem(IDC_CMBROUND);
	pcCmb->AddString("‰ñ“]‚È‚µ");
	pcCmb->AddString("‚X‚O“x‰ñ“]");
	pcCmb->AddString("‚P‚W‚O“x‰ñ“]");
	pcCmb->AddString("‚Q‚V‚O“x‰ñ“]");
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
	m_nXSize = atoi(csStr);

	pcEdit = (CEdit*)GetDlgItem(IDC_EDYSIZE);
	pcEdit->GetWindowText(csStr);
	m_nYSize = atoi(csStr);
	
	pcEdit = (CEdit*)GetDlgItem(IDC_EDQUALITY);
	pcEdit->GetWindowText(csStr);
	m_nQuality = atoi(csStr);

	pcCmb = (CComboBox*)GetDlgItem(IDC_CMBROUND);
	m_nRotate = pcCmb->GetCurSel();
	return(ret);
}
