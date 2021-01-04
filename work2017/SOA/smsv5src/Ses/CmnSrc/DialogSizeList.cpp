// DialogSizeList.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "DialogSizeList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeList �_�C�A���O


CDialogSizeList::CDialogSizeList(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSizeList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSizeList)
	m_dXSize = 0.0;
	m_dYSize = 0.0;
	m_nYCount = 0;
	m_nXCount = 0;
	//}}AFX_DATA_INIT
}


void CDialogSizeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSizeList)
	DDX_Text(pDX, IDC_XSIZE, m_dXSize);
	DDX_Text(pDX, IDC_YSIZE, m_dYSize);
	DDX_Text(pDX, IDC_XCOUNT, m_nXCount);
	DDX_Text(pDX, IDC_YCOUNT, m_nYCount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSizeList, CDialog)
	//{{AFX_MSG_MAP(CDialogSizeList)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeList ���b�Z�[�W �n���h��