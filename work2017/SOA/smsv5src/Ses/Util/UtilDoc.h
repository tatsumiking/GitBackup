
// UtilDoc.h : CTonDoc クラスのインターフェイス
//


#pragma once

//#include	"../../Object/Data.h"
#include	"../../Object/Vect.h"
#include	"../../Object/MojiVect.h"

class CTonView;

class CTonDoc : public CDocument
{
protected: // シリアル化からのみ作成します。
	CTonDoc() noexcept;
	DECLARE_DYNCREATE(CTonDoc)
public:
	virtual ~CTonDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// 属性
public:
	CFSF1Font* m_pcFSF1Font;
	CESF0Font* m_pcESF0Font;
	CESF1Font* m_pcESF1Font;
	CFBU0Font* m_pcFBU0Font;
	CWinTTFont* m_pcWNTTFont;

	CTonView *m_pcView;
	TCHAR	m_szSrcFontName[QRECMAX];
	TCHAR	m_szDstFontName[QRECMAX];
	int		m_nTateGaki;
// 操作
public:
	CData* NewData(int nItemID);
	void FontConvertMain();
	void FontConvertLoop(int skuten, int ekuten);
	void FSF1FontDraw(CDC *pcDC);
	void FontInitCreate(TCHAR *lpFontName);
	void FontDataLoad(TCHAR *lpFontName, int nTateGaki, int kuten);
	void FontDataSave(TCHAR *lpFontName, int nTateGaki, int kuten);

	// オーバーライド
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// 実装

protected:

// 生成された、メッセージ割り当て関数
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// 検索ハンドラーの検索コンテンツを設定するヘルパー関数
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	void SetCrtView(CTonView *pcView);
};
