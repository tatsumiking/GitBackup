
#pragma once

// DWkPpAdd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWkPaperAdd �_�C�A���O
class CTonView;

class CDialogWkPaperAdd : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogWkPaperAdd(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonView	*m_pcView;
	//{{AFX_DATA(CDialogWkPaperAdd)
	enum { IDD = IDD_WAKU_PAPERADD };
	CString	m_csPaperPath;
	CString	m_csPaperName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogWkPaperAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	BOOL CheckFileName(LPTSTR lpFileName);
protected:
	void CBInitPaperName();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWkPaperAdd)
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangePapername();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

