#if !defined(AFX_DLGPTNFA_H__4176BEA3_4E35_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGPTNFA_H__4176BEA3_4E35_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgPtnFA.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogPtnFiguArc �_�C�A���O

class CDialogPtnFiguArc : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogPtnFiguArc(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogPtnFiguArc)
	enum { IDD = IDD_PTNFIGU_ARC };
	double	m_dCX;
	double	m_dCY;
	double	m_dR;
	double	m_dSRag;
	double	m_dERag;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogPtnFiguArc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogPtnFiguArc)
		// ����: ClassWizard �͂��̈ʒu�Ƀ����o�֐���ǉ����܂��B
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGPTNFA_H__4176BEA3_4E35_11D2_99B2_004026171D2B__INCLUDED_)