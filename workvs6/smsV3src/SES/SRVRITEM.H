// SrvrItem.h : CSesSrvrItem クラスの宣言およびインターフェイスの定義をします。
//

#if !defined(AFX_SRVRITEM_H__A4B45ED3_417C_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_SRVRITEM_H__A4B45ED3_417C_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CTonSrvrItem : public COleServerItem
{
	DECLARE_DYNAMIC(CTonSrvrItem)

// コンストラクタ
public:
	CTonSrvrItem(CTonDoc* pContainerDoc);

// アトリビュート
	CTonDoc* GetDocument() const
		{ return (CTonDoc*)COleServerItem::GetDocument(); }

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonSrvrItem)
	public:
	virtual BOOL OnDraw(CDC* pDC, CSize& rSize);
	virtual BOOL OnGetExtent(DVASPECT dwDrawAspect, CSize& rSize);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	~CTonSrvrItem();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	virtual void Serialize(CArchive& ar);   // ドキュメント I/O に対してオーバーライドされます。
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_SRVRITEM_H__A4B45ED3_417C_11D2_99B2_004026171D2B__INCLUDED_)
