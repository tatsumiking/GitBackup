// DlgChkOut.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DlgChkOut.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogCheckOutput �_�C�A���O
extern "C" {
	int	senvsetdrawmode(int mode);
}

CDialogCheckOutput::CDialogCheckOutput(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogCheckOutput::IDD, pParent)
{
	m_pcView = (CTonView*)pParent;
	//{{AFX_DATA_INIT(CDialogCheckOutput)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CDialogCheckOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogCheckOutput)
	DDX_Control(pDX, IDC_STTOP, m_stTop);
	DDX_Control(pDX, IDC_STIMG, m_stImg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogCheckOutput, CDialog)
	//{{AFX_MSG_MAP(CDialogCheckOutput)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogCheckOutput ���b�Z�[�W �n���h��

void CDialogCheckOutput::OnPaint() 
{
	CRect	imgrect;
	CRect	toprect;

	CPaintDC dc(this);
	m_stTop.GetWindowRect(&toprect);
	m_stImg.GetWindowRect(&imgrect);
	imgrect.left -= toprect.left;
	imgrect.right -= toprect.left;
	imgrect.top -= toprect.top;
	imgrect.bottom -= toprect.top;
	senvsetdrawmode(PREVIEW);
	m_pcView->DrawWakuInRect(&dc, imgrect);
}
