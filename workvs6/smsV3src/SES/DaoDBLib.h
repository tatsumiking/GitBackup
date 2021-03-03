#if !defined(DAODBLIB_H)
#define DAODBLIB_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DaoDBLib.h : ヘッダー ファイル
//
#include	"ExcelLib.h"

/////////////////////////////////////////////////////////////////////////////
// CDaoDBLib

// [クラス名] CDaoDBLib
// [親クラス] CObject
// [クラス概要] XLS、CSV、MDBリンク処理クラス
class CDaoDBLib : public CObject
{
public:
	CDaoDBLib();
	DECLARE_DYNCREATE(CDaoDBLib);
	virtual ~CDaoDBLib();
// アトリビュート
public:
	int	m_nType;	// リンクデータベース区分
	int	m_nDBOpen;	// リンクオープン状態フラグ
	CString	m_csFileName;	// リンクファイル名
	CString	m_csTableName;	// リンクテーブル、シート名称

	CDaoRecordset* m_pcDaoRecordset;	// データベースレコードセット
	CDaoDatabase* m_pcDaoDatabase;	// データベース
	CDaoTableDef *m_pcTableDef;	// データベーステーブル
	CDaoQueryDef *m_pcQueryDef;	// データベースクエリ
	FILE	*m_fpCSV;	// CSVファイルポインタ
	LPSTR	m_lpDBBuf;	// 読み込みデータ格納バッファ
	LPSTR	m_lpCsvsp[DBFILDMAX];	// 読み込みデータ格納バッファ
	int		m_nFildMax;	// フィールド数
	CExcelLib	m_cExcelLib;	// エクセルファイル処理ライブラリ
	int		m_nCrtLine;	// 読み込み行番号

// オペレーション
public:
	int CheckCsvFile(LPSTR lpFileName);
	CString ConversionVerToCStr(COleVariant var);
	void	Open();
	void	Close();
	int		InitGetFild();
	void	EndGetFild();
	int		InitGetRecord();
	void	EndGetRecord();
	int	GetFildName(int idx, LPSTR lpFildName);
	int	SetNextRecord();
	int	GetItemStr(int idx, LPSTR lpStr);
// インプリメンテーション
protected:
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
private:
	int IsKanji(UINT far hc);
	int DivCsvSetSp(LPSTR csvsp[], LPSTR csvbuf);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。
#endif // !defined(DAODBLIB_H)
