#if !defined(DSETSP_H_)
#define DSETSP_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DSetSp.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSetSpace �_�C�A���O

class CDialogSetSpace : public CDialog
{
// �R���X�g���N�V����
public:
	void SetCrtView(CTonView *pcView);
	CDialogSetSpace(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogSetSpace)
	enum { IDD = IDD_SETSPACE };
	CString	m_csTopSpace;
	CString	m_csBottomSpace;
	CString	m_csLeftSpace;
	CString	m_csRightSpace;
	BOOL	m_nTonnbo;
	//}}AFX_DATA


// �I�[�o�[���C�h
public:
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSetSpace)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
protected:
	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSetSpace)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(DSETSP_H_)
