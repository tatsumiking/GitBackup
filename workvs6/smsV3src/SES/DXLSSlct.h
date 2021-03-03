#if !defined(DXLSSLCT_H)
#define DXLSSLCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DXLSSlct.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogXLSSelect �_�C�A���O
#include	"ExcelLib.h"

class CDialogXLSSelect : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogXLSSelect(CWnd* pParent = NULL);
// �_�C�A���O �f�[�^
public:
	CString	m_csFileName;
	CString	m_csSheetName;
	CExcelLib	m_cExcelLib;
	//{{AFX_DATA(CDialogXLSSelect)
	enum { IDD = IDD_XLSSLCT };
	CListBox	m_lstName;
	//}}AFX_DATA


// �I�[�o�[���C�h
public:
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogXLSSelect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitNameList();

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogXLSSelect)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DXLSSLCT_H)
