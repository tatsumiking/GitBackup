// UPTView.h : CTonView クラスの宣言およびインターフェイスの定義をします。
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_UPTVIEW_H__CEEECC91_3DCA_4EB1_90E8_4E6D5DC5D6B8__INCLUDED_)
#define AFX_UPTVIEW_H__CEEECC91_3DCA_4EB1_90E8_4E6D5DC5D6B8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTonView : public CView
{
protected: // シリアライズ機能のみから作成します。
	CTonView();
	DECLARE_DYNCREATE(CTonView)

// アトリビュート
public:
	CTonDoc* GetDocument();

// オペレーション
public:

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CTonView)
	public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画する際にオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	virtual ~CTonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成されたメッセージ マップ関数
protected:
	//{{AFX_MSG(CTonView)
		// メモ -  ClassWizard はこの位置にメンバ関数を追加または削除します。
		//         この位置に生成されるコードを編集しないでください。
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // UPTView.cpp ファイルがデバッグ環境の時使用されます。
inline CTonDoc* CTonView::GetDocument()
   { return (CTonDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_UPTVIEW_H__CEEECC91_3DCA_4EB1_90E8_4E6D5DC5D6B8__INCLUDED_)
