#if !defined(AFX_DIALOGBMPROUND_H__EBA70121_6508_11D2_80A6_004026171D2B__INCLUDED_)
#define AFX_DIALOGBMPROUND_H__EBA70121_6508_11D2_80A6_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogBmpRound.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogBmpRound �_�C�A���O

//C �a�l�o��]�_�C�A���O�N���X
class CDialogBmpRound : public CDialog
{
// �R���X�g���N�V����
public:
	int m_nMode;
	CDialogBmpRound(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogBmpRound)
	enum { IDD = IDD_BMP_ROUND };
	CComboBox	m_cbRoundKind;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogBmpRound)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogBmpRound)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGBMPROUND_H__EBA70121_6508_11D2_80A6_004026171D2B__INCLUDED_)
