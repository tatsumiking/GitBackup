#if !defined(AFX_DLGMJSV_H__C6DE4682_FED5_11D1_A0A2_444553540000__INCLUDED_)
#define AFX_DLGMJSV_H__C6DE4682_FED5_11D1_A0A2_444553540000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgMjSv.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMojiSave �_�C�A���O

//C �����ۑ��_�C�A���O�N���X
class CDialogMojiSave : public CDialog
{
// �R���X�g���N�V����
public:
	void FontCBInit();
	CDialogMojiSave(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogMojiSave)
	enum { IDD = IDD_MOJISAVE };
	CString	m_csFontName;
	CString	m_csMojiCode;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogMojiSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogMojiSave)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DLGMJSV_H__C6DE4682_FED5_11D1_A0A2_444553540000__INCLUDED_)
