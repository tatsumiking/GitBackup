#if !defined(AFX_MYBUTTON_H__0D2F6505_1E16_11D3_9F64_0000E835797C__INCLUDED_)
#define AFX_MYBUTTON_H__0D2F6505_1E16_11D3_9F64_0000E835797C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyButton.h : �w�b�_�[ �t�@�C��
//

/////////////////////////////////////////////////////////////////////////////
// CMyButton �E�B���h�E

class CMyButton : public CButton
{
// �R���X�g���N�V����
public:
	CMyButton();

// �A�g���r���[�g
public:

// �I�y���[�V����
public:

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CMyButton)
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	virtual ~CMyButton();

	// �������ꂽ���b�Z�[�W �}�b�v�֐�
protected:
	//{{AFX_MSG(CMyButton)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_MYBUTTON_H__0D2F6505_1E16_11D3_9F64_0000E835797C__INCLUDED_)
