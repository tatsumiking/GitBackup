
#pragma once

// DSizeTrns.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSizeTrns �_�C�A���O

class CDialogSizeTrns : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogSizeTrns(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
public:
	CTonView	*m_pcView;
	CTonDoc		*m_pcDoc;
	//{{AFX_DATA(CDialogSizeTrns)
	enum { IDD = IDD_SIZETRNS };
	CButton		m_btnProperty;
	CComboBox	m_cmbPlotName;
	CString	m_csDstXSize;
	CString	m_csDstYSize;
	CString	m_csSrcXSize;
	CString	m_csSrcYSize;
	CString	m_csPlotName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogSizeTrns)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	void SetBtnEnable();
	void InitLoadFirstWakuData();
	void InitCmbPlotName();

protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogSizeTrns)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCmbplotname();
	afx_msg void OnDeltaposSpndstxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpndstysize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnsrcxsize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpnsrcysize(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	afx_msg void OnBtnproperty();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

