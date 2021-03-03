// CntrItem.h : CSgsCntrItem �N���X�̐錾����уC���^�[�t�F�C�X�̒�`�����܂��B
//

#if !defined(AFX_CNTRITEM_H__D2ED8FB7_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_CNTRITEM_H__D2ED8FB7_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTonDoc;
class CTonView;

class CTonCntrItem : public COleClientItem
{
	DECLARE_SERIAL(CTonCntrItem)

// �R���X�g���N�^
public:
	CStrgVect*	m_pStrgVect;

	CTonCntrItem(CTonDoc* pContainer = NULL);
		// ����: pContainer �� NULL �ɂ���� IMPLEMENT_SERIALIZE ���s�����Ƃ��ł��܂��B
		//  IMPLEMENT_SERIALIZE ���s�����߂ɂ̓N���X���Ɉ����̂Ȃ��R���X�g���N�^���K�v�ł��B
		//  �ʏ�AOLE �A�C�e���� NULL �łȂ��h�L�������g �|�C���^�őg�ݍ��܂�Ă��܂��B
		//

// �A�g���r���[�g
public:
	CTonDoc* GetDocument()
		{ return (CTonDoc*)COleClientItem::GetDocument(); }
	CTonView* GetActiveView()
		{ return (CTonView*)COleClientItem::GetActiveView(); }

	void RefreshSize();
	CRect m_rectPosition;

	// ClassWizard �͉��z�֐��̃I�[�o�[���C�h�𐶐����܂��B
	//{{AFX_VIRTUAL(CTonCntrItem)
	public:
	virtual void OnChange(OLE_NOTIFICATION wNotification, DWORD dwParam);
	virtual void OnActivate();
	protected:
	virtual void OnGetItemPosition(CRect& rPosition);
	virtual void OnDeactivateUI(BOOL bUndoable);
	virtual BOOL OnChangeItemPosition(const CRect& rectPos);
	virtual BOOL CanActivate();
	//}}AFX_VIRTUAL

// �C���v�������e�[�V����
public:
	~CTonCntrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void Serialize(CArchive& ar);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio �͑O�s�̒��O�ɒǉ��̐錾��}�����܂��B

#endif // !defined(AFX_CNTRITEM_H__D2ED8FB7_417C_11D2_99B2_004026171D2B__INCLUDED_)
