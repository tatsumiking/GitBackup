#if !defined(AFX_DLGFCHNG_H__7F66C362_8745_11D2_940A_0000E835797C__INCLUDED_)
#define AFX_DLGFCHNG_H__7F66C362_8745_11D2_940A_0000E835797C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgFChng.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFontChange �_�C�A���O
class CTonView;

//C ���̕ύX�_�C�A���O�N���X
class CDialogFontChange : public CDialog
{
// �R���X�g���N�V����
public:
	CTonView*	m_pcView;

public:
	void FontCBInit();
	CDialogFontChange(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogFontChange)
	enum { IDD = IDD_FONTCHNG };
	CString	m_csFontName;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B


	//{{AFX_VIRTUAL(CDialogFontChange)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogFontChange)
	virtual BOOL OnInitDialog();
	afx_msg void OnDlgreset();
	afx_msg void OnDlgset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGFCHNG_H__7F66C362_8745_11D2_940A_0000E835797C__INCLUDED_)
