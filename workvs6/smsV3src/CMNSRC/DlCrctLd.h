#if !defined(AFX_DLCRCTLD_H__2D488C21_877C_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_DLCRCTLD_H__2D488C21_877C_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlCrctLd.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCrctLoad �_�C�A���O

class CDialogCrctLoad : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogCrctLoad(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogCrctLoad)
	enum { IDD = IDD_CRCTLOAD };
	CListBox	m_lstCrct;
	//}}AFX_DATA
	int		m_nSlctIdx;

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogCrctLoad)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogCrctLoad)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstCrct();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLCRCTLD_H__2D488C21_877C_11D3_9F64_0000E835797C__INCLUDED_)
