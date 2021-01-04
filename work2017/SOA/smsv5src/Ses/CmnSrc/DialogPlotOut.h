
#pragma once

// DlgPlOut.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPlotOut �_�C�A���O

class CDialogPlotOut : public CDialog
{
// �R���X�g���N�V����
public:
	void InitDataCmb();
	void InitFileCmb();
	CDialogPlotOut(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	int		m_nFileNo;
	int		m_nDataNo;
	//{{AFX_DATA(CDialogPlotOut)
	enum { IDD = IDD_PLOTOUT };
	CComboBox	m_cmbFile;
	CComboBox	m_cmbData;
	BOOL	m_fHomeStop;
	BOOL	m_fLineOut;
	BOOL	m_fOmitOut;
	CString	m_csCmbData;
	CString	m_csCmbFile;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogPlotOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogPlotOut)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbdata();
	afx_msg void OnSelchangeCmbfile();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

