#if !defined(AFX_DSLCTMADRS_H__C613E70F_B60E_42D0_953B_EDFF99B2519D__INCLUDED_)
#define AFX_DSLCTMADRS_H__C613E70F_B60E_42D0_953B_EDFF99B2519D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSlctMAdrs.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSlctMailAdrs �_�C�A���O

class CDialogSlctMailAdrs : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogSlctMailAdrs(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CString		m_csAdrs;
	//{{AFX_DATA(CDialogSlctMailAdrs)
	enum { IDD = IDD_SLCTADRS };
	CComboBox	m_cmbMailAdrs;
	CString	m_csSubject;
	CString	m_csMailAdrs;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSlctMailAdrs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void InitCmbMailAdrs();
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSlctMailAdrs)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DSLCTMADRS_H__C613E70F_B60E_42D0_953B_EDFF99B2519D__INCLUDED_)
