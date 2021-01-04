
#pragma once

// DDBEdit.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDBEdit ダイアログ
class	CDaoDBLib;

class CDialogDBEdit : public CDialog
{
// コンストラクション
public:
	CDialogDBEdit(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
public:
	CTonApp		*m_pcApp;
	CTonDoc		*m_pcDoc;	// ドキュメントクラス
	CTonView	*m_pcView;	// ビュークラス
	CDaoDBLib	*m_pcDaoDBLib;	// XLS、CSV、MDBリンク処理クラス
	int		m_nFirst;
	int		m_nMode;
	int		m_nFildMax;
	int		m_nLineMax;
	int		m_nCrtCol;	// 選択カラム番号を保持
	UINT	m_nCrtLine;	// 選択レコード番号を保持
	//{{AFX_DATA(CDialogDBEdit)
	enum { IDD = IDD_DBEDIT };
	CStatic	m_stTop;
	CListCtrl	m_lstFileList;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogDBEdit)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void GetFildElement(int nFildNo, LPTSTR LPTSTR);
	int GetFildNo(LPTSTR lpFildName);
	int GetFildElement(LPTSTR lpFildName, LPTSTR LPTSTR);
	void GetListCtrlSize(int *xsize, int *ysize, int *oneysize);
	int InitRecord();
	int InitFildName();
	void EndDaoDBLib();
	void InitDaoDBLib();
	void InitItemElement();
	void SetModaless();
	void GetFildName(int nFildNo, LPTSTR lpFildName);
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogDBEdit)
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClickLstitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

