#if !defined(AFX_DIALOGDIVPLOTOUT_H__22574DC2_989D_11D2_940A_0000E835797C__INCLUDED_)
#define AFX_DIALOGDIVPLOTOUT_H__22574DC2_989D_11D2_940A_0000E835797C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogDivPlotOut.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDivPlotOut �_�C�A���O

class CTonView;

//C �����v���b�^�o�͐ݒ�_�C�A���O�N���X
class CDialogDivPlotOut : public CDialog
{
// �R���X�g���N�V����
public:
	CDialogDivPlotOut(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogDivPlotOut)
	enum { IDD = IDD_DIVPLOTER };
	BOOL	m_chkAreaBoxs;
	BOOL	m_chkStopChk;
	double	m_dNorisiro;
	double	m_dPlotXSize;
	double	m_dPlotYSize;
	//}}AFX_DATA

	CTonView*	m_pcView;


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogDivPlotOut)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogDivPlotOut)
	afx_msg void OnDlgreset();
	afx_msg void OnDlgset();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGDIVPLOTOUT_H__22574DC2_989D_11D2_940A_0000E835797C__INCLUDED_)
