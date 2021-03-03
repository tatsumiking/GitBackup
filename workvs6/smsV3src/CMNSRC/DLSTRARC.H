#if !defined(AFX_DIALOGSTRGARC_H__EC270A83_9192_11D2_940A_0000E835797C__INCLUDED_)
#define AFX_DIALOGSTRGARC_H__EC270A83_9192_11D2_940A_0000E835797C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DialogStrgArc.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CDialogStrgArc �_�C�A���O

class CDialogStrgArc : public CDialog
{
// �R���X�g���N�V����
public:
	void SetItemElement();
	void GetItemElement();
	int m_nSetMode;
	CTonView	*m_pcView;
	CDialogStrgArc(CWnd* pParent = NULL);   // �W���̃R���X�g���N�^

// �_�C�A���O �f�[�^
	//{{AFX_DATA(CDialogStrgArc)
	enum { IDD = IDD_STRG_ARC };
	CStatic	m_stTop;
	CStatic	m_stSetMode3;
	CStatic	m_stSetMode2;
	CStatic	m_stSetMode1;
	BOOL	m_chkStrgEqual;
	BOOL	m_chkNoround;
	double	m_dERag;
	double	m_dSRag;
	double	m_dR;
	//}}AFX_DATA


// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B

	//{{AFX_VIRTUAL(CDialogStrgArc)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �T�|�[�g
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
protected:

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
	//{{AFX_MSG(CDialogStrgArc)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnReset();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_DIALOGSTRGARC_H__EC270A83_9192_11D2_940A_0000E835797C__INCLUDED_)
