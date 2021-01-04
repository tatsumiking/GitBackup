
// UtilView.h : CTonView クラスのインターフェイス
//

#pragma once


class CTonView : public CView
{
protected: // シリアル化からのみ作成します。
	CTonView() noexcept;
	DECLARE_DYNCREATE(CTonView)
public:
	virtual ~CTonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 属性
public:
	CTonDoc* GetDocument() const;
	int m_nFirstFlag;
	int m_nCommand;
// 操作
public:

// オーバーライド
public:
	virtual void OnDraw(CDC* pDC);  // このビューを描画するためにオーバーライドされます。
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 実装

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnFontconvert();
};

#ifndef _DEBUG  // UtilView.cpp のデバッグ バージョン
inline CTonDoc* CTonView::GetDocument() const
   { return reinterpret_cast<CTonDoc*>(m_pDocument); }
#endif

