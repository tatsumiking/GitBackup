#pragma once

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DWkBarG.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogWakuBarG1 �_�C�A���O

class CDialogWakuBarG1 : public CDialogBar
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	CDialogWakuBarG1(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogWakuBarG1)
	enum { IDD = IDD_WAKUBARG1 };
	CString	m_csWorkXSize;
	CString	m_csWorkYSize;
	//}}AFX_DATA

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogWakuBarG1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogWakuBarG1)
	afx_msg void OnChangeEdworkxsize();
	afx_msg void OnChangeEdworkysize();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B
