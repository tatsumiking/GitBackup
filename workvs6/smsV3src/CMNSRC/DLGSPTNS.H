#if !defined(AFX_DLGSPTNS_H__6F81D002_4EF8_11D2_99B2_004026171D2B__INCLUDED_)
#define AFX_DLGSPTNS_H__6F81D002_4EF8_11D2_99B2_004026171D2B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// DlgSPtnS.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSPtnSave ダイアログ

class CDialogSPtnSave : public CDialog
{
// コンストラクション
public:
	int m_nSlctidx;
	CDialogSPtnSave(CWnd* pParent = NULL);   // 標準のコンストラクタ

// ダイアログ データ
	//{{AFX_DATA(CDialogSPtnSave)
	enum { IDD = IDD_SPTNSAVE };
	CListBox	m_lstSPtn;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。

	//{{AFX_VIRTUAL(CDialogSPtnSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogSPtnSave)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLstsptn();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGSPTNS_H__6F81D002_4EF8_11D2_99B2_004026171D2B__INCLUDED_)
