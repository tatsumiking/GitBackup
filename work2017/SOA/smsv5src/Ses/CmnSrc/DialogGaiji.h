
#pragma once

// DlgGaiji.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogGaiji �_�C�A���O
#define	GAIJIEDITMAX	40

class CDialogGaiji : public CMyDialog
{
// �R���X�g���N�V����
public:
	CDialogGaiji(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

public:
	CMyEdit	*m_pcEdit[GAIJIEDITMAX];
	TCHAR	m_szGaijiText[QRECMAX];
public:
	void IniTCHAREdit();
	void InitTitleCombo();
	void SetGaijiText(int x, int y);

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogGaiji)
	enum { IDD = IDD_GAIJI };
	CStatic	m_stTop;
	CComboBox	m_cmbTitle;
	CString	m_csTitle;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogGaiji)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogGaiji)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCombo1();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

