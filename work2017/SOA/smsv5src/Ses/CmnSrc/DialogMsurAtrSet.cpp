// DlgMsAtr.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogMsurAtrSet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMsurAtrSet �_�C�A���O


CDialogMsurAtrSet::CDialogMsurAtrSet(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMsurAtrSet::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMsurAtrSet)
	m_csMsurStrg = _T("");
	m_dFontSize = 0.0;
	//}}AFX_DATA_INIT
}


void CDialogMsurAtrSet::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMsurAtrSet)
	DDX_Text(pDX, IDC_MSUR_STRG, m_csMsurStrg);
	DDX_Text(pDX, IDC_MSUR_FONTSIZE, m_dFontSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMsurAtrSet, CDialog)
	//{{AFX_MSG_MAP(CDialogMsurAtrSet)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMsurAtrSet ���b�Z�[�W �n���h��