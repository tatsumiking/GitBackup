#if !defined(AFX_DLEDDATA_H__42E0CB21_95F9_11D3_9DFE_00A0B0086478__INCLUDED_)
#define AFX_DLEDDATA_H__42E0CB21_95F9_11D3_9DFE_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlEdData.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogEditData ダイアログ

class CDialogEditData : public CDialog
{
// コンストラクション
public:
	CDialogEditData(CWnd* pParent = NULL);   // 標準のコンストラクタ
public:

// ダイアログ データ
public:
	CAh32Dlg	*m_pcAh32Dlg;
	HGLOBAL m_hEditBuf;
	LPSTR	m_lpEditBuf;
	//{{AFX_DATA(CDialogEditData)
	enum { IDD = IDD_EDIT_DATA };
	CEdit	m_edData;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogEditData)
	public:
	virtual BOOL DestroyWindow();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:
	void SetStrgTextToFile();
	LPSTR CrDelFgets(char * buf, int max, FILE * fp);
	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogEditData)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLEDDATA_H__42E0CB21_95F9_11D3_9DFE_00A0B0086478__INCLUDED_)
