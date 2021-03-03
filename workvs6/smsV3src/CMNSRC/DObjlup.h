#if !defined(AFX_DOBJLUP_H)
#define AFX_DOBJLUP_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DObjLup.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogObjLineup ダイアログ

class CDialogObjLineup : public CDialog
{
// コンストラクション
public:
	CDialogObjLineup(CWnd* pParent = NULL);   // 標準のコンストラクタ

public:
	int m_nMode;
	int	m_nSlctMode;
// ダイアログ データ
	//{{AFX_DATA(CDialogObjLineup)
	enum { IDD = IDD_OBJ_LINEUP };
		// メモ: ClassWizard はこの位置にデータ メンバを追加します。
	//}}AFX_DATA

// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogObjLineup)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
public:
	void EndProcess();
	void SetModal();
	void SetModaless();
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogObjLineup)
	afx_msg void OnMode1();
	afx_msg void OnMode2();
	afx_msg void OnMode3();
	afx_msg void OnMode4();
	afx_msg void OnMode5();
	afx_msg void OnMode6();
	afx_msg void OnMode7();
	afx_msg void OnMode8();
	afx_msg void OnMode9();
	afx_msg void OnMode10();
	afx_msg void OnMode11();
	afx_msg void OnMode12();
	afx_msg void OnMode13();
	afx_msg void OnMode14();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DOBJLUP_H)
