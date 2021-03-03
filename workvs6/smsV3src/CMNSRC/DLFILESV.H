#if !defined(AFX_DLGFILESV_H__217D1D04_1E79_11D3_A712_00A0B0086478__INCLUDED_)
#define AFX_DLGFILESV_H__217D1D04_1E79_11D3_A712_00A0B0086478__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlFileSv.h : ヘッダー ファイル
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFileSave ダイアログ

class CDialogFileSave : public CDialog
{
// コンストラクション
public:
	int m_nImgSize;
	int m_nImgSY;
	int m_nImgSX;
	HANDLE	m_hDib;
	long	m_lDibSize;
	long	m_lImgFileSize;

	void LoadCmbKouji();
	void LoadCmbGyousya();
	void SaveCmbKouji();
	void SaveCmbGyousya();
	void BitMapImageDisp(CDC *dc);
	void BitMapImageLoad(CArchive& ar);
	void FileHeadLoad();
	void SlctFileListInit();
	CDialogFileSave(CWnd* pParent = NULL);   // 標準のコンストラクタ

	void UpDateFileList();
	void SetInitPath(LPSTR path);
	void SetSafixs(LPSTR safixs);

	char m_szSafixs[FNAMEMAX];
	char m_szPathName[FNAMEMAX];
	char m_szFileName[FNAMEMAX];
	int	m_nFirst;
	char m_szGyousya[QRECMAX];	
	char m_szKouji[QRECMAX];	
	char m_szComent1[QRECMAX];	
	char m_szComent2[QRECMAX];	
	char m_szComent3[QRECMAX];	
	// ダイアログ データ
	//{{AFX_DATA(CDialogFileSave)
	enum { IDD = IDD_FILESAVE };
	CStatic	m_stTop;
	CStatic	m_stImg;
	CEdit	m_edSlctFile;
	CComboBox	m_cmbGyousya;
	CComboBox	m_cmbKouji;
	CEdit	m_edComent1;
	CEdit	m_edComent2;
	CEdit	m_edComent3;
	CListBox	m_lstFile;
	CListBox	m_lstSlctFile;
	CString	m_csPath;
	//}}AFX_DATA


// オーバーライド
	// ClassWizard は仮想関数のオーバーライドを生成します。
	//{{AFX_VIRTUAL(CDialogFileSave)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV サポート
	//}}AFX_VIRTUAL

// インプリメンテーション
protected:

	// 生成されたメッセージ マップ関数
	//{{AFX_MSG(CDialogFileSave)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnSelchangeLstslctfile();
	afx_msg void OnSelchangeLstslctdir();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ は前行の直前に追加の宣言を挿入します。

#endif // !defined(AFX_DLGFILESV_H__217D1D04_1E79_11D3_A712_00A0B0086478__INCLUDED_)

