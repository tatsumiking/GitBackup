#if !defined(AFX_DLGCHKOUT_H__258BD15F_1A21_4F82_94AD_AF16E8B958AF__INCLUDED_)
#define AFX_DLGCHKOUT_H__258BD15F_1A21_4F82_94AD_AF16E8B958AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgChkOut.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCheckOutput �_�C�A���O

class CDialogCheckOutput : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogCheckOutput(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogCheckOutput)
	enum { IDD = IDD_CHECKOUTPUT };
	CStatic	m_stTop;
	CStatic	m_stImg;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CDialogCheckOutput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

public:
	CTonView	*m_pcView;
public:
// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogCheckOutput)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGCHKOUT_H__258BD15F_1A21_4F82_94AD_AF16E8B958AF__INCLUDED_)
