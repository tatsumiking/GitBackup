// DialogSendDetails.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "es98.h"
#include "DialogSendDetails.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSendDetails �_�C�A���O


CDialogSendDetails::CDialogSendDetails(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSendDetails::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSendDetails)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_INIT
}


void CDialogSendDetails::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSendDetails)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSendDetails, CDialog)
	//{{AFX_MSG_MAP(CDialogSendDetails)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSendDetails ���b�Z�[�W �n���h��
