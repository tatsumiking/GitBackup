#if !defined(AFX_DLGOUTLN_H__15572482_1D99_11D2_A914_004026171D2B__INCLUDED_)
#define AFX_DLGOUTLN_H__15572482_1D99_11D2_A914_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgOutLn.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogOutLine �_�C�A���O

class CDialogOutLine : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogOutLine(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^
	void SetTitle(LPSTR lpTitle);

	LPSTR	m_lpTitle;
// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogOutLine)
	enum { IDD = IDD_OUTLINE };
	double	m_dOutLineMM;
	int		m_nOutLineKind;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogOutLine)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogOutLine)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGOUTLN_H__15572482_1D99_11D2_A914_004026171D2B__INCLUDED_)
