// MyDlgBar.cpp : �C���v�������e�[�V���� �t�@�C��
//

#include "stdafx.h"
#include "MyDialogBar.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar �_�C�A���O


CMyDialogBar::CMyDialogBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
}

void CMyDialogBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyDialogBar)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyDialogBar, CDialogBar)
	//{{AFX_MSG_MAP(CMyDialogBar)
		// ���� - ClassWizard �͂��̈ʒu�Ƀ}�b�s���O�p�̃}�N����ǉ��܂��͍폜���܂��B
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyDialogBar ���b�Z�[�W �n���h��

void CMyDialogBar::KeyIn(UINT nTCHAR, UINT nID)
{

}

void CMyDialogBar::SelectEdit(UINT nID)
{

}

