#if !defined(RDSOUTCRCT_H)
#define RDSOUTCRCT_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// OutCrctMDB.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CRdsOutCrct DAO レコードセット

class CRdsOutCrct : public CDaoRecordset
{
public:
	CRdsOutCrct(CDaoDatabase* pDatabase = NULL);
	DECLARE_DYNAMIC(CRdsOutCrct)

// フィールド/パラメータ データ
	//{{AFX_FIELD(CRdsOutCrct, CDaoRecordset)
	long	m_column1;
	CString	m_column2;
	CString	m_column3;
	CString	m_column4;
	CString	m_column5;
	CString	m_column6;
	CString	m_column7;
	CString	m_column8;
	//}}AFX_FIELD

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CRdsOutCrct)
	public:
	virtual CString GetDefaultDBName();		// デフォルト データベース名
	virtual CString GetDefaultSQL();		// レコードセットのデフォルト SQL
	virtual void DoFieldExchange(CDaoFieldExchange* pFX);  // RFX サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(RDSOUTCRCT_H)
