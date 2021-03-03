// CntrItem.h : CSgsCntrItem クラスの宣言およびインターフェイスの定義をします。
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

// コンストラクタ
public:
	CStrgVect*	m_pStrgVect;

	CTonCntrItem(CTonDoc* pContainer = NULL);
		// メモ: pContainer を NULL にすると IMPLEMENT_SERIALIZE を行うことができます。
		//  IMPLEMENT_SERIALIZE を行うためにはクラス内に引数のないコンストラクタが必要です。
		//  通常、OLE アイテムは NULL でないドキュメント ポインタで組み込まれています。
		//

// アトリビュート
public:
	CTonDoc* GetDocument()
		{ return (CTonDoc*)COleClientItem::GetDocument(); }
	CTonView* GetActiveView()
		{ return (CTonView*)COleClientItem::GetActiveView(); }

	void RefreshSize();
	CRect m_rectPosition;

	// ClassWizard は仮想関数のオーバーライドを生成します。
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

// インプリメンテーション
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
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_CNTRITEM_H__D2ED8FB7_417C_11D2_99B2_004026171D2B__INCLUDED_)
