// SrvrItem.h : CSgsSrvrItem �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(AFX_SRVRITEM_H__D2ED8FB3_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_SRVRITEM_H__D2ED8FB3_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTonSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CTonSrvrItem)

// �R���X�g���N�^
public:
	CTonSrvrItem(CTonDoc* pContainerDoc);

// �A�g���r���[�g
	CTonDoc* GetDocument() const
		{ return (CTonDoc*)COleServerItem::GetDocument(); }

// �I�[�o�[���C�h
	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	~CTonSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // �h�L�������g I/O �ɑ΂��ăI�[�o�[���C�h����܂��B
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_SRVRITEM_H__D2ED8FB3_417C_11D2_99B2_004026171D2B__INCLUDED_)
